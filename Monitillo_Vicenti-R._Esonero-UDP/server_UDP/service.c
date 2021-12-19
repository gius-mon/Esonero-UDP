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

void errorhandler(char *errorMessage) {
	printf("%s", errorMessage);
	fflush(stdout);
}
void clearwinsock() {
#if defined WIN32
	WSACleanup();
#endif
}

float division(int a, int b) {
	return (float) a / b;
}

float add(int a, int b) {
	return (float) a + b;
}

float mult(int a, int b) {
	return (float) a * b;
}

float sub(int a, int b) {
	return (float) a - b;
}

//recognize an split the string in parts that will be calculated

int parser(char buf[ECHOMAX], char *operator, int *first_operand,
		int *second_operand) {

	int error = 0;
	int count, count_temp = 0;
	char temp[ECHOMAX] = { };
	char temp2[ECHOMAX] = { };

	if (buf[0] == '+' || buf[0] == '/' || buf[0] == '-' || buf[0] == '*'
			|| buf[0] == 'x') {

		*operator = buf[0];

		if (buf[1] != ' ' || buf[2] == ' ') { //verify that the second character is a space and the third is not a space
			error = 1;
		} else {

			count = 2;
			while (buf[count] != ' ') {

				temp[count_temp] = buf[count];
				count++;
				count_temp++;
			}

			*first_operand = atoi(temp);

			count_temp = 0;
			count++;

			if (buf[count] == ' ' || buf[count - 1] != ' ') { //verify that the character after the first number is a space and the next is not a space
				error = 1;
			} else {

				while (buf[count] != '\0') {
					temp2[count_temp] = buf[count];
					count++;
					count_temp++;

				}

				*second_operand = atoi(temp2);

			}
		}

	} else {

		error = 1;
	}

	return error;
}


//do the operation between firs and second and modify the op_result by adress
//return if the operation was successful

int operation(char operator, int first, int second, float *op_result) {

	int status = 0;

	if (operator == '/' && second == 0) {

		status = -1;
		*op_result = 0;

	} else {

		switch (operator) {

		case '+':
			*op_result = add(first, second);
			break;

		case '-':
			*op_result = sub(first, second);
			break;

		case '*':
		case 'x':
			*op_result = mult(first, second);
			break;

		case '/':
			*op_result = division(first, second);

			break;

		}
	}

	return status;
}

boolean isInteger(double val) {
	int truncated = (int) val;
	return (val == truncated);
}

