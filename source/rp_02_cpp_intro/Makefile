CXX=g++
CXXFLAGS=-std=c++17 -Wall
BINS=01_references 02_structs_with_methods 03_constructors 04_vec_f

.phony:	clean all

all:	$(BINS)

%: 	%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf *~ *.o $(BINS)
