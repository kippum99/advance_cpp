CXXFLAGS = -std=c++17
SOURCES = regex.cpp engine.cpp test_regex.cpp testbase.cpp

EXENAME = test_regex

all: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(EXENAME) $(INCLUDE) $^

clean:
	rm -f *.o $(EXENAME)

.PHONY: all clean
