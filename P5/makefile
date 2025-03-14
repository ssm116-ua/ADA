# Salvador Pardo Saiz 51253936-T
OPTS = -Wall -Wextra -O3 -std=c++0x

all: pow2 pow2.steps graphs

graphs: pow2.steps pow2.gpi
	gnuplot pow2.gpi

pow2: pow2.cc
	g++ ${OPTS} -o pow2 pow2.cc	 

pow2.steps: pow2
	./pow2 > pow2.steps

tar: pow2.cc pow2.gpi makefile
	tar cvfz 51253936T.tar.gz pow2.cc pow2.gpi makefile

clean: 
	rm -f pow2 pow2.steps fit*.log pow2*.png *~
 
