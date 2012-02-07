import DibosonConfig

def theConfig(Nj, mcdir = '', initFile = ''):
    fitterPars = DibosonConfig.theConfig(Nj, mcdir, initFile)
    fitterPars.includeElectrons = False
    fitterPars.includeMuons = True

    mvacutval = 0.005
    if Nj == 3:
        mvacutval = -0.005

    mvacut = 'mva%ijdibosonmu > %0.3f' % (Nj, mvacutval)
    #fitterPars.cuts += '&& (%s) ' % mvacut

    print 'full cuts:',fitterPars.cuts
    
    return fitterPars
