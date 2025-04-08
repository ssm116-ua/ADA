# Salvador Pardo Saiz 51253936-T
OPTS = -Wall -Wextra -O3 -std=c++0x

all: maze

maze: maze_greedy.cc
	g++ ${OPTS} -o maze_greedy maze_greedy.cc	 

tar: maze_greedy.cc makefile
	tar cvfz 51253936T.tar.gz maze_greedy.cc makefile

clean: 
	rm -f maze *~
 
