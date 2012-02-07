import DibosonConfig

def theConfig(Nj, mcdir = '', initFile = ''):
    fitterPars = DibosonConfig.theConfig(Nj, mcdir, initFile)
    fitterPars.includeElectrons = True
    fitterPars.includeMuons = False

    mvacutval = 0.0
    if Nj == 3:
        mvacutval = -0.040

    mvacut = 'mva%ijdibosonel > %0.3f' % (Nj, mvacutval)
    fitterPars.cuts += '&& (%s) ' % mvacut

    print 'full cuts:',fitterPars.cuts
    
    return fitterPars
