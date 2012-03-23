import DibosonConfig

def theConfig(Nj, mcdir = '', initFile = ''):
    fitterPars = DibosonConfig.theConfig(Nj, mcdir, initFile)
    fitterPars.includeElectrons = True
    fitterPars.includeMuons = False

    mvacutval = -0.1
    if Nj == 3:
        mvacutval = -0.040

    mvacut = 'mva%ijdibosonel > %0.3f' % (Nj, mvacutval)
    fitterPars.cuts += '&& (%s) ' % mvacut

    qglcut = 'qgld_Summer11CHS[1] > %0.2f' % (0.35)
    fitterPars.cuts += '&& (%s) ' % (qglcut)

    print 'full cuts:',fitterPars.cuts
    
    return fitterPars
