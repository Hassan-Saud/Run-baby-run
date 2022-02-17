//0089-bscs-19(gcu lahore)
//hassan saud
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <ctype.h>
#include  <string.h>

#define COMMAND_LENGTH 50
#define MESSAGE_LENGTH 1234

int location(const char* cmd){
	int l1;
	for(int num=0; num<strlen(cmd); num++){
			if(  isspace(cmd[num])  || isblank(cmd[num])) l1=num;
			while(  (isspace(cmd[num])  || isblank(cmd[num]))   && num<(strlen(cmd)-1)){
				num+=1;
				if(  !( isspace(cmd[num]) || isblank(cmd[num]) )  ) l1=-1;
			}
		}
	return l1;
}
int main(int num,  char* argv[]){
	
	int mainSocket=0,  childSocket=0,  mainPort=htons(12345),  returnStatus=0;
	struct sockaddr_in mainAddressStructure;
	struct sockaddr_in childAddressStructure;
	int childNameLength=0;
	mainSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(mainSocket<0) 
		{  printf("\n%s\n","ERROR:: Couldn't initiate SOCKET.");  exit (1);  }
	else
		{  printf("\n%s\n","Socket is initialized properly.");  }
	bzero(&mainAddressStructure,sizeof(mainAddressStructure));
	mainAddressStructure.sin_family=AF_INET;
	mainAddressStructure.sin_addr.s_addr=htonl(INADDR_ANY);
	mainAddressStructure.sin_port=mainPort;
	returnStatus=bind(mainSocket,  (struct sockaddr*) &mainAddressStructure,  sizeof(mainAddressStructure));
	if(returnStatus==0) { 
		printf("\n%s\n","Bind Complete"); }
	else 
		{  printf("\n%s\n","Can't bind properly."); 
			 close (mainSocket); 
			 exit (1);  }
	returnStatus=listen(mainSocket,5);
	if(returnStatus==-1)
		{  printf("\n%s\n","Can't listen on this socket.");  close (mainSocket);  exit (1);  }
	while(1)  {
		
		childSocket = accept(mainSocket,  (struct sockaddr*) &childAddressStructure,  &childNameLength);
		if(childSocket==-1)  {
					printf("\n%s\n","Can't accept Connections.");
					close(mainSocket);
					exit (1);
					return 1;}
		else if(childSocket>1){
			printf("\n%s\n", "Connected Successfully."); }
			printf("\n%s\n","Connected with the Client.");
			 while(1){
			char message[MESSAGE_LENGTH]=" ", command[COMMAND_LENGTH]=" ";
			printf("\nInitially Command contains: %s\n", command);
			printf("%s","Enter Command to execute: ");
			fgets(command,COMMAND_LENGTH-2,stdin);
			printf("\nCommand SENT: %s \n", command );
			int index=location(command);
			char modifiedCommand[index+1];
			strncpy(modifiedCommand, command, index);
			printf("\nModified Command becomes: %s\n", modifiedCommand); //remove this
			modifiedCommand[index]='\0';
			int bytesWritten = -1;	
			if((bytesWritten=write(childSocket,modifiedCommand,strlen(modifiedCommand)))<strlen(modifiedCommand)){
					printf("\n%s\n","Can't Write  COMMAND to Child connection specified.");
					close(childSocket);
					close(mainSocket);
					exit(1);
				}
			int bytesRead=-1;	if((bytesRead=read(childSocket,message,MESSAGE_LENGTH)) < 0){
					printf("\n%s\n","Can't Read The results from The Child Connection.");
					close(childSocket);
					close(mainSocket);
					exit(1);
				}
			printf("\n\nCommand Output: \n%s\n",  message);
			}
			close(childSocket);
	}
	close (mainSocket);
	return 0;





}
