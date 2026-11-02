#!/usr/bin/env python3
from pathlib import Path
import re, sys

root = Path(__file__).resolve().parents[1]
errors = []

for rel in ['tinyc_part2/tinyc2.y', 'tinyc_part3/tinyc3.y']:
    p = root / rel
    s = p.read_text()
    # Basic semantic type checks: every %type name must be a known union member.
    union = re.search(r'%union\s*\{(.*?)\n\}', s, re.S)
    if not union:
        errors.append(f'{rel}: missing %union')
        continue
    members = set(re.findall(r'(?:char\s*\*|std::size_t|ASTNode\s*\*|tinyc::BaseKind|tinyc3::[A-Za-z_][A-Za-z0-9_]*\s*\*|std::vector<[^;]+>\s*\*)\s*(\w+)\s*;', union.group(1)))
    for typeline in re.findall(r'^%type\s+<([^>]+)>\s+(.+)$', s, re.M):
        member, names = typeline
        if member not in members:
            errors.append(f'{rel}: %type <{member}> has no %union member')
    # Duplicate union member names
    fields = re.findall(r'\n\s*(?:[^;]+?)\s+(\w+)\s*;', union.group(1))
    dup = sorted({x for x in fields if fields.count(x) > 1})
    if dup:
        errors.append(f'{rel}: duplicate %union fields: {dup}')

if errors:
    print('\n'.join(errors))
    sys.exit(1)
print('Bison source static checks: PASS')
