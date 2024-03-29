/*
 * mp3channel.h
 *
 * UIUC - CS438 MP3 - Channel emulation library
 *
 *  Created on: Feb 23, 2013
 *      Author: Riccardo Crepaldi
 */
#include <sys/socket.h>
#include <sys/types.h>

#ifndef MP3CHANNEL_H_
#define MP3CHANNEL_H_

#define MAX_PKTSIZE 200


/*
 * This function works exactely like sendto, with the difference that
 * packets might be garbled, lost and delayed according to the values
 * specified at the beginning of the simulations
 *
 * returns the number of bytes actually sent, which is capped at 200
 */
ssize_t mp3_sendto(int sockfd, void *buff, size_t nbytes, int flags,
		   const struct sockaddr *to, socklen_t addrlen);


/*
 * This function works exactely like close, but purges the queue before
 * closing the socket. Use this function only for the socket, not for files
 */
int mp3_close(int);


/*
 * This function MUST be called before anything else
 */
void mp3_init();

/*
 * This function can be used to set the channel parameters:
 *
 * _congestion: 
 * 0 for no congestion, 1 for generating random packets 
 *
 * _qsize :
 * the maximum number of packets that can be scheduled to be sent. If more
 * packets are queued when the queue is full, those packets will be dropped
 *
 * _tx_time :
 * packet propagation time expressed in milliseconds
 *
 * _droprate :
 * this value (between 0.0 and 1.0)
 * indicates the probability with which scheduled packets are dropped
 *
 * _garblerate :
 * this value (between 0.0 and 1.0)
 * indicates the probability with which sent packets (not dropped) will see one
 * byte garbled
 *
 */
void setMP3Params(int _qsize, int congestion, int _tx_time, float _droprate, float _garblerate);


/*
 * Calling this function will print statistics about your program
 * behavior, including packet and byte counts, and total time since the first
 * packet was scheduled.
 */
void printMP3Statistics();

#endif /* MP3CHANNEL_H_ */
