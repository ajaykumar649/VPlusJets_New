foreach flavor ( "Muon" "Electron" )
    echo "fitting $flavor no btags"
    python runDibosonFitter.py -b -m Diboson"$flavor"Config -j 2 -W -1 >&! log_Diboson_"$flavor".log
    echo "fitting $flavor wit btags"
    python runDibosonFitter.py -b -m Diboson"$flavor"Config -j 2 -W 9 -i WpJWZ"$flavor"Shape.txt --btag >&! log_Diboson_"$flavor"_btag.log
end
