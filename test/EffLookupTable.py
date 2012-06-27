class EffLookupTable:
    def __init__(self, fname):
        self.filename = fname     
        self.EffTable = []

        for line in open(fname).readlines():
            #print line,
            fields = line.rstrip().split()
            #print fields
            row = {'ptMin' : eval(fields[0]), 'ptMax' : eval(fields[1]),
                   'eff' : eval(fields[-2]), 'effErr' : eval(fields[-1]),
                   #'etaMin' : -999., 'etaMax' : 999.
                   }
            if len(fields) > 4:
                row['etaMin'] = eval(fields[2])
                row['etaMax'] = eval(fields[3])
            #print row
            self.EffTable.append(row)

    def findBand(self, pt, eta):
        for row in self.EffTable:
            #print row
            if (row['ptMin'] <= pt) and (pt < row['ptMax']):
                if (len(row) > 4):
                    if (row['etaMin'] <= eta) and (eta < row['etaMax']):
                        return row
                else:
                    return row
        return None

    def GetEfficiency(self, pt, eta):
        theBand = self.findBand(pt, eta)
        if theBand and ('eff' in theBand):
            return theBand['eff']

    def GetError(self, pt, eta):
        theBand = self.findBand(pt, eta)
        if theBand and ('effErr' in theBand):
            return theBand['effErr']

    def __str__(self):
        printout = 'table from filename: %s' % (self.filename)
        for row in self.EffTable:
            printout += '\n%0.2f\t%0.2f' % (row['ptMin'],row['ptMax'])
            if len(row) > 4:
                printout += '\t%0.2f\t%0.2f' % (row['etaMin'],row['etaMax'])
                
            printout += '\t%0.4f\t%0.4f' % (row['eff'],row['effErr'])
        return printout + '\n'

            
if __name__ == '__main__':
    table1 = EffLookupTable('EffTableDir/eleEffsHLTEle2jPfMht_data_LWA_PfMht.txt')
    print 'eff of met=40:',table1.GetEfficiency(40, 0)
    print table1

    table2 = EffLookupTable('EffTable2012/scaleFactor-2012A-PromptReco-v1-PFMM-RecoToIso.txt')

    print 'eff of pt=40, eta = -0.5:', table2.GetEfficiency(40, -0.5)
    print table2
