/*
 * client.c
 *
 *  Created on: 11 nov 2021
 *      Author: giuse
 */

#include "comunication_protocol.h"
#include <stdio.h>
#include <winsock.h>
#include "service.h"

int main(void) {

	char ip_address[] = DEFAULT_CLIENT_IP;
	int port_number = PROTOPORT_CLIENT;

#if defined WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("error at WSASturtup\n");
		system("PAUSE");

		fflush(stdout);
		return -1;
	}
#endif

	set_ip_port(&ip_address, &port_number);

	// SOCKET CREATION
	int Csocket;
	struct sockaddr_in fromAddr;
	int fromSize;
	char operator = "NULL";
	int first_operand = 0;
	int second_operand = 0;

	Csocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (Csocket < 0) {
		ErrorHandler(" socket creation failed.\n");
		system("PAUSE");

		closesocket(Csocket);
		ClearWinSock();
		return -1;
	}

	// BUILD SERVER ADRESS
	struct sockaddr_in echoServAddr;
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = PF_INET;
	echoServAddr.sin_addr.s_addr = inet_addr(ip_address); // server IP
	echoServAddr.sin_port = htons(port_number); // Server port

	while (1 == 1) {

		printf(
				"\n\n Please, enter the operation to execute as follows \n Ex. + 34 7 \n operation entered-> ");
		fflush(stdout);

		char operation[ECHOMAX] = { };

		printf("%s ", operation);
		fflush(stdout);	//
		fflush(stdin);

		gets(operation);

		int size = sizeof(operation);

		if (operation[0] == '=') {
			closeConnection(Csocket);
			printf("\n **program closed** \n");
			printf(" Press a key to continue...");
			getch();

			return (0);

		}

		fflush(stdout);
		// SEND DATA TO THE SERVER
		if (sendto(Csocket, operation, size, 0,
				(struct sockaddr*) &echoServAddr, sizeof(echoServAddr))
				!= size) {

			ErrorHandler(
					"send() sent a different number of bytes than expected \n");
			printf("Press a key to continue...");
			getch();

			closesocket(Csocket);
			ClearWinSock();
			return -1;
		}


		// RECEIVE DATA FROM THE SERVER
		int bytesRcvd = 0;
		char buf[ECHOMAX] = { }; // buffer for data from the server

		fromSize = sizeof(fromAddr);
		if ((bytesRcvd = recvfrom(Csocket, buf, ECHOMAX - 1, 0,
				(struct sockaddr*) &fromAddr, &fromSize)) <= 0) {
			ErrorHandler("recv() failed or connection closed prematurely \n");
			printf(" \nPress a key to continue...");
			getch();

			closesocket(Csocket);
			ClearWinSock();
			return -1;
		}

		parser(operation, &operator, &first_operand, &second_operand);

		struct hostent *serverName;
		serverName = gethostbyaddr((char*) &echoServAddr.sin_addr, 4, AF_INET);
		char *canonical_name = serverName->h_name;

		if(strcmp(buf,"**Math error**")==0 || strcmp(buf,"**Input not valid**")==0) {

			printf("\n Receveid result from server %s, ip %s: %s \n",
					canonical_name, inet_ntoa(echoServAddr.sin_addr), buf);
			fflush(stdout); // Print the echo buffer
			printf(" Press a key to continue...");

		}
		else {
			printf("\n Receveid result from server %s, ip %s: %d %c %d = %s \n",
					canonical_name, inet_ntoa(echoServAddr.sin_addr), first_operand,
					operator, second_operand, buf);
			fflush(stdout); // Print the echo buffer
			printf(" Press a key to continue...");
			getch();
		}
	}

}

