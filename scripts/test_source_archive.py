#!/usr/bin/env python3
"""Reproduce a source-only ZIP whose files all extract without executable bits."""
from pathlib import Path
import json
import subprocess
import tempfile
import zipfile

ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'build'/'revision-audit'
EXCLUDED={'.git','.agents','.codex','.aws','build','__pycache__','out'}
GENERATED={'lex.yy.c','tinyc2.tab.c','tinyc2.tab.h','tinyc3.tab.c','tinyc3.tab.h',
           'tinyc1','tinyc2','tinyc3','ir_simulator','x86_64_backend','vectorizer_demo'}


def main():
    OUT.mkdir(parents=True,exist_ok=True)
    archive=OUT/'source-without-execute-bits.zip'
    with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
        for p in ROOT.rglob('*'):
            rel=p.relative_to(ROOT)
            if not p.is_file() or any(x in EXCLUDED for x in rel.parts):continue
            if p.name in GENERATED or p.suffix in {'.o','.so','.pyc'}:continue
            info=zipfile.ZipInfo(str(rel));info.external_attr=0o100644<<16
            z.writestr(info,p.read_bytes(),compress_type=zipfile.ZIP_DEFLATED)
    with tempfile.TemporaryDirectory(prefix='tinyc ZIP with spaces ') as temp:
        dest=Path(temp)
        with zipfile.ZipFile(archive) as z:z.extractall(dest)
        for p in dest.rglob('*'):
            if p.is_file():p.chmod(0o644)
        commands=[['make','build'],['make','test','test-tinyc1','test-tinyc2','test-p3-suite','test-btp-scalar']]
        with (OUT/'source-archive.log').open('w') as log:
            for cmd in commands:
                log.write('COMMAND: '+' '.join(cmd)+'\n');log.flush()
                subprocess.run(cmd,cwd=dest,stdout=log,stderr=subprocess.STDOUT,check=True)
        assert not (dest/'scripts/run_smoke_tests.sh').stat().st_mode & 0o111
    print('PASS: source-only ZIP, files mode 0644, path with spaces, Flex/Bison rebuild and original regression targets.')

if __name__=='__main__':main()
