# Salvador Pardo Saiz 51253936-T
OPTS = -Wall -Wextra -O3 -std=c++0x

all: quickSort quickSort.CPUtimes graphs

graphs: quickSort.CPUtimes quickSort.gpi
	gnuplot quickSort.gpi

quickSort: quickSort.cc
	g++ ${OPTS} -o quickSort quickSort.cc	 

quickSort.CPUtimes: quickSort
	./quickSort > quickSort.CPUtimes

tar: quickSort.cc quickSort.gpi makefile
	tar cvfz 51253936T.tar.gz quickSort.cc quickSort.gpi makefile

clean: 
	rm -f quickSort quickSort.CPUtimes fit.log quickSort*.png *~
 
