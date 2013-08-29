from optparse import OptionParser

parser = OptionParser()

parser.add_option('-p', action='append', dest='parameters')
parser.add_option('-a', action='store_true', dest='fixAll', default=False)

(opts, args) = parser.parse_args()

fixed = ['offset', 'width']
if len(opts.parameters) > 0:
    fixed = opts.parameters

for fname in args:
    print fname
    lines = open(fname).readlines()
    # outf = open(fname.replace('.txt', '_new.txt'), 'w')
    outf = open(fname, 'w')
    for line in lines:
        for fixer in fixed:
            if (line.startswith(fixer)) or opts.fixAll:
                line = line.replace('L(', 'C L(')
        #print line,
        outf.write(line)

    outf.close()
