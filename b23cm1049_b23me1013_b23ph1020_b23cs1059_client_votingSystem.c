#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
//#include "b23cm1049_b23me1013_b23ph1020_b23cs1059_client_barGraph.c"


#define voteDataFileName "votingDataClient"
#define userDataFileName "userDataClient.csv"
#define MaxParties  63          // 6 National Parties, 57 State Parties
#define MaxRegions  36          // 28 States, 8 Union Territories
#define MaxNameLength 1000       // Maximum length of any string which might be taken in the code is fixed to be maximum at 200. 
#define LineWidth 50            // Fixed Width of 50 spaces for now for the outer borders

// defining empty strings for storage
char partyNameString[MaxNameLength] = "";
char myConsituencyNameString[MaxNameLength] = "";
char allConsituencyNameString[MaxNameLength] = "";
//char * line = (char *)malloc(MaxNameLength);
//char * dline = (char *)malloc(MaxNameLength);

int setParties(char *input, char arr[MaxParties][MaxNameLength]) {
    char* token = strtok(input, ",");
    int num_tokens = 0;

    // Split the string and store each token in the array
    while (token != NULL && num_tokens < MaxParties) {
        strncpy(arr[num_tokens], token, MaxNameLength - 1);
        arr[num_tokens][MaxNameLength - 1] = '\0'; // Ensure null termination
        num_tokens++;
        
        token = strtok(NULL, ",");
    }

    return num_tokens;
}


int setConsituencies(char *input, char arr[MaxRegions][MaxNameLength]) {
    char* token = strtok(input, ",");
    int num_tokens = 0;

    // Split the string and store each token in the array
    while (token != NULL && num_tokens < MaxParties) {
        strncpy(arr[num_tokens], token, MaxNameLength - 1);
        arr[num_tokens][MaxNameLength - 1] = '\0'; // Ensure null termination
        num_tokens++;
        
        token = strtok(NULL, ",");
    }

    return num_tokens;
}




// function to display the voting window
void displayWindow(char *state, char parties[MaxParties][MaxNameLength], int numberOfParties){
    const char *border = "--------------------------------------------------";       // define a constant string depicting the upper borders of the window. constant string so as to prevent any further changes by mistake.
    printf("%s\n", border);
    int electionLen = strlen("Election: ");
    int stateLen = strlen(state);
    int padding = (LineWidth - (electionLen + stateLen)) / 2;   // padding ensured to keep the header text in middle. 
    printf("|%*sElection: %s%*s|\n", padding, "", state, LineWidth - (electionLen + stateLen + padding), "");
    printf("%s\n", border);

    // Display all the parties contesting in the election
    for (int i = 0; i < numberOfParties; i++) {
        printf("|  [%d] %-44s|\n", i + 1, parties[i]);
    }

    printf("%s\n", border);
    const char *instructions = "Instructions and Options";
    int instructionsLen = strlen(instructions);
    padding = (LineWidth - instructionsLen) / 2;         // padding ensured again. 
    printf("|%*s%s%*s|\n", padding, "", instructions, LineWidth - instructionsLen - padding, "");
    printf("%s\n", border);

    printf("|  Select a party by entering their number         |\n");
    for (int i = 0; i < numberOfParties; i++) {
        printf("|  [%d] - %-42s|\n", i + 1, parties[i]);
    }
    printf("|  [S] - Submit my vote                            |\n");
    // 
    printf("|  [R] - Give Next Vote                            |\n");
    printf("|  [E] - End                                       |\n");
    printf("%s\n", border);
}


// Function to create a random number which will be the number of votes pre-existing in the file. The vote for each party in each state will be a random number between 500 and 1000
int randomiseVote() {
    return 500 + (rand() % 501); 
}


// Function to create the file which stores the information of the parties, states, number of votes.
void createFile(const char *filename, char states[MaxRegions][MaxNameLength], char parties[MaxParties][MaxNameLength], int numberOfStates, int numberOfParties) {
    char filename2[100];
    sprintf(filename2, "%s%s.csv",voteDataFileName,myConsituencyNameString);
    
    FILE *file = fopen(filename2, "w");

    // write the header row
    fprintf(file, "State");
    for (int i=0; i<numberOfParties; i++) {
        fprintf(file, ",%s", parties[i]);        // the format of the first row of the csv file is like State, Party 1, Party 2, Party 3, .....
    }
    fprintf(file, "\n");

    /* write the further rows of the file. The format of the file is like 
        State, Party 1, Party 2, Party 3, ..........  (Header Row)
        Delhi, 333, 456, 123, ......................  (Further Rows) */      // the votes are randomly generated votes in between 500 and 1000 using the randomiseVote() function.
    for (int i=0; i<numberOfStates; i++) {
        fprintf(file, "%s", states[i]);
        for (int j=0; j<numberOfParties; j++) {
            int randomVote = randomiseVote();
            fprintf(file, ",%d", randomVote);
        }
        fprintf(file, "\n");
    }
    //printf("\nCSV file created with the name '%s'.\n", filename2);
    fclose(file);
}

