"""Count only emitted vector bodies in an assembled/linked binary (GNU local labels retained)."""
import re
import subprocess


def vector_bodies(binary, function):
    def run(args):
        return subprocess.check_output([str(x) for x in args],text=True)
    symbols={name:int(addr,16) for addr,kind,name in
             (line.split() for line in run(['nm','-a',binary]).splitlines() if len(line.split())==3)
             if kind.lower()=='t'}
    starts=sorted((addr,name) for name,addr in symbols.items()
                  if re.fullmatch(r'\.L'+re.escape(function)+r'_vector\d+',name))
    tails=sorted(addr for name,addr in symbols.items()
                 if re.fullmatch(r'\.L'+re.escape(function)+r'_tail\d+',name))
    result=[]
    for start,name in starts:
        end=next(addr for addr in tails if addr>start)
        text=run(['objdump','-d','--no-show-raw-insn',f'--start-address={start}',f'--stop-address={end}',binary])
        loads=stores=alu=0;registers=set()
        for line in text.splitlines():
            ins=re.search(r'^\s*[0-9a-f]+:\s+(\w+)\s*(.*)',line)
            if not ins:continue
            op,operands=ins.groups()
            registers.update(int(x) for x in re.findall(r'%ymm(\d+)',operands))
            if op=='vmovdqu':
                if operands.startswith('%ymm'):stores+=1
                elif '(' in operands and re.search(r',%ymm\d+$',operands):loads+=1
                else:raise AssertionError('Unexpected vector move: '+line)
            elif op in ['vpaddd','vpsubd','vpmulld']:alu+=1
        assert all(0<=r<16 for r in registers)
        result.append(dict(label=name,loads=loads,stores=stores,alu=alu,registers=sorted(registers),disassembly=text))
    return result
