set syst=0
#echo "number of args: $#argv"
if ($#argv > 0) then 
    set syst=$1
endif

echo "systematic $syst"

#exit 0

foreach flavor ( "Electrons" )
    foreach mH ( 350 400 450 500 550 600 )
    	echo "Higgs mass: $mH $flavor 2 jets"
    	python runHWWFitter.py -b -m HWW"$flavor"Config -j 2 -H $mH -W 0 -s $syst --lumi 11900 >&! H"$mH"_"$flavor"_2jets_FitterOutput.out
    end

    foreach mH ( 170 180 190 200 250 300 )
	echo "Higgs mass: $mH $flavor 2 jets"
	python runHWWFitter.py -b -m HWW"$flavor"Config -j 2 -H $mH -W 0 -i WpJHWW"$mH"Parameters.txt -s $syst --lumi 11900 >&! H"$mH"_"$flavor"_2jets_FitterOutput.out
    end
end

# echo "non Poisson fractional errors"
# grep -i 'Poisson' H???_*_?jets_*out | sort | awk -F '[_ =/]+' -f formatErrs.awk