/* Function to check user ID and password from file. 
    Temporarily named details.csv. In this, the format of userid and password will be like:

    userid//password
    for example, 
    ayaan123//ayawnpass12
    yash47//iitjodhpur23
    and so on. The userid and password are separated by "//"
*/

int checkIdAndPass(const char *userID, const char *password, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open details.csv file");
        return 0;
    }
    char line[257];
    char dline[257];
    int userFound = 0; // to check if the input user-id exists in the file or not
    // start checking file for user-id and password
    while (fgets(line, sizeof(line), file)) {
        
        // newline character removed
        line[strcspn(line, "\n")] = '\0';
        strcpy(dline, decrypt(line, 5));
        printf("%s\n", dline);
        // tokenise the line from "//"
        char *token = strtok(dline, "//");
        if (token && strcmp(token, userID) == 0) {
            userFound = 1; // change variable value to 1 if user-id is found in the file
            token = strtok(NULL, "//");
            if (token && strcmp(token, password) == 0) {
                fclose(file);
                return 1; // user-id and password matches. Successful
            } else {
                printf("Wrong password. Kindly re-enter your details..\n");
                line[0] = '\0';
                dline[0] = '\0';
                fclose(file);
                return 0; // Un-Successful.
            }
        }
    }
    
    fclose(file);
    
    if (!userFound) {
        printf("User not registered. Please try again.\n");
    }
    
    return 0; // Authentication failed
}
/*
// Function to update the vote in the CSV file
void updateVote(const char *state, int partyIndex, const char *filename) {
    char line[MaxNameLength];
    char filename2[100];
    sprintf(filename2, "%s%s.csv",voteDataFileName,myConsituencyNameString);
    FILE *file = fopen(filename2, "r");
    if (file == NULL) {
        printf("Error Reading File");
    }
    char temp2[100];
    sprintf(temp2, "temp%s.csv",myConsituencyNameString);
    FILE *tempFile = fopen(temp2, "w");
    if (tempFile == NULL) {
        printf("Error Writing File");
    }
    int stateFound = 0;

    while (fgets(line, sizeof(line), file)) {
        char tempLine[MaxNameLength];
        strcpy(tempLine, line);
        char *token = strtok(tempLine, ",");

        if (token && strcmp(token, state) == 0) {
            stateFound = 1;
            fprintf(tempFile, "%s", token);
            int currentPartyIndex = 0;
            token = strtok(NULL, ",");

            while (token) {
                int voteCount = atoi(token);

                if (currentPartyIndex == partyIndex) {
                    voteCount++;
                }

                fprintf(tempFile, ",%d", voteCount);
                token = strtok(NULL, ",");
                currentPartyIndex++;
            }
            fprintf(tempFile, "\n");
        } else {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);
    remove(filename2);
    rename(temp2, filename2);
}
*/


void updateVote(const char *state, int partyIndex, char *filename) {
    char line[MaxNameLength];
    char filename2[257];
    sprintf(filename2, "%s%s.csv", voteDataFileName, myConsituencyNameString);
    FILE *file = fopen(filename2, "r");
    if (file == NULL) {
        printf("Error opening file for reading: %s\n", filename2);
        return;
    }
    char tempString[1000] = ""; // defining a string for temporary storage
    int stateFound = 0;

    while (fgets(line, sizeof(line), file)) {
        char tempLine[MaxNameLength];
        strcpy(tempLine, line);
        char *token = strtok(tempLine, ",");

        if (token && strcmp(token, state) == 0) {
            stateFound = 1;
            strcat(tempString, token); // Concatenate token to tempString
            int currentPartyIndex = 0;
            token = strtok(NULL, ",");

            while (token) {
                int voteCount = atoi(token);

                if (currentPartyIndex == partyIndex) {
                    voteCount++;
                }

                char partyCount[MaxNameLength];
                sprintf(partyCount, ",%d", voteCount); // Storing the party vote count in partyCount
                strcat(tempString, partyCount); // Concatenate partyCount to tempString
                token = strtok(NULL, ",");
                currentPartyIndex++;
            }
            strcat(tempString, "\n"); // Add a newline to tempString
        } else {
            strcat(tempString, line); // Concatenate line to tempString
        }
    }

    fclose(file);
    FILE *file2 = fopen(filename2, "w");
    fputs(tempString, file2);
    fclose(file2);
}

