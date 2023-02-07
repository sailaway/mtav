CC=gcc
CPP=g++
#CFLAGS+=-g
#CFLAGS+='-D DEBUG=1'
CFLAGS+=`pkg-config --cflags opencv`
CFLAGS+='-std=c++11'
CFLAGS+='-O3'
CFLAGS+='-D RELEASE_BUILD=1'
LDFLAGS+=`pkg-config --libs opencv`

.PHONY: all clean

PROG=mtav
OBJS= src/main.o \
	  src/Commands.o \
	  src/jsonxx/jsonxx.o \
	  src/utils/cvutil.o \
	  src/utils/planeTransform.o \
	  src/utils/ImageProcess.o \
	  src/utils/strutil.o \
	  src/utils/algutils.o \
	  src/commands/MutilTrackerGen.o \
	  src/multitracker/MultiTracker.o\
	  src/multitracker/MultiTrackerVideoGen.o\
	  src/multitracker/trakers/SimpleImage.o\
	  src/multitracker/trakers/SimpleVideo.o
	  
$(PROG): $(OBJS)
	$(CPP) -o $(PROG) $(OBJS) $(LDFLAGS)

%.o: %.cpp $(DEPS)
	$(CPP) -c $(CFLAGS) $< -o $@

install : $(PROG)
	cp -f $(PROG) ./bin/

all: $(PROG)

clean:
	rm -f $(OBJS) $(PROG)
