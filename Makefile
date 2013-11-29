VPATH = src obj
vpath %.cpp src
vpath %.h src
vpath %.o obj

CXX=g++
CXXFLAGS=-Wall -O3 -pedantic -Wextra -Werror

srcdir=src
bindir=bin
objdir=obj

objects=$(patsubst $(srcdir)/%.cpp,$(objdir)/%.o,$(wildcard $(addsuffix /*.cpp, $(srcdir))))

all: $(bindir) $(objdir) $(bindir)/arj

$(bindir): 
	mkdir -p $(bindir)
$(objdir):
	mkdir -p $(objdir)

-include Makefile.dep
Makefile.dep: $(srcdir)/*.cpp
	$(CXX) $(CXXFLAGS) -MM  $(srcdir)/*.cpp | sed -e 's/^\(\S.*\)/objs\/\1/' > $@

$(bindir)/arj: $(objects) 
	$(CXX) $(CXXFLAGS) $(objects) -o $@

$(objdir)/%.o: $(srcdir)/%.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@	

test: all  
	./bin/arj -c -i input.txt -o test.a
	./bin/arj -d -i test.a -o output.txt
	diff input.txt output.txt
	rm -f test.a

.PHONY: clean

clean:
	rm -rf bin/
	rm -rf obj/
	rm -rf Makefile.dep