// function to display the result of each constituency 
void showConstituencyResults(const char *filename, char parties[MaxParties][MaxNameLength], int numParties) {
    char filename2[257];
    sprintf(filename2, "%s%s.csv", voteDataFileName, myConsituencyNameString);
    FILE *file = fopen(filename2, "r");
    if (!file) {
        perror("Unable to open CSV file");
        return;
    }

    char line[MaxNameLength];
    fgets(line, sizeof(line), file); 

    int constituencyCount = 0;
    int winnersCount[MaxParties] = {0}; // Array to store the count of winners for each party

    // read data for each constituency which is hosting the election
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        if (token) {
            if (strcmp(token, myConsituencyNameString) == 0) {
                printf("\nConstituency: %s\n", token);
                printf("Party Votes\n");
                int maxVotes = 0;
                char constituencyWinner[MaxNameLength] = "";

                // read the vote counts for each party in the constituency
                for (int i = 0; i < numParties; i++) {
                    token = strtok(NULL, ",");
                    int votes = atoi(token);
                    printf("%-6s %d\n", parties[i], votes);

                    // check if the party has the highest number of votes 
                    if (votes > maxVotes) {
                        maxVotes = votes;
                        strcpy(constituencyWinner, parties[i]);
                    }
                }

                printf("Winner: %s\n", constituencyWinner);

                // increment the count of winners for the winning party
                for (int i = 0; i < numParties; i++) {
                    if (strcmp(parties[i], constituencyWinner) == 0) {
                        winnersCount[i]++;
                        break;
                    }
                }

                constituencyCount++;
            }
        }
    }
    fclose(file);

    // // Find the party with the maximum number of wins
    // int maxWins = 0;
    // int overallWinnerCount = 0; // Count of constituencies won by the overall winner
    // char overallWinner[MaxNameLength] = "";
    // for (int i = 0; i < numParties; i++) {
    //     if (winnersCount[i] > maxWins) {
    //         maxWins = winnersCount[i];
    //         strcpy(overallWinner, parties[i]);
    //         overallWinnerCount = winnersCount[i];
    //     }
    // }

    // printf("\nOverall Winner: %s (Won in %d out of %d constituencies)\n", overallWinner, overallWinnerCount, constituencyCount);
    // char name[100];
    // sprintf(name, "%s.txt", myConsituencyNameString);
    // FILE * fl = fopen(name, "w");
    // char str[100] ="";
    // sprintf(str,"%s,%d", filename2, numParties);
    // fputs(str, fl);
    // fclose(fl);
}

// to check if the state is in the CSV file
int stateCheck(const char *state, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open CSV file");
        return 0;
    }

    char line[MaxNameLength];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        if (token && strcmp(token, state) == 0) {
            fclose(file);
            return 1; // State is found in the list. 
        }
    }

    fclose(file);
    return 0; // State not found in the list. 
}


// to ask the user whether to continue voting or end the program
int askToContinue(const char *filename, char parties[MaxParties][MaxNameLength], int numParties) {
    char response;
    printf("\nWould you like to vote again? Enter [R] for revote or [E] to end: ");
    scanf(" %c", &response);

    if (response == 'R' || response == 'r') {
        return 1;
    } else if (response == 'E' || response == 'e') {
        printf("\nExiting the voting system.\n");
        printf("Printing Result...\n");
        showConstituencyResults(filename, parties, numParties);
        return 0;
    } else {
        printf("Invalid option. Please try again.\n");
        return askToContinue(filename, parties, numParties);
    }
}


int startVoting() {
    srand(time(NULL));
    // define the required variables
    char parties[MaxParties][MaxNameLength];
    char states[MaxRegions][MaxNameLength];
    int numParties, numStates;
    char filename[] = voteDataFileName;
    char detailsFile[] = userDataFileName;
    char temp[] = "A,B,C";
    numParties = setParties(partyNameString, parties);
    numStates = setConsituencies(allConsituencyNameString, states);


    createFile(filename, states, parties, numStates, numParties);
    // The procedure of voting begins. 
    while (1) {
        char electionID[MaxNameLength];
        char password[MaxNameLength];

        printf("\nEnter your election ID: ");
        scanf(" %[^\n]s", electionID);
        
        
        
        // Display the voting interface
        displayWindow(myConsituencyNameString, parties, numParties);
        
        char option;
        int partyChoice;
        
        // Get client choice
        printf("If you want to vote, press [S] to proceed: ");
        scanf(" %c", &option);
        
        if (option == 'S' || option == 's') {
            printf("\nEnter the party number you want to vote for: ");
            scanf("%d", &partyChoice);
            getchar();
            if (partyChoice >= 1 && partyChoice <= numParties) {
                
                updateVote(myConsituencyNameString, partyChoice - 1, filename);
                printf("Vote submitted successfully!\n");
            } else {
                printf("Invalid party number.\n");
            }
        } else if (option == 'R' || option == 'r') {
            // continue the process of coting
            continue;
        } else if (option == 'E' || option == 'e') {
            printf("\nExiting the voting system.\n");
            printf("Printing Result...\n");
            showConstituencyResults(filename, parties, numParties);
            // ending the process of voting
            break;
        } else {
            printf("Invalid option. Please try again.\n");
        }
        
        // Ask the user if they want to continue voting
        if (!askToContinue(filename, parties, numParties)) {
            break;
        }
    }
    
    return 0;
}

