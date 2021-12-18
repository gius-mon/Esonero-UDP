/*
 * service.c
 *
 *  Created on: 23 nov 2021
 *      Author: giuse
 */

#include <stdio.h>
#include <winsock.h>

#include "comunication_protocol.h"

void ErrorHandler(char *errorMessage) {
	printf("%s", errorMessage);
	fflush(stdout);
}

void ClearWinSock() {
#if defined WIN32
	WSACleanup();
#endif
}

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

int closeConnection(int socket) {

	closesocket(socket);
	ClearWinSock();
	printf("\n");
	fflush(stdout);

	return 0;

}

void set_ip_port(char *ip_address, int *port_number) {

	char choice = 'Y';
	char server_information[ECHOMAX] = { };
	char host_name[ECHOMAX]= { };
	char port_numer_s[ECHOMAX] = { };


	printf(" Welcome!\n\n"); //prints welcome message

	printf(" Do you want to enter the port number and the host name? Y/N \n");
	fflush(stdout);
	printf(" -> ");
	fflush(stdout);
	scanf("%c", &choice);

	while (choice != 'y' && choice != 'Y' && choice != 'N' && choice != 'n') {


		fflush(stdin);

		printf(
				"Error !Enter Y if you want to enter the port number and the host name, enter N otherwise \n");
		scanf("%c", &choice);
		fflush(stdout);
	}

	/*switch case of persons choice*/

	if (choice == 'y' || choice == 'Y') {

		//srv.di.uniba.it:56700
		struct hostent* host;
		int flag=0;
		do
		{



			if(flag == 1 ){
				printf("\n **Wrong port number** \n");
				fflush(stdout);
			}

			printf(
					"\n Please insert host name and port in this format " "srv.di.uniba.it:56700" "\n");
			fflush(stdout);
			printf(" -> ");
			fflush(stdout);
			scanf("%s", server_information);



			int i = 0, j = 0, k = 0;

			while (server_information[i] != ':' && i<ECHOMAX ) {
				host_name[k] =server_information[i];
				i++;
				k++;
			}
			i++;
			while (server_information[i] != ' ' &&  i< ECHOMAX) {
				port_numer_s[j] = server_information[i];
				i++;
				j++;
			}

			*port_number = atoi (port_numer_s);

			if( *port_number < 0 ||  *port_number > 65535){
				flag = 1;
			}


		} while ( *port_number < 0 ||  *port_number > 65535);

		host =   gethostbyname(host_name);




		fflush(stdout);

		while(host == NULL){

			fprintf(stderr, "\n gethostbyname() failed.\n");

			printf("\n Host name: ");
			fflush(stdout);

			fflush(stdin);
			scanf("%s", host_name);

			host = gethostbyname(host_name);

		}

		struct in_addr* ina = (struct in_addr*) host->h_addr_list[0];

		ip_address = inet_ntoa (*ina);




		printf("ip: %s, nome: %s", inet_ntoa (*ina), host->h_name);
		fflush(stdout);



	}

}
