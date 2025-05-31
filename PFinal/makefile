# Salvador Pardo Saiz 51253936-T
OPTS = -Wall -Wextra -O3 -std=c++0x

all: maze

maze: maze_bb.cc
	g++ ${OPTS} -o maze_bb maze_bb.cc	 

tar: maze_bt.cc makefile
	tar cvfz 51253936T.tar.gz maze_bb.cc memoria.pdf makefile

clean: 
	rm -f maze *~
 
