#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>                             //Contains operating system API functions
#include <winsock.h>                            //Containing Socket Structures And Command
#include <time.h>                               //Used for mimiccing delay
#include <ctype.h>

#include "b23cm1049_b23me1013_b23ph1020_b23cs1059_encryptor.c"


#define maxClientsAllowed 29                    //For testing purposes, maximum number of consituencies are 29 only.
#define voteDataFileName "votingDataServer.csv"
#define userDataFileName "userDataServer.csv"

int arrClients[maxClientsAllowed];
int currClients = 0;
int key = 5; 
int newSocketFD;
int portNo = 9999;
int totalConsituencies = 0;
char * partyNames = NULL;
char * consituencyNames = NULL;
int numResultRecived = 0;


void sendEncryptedData(int socket, char * message, int size, int b) {
    send(socket, encrypt(message, key), size, b);
}


char * decryptData(char * data) {
    return decrypt(data, key);
}




void sendFile(int clientFD, const char *filename) {
    FILE *file = fopen(filename, "r"); // Open the CSV file for reading
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
    
    send(clientFD, data, strlen(data), 0);

    fclose(file);
}





/*void receiveFile(int clientFD, const char *filename) {
    char data[1024];
    int bytesRead = recv(clientFD, data, sizeof(data), 0);
    if (bytesRead < 0) {
        perror("Error receiving file");
        return;
    }

    FILE *file = fopen("tempVoteData.csv", "w"); // Open the file for writing (creates if not exists, truncates otherwise)
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Add null terminator at the end of received data
    data[bytesRead] = '\0';
    size_t bytesWritten = fwrite(data, sizeof(char), bytesRead, file);
    if (bytesWritten != bytesRead) {
        perror("Error writing to file");
    }
    
    // Write the data into the file
    if (numResultRecived == 0) {
        FILE * f2 = fopen(voteDataFileName, "w");
        if (f2 == NULL) {
            perror("Error opening file");
            return;
        }

        size_t bytesWritten = fwrite(data, sizeof(char), bytesRead, f2);
        if (bytesWritten != bytesRead) {
            perror("Error writing to file");
        }
        fclose(f2);
        numResultRecived++;
    } else {
        //parseData(voteDataFileName, "tempVoteData.csv", "tempCombinedData.csv");
        rename("tempCombinedData.csv", voteDataFileName);
    }
    fclose(file);
    remove("tempVoteData.csv");
}*/


//function to check if the given consituency name by the client is correct or not
char * checkConsituencyNameGiven(char * name) {
    char * match = strstr(consituencyNames, name);
    char * result = malloc(6); // Allocate memory for "right" or "wrong"
    if (result == NULL) {
        // Handle memory allocation error
        return NULL;
    }

    if (match != NULL) {
        // Check if name is surrounded by delimiter characters or is at the beginning/end
        if ((match == consituencyNames || *(match - 1) == ',' || *(match - 1) == '(') &&
            (*(match + strlen(name)) == ',' || *(match + strlen(name)) == ')' || *(match + strlen(name)) == '\0')) {
            strcpy(result, "right");
        } else {
            strcpy(result, "wrong");
        }
    } else {
        strcpy(result, "wrong");
    }
    return result;
}


