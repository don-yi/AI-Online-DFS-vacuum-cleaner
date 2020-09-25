PRG=gnu.exe
GCC=g++
GCCFLAGS=-O2 -Wall -Wextra -std=c++11 -pedantic

VALGRIND_OPTIONS=-q --leak-check=full
DIFF_OPTIONS=-y --strip-trailing-cr --suppress-common-lines

OBJECTS0=environment.cpp agent.cpp
DRIVER0=driver.cpp

OSTYPE := $(shell uname)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

gcc0:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0) $(OBJECTS0) $(GCCFLAGS)
2:
	echo "running test $@.txt"
	@echo "should run in less than 2000 ms"
	./$(PRG) $@.txt > studentout$@
	echo "There is no fixed expected output for this assignment, below is what you agent generated. DO NOT pay attention to \"failed\" above" >difference$@
	@echo "lines after the next are mismatches with master output -- see out$@"
	diff studentout$@ out$@ --strip-trailing-cr
clean:
	rm -f *.exe *.o  student* difference*
