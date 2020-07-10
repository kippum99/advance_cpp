CXXFLAGS = -std=c++17
SOURCES = mbrot.cpp bbrot.cpp

EXENAME = bbrot

all: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(EXENAME) $(INCLUDE) $^

clean:
	rm -f *.o $(EXENAME)

.PHONY: all clean
