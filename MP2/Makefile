CC=/usr/bin/gcc
CC_OPTS=-g3
CC_LIBS=
CC_DEFINES=
CC_INCLUDES=
CC_ARGS=${CC_OPTS} ${CC_LIBS} ${CC_DEFINES} ${CC_INCLUDES}

# clean is not a file
.PHONY=clean

#target "all" depends on all others
all: router test

router: router.c router.h NodeGraph.h NodeGraph.c NetworkUtils.h NetworkUtils.c
	${CC} ${CC_ARGS} -o router NodeGraph.c NetworkUtils.c router.c

test: GraphTests.c NodeGraph.h NodeGraph.c Djikstras.h Djikstras.c heap.h heap.c
	${CC} ${CC_ARGS} -o GraphTests GraphTests.c NodeGraph.c Djikstras.c heap.c

clean:
	rm -f router GraphTests *.o log/*