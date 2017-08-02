
_DEPS= MyHeader.h DataFormats.h alignToolbox.h runManager.h HLobjects.h LinkDef.h
_OBJ =  alignToolbox.o DataFormats.o runManager.o HLobjects.o Dict.o
_PLUGINS = memoryTest.out

IDIR = ./include
LDIR = ./lib
ODIR = ./src
PDIR = ./plugins
BDIR = ./bin

PLUGINS = $(patsubst %,$(BDIR)/%,$(_PLUGINS))


CFLAGS = -I$(IDIR) -I./
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ  = $(patsubst %,$(ODIR)/%,$(_OBJ))
CC = g++
COpt = -Wall -Wextra  `root-config --cflags --libs` -std=gnu++11

DICTC = Dict.cc
DICTH = Dict.h

HDRS_DICT = include/DataFormats.h include/MyHeader.h include/Linkdef.h

all:
	@echo "############# COMPILING ALL #############"
	@echo "###### CINT..."
	@make cint
	@echo "###### DONE !"
	@echo "###### COMPILING DEPENDENCIES..."
	@make  -j4 $(OBJ)
	@echo "###### COMPILING $(PLUGINS)..."
	@make -j4 $(PLUGINS)
	
	 
cint: $(ODIR)/$(DICTC)

$(ODIR)/$(DICTC): $(HDRS_DICT)
	rootcint -f $@ -c $(CFLAGS) $^ 
	@mv src/Dict_rdict.pcm .


%.o: %.cc
	$(CC) $(COpt) -c -o $@ $< $(CFLAGS) $(ROOTLIBS) -L$(ROOTSYS)/lib


bin/%.out: plugins/%.cc $(OBJ)
	$(CC) $(COpt) -o $@ $^ $(CFLAGS) $(ROOTLIBS) -L$(ROOTSYS)/lib

#%.out : %.cc $(OBJ)
#	@echo "###### ------> $@"
#	@echo $^
#	#$(CC) $(COpt) -o $@ $(patsubst %.out,%.cc,$(subst $(BDIR),$(PDIR),./$@)) $^ $(CFLAGS) $(ROOTLIBS) -L$(ROOTSYS)/lib
#	$(CC) $(COpt) -o $@ $^ $(CFLAGS) $(ROOTLIBS) -L$(ROOTSYS)/lib
	

.PHONY: clean

clean:
	rm -rf $(ODIR)/*.o hellomake src/Dict.cc include/Dict.h bin/* Dict_rdict.pcm plugins/*.out
