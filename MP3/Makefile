GCC=gcc
COPTS=-g3

all: mp3_sender mp3_receiver

.PHONY: clean

receiver: receiver.c  
	${GCC} ${COPTS} -L. -c -o receiver.o receiver.c
	
sender: sender.c  
	${GCC} ${COPTS} -L. -c -o sender.o sender.c
	
mp3_sender: mp3_sender.c sender
	${GCC} ${COPTS} -L. -lmp3channel -lpthread -lm -o mp3_sender sender.o mp3_sender.c
	
mp3_receiver: mp3_receiver.c receiver	
	${GCC} ${COPTS} -L. -lmp3channel -lpthread -lm -o mp3_receiver receiver.o mp3_receiver.c
	
clean:
	rm *.o mp3_receiver mp3_sender