//Set the party Names into the variable
void setPartyNames(char * pN) {
    partyNames = (char *)malloc(strlen(pN) + 1);
    if (partyNames == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    strcpy(partyNames, pN);
}


//Set the party Names into the variable
void setConsituencyNames(char * cN) {
    consituencyNames = (char *)malloc(strlen(cN) + 1);
    if (consituencyNames == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    strcpy(consituencyNames, cN);
}


//Function asks message from the user
char * askForMessage() {
    char * m = (char *)malloc(100 * sizeof(char));
    fgets(m, 100, stdin);
    return m;
}

//function to send message to all clients connected to admin
void sendMessageToAll(char * message) {
    for (int i = 0; i < currClients; i++)
    {
        sendEncryptedData(arrClients[i], message, strlen(message), 0);
    }
}


/*void enterRecievingMode() {
    char buffer[257];
    printf("Entered Receiving Mode...\n");
    
    int clientFd;
    int output;
    printf("%d", currClients); // Ensure currClients is defined somewhere
    while(1) {
        if (numResultRecived == currClients) {
            break;
        }
        for (int i = 0; i < maxClientsAllowed; i++) {
            clientFd = arrClients[i];
            receiveFile(clientFd, voteDataFileName);
        }
    }
}*/

void receiveMessageProcess(int clientFD)
{   
    //Create buffer for storing Message
    char buffer[257];
    //If somehow connection Broke
    if (recv(clientFD, buffer, 256, 0) < 0)
    {
        printf("Failed to connect to client: %d\nClosing Connection\n", clientFD);
        closesocket(clientFD);
        //Remove from the array
        for (int i = 0; i < maxClientsAllowed; i++)
        {
            if (arrClients[i] == clientFD)
            {
                arrClients[i] = 0;
                break;
            }
        }
    }
    else
    {
        //Print the message
        printf("Message Recieved From Client %d is: %s\n", clientFD, decryptData(buffer));
        printf("Enter Reply: ");
        char * m = askForMessage();
        sendEncryptedData(clientFD, m, sizeof(m), 0);
    }
}

void newConnectionRequest(int *newSocketFd, struct fd_set *fr)
{
    // If it is connection Request
    if (FD_ISSET(*newSocketFd, fr))
    {   
        //Check If maximum Number of Clients are reached
        if (currClients >= maxClientsAllowed) 
        {
            printf("Max Clients Capacity Reached\n");
            return;
        }

        // fulfill the new request
        int len = sizeof(struct sockaddr);
        //Accpet the request
        int socketAcceptOutput = accept(*newSocketFd, NULL, &len);

        //Add it into the client array
        if (socketAcceptOutput > 0)
        {
            arrClients[currClients++] = socketAcceptOutput;
            
            char buffer[100];

            //Recieve Consituency Name
            while (1) {
                sendEncryptedData(socketAcceptOutput, consituencyNames, strlen(consituencyNames), 0);
                recv(socketAcceptOutput, buffer, 100 , 0);
                char * r = checkConsituencyNameGiven(decryptData(buffer));
                send(socketAcceptOutput, r, strlen(r), 0);
                if (strcmp("right", r) == 0) {
                    break;
                }
            }
            
            printf("EVM (%s) Connected\n", decryptData(buffer));printf("\n");printf("\n");
            sendEncryptedData(socketAcceptOutput, partyNames, strlen(partyNames), 0);

            //send file temp
            sendFile(socketAcceptOutput, userDataFileName);
            printf("Waiting for %d more to connect...\n", (totalConsituencies - currClients));printf("\n");printf("\n");
        }
        else
        {
            printf("Failed to connect\n");
            return;
        }
    //Else it is a message Request
    } else {                                          
        for (int i = 0; i < maxClientsAllowed; i++) {
            if (FD_ISSET(arrClients[i], fr)) {
                receiveMessageProcess(arrClients[i]);   //Call function to process the message request
            }
        }
    }
    
}


//funtion to start the server
int startServer(int totalConst)
{   
    totalConsituencies = totalConst;
    printf("Trying to Start Server...\n");printf("\n");printf("\n");
    Sleep(1000);                                        //Create Some Delay
    //Starting Port on Default Port #9999

    /*while (TRUE)
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
    // Initialize WSA variables for Windows, Without these socket programming doesnt works in windows
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
    {
        printf("Error Creating WSA\n");
        WSACleanup();
        return -1;
        exit(1);
    }

    newSocketFD = socket(AF_INET, SOCK_STREAM, 0);    //Creating a new Socket File Descriptor which will be out socket

    if (newSocketFD < 0)
    {
        printf("Error Entering Socket\n");
        WSACleanup();
        return -1;
        exit(1);
    }
    else
    {
        printf("Socket Connected Successfully\n");printf("\n");printf("\n");
        Sleep(1000);
    }

    // Set server (struct sockaddr_in) variables
    srv.sin_family = AF_INET;                           //AF_INET refers to Address Family IPv4 
    srv.sin_port = htons(portNo);                       //Set the port Number
    //srv.sin_addr.s_addr = INADDR_ANY;                   // Same as inet_add = "127.0.0.1", Sets the default IP address
    srv.sin_addr.s_addr = inet_addr("172.31.23.1"); // Replace with the server's IP address

    
    memset(&(srv.sin_zero), 0, 8);                      //Create extra space, set it zero, it is used by winsock in background


    // BEFORE BINDING we need to set server option
    //  we will set SO_REUSEADDR
    // it will allow same port to be used by multiple sockets

    int optval = 0;
    int optsize = sizeof(optval);
    if (setsockopt(newSocketFD, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, optsize) != 0)
    {
        printf("Error Setting Server Options\n");
        WSACleanup();
        return -1;
        exit(1);
    }

    //Binding to the port and socket
    int BindOutput = bind(newSocketFD, (struct sockaddr *)&srv, sizeof(struct sockaddr));
    if (BindOutput < 0)
    {
        printf("Error Binding to Port %d\n", portNo);
        WSACleanup();
        return -1;
        exit(1);
    }
    else
    {
        printf("Binded to Port %d Successfully\n", portNo);printf("\n");printf("\n");
        Sleep(1000);
    }

    return 0;
}


int startListening() {
    //Start Listening On Port
    int listenOutput = listen(newSocketFD, maxClientsAllowed);
    if (listenOutput < 0)
    {
        printf("Error Listening to Port %d\n", portNo);
        WSACleanup();
        return -1;
        exit(1);
    }
    else
    {
        printf("Started Listing to Port %d\n", portNo);
        Sleep(1000);
        printf("Waiting For %d EVM's To Join...\n", totalConsituencies);
    }

    //Create FD_SETS which will store incoming requests
    struct fd_set fr, fw, fe; // fr/w/e - set of file driscriptors ready to read/write/exception throwing
                              //  max = 64 fds......if > 64, not any error, just it will only add 64

    int newMaxFD = newSocketFD; // because 1 socket can only contain 64 fds, we everytime create new by newMaxFD + 1 so that we are not limited by 64
    struct timeval tv;          // how much time it will wait to see how much time they are ready
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    //This Loop Continues Listening
    while (1)
    {   
        if (currClients == totalConsituencies) {
            break;
        }
        //Set fd sets initially to zero
        FD_ZERO(&fr);
        FD_ZERO(&fw);
        FD_ZERO(&fe);
        FD_SET(newSocketFD, &fr);
        FD_SET(newSocketFD, &fe);
        for (int i = 0; i < maxClientsAllowed; i++)
        {
            if (arrClients[i] != 0)
            {
                FD_SET(arrClients[i], &fr);
                FD_SET(arrClients[i], &fe);
            }
        }

        int selectOuptut = select(newMaxFD + 1, &fr, &fw, &fe, &tv);

        if (selectOuptut > 0)
        {
            // When Someone Connects or sendEncryptedData message to the port
            newConnectionRequest(&newSocketFD, &fr);
        }
        else if (selectOuptut == 0)
        {
            // When no one connects or sendEncryptedData message to the port
        }
        else
        {
            // When Error
            printf("Error Initializing Socket Descriptors\n");
            WSACleanup();
            return -1;
            exit(1);
        }
    }
    return 0;
}

/*
int main() {
    return 0;
}
*/