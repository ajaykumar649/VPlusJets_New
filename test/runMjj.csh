foreach mode ( Electrons Muons )
    foreach jets ( 2 3 )
	echo "running $mode $jets jets"
	python runMjjFitter.py -b -j $jets -m Mjj"$mode"Config --NP --Err 0 >&! Mjj_"$mode"_"$jets"jets_output.out
	cp lastWjjFitParams.txt Mjj_"$mode"_"$jets"jets_FitParams.txt
	python makeNewPhysicsShapes.py -b -j $jets -m Mjj"$mode"Config >&! Mjj_"$mode"_"$jets"jets_NewPhysics.out
    end
end

grep 'acceptance' *NewPhysics.out | awk -F '[_: ]+' '{print $2,$3,$5,$NF}' | sort
