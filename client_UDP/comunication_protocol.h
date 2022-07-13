/*
 * comunication_protocol.h
 *
 *  Created on: 11 nov 2021
 *      Author: giuse
 */

#ifndef COMUNICATION_PROTOCOL_H_
#define COMUNICATION_PROTOCOL_H_

#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#define PROTOPORT_SERVER 50000 // default protocol port number
#define PROTOPORT_CLIENT 50000 // default protocol port number
#define QLEN 6 // size of request queue
#define ECHOMAX 255
#define DEFAULT_SERVER_IP "127.0.0.1"
#define DEFAULT_CLIENT_IP "127.0.0.1"


#endif /* COMUNICATION_PROTOCOL_H_ */
