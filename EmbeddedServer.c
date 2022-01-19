#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<pthread.h>
#include<errno.h>
#include<sys/stat.h>

//servers listens on a port
//HTTP port is 80
#define SERVERPORT 8888

#define MAXLINE 4096
pthread_mutex_t lock;

void* connectionHandler(void* arg){
	int id = *(int*)arg;
	pthread_mutex_lock(&lock);
	FILE* usrptr;

    //create a userFile for each user
    char filename[100] = {0};
    char ext[10];
    sprintf(ext, "%d", id);
    strcpy(filename, "userFile");
    strcat(filename, ext);
    strcat(filename, ".txt");
	usrptr = fopen(filename, "w+");
    //Prompt the user to enter the name of the program
	char* message;
	message = "\nI am your socket handler\n";
	write(id, message, strlen(message));
	message = "Enter the name of the program\n";
	//Program name as entered by the user
	write(id, message, strlen(message));
    //Collect the reply in 'reply'
	char reply[MAXLINE];
	memset(reply, 0, MAXLINE);
	int readSize;
	read(id, reply, MAXLINE);
    //write the program name to file userFile'id'.txt
	fprintf(usrptr, "%s", reply);
	//
	//Email address of the user
	message = "Enter your email address\n";
	write(id, message, strlen(message));
	char emailId[100];
	memset(emailId, 0, 100);
	read(id, emailId, 100);
    //write the email address to the same file
	fprintf(usrptr, "%s", emailId);
    //set the pointer to the top start
	fseek(usptr, 0, SEEK_SET);
    char userSensor[100];
    fscanf(usrptr, "%s", userSensor);
    char emailAddr[100];
	fscanf(usrptr, "%s", emailAddr);
    fclose(usrptr);
	//produce output

    //create a bash file to execute the code
    FILE* bashS;
    char* bashScript = (char*)malloc(strlen("binFile")+strlen(ext)+strlen(".sh"));
    strcpy(bashScript, "binFile");
    strcat(bashScript, ext);
    strcat(bashScript, ".sh");
    bashS = fopen(bashScript, "w+");
    char* command1 = "chmod 777 ";
    char* command2 = "chmod +x ";
    char* scriptC1 = (char*)malloc(strlen(bashScript)+strlen(command1)+1);
    strcpy(scriptC1, command1);
    strcat(scriptC1, bashScript);
    char* scriptC2 = (char*)malloc(strlen(bashScript)+strlen(command2)+1);
    strcpy(scriptC2, command2);
    strcat(scriptC2, bashScript);
    system(scriptC1);
    system(scriptC2);

    char* sensorNameTxt = (char*)malloc(strlen("Sensor")+strlen(ext)+1);
    strcpy(sensorNameTxt, "Sensor");
    strcat(sensorNameTxt, ext);
    strcat(sensorNameTxt, ".txt");
    //create a bash script

    char userFileEX = (char*)malloc(strlen("user")+strlen(ext)+1);
    strcpy(userFileEX, "user");
    strcat(userFileEX, ext);
    
    fprintf(bashS, "%s\n", "#!/bin/bash");
    fprintf(bashS, "\n%s %s %s %s\n", "gcc", userSensor, "-o", userFileEX);
    fprintf(bashS, "%s%s %s%s\n", "./", userFileEX, " >> ",sensorNameTxt);
    fclose(bashS);

    //read the filename and email Id from the file
    char* execCode = (char*)malloc(strlen(bashScript)+strlen("./")+1);
    strcpy(execCode, "./");
    strcat(execCode, bashScript);
    system(execCode);
	//email output to user
    char* commandToCode = (char*)malloc(strlen("python3 emailSend.py ")+strlen(emailAddr)+strlen(" ")+strlen(sensorNameTxt)+1);
    strcpy(commandToCode, "python3 emailSend.py ");
    strcat(commandToCode, emailAddr);
    strcat(commandToCode, " ");
    strcat(commandToCode, sensorNameTxt);
    system(commandToCode);
    char* rem = (char*)malloc(strlen(bashScript)+strlen("rm ")+1);
    strcpy(rem, "rm ");
    strcat(rem, bashScript);
    system(rem);
    rem = NULL;
    rem = (char*)malloc(strlen(userFileEX)+strlen("rm ")+1);
    strcpy(rem, "rm ");
    strcat(rem, userFileEX);
    system(rem);
    rem = NULL;
    rem = (char*)malloc(strlen(sensorNameTxt)+strlen("rm ")+1);
    strcpy(rem, "rm ");
    strcat(rem, sensorNameTxt);
    system(rem);
    //close all pointers
    bashScript = NULL;
    userFile = NULL;
    command1 = NULL;
    command2 = NULL;
    scriptC1 = NULL;
    scriptC2 = NULL;
    free(bashScript);
    free(command1);
    free(command2);
    free(scriptC1);
    free(scriptC2);
    free(execCode);
    free(rem);
	free(arg);
	close(id);
	pthread_mutex_unlock(&lock);
}

int main(int argc, char const *argv[])
{
	int listenfd, n;

	struct sockaddr_in server;
	uint8_t buffer[MAXLINE-1];
	uint8_t reply[MAXLINE-1];
	//create the socket
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("socket creation failed.");
		return 1;
	}
	printf("Socket created\n");
	// bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(SERVERPORT);
	//bind and listen
	//socket bind 
	int option = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if((bind(listenfd, (struct sockaddr*)&server, sizeof(server)))<0){
		perror("bind failed");
		return 1;
	}
	//listen
	if((listen(listenfd, 3))<0){
		perror("listen error");
		return 1;
	}
	printf("Waiting for a connection...\n");
	fflush(stdout);
	int size = sizeof(struct sockaddr_in);
	struct sockaddr_in client;
	int sockClient;
	char* message;
	int* sockPtr;
	while((sockClient = accept(listenfd, (struct sockaddr*)&client, (socklen_t*)&size))){
		puts("Connection accepted");
		message = "Hello Client! I have received your connection.\nI will assign a handler.";
		write(sockClient, message, strlen(message));
		pthread_t handler;
		sockPtr = malloc(1);
		*sockPtr = sockClient;
		if(pthread_create(&handler, NULL, connectionHandler, (void*)sockPtr)<0){
			perror("Could not create thread");
			return 1;
		}
		// pthread_join(handler, NULL);
	}
	if(sockClient<0){
		perror("accept failed");
		return 1;
	}
	return 0;
}