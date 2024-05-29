#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>                     //contains operating system API functions
#include <winsock.h>                    //Containing Socket Structures And Command
#include <time.h>
#include <ctype.h>
#define userDataFileName "userDataClient.csv"

int portNo = 9999; //Setting Default Port to 9999
int newClientFD;
int key = 5;
char buffer[257];
char copyOfBuffer[257];
char partyNameSt[257] = "";
char allConsituencyNamesSt[257] = "";
char myConsituencyNameCl[257] = "";


//Fxns for copying the data into the program strings
void setMyConsituencyNameCl(char *buffer) {
    strcpy(myConsituencyNameCl, buffer);
}


void setAllConsituencyNames(char *buffer) {
    strcpy(allConsituencyNamesSt, buffer);
}

void setPartyNames(char *buffer) {
    strcpy(partyNameSt, buffer);
}



char * decryptData(char * data) {
    return decrypt(data, key);
}

void sendEncryptedData(int socket, char * message, int size, int b) {
    send(socket, encrypt(message, key), size, b);
}






void sendFile(int serverFD, const char *filename) {
FILE *file = fopen(filename, "r"); 
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Calculate the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory to store file contents
    char *data = (char *)malloc(fileSize + 1); // Allocate memory for file size + 1 for null terminator
    if (data == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return;
    }

    // Read the entire file into data
    size_t bytesRead = fread(data, 1, fileSize, file);

    // Add null terminator to indicate end of string
    data[bytesRead] = '\0';

    //Send the data to the client
    send(serverFD, data, strlen(data), 0);

    fclose(file);
}


//Fxn to recieve file from the client
void receiveFile(int serverFD, const char *filename) {
    char data[1024];
    int bytesRead = recv(serverFD, data, sizeof(data), 0);
    if (bytesRead < 0) {
        perror("Error receiving file");
        return;
    }

    FILE *file = fopen(filename, "w"); 
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    
    data[bytesRead] = '\0';

    // Write the recieved data into the file
    size_t bytesWritten = fwrite(data, sizeof(char), bytesRead, file);
    if (bytesWritten != bytesRead) {
        perror("Error writing to file");
    }

    fclose(file);
}




void recieveMessage() {
    recv(newClientFD, buffer, 256, 0);
    printf("%s\n", decryptData(buffer));
    printf("\n");
}



int setConnection()
{   
    //Setting Defualt Port
    /*
    while (TRUE)
    {
        printf("Please Give Port No: ");
        scanf("%d", &portNo);
        if (portNo < 256)
        {
            printf("System Occupied Port\n");
        }
        else
        {
            break;
        }
    }*/

    struct sockaddr_in srv;
    // Initialize WSA variables for Windows
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
    {
        printf("Error Creating WSA\n");
        WSACleanup();
        exit(1);
    }


    //Creating a new Client File Dresciptor
    newClientFD = socket(AF_INET, SOCK_STREAM, 0);

    if (newClientFD < 0)
    {
        printf("Error Entering Socket\n");
        WSACleanup();
        exit(1);
    }
    else
    {
        printf("Socket Connected Successfully\n");
        printf("\n");printf("\n");
    }


    // Set server (struct sockaddr_in) variables
    srv.sin_family = AF_INET;
    srv.sin_port = htons(portNo);
    //srv.sin_addr.s_addr = inet_addr("127.0.0.1"); // For client we should not use INET_ADDR variable
    srv.sin_addr.s_addr = inet_addr("172.31.23.1");

    memset(&(srv.sin_zero), 0, 8);

    //Connect to Admin Server
    printf("Welcome TO EVM CLient Side...\n");
    printf("\n");printf("\n");
    Sleep(1000);
    printf("Trying to connect to server...\n");
    printf("\n");printf("\n");
    Sleep(2000);
    if (connect(newClientFD, (struct sockaddr *) &srv, sizeof(srv)) < 0) {
        printf("Error Connecting to Client\n");
        printf("Make Sure Server is running\n");
        WSACleanup();
        exit(1);
        
    } else {
        //Recieve and set Consituency Name
        while(1) {
            recv(newClientFD, buffer, 256, 0);
            strcpy(copyOfBuffer, decryptData(buffer));
            printf("Enter Consituency Name - (%s): ", copyOfBuffer);
            setAllConsituencyNames(copyOfBuffer);
            char cN[100];
            fgets(cN, 100, stdin);
            cN[strcspn(cN, "\n")] = '\0';               //Remove \n at end
            sendEncryptedData(newClientFD, cN, 100, 0);
            char r[10];
            recv(newClientFD, r, 10, 0);
            if (strcmp("right", r) == 0) {
                printf("Connected Successfully...\n");printf("\n");printf("\n");
                setMyConsituencyNameCl(cN);
                break;
            } else {
                printf("Wrong Consituency Name\nNot existing or already taken\n");
            }
        }
        memset(buffer, '\0', sizeof(buffer));
        //temp recv file
        memset(buffer, '\0', sizeof(buffer));
        recv(newClientFD, buffer, 256, 0);
        strcpy(copyOfBuffer, decryptData(buffer));
        setPartyNames(copyOfBuffer);
        //DO PASSWORD SHIT
        receiveFile(newClientFD, userDataFileName);
    }
    return 0;
}
/*
int main() {
    setConnection();
}*/