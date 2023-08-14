CXX = g++ -std=c++14
CXXFLAGS = -Werror=vla -g -Wall -MMD
OBJECTS = main.o board.o creature.o player.o dwarf.o elves.o human.o orc.o combat.o gold.o barriersuit.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = cc3k

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

clean :
	rm ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS} # reads the .d files and reruns dependencies
