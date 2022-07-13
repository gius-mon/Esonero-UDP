/*
 * service.h
 *
 *  Created on: 23 nov 2021
 *      Author: giuse
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h> // for atoi()
#include <string.h>
#include <math.h>

#include "comunication_protocol.h"
void errorhandler(char *errorMessage);

void clearwinsock();

float division(int a, int b);

float add(int a, int b);

float mult(int a, int b);

float sub(int a, int b);

int parser(char buf[ECHOMAX], char *operator, int *first_operand, int *second_operand);

int operation(char operator, int first, int second, float *op_result);

boolean isInteger(double val);


#endif /* SERVICE_H_ */
