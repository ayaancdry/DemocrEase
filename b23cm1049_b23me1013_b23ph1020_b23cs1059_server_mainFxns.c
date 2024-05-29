#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>                             //Contains operating system API functions
#include <winsock.h>                            //Containing Socket Structures And Command
#include <time.h>              
#include "b23cm1049_b23me1013_b23ph1020_b23cs1059_server_clientConnector.c"                             //Main Server File
#include "b23cm1049_b23me1013_b23ph1020_b23cs1059_server_idCreator.c"
//Function Used For Printing Welcome Message
void printWelcomeScreen() {
    printf("Welcome To Elections Server\n");printf("\n");printf("\n");
}
//Functions to get number of Parties
int getNumParties() {
    printf("Please Enter Number of Parties Standing: ");
    int numParties;
    scanf("%d", &numParties);
    return numParties;
}

//Function used for returning a pointer to array of strings which stores name of parties
char ** getPartyNames(int numParties) {
    // Buffer to store each party name
    char buffer[100];                                                     
    
    // Allocate memory for partyNames
    char **partyNames = (char **)malloc(numParties * sizeof(char *));     //Multiplied by numbere of parties
    if (partyNames == NULL) {                                             //If memory allocation failed  
        printf("Some Error Occured (Memory allocation failed)\n");
        exit(1);
    }

   //Remove the newline character left from previous response
    while (getchar() != '\n');
    
    //Take input of parties
    for (int i = 0; i < numParties; i++) {
        printf("Enter party name %d: ", i + 1);
        fgets(buffer, sizeof(buffer), stdin);                             //Get party name 
        buffer[strcspn(buffer, "\n")] = '\0';                             //Remove \n
        
        // Allocate memory for the party name
        partyNames[i] = (char *)malloc((strlen(buffer) + 1) * sizeof(char)); // as \0 will also come
        strcpy(partyNames[i], buffer);                                    //Copy into array of strings
    }
    
    return partyNames;              
}

//Funtion to get Number of Consituencies
int getNumConstituency() {
    printf("Please Enter Number of Constituency: ");
    int numConstituency;
    scanf("%d", &numConstituency);
    return numConstituency;
}

//Function used for returning a pointer to array of strings which stores name of consituencies
char ** getConsituencyNames(int numConstituency) {
    char buffer[100];                                                     // Buffer to store each Constituency name
    
    // Allocate memory for ConstituencyNames
    char **constituencyNames = (char **)malloc(numConstituency * sizeof(char *));     //Multiplied by numbere of Constituency
    if (constituencyNames == NULL) {                                             //If memory allocation failed  
        printf("Some Error Occured (Memory allocation failed)\n");
        exit(1);
    }

   //Remove the newline character left from previous response
    while (getchar() != '\n');
    
    //Take input of parties
    for (int i = 0; i < numConstituency; i++) {
        printf("Enter constituency name %d: ", i + 1);
        fgets(buffer, sizeof(buffer), stdin);                             //Get party name 
        buffer[strcspn(buffer, "\n")] = '\0';                             //Remove \n
        
        // Allocate memory for the party name
        constituencyNames[i] = (char *)malloc((strlen(buffer) + 1) * sizeof(char)); // as \0 will also come
        strcpy(constituencyNames[i], buffer);                                    //Copy into array of strings
    }
    
    return constituencyNames;              
}

//Function to get Party Names separated by Comma
char * combinePartyNames(char **parties, int numParties) {
    // Calculate the total length needed for the combined string
    int totalLength = 0;
    for (int i = 0; i < numParties; i++) {
        totalLength += strlen(parties[i]);
    }
    // Add space for commas
    totalLength += numParties - 1; 
    
    // Allocate memory for the combined string
    char *combined = (char *)malloc((totalLength + 1) * sizeof(char)); 
    if (combined == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    // Copy party names and commas to the combined string
    int index = 0;
    for (int i = 0; i < numParties; i++) {
        strcpy(combined + index, parties[i]);
        index += strlen(parties[i]);
        //If not reached Last Party, Add comma
        if (i < numParties - 1) {
            combined[index++] = ',';
        }
    }

    //Add \0 to the string
    combined[index] = '\0';

    return combined;
}

//Same Function for Consituency Names
char * combineConstituencyNames(char **constituencies, int numConstituencies) {
    // Calculate the total length needed for the combined string
    int totalLength = 0;
    for (int i = 0; i < numConstituencies; i++) {
        totalLength += strlen(constituencies[i]);
    }
    // Add space for commas
    totalLength += numConstituencies - 1; 
    
    // Allocate memory for the combined string
    char *combined = (char *)malloc((totalLength + 1) * sizeof(char));
    if (combined == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    // Copy constituency names and commas to the combined string
    int index = 0;
    for (int i = 0; i < numConstituencies; i++) {
        strcpy(combined + index, constituencies[i]);
        index += strlen(constituencies[i]);
        //If not reached Last Constituency, Add comma
        if (i < numConstituencies - 1) {
            combined[index++] = ',';
        }
    }

    //Add \0 to the string
    combined[index] = '\0';

    return combined;
}

//Function to start main server
int startServerAdmin(int numConsituencies) {
    return startServer(numConsituencies);
}


int main() {
    printWelcomeScreen();
    printf("1. Create Electoral ID's\n2. Start Elections\nEnter Choice: ");
    int x;
    scanf("%d", &x);
    if (x == 1) {
        char ch = 'y';
        while (ch == 'y') {
            createUser();
            printf("Do you want to add more (y/n): ");
            scanf(" %c", &ch);
        }
    } else {
        int numParties = getNumParties();
        char **parties = getPartyNames(numParties);
        int numConsituencies = getNumConstituency();
        char **consituencies = getConsituencyNames(numConsituencies);
        char * combinedPartyNames = combinePartyNames(parties, numParties);
        char * combinedConsituencyNames = combineConstituencyNames(consituencies, numConsituencies);
        setPartyNames(combinedPartyNames);
        setConsituencyNames(combinedConsituencyNames);
        //Start the server
        if (startServerAdmin(numConsituencies) < 0) {
            printf("Error in Creating Server\nExiting\n");
            exit(1);
        }
        if (startListening() == 0) {
            printf("Connected To All Consituencies...\n");printf("\n");printf("\n");
            Sleep(1000);
            printf("Sending Parties to EVM's...\n");
            //send party names to all the clients
            sendMessageToAll(combinedPartyNames);
            Sleep(3000);
            printf("Sent Successfully\n");printf("\n");printf("\n");
            printf("Closing Now...\n");
        } else {
            printf("Failed To Connect To EVM's\nClosing...\n");
            exit(1);
        }
        //enterRecievingMode();
    }
    
}