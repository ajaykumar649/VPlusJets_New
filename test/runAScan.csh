set mH=$1
set electrons=$2
if ($2) then
    echo "electrons"
    set mu="el"
    set electrons="--electrons "
else
    echo "muons"
    set mu="mu"
    set electrons=""
endif

foreach cut ( 0.5 0.6 0.7 0.8 0.9 )
    echo "Fit shapes $mu Higgs mass: $mH, cut: $cut"
    python run1D2FitsShapeFits.py --mH $mH "$electrons"--mva $cut >&! HWW"$mH"_"$mu"_shapes_mva"$cut".txt
    python fixErfParams.py 1D2FitsParameters/WpJHWW"$mH"*_mWW.txt
    echo "Fit $mu Higgs mass: $mH, cut: $cut"
    python runA1D2FitsFit.py --mH $mH "$electrons"--mva $cut --expLimit 20 >&! HWW"$mH"_"$mu"_fit_mva"$cut".txt
end
