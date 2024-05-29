#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>                       //Used for creating delays
#include <unistd.h>                     //contains operating system API functions
#include <winsock.h>                    //Containing Socket Structures And Command
#include <ctype.h>                      //To use Character Classification Functions
#include "b23cm1049_b23me1013_b23ph1020_b23cs1059_encryptor.c"          //File used for encryption and decryption
#include "b23cm1049_b23me1013_b23ph1020_b23cs1059_client_votingSystem.c"        //Used for creating the voting ecosystem
#include "b23cm1049_b23me1013_b23ph1020_b23cs1059_client_serverConnector.c"     //Used for connection between client and server
#define MAX_NAME_LENGTH 100
#define voteDataFileName "votingDataClient"         //File Name for storing voting data
#define userDataFileName "userDataClient.csv"       //File Name for storing used Id's and Password

int main() {
    setConnection();                    //Used to setup connection between the client and server
    strcpy(partyNameString, partyNameSt);       //Copy the party names given ny server into client
    strcpy(myConsituencyNameString, myConsituencyNameCl);       //Copy the consituency names into the client
    strcpy(allConsituencyNameString, allConsituencyNamesSt);    //Copy the list of consituency names given by the server
    startVoting();                      //Used to start voting and end the voting and display results
    Sleep(2000);
}