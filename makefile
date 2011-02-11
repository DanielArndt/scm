CC = g++

INCLUDE = misc.hpp learner.hpp team.hpp point.hpp env.hpp scm.hpp

FLAGS =  -g -O3 -c -Wall -Wextra -Wredundant-decls -Wno-sign-compare
#FLAGS =  -g -c -Wall -Wextra -Wredundant-decls -Wno-sign-compare -DMYDEBUG
#FLAGS =  -g -c -Wall -Wextra -Wredundant-decls -Wno-sign-compare
#FLAGS =  -g -O -c -Wall -Wextra -Wredundant-decls -Wno-sign-compare
#FLAGS =  -g -O3 -c -Wall -Wextra -Wredundant-decls -Wno-sign-compare -DMYDEBUG
#FLAGS =  -g -O3 -c -Wall -Wextra -Wredundant-decls -Wno-sign-compare -DSHOWEXEC
#FLAGS =  -g -pg -c -Wall -Wextra -Wredundant-decls -Wno-sign-compare
#FLAGS =  -g -pg -O -c -Wall -Wextra -Wredundant-decls -Wno-sign-compare

EXECS = scm rerun

all : $(EXECS)

scm : main.o misc.o learner.o team.o point.o env.o scm.o
	$(CC) -o scm main.o misc.o learner.o team.o point.o env.o scm.o
#	$(CC) -pg -o scm main.o misc.o learner.o team.o point.o env.o scm.o

rerun : rerun.o misc.o learner.o team.o point.o
	$(CC) -o rerun rerun.o misc.o learner.o team.o point.o

rerun.o : rerun.cpp misc.hpp learner.hpp team.hpp point.hpp
	$(CC) $(FLAGS) rerun.cpp

misc.o : misc.cpp $(INCLUDE)
	$(CC) $(FLAGS) misc.cpp

main.o : main.cpp $(INCLUDE)
	$(CC) $(FLAGS) main.cpp

env.o : env.cpp $(INCLUDE)
	$(CC) $(FLAGS) env.cpp

learner.o : learner.cpp $(INCLUDE)
	$(CC) $(FLAGS) learner.cpp

point.o : point.cpp $(INCLUDE)
	$(CC) $(FLAGS) point.cpp

scm.o : scm.cpp $(INCLUDE)
	$(CC) $(FLAGS) scm.cpp

team.o : team.cpp $(INCLUDE)
	$(CC) $(FLAGS) team.cpp

clean :
	$ rm *.o *~ scm
