foreach mH ( 170 180 190 200 250 300 350 400 450 500 550 600 )
    # echo "Higgs mass: $mH electron shapes"
    # python run1D2FitsShapeFits.py --mH $mH --electrons >&! HWW"$mH"_el_shapes.txt
    echo "Higgs mass: $mH electron fit"
    python runA1D2FitsFit.py --mH $mH --electrons >&! HWW"$mH"_el_fit.txt
end
