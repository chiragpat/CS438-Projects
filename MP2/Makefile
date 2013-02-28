CC=/usr/bin/gcc
CC_OPTS=-g3
CC_LIBS=
CC_DEFINES=
CC_INCLUDES=
CC_ARGS=${CC_OPTS} ${CC_LIBS} ${CC_DEFINES} ${CC_INCLUDES}

# clean is not a file
.PHONY=clean

#target "all" depends on all others
all: router

router: router.c router.h NodeList.h NodeList.c NetworkUtils.h NetworkUtils.c
	${CC} ${CC_ARGS} -o router NodeList.c NetworkUtils.c router.c

clean:
	rm -f router *.o