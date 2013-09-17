set masspts = ( 170 180 190 200 250 300 350 400 450 500 550 600 )
if ($#argv >= 1) then
    set masspts = ($argv[*])
endif

foreach mH ( $masspts )
    echo "Higgs mass: $mH electron shapes"
    python run1D2FitsShapeFits.py --mH $mH --electrons >&! HWW"$mH"_el_shapes.txt
    python fixErfParams.py 1D2FitsParameters/WpJHWW"$mH"*_mWW.txt
    # echo "Higgs mass: $mH electron fit"
    # python runA1D2FitsFit.py --mH $mH --electrons >&! HWW"$mH"_el_fit.txt
end
