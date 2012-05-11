foreach flavor ( "Muons" "Electrons" )
    foreach NJ ( 2 3 )
	foreach MUSUsystopt ( 0 1 -1 2 -2 )
	    echo "fitting $flavor, $NJ Jets, MUSUsystopt=$MUSUsystopt"
		python runMjjFitter.py -j "$NJ" -m Mjj"$flavor"Config -i Mjj"$flavor""$NJ"Jets.txt --TTbarMUSUsystopt "$MUSUsystopt" >&! logMjjTTbarMUSUErrStudies_"$flavor""$NJ"Jets_MUSUsystopt"$MUSUsystopt".log

	end
    end
end
