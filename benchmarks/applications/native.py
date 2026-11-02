"""Inspect linked code. Counts are static instructions, never inferred DRAM traffic."""
import hashlib
import re
import subprocess


def run(args):
    return subprocess.check_output([str(x) for x in args],text=True)


def function_info(binary,name):
    for line in run(['nm','-S','--defined-only',binary]).splitlines():
        fields=line.split()
        if len(fields)==4 and fields[3]==name:return int(fields[0],16),int(fields[1],16)
    raise ValueError('missing function '+name)


def fingerprint(binary,name):
    start,size=function_info(binary,name)
    for line in run(['objdump','-h',binary]).splitlines():
        fields=line.split()
        if len(fields)>4 and fields[1]=='.text':base=int(fields[3],16);break
    else:raise ValueError('missing .text')
    path=binary.with_suffix('.text.bin')
    subprocess.run(['objcopy','--dump-section',f'.text={path}',str(binary)],check=True,capture_output=True)
    body=path.read_bytes()[start-base:start-base+size]
    assert len(body)==size and size>0
    return hashlib.sha256(body).hexdigest()


def instructions(binary,name):
    start,size=function_info(binary,name)
    text=run(['objdump','-d','--no-show-raw-insn',f'--start-address={start}',f'--stop-address={start+size}',binary])
    result=[]
    for line in text.splitlines():
        m=re.match(r'\s*([0-9a-f]+):\s+(\w+)\s*(.*)',line)
        if m:result.append((int(m[1],16),m[2],m[3]))
    return result


def operands(text):
    # AT&T commas inside base/index addressing do not separate operands.
    level=0;at=0;out=[]
    for i,c in enumerate(text):
        if c=='(':level+=1
        elif c==')':level-=1
        elif c==',' and level==0:out.append(text[at:i].strip());at=i+1
    out.append(text[at:].strip());return out


def counts(ins):
    loads=stores=arithmetic=broadcasts=0;ops={};steps=set()
    for _,op,args in ins:
        if op=='add':
            m=re.match(r'\$0x(20|40|60|80),%r',args)
            if m:steps.add(int(m[1],16))
        if '%ymm' not in args:continue
        ops[op]=ops.get(op,0)+1
        parts=operands(args.split('#')[0])
        # Folded memory operands in arithmetic count as reads too.
        if any('(' in p for p in parts[:-1]):loads+=1
        if '(' in parts[-1]:stores+=1
        if op.startswith(('vpadd','vpsub','vpmul','vpsll','vpsrl','vpsra')):arithmetic+=1
        if op.startswith('vpbroadcast'):broadcasts+=1
    return dict(loads=loads,stores=stores,arithmetic=arithmetic,broadcasts=broadcasts,
                opcodes=ops,byte_increments=sorted(steps))


def inspect(binary,name,generated):
    ins=instructions(binary,name);regions=[]
    if generated:
        symbols={fields[2]:int(fields[0],16) for line in run(['nm','-a',binary]).splitlines()
                 if len(fields:=line.split())==3 and fields[1].lower()=='t'}
        starts=sorted((addr,label) for label,addr in symbols.items() if re.fullmatch(r'\.L'+name+r'_vector\d+',label))
        tails=sorted(addr for label,addr in symbols.items() if re.fullmatch(r'\.L'+name+r'_tail\d+',label))
        for start,label in starts:
            end=next(addr for addr in tails if addr>start)
            body=[x for x in ins if start<=x[0]<end]
            regions.append(dict(label=label,start=start,end=end,scope='loop_iteration_8_elements',**counts(body)))
    else:
        # Natural backward branches containing YMM operations identify GCC's main
        # vector loops. Nested enclosing ranges are excluded. All ranges are saved
        # for manual review; SSE epilogues are not mixed into 256-bit counts.
        ranges=[]
        for addr,op,args in ins:
            m=re.match(r'([0-9a-f]+)\s',args)
            if op.startswith('j') and m and int(m[1],16)<addr:
                start=int(m[1],16);body=[x for x in ins if start<=x[0]<=addr]
                if any('%ymm' in x[2] for x in body):ranges.append((start,addr+1))
        ranges=sorted(set(ranges))
        ranges=[r for r in ranges if not any(r!=s and r[0]<=s[0] and r[1]>=s[1] for s in ranges)]
        for start,end in ranges:
            c=counts([x for x in ins if start<=x[0]<end])
            regions.append(dict(label=hex(start),start=start,end=end,scope='gcc_256bit_loop_iteration',**c))
        if not regions and any('%ymm' in x[2] for x in ins):
            regions.append(dict(label='straight_line',start=ins[0][0],end=ins[-1][0]+1,
                                scope='whole_function_256bit_static_instructions',**counts(ins)))
    return dict(code_sha256=fingerprint(binary,name),regions=regions,
                ymm_instructions=sum('%ymm' in x[2] for x in ins),
                xmm_instructions=sum('%xmm' in x[2] for x in ins))
