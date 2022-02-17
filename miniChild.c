//0089-bscs-19(gcu lahore)
//hassan saud
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include  <unistd.h>
#include <string.h>
#include <ctype.h>

#define COMMAND_LENGTH 50
#define MESSAGE_LENGTH 1234


int location(const char* cmd){
	int l1;
	for(int num=0; num<strlen(cmd); num++){
		if(isspace(cmd[num])) l1=num;
		while(isspace(cmd[num]) && num<(strlen(cmd)-1)){
			num+=1;
			if(!(isspace(cmd[num]))) l1=-1;
		}
	}
	return l1;
}

int main(int num, char* argv[]){
	int mainSocket,  mainPort=htons(12345),  returnStatus=0;
	struct sockaddr_in  mainAddressStructure;
	char path[]=" > /root/commandResult"  ;
	bzero(&mainAddressStructure,  sizeof(mainAddressStructure));
	mainAddressStructure.sin_family=AF_INET;
	inet_pton(AF_INET,  argv[1]  ,  &mainAddressStructure.sin_addr.s_addr);
	mainAddressStructure.sin_port=mainPort;
	mainSocket = socket(AF_INET,  SOCK_STREAM, IPPROTO_TCP);
	if(mainSocket<0){
		printf("\n%s\n", "Can't open Socket");
		exit (1);
	}
	returnStatus=connect(mainSocket,  (struct sockaddr*) &mainAddressStructure,  sizeof(mainAddressStructure));
	if(returnStatus==0){
		printf("\n%s\n", "Connection Successfull.");
	}
	else{
		printf("\n%s\n",  "Can't connect to Socket.");
		close (mainSocket);
		exit (1);
	}
	while(1){
	char command[COMMAND_LENGTH]=" ", message[MESSAGE_LENGTH]=" ";
	
	returnStatus=read(mainSocket,  command,  COMMAND_LENGTH);
	if(returnStatus>0){
		printf("\nCommand RECEIVED: %s\n", command);
		strcat(command,path);
		
		
	}
	else if(returnStatus < 0){
		printf("\n%s\n", "can't read from connection.");
		printf("\nBuffer contains:  %s\n", command );
		close (mainSocket);
		exit (1);
	}
		int size=strlen(command);
		char modifiedCommand[size];
		strncpy(modifiedCommand, command, size);
		modifiedCommand[size]='\0';
		printf("\nCommand Converted to: %s\n", modifiedCommand);
		printf("\nNow actual Command becomes: %\n", modifiedCommand);
		system(modifiedCommand);
		int  fd; if((fd=open("/root/commandResult", O_RDONLY )) < 0){
				printf("\n%s\n","Can't open File specified.");
				close(mainSocket);
				exit (1);
			}
		int bytesRead; if((bytesRead=read(fd,message, MESSAGE_LENGTH)) < 0 ){
			printf("\n%s\n", "Can't read from File");
			close(mainSocket);
			close(fd);
			exit(1);
			}
		int bytesWritten=write(mainSocket, message, MESSAGE_LENGTH);
		if(bytesWritten < 0){printf("\n%s\n", "Can't Write back  to  Server Connection specified. ");
			close(mainSocket);
			close(fd);
			exit(1);
		}
		printf("\n%s\n%s\n", "Command Result Sent: ", message);
		//for(int num=0;num<COMMAND_LENGTH;num++)command[num]=' ';
		//for(int num=0; num<MESSAGE_LENGTH; num++)message[num]=' ';
	}

	close (mainSocket);
	return 0;


}
