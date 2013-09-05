from optparse import OptionParser
import re

parser = OptionParser()

parser.add_option('-p', action='append', dest='parameters')
parser.add_option('-a', action='store_true', dest='fixAll', default=False)

(opts, args) = parser.parse_args()

fixed = ['offset', 'width']
if opts.parameters and (len(opts.parameters) > 0):
    fixed = opts.parameters

for fname in args:
    print fname
    lines = open(fname).readlines()
    # outf = open(fname.replace('.txt', '_new.txt'), 'w')
    outf = open(fname, 'w')
    for line in lines:
        print line.rstrip(), '=>',
        for fixer in fixed:
            if (line.startswith(fixer)) or opts.fixAll:
                line = re.sub(r'( C)* L\(', r' C L(', line)
        print line,
        outf.write(line)

    outf.close()
