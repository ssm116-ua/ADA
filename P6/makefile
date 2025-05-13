# Salvador Pardo Saiz 51253936-T
OPTS = -Wall -Wextra -O3 -std=c++0x

all: maze

maze: maze.cc
	g++ ${OPTS} -o maze maze.cc	 

tar: maze.cc makefile
	tar cvfz 51253936T.tar.gz maze.cc makefile

clean: 
	rm -f maze *~
 
