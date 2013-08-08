from ROOT import TFile, TMath

def SetSystematicAlphas(w_mWW):
    params = w_mWW.pdf("ggH").getParameters(w_mWW.set("obsSet"))
    params_up = w_mWW.pdf("ggH_interf_ggHUp").getParameters(w_mWW.set("obsSet"))
    params_down = w_mWW.pdf("ggH_interf_ggHDown").getParameters(w_mWW.set("obsSet"))

    # params.Print("v");
    # params_up.Print("v");
    # params_down.Print("v");

    upParamIter = params_up.createIterator()
    p_up = upParamIter.Next()

    while p_up:
        p_up_name = p_up.GetName();
        p_down_name = p_up_name.replace("ggHUp", "ggHDown")
        p_nom_name = p_up_name.replace("_interf_ggHUp", "")
        nom_val = params.getRealValue(p_nom_name, 0, True)

        diff1 = p_up.getVal() - nom_val
        diff2 = nom_val - params_down.getRealValue(p_down_name, 0, True)

        # we could use the maximum difference or the geometric mean.
        maxDiff = TMath.Max(TMath.Abs(diff1), TMath.Abs(diff2))
        geomMean = TMath.Sqrt(TMath.Abs(diff1)*TMath.Abs(diff2))
        if (geomMean > 0.) and (nom_val != 0):
            fracGeomMean = geomMean/TMath.Abs(nom_val);

            if (diff1 != 0):
                fracLnNMult = 1. + diff1/TMath.Abs(diff1)*fracGeomMean;

            try:
                w_mWW.var("kappa_%s" % p_nom_name).setVal(fracLnNMult)
                print p_nom_name, fracLnNMult
            except ReferenceError as e:
                pass

        p_up = upParamIter.Next()

    params.IsA().Destructor(params)
    params_up.IsA().Destructor(params_up)
    params_down.IsA().Destructor(params_down)

def GetLnNKappas(w_mWW):
    ggHPars = w_mWW.allVars().selectByName("n_ggH*")

    ps = [0., 0., 0.]
    pIter = ggHPars.createIterator()
    p = pIter.Next()

    while p:
        # p.Print()
        if p.GetName().count('ggHDown'):
            ps[0] = p.getVal()
        elif p.GetName().count('ggHUp'):
            ps[2] = p.getVal()
        else:
            ps[1] = p.getVal()
        p = pIter.Next()

    # print ps

    diffDown = (ps[0]-ps[1])/ps[1]
    diffUp = (ps[2]-ps[1])/ps[1]

    return (diffDown, diffUp)

if __name__ == "__main__":
    import pyroot_logon

    f = TFile("HWW500lnujj_muon_2jets_1D2Fit_output.root")
    w_mWW = f.Get("w_mWW")

    SetSystematicAlphas(w_mWW)

    # diffDown = (w_mWW.var('n_ggH_mu').getVal() - w_mWW.var('n_ggH_interf_ggHDown_mu').getVal())/w_mWW.var('n_ggH_mu').getVal()
    # diffUp = (w_mWW.var('n_ggH_interf_ggHUp_mu').getVal() - w_mWW.var('n_ggH_mu').getVal())/w_mWW.var('n_ggH_mu').getVal()

    (diffDown, diffUp) = GetLnNKappas(w_mWW)

    
    print
    print 'interf_ggH lnN %.2f/%.2f' % (min(1+diffDown, 1.),max(1., 1+diffUp))
    print
