CC=/usr/bin/gcc
CC_OPTS=-g3
CC_LIBS=-pthread
CC_DEFINES=
CC_INCLUDES=
CC_ARGS=${CC_OPTS} ${CC_LIBS} ${CC_DEFINES} ${CC_INCLUDES}

# clean is not a file
.PHONY=clean

#target "all" depends on all others
all: router router1 router2 test


router: router.c router.h NodeGraph.h NodeGraph.c NetworkUtils.h NetworkUtils.c
	${CC} ${CC_ARGS} -o router NodeGraph.c NetworkUtils.c router.c

router1: router1.c router1.h NodeGraph.h NodeGraph.c NetworkUtils.h NetworkUtils.c Djikstras.h Djikstras.c
	${CC} ${CC_ARGS} -o router1 NodeGraph.c NetworkUtils.c router1.c Djikstras.c

router2: router2.c router2.h NodeGraph.h NodeGraph.c NetworkUtils.h NetworkUtils.c Djikstras.h Djikstras.c  
	${CC} ${CC_ARGS} -o router2 NodeGraph.c NetworkUtils.c router2.c Djikstras.c 

test: GraphTests.c NodeGraph.h NodeGraph.c Djikstras.h Djikstras.c
	${CC} ${CC_ARGS} -o GraphTests GraphTests.c NodeGraph.c Djikstras.c

clean:
	rm -r -f router router1 router2 GraphTests *.o log/* *.dSYM/
