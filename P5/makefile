# Salvador Pardo Saiz 51253936-T
OPTS = -Wall -Wextra -O3 -std=c++0x

all: pow2

graphs: qs-vs-hs.Msteps qs-vs-hs.gpi
	gnuplot qs-vs-hs.gpi

pow2: pow2.cc
	g++ ${OPTS} -o pow2 pow2.cc	 



tar: qs-vs-hs.cc qs-vs-hs.gpi makefile
	tar cvfz 51253936T.tar.gz qs-vs-hs.cc qs-vs-hs.gpi makefile

clean: 
	rm -f qs-vs-hs qs-vs-hs.Msteps fit*.log quickSort*.png heapSort*.png qs-vs-hs-RA*.png qs-vs-hs-SA*.png heapsort*.png qs-vs-hs-RSA*.png *~
 
