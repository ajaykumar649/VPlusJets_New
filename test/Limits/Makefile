CXXFLAGS := `root-config --cflags` -O2 -g -Wall -DMAIN

LDFLAGS := `root-config --libs`

%.o: %.cc
	${CXX} -c $*.cc -o $@ ${CXXFLAGS} 

%.o: %.C
	${CXX} -c $*.C -o $@ ${CXXFLAGS} 

all : exe

exe: makeDataCardSingleBin.exe makeDataCardShapes.exe makeDataCardFromTable.exe makeDataCardShapes4tgc.exe

makeDataCardSingleBin.exe: makeDataCardSingleBin.o systematics.o
	${CXX} -g -o $@ $^  ${LDFLAGS}

makeDataCardShapes.exe: makeDataCardShapes.o
	${CXX} -g -o $@ $^  ${LDFLAGS}

makeDataCardShapes4tgc.exe: makeDataCardShapes4tgc.o
	${CXX} -g -o $@ $^  ${LDFLAGS}

makeDataCardFromTable.exe: makeDataCardFromTable.o
	${CXX} -g -o $@ $^  ${LDFLAGS}

makeDataCardShapes.o: makeDataCardShapes.cc hwwshapes.C dcutils.C hwwinputs.h mjjinputs.h hwwutils.C

makeDataCardShapes4tgc.o: makeDataCardShapes4tgc.cc dcutils.C atgcinputs.h

makeDataCardSingleBin.o: systematics.h makeDataCardSingleBin.hh

systematics.o: systematics.h
