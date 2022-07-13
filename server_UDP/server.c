/*
 * server.c
 *
 *  Created on: 11 nov 2021
 *      Author: giuse
 */
#include "service.h"

int main() {

	char operator;
	int first_operand = 0;
	int second_operand = 0;
	char sendString[ECHOMAX] = { };
	float op_result = 0;

	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	int cliAddrLen;
	char echoBuffer[ECHOMAX];
	int recvMsgSize;

	int port = PROTOPORT_SERVER;

#if defined WIN32 // initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0) {
		errorhandler("Error at WSAStartup()\n");
		return 0;
	}
#endif

	// SOCKET CREATION

	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		errorhandler("socket creation failed.\n");
		clearwinsock();
		return -1;
	}
	// ASSIGNING A SOCKET ADDRESS
	memset(&echoServAddr, 0, sizeof(echoServAddr)); // ensures that extra bytes contain 0
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = inet_addr(DEFAULT_SERVER_IP);
	echoServAddr.sin_port = htons(port); /* converts values between the host and
	 network byte order. Specifically, htons() converts 16-bit quantities
	 from host byte order to network byte order. */

	if (bind(sock, (struct sockaddr*) &echoServAddr, sizeof(echoServAddr))
			< 0) {
		errorhandler("bind() failed.\n");

	}

	printf(" Server started... \n\n");
	fflush(stdout);

	while (1) {

		int totalBytesRcvd = 0;

		cliAddrLen = sizeof(echoClntAddr);

		recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0,
				(struct sockaddr*) &echoClntAddr, &cliAddrLen);


		if (recvMsgSize > 0) { //this loop continues until the client close connection and recv fail

			totalBytesRcvd += recvMsgSize;

			struct hostent * clName;
			clName= gethostbyaddr( (char *) &echoClntAddr.sin_addr, 4, AF_INET);
			char* canonical_name = clName->h_name;

			printf("\n  Required operation %s  from client: %s,  ip: %s \n", echoBuffer, canonical_name, inet_ntoa(echoClntAddr.sin_addr ));

			fflush(stdout); // Setup to print the echoed string

			operator = 0;
			first_operand = 0;
			second_operand = 0;

			int risp = parser(echoBuffer, &operator, &first_operand,
					&second_operand); //allows to check if the input is correct and splits the string into the various parts

			echoBuffer[0] = '\0';

			if (risp == 0) {

				int status = 0;

				status = operation(operator, first_operand, second_operand, //do the operation between the two operand, and the result allows to check if the operation is possible
						&op_result);

				if (status == 0) {

					if (isInteger(op_result)) { // IF THE RESULT IS AN INTEGER DO CASTING
						itoa(op_result, sendString, 10);

					} else

					{
						gcvt(op_result, 10, sendString); //convert floats number in string
					}

				}

				if (status == -1) {

					strcpy(sendString,"**Math error**");

				}

			} else {
				strcpy(sendString, "**Input not valid**");
			}

			int size = strlen(sendString);

			fflush(stdin);

			if (sendto(sock, sendString, size, 0,
					(struct sockaddr*) &echoClntAddr, sizeof(echoClntAddr))
					!= size) {  // send the result of the operations as string

				errorhandler(
						"send() sent a different number of bytes than expected \n");
				closesocket(sock);
				clearwinsock();
				return -1;
			}

		}

	} // end-while

} // end-main

