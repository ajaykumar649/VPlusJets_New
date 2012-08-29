def createTree(fnames, altTrue = {}, cut = ''):
    from array import array
    from ROOT import TTree
    cols = {}
    data = None
    line = 0
    for fname in fnames:
        # print 'opening',fname
        fin = open(fname, 'r')
        for currLine in fin:
            var = False
            err = False
            pull = False
            gen = False
            varName = 'nll'
            toks = currLine.split()
            doFill = True
            for key in cols:
                cols[key][0] = -9999.99
            for tok in toks:
                #print cols
                try:
                    #print varName,tok,var,err,pull
                    float(tok)
                    if (not var):
                        if (not (varName in cols)):
                            cols[varName] = array('d', [0.0])
                        cols[varName][0] = float(tok)
                        var = True
                    elif (not err):
                        if (not (varName + '_err' in cols)):
                            cols[varName + '_err'] = array('d', [0.0])
                        cols[varName + '_err'][0] = abs(float(tok))
                        err = True
                        ## if (float(tok) > 10000):
                        ##     doFill = False
                        ##     print 'bad error',varName + '_err',
                    elif (not pull):
                        #val = cols[varName][0]
                        #errval = cols[varName + '_err'][0]
                        if (not (varName + '_true' in cols)):
                            cols[varName + '_true'] = array('d', [0.0])
                        tval = float(tok)
                        if (varName in altTrue):
                            tval = altTrue[varName]
                        cols[varName + '_true'][0] = tval
                        pull = True
                        ## if abs((val - tval)/errval) > 1000:
                        ##     doFill = False
                    elif (not gen):
                        if (not (varName + '_gen' in cols)):
                            cols[varName + '_gen'] = array('d', [0.0])
                        cols[varName + '_gen'][0] = float(tok)
                except ValueError:
                    varName = tok.replace('-', '__').replace('.','_')
                    var = False
                    err = False
                    pull = False
            if (data == None):
                data = TTree('data', 'data')
                for key in cols.keys():
                    data.Branch(key, cols[key], key + '/D')

                #data.Print()
            for key in cols:
                if cols[key][0] == -9999.99:
                    print 'unfilled column',key,
                    doFill = False
            if (doFill):
                data.Fill()
            else:
                print 'rejecting event',line,':',cols
            line += 1
        # print 'completed',fname

    #data.Print()
    if cut and (len(cut) > 0):
        return data.CopyTree(cut)
    else:
        return data
