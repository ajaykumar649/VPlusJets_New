foreach mH ( 170 180 190 200 250 300 350 400 450 500 550 600 )
    echo "Higgs mass: $mH muon shapes"
    python run1D2FitsShapeFits.py --mH $mH >&! HWW"$mH"_mu_shapes.txt
    echo "Higgs mass: $mH muon fit"
    python runA1D2FitsFit.py --mH $mH >&! HWW"$mH"_mu_fit.txt
end
