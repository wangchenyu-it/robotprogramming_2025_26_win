CXX=g++
CXX_FLAGS=-O3 -Wall

OBJS=dmap_simple.o

.phony:	clean all

all:	dmap_simple_test

clean:
	rm *~ *.o dmap_simple_test

dmap_simple.o:	dmap_simple.cpp dmap_simple.h dmap_simple_impl.cpp grid_.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

dmap_simple_test: dmap_simple.o dmap_simple_test.cpp
	$(CXX) $(CXX_FLAGS) -o $@ $^
