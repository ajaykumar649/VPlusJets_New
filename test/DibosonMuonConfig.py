import DibosonConfig

def theConfig(Nj, mcdir = '', initFile = '', btag = False):
    fitterPars = DibosonConfig.theConfig(Nj, mcdir, initFile, btag)
    fitterPars.includeElectrons = False
    fitterPars.includeMuons = True

    mvacutval = -0.1
    if Nj == 3:
        mvacutval = -0.005

    #mvacut = 'mva%ijdibnoqgmu > %0.3f' % (Nj, mvacutval)
    #fitterPars.cuts += '&& (%s) ' % (mvacut)

    #qglcut = 'qgld_Summer11CHS[1] > %0.2f' % (0.35)
    #fitterPars.cuts += '&& (%s) ' % (qglcut)

    print 'full cuts:',fitterPars.cuts
    
    return fitterPars
