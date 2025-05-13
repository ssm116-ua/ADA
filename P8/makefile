# Salvador Pardo Saiz 51253936-T
OPTS = -Wall -Wextra -O3 -std=c++0x

all: maze

maze: maze_bt.cc
	g++ ${OPTS} -o maze_bt maze_bt.cc	 

tar: maze_bt.cc makefile
	tar cvfz 51253936T.tar.gz maze_bt.cc makefile

clean: 
	rm -f maze *~
 
