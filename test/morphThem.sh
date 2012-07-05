#!/bin/bash

# 172-198 GeV
for nj in 2 3 
do
    for flavor in Electron Muon
    do
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 182 --basisFile H180_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 190
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 184 --basisFile H180_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 190
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 186 --basisFile H190_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 180
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 188 --basisFile H190_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 180

      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 192 --basisFile H190_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 200
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 194 --basisFile H190_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 200
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 196 --basisFile H200_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 190
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 198 --basisFile H200_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 190
    done
done

# 202-222 GeV
for nj in 2 3 
do
    for flavor in Electron Muon
    do
	for (( mH= 202; mH<=222; mH += 2 ))
	do
	    python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH $mH --basisFile H200_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 250
	done
    done
done

# 224-248 GeV
for nj in 2 3 
do
    for flavor in Electron Muon
    do
	for (( mH= 224; mH<=248; mH += 2 ))
	do
	    python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH $mH --basisFile H250_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 200
	done
    done
done

# 252-274 GeV
for nj in 2 3 
do
    for flavor in Electron Muon
    do
	for (( mH= 252; mH<=274; mH += 2 ))
	do
	    python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH $mH --basisFile H250_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 300
	done
    done
done

# 276-295 GeV
for nj in 2 3 
do
    for flavor in Electron Muon
    do
	for (( mH= 276; mH<=290; mH +=2 ))
	do
	    python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH $mH --basisFile H300_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 250
	done
	python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 295 --basisFile H300_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 200
    done
done

# 305-325 GeV
for nj in 2 3 
do
    for flavor in Electron Muon 
    do
	for (( mH = 305; mH <= 325; mH += 5 ))
	do
	    python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH $mH --basisFile H300_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 350
	done
    done
done

# 330-345 GeV
for nj in 2 3 
do
    for flavor in Electron Muon 
    do
	for (( mH = 330; mH <= 345; mH += 5 ))
	do
	    python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH $mH --basisFile H350_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 300
	done
    done
done

# 350-580 GeV
for nj in 2 3 
do
    for flavor in Electron Muon 
    do
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 360 --basisFile H350_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 400
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 370 --basisFile H350_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 400
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 380 --basisFile H400_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 350
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 390 --basisFile H400_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 350
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 420 --basisFile H400_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 450
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 440 --basisFile H450_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 400
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 460 --basisFile H450_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 500
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 480 --basisFile H500_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 450
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 520 --basisFile H500_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 550
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 540 --basisFile H550_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 500
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 560 --basisFile H550_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 600
      python templateMorph.py -j $nj -m HWW"$flavor"sConfig --mH 580 --basisFile H600_"$flavor"_"$nj"Jets_Fit_Shapes.root --mHmorph 550
    done
done
