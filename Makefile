TARGET  = SimPHITSRooTracker.exe
CXX     = g++
OBJECT  = SimPHITSRooTracker.o PHITStoRooTracker.o TUserPHITSRooTracker.o
CCLIBS  = 
CCFLAGS = 

ROOTLIBS  = `root-config --evelibs`
ROOTFLAGS = `root-config --cflags`

CCLIBS  += $(ROOTLIBS)
CCFALGS += $(ROOTFLAGS)

.PHONY: all
all: $(TARGET)

$(TARGET):$(OBJECT)
	$(CXX) $(ROOTLIBS) $^ -o $@

%.o:%.cpp
	$(CXX) $(ROOTFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJECT)


