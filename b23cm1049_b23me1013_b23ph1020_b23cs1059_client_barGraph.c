#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pbPlots.h"
#include "supportLib.h"

void bargraphthedata(FILE *ptr, int num, char * party) {
    int length = strlen(party);
    party[length - 1] = '\0';
    char line[1000];//using it to read strings in files
    int Votes;
    int storage[50];//array to store data of votes in the correct order
    int count=0;
    char ch;
    char temp[100];

    int found=0;//FLAG VARIABLE

    char States[50];
    char *StoreStates[50];
    int q=0;
    //scan first line
    fgets(line,1000,ptr);
    //TOKENIZE VARIABLE
    char *data=strtok(line,",");//strtok returns a pointer therefore a pointer has been alloted
    //COMMA ACTS AS THE DELIMITER OR A WAY TO RECOGNIZE TERMS

    while(fgets(line,1000,ptr)!=NULL){
        if(strstr(line,party)!=NULL){//we stringcompare because else the memory addresses of the strings will be compared
        found=1;
            //printf("%s",line);//checkpoint
            if(strlen(party)>6){
            data=strtok(NULL,",");//else it doesn't tackle the full string & takes any part of the string.
            }
           
            for(int i=0;i<num;i++){//CHANGE
                data=strtok(NULL,",");
                // printf("%s",data);//CHECKPOINT
                storage[count++]=atoi(data);//using atoi convert the data to integer
               
            }

           //CHECKPOINT FOR STORAGE
            //  for(int j=0;j<5;j++){
            //         printf("%d",storage[j]);
            //     }
            
            break;
        }
    }

    if(found==0){
        printf("not found\n");//checkpoint
    }
    else{
        //printf("\033[0;33m");//yellow colour
        printf("\nKindly open the PNG file to check the visuals of the stats.\n\n");
        //printf("\033[1;32m");
    }

    rewind(ptr);//TAKING POINTER BACK TO START OF THE FILE
    // Read the first line of the file
    char firstline[50];
    if (fgets(firstline, sizeof(firstline),ptr) != NULL) {
        // Tokenize the line and print each string
        char *token2 = strtok(firstline, ",");
        token2 = strtok(NULL, ",");//observe first word that is just the Title

        while (token2 != NULL) {
            //printf("%s\n", token2);//checkpoint
            StoreStates[q]=token2;//store the first updated value
            token2 = strtok(NULL, ",");
            q++;
        }
        //int l=strlen(token2);
        //token2[l-1]='\n';
    } else {
        printf("File is empty\n");
    }
    rewind(ptr);//pointer back to start

    for(int k=0;k<num;k++){//CHANGE
        printf("%d votes gained by party %d:%s\n",storage[k],k+1,StoreStates[k]);
    }
    // Convert integer array to double array
    double doubleArray[num];//CHANGE
    for (int i = 0; i < num; i++) {//CHANGE
        doubleArray[i] = (double)storage[i];
    }

    //Bar Graph Code
     wchar_t *title = L"Bar Plot Title";//wide character string 

    double X_Axis [num] ;
    for(int k=0;k<num;k++){
        X_Axis[k]=(double)k+1;
    }
	double Y_Axis [num] ;
    for(int k=0;k<num;k++){
        Y_Axis[k]=doubleArray[k];
    }
	_Bool success;
    _Bool success1;

	StartArenaAllocator();//internal barplotting system for memory allocation
    //creation of the canvas where image of bar plot will be made
	RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
    //stores error texts
	StringReference *ErrorText = CreateStringReference(L"", 0);
    //DrawText(canvasReference->image, 300, 30, title, wcslen(title), GetBlack());//didn't use this

   success = DrawBarPlot(canvasReference, 800, 600, Y_Axis, num,ErrorText);
  
    //Byte Array (canvasreference)stores binary data like pixels of image.
    if (success) {
        ByteArray *pngdata = ConvertToPNG(canvasReference->image);
        char str[100];
        sprintf(str, "%s.png", party);
        WriteToFile(pngdata, str);
        DeleteImage(canvasReference->image);
    } else {
        fprintf(stderr, "Error: ");
        for (int i = 0; i < ErrorText->stringLength; i++) {
            fprintf(stderr, "%c", ErrorText->string[i]);
        }
        fprintf(stderr, "\n");
    }

    FreeAllocations();
}

int calcBarGraph(char *fileName, int numParties, char * myConsituencyNameString) {
    FILE *ptr = fopen(fileName, "r");
    if (ptr == NULL) {
        printf("File couldn't be opened.\n");
        return 1;
    }

    printf("Horizontal Bar Graphs help us understand the stats of different political parties in various states.\n");
    printf("Furthermore, it helps parties to strategically plan based on their performance.\n\n");



    bargraphthedata(ptr, numParties,myConsituencyNameString);


    fclose(ptr);
    printf("Thank you for using our efficient Voting System. We hope you liked it!\n");
    return 0;
}

int main() {
    char myConsituencyNameString[100];
    printf("Enter Consituency Name: ");
    scanf("%s", myConsituencyNameString);
    char name[100];
    sprintf(name, "%s.txt", myConsituencyNameString);
    size_t len = strlen(myConsituencyNameString);
    if (len > 0 && myConsituencyNameString[len - 1] == '\n') {
        myConsituencyNameString[len - 1] = '\0';
    }
    FILE *f = fopen(name, "r");
    char line[100];
    char filename[100];
    int num = 0;
    int read;
    do {
        fgets(line, sizeof(line), f);
        char *token = strtok(line, ",");
        strcpy(filename, token);
        token = strtok(NULL, ",");
        num = atoi(token);
    } while (!feof(f));
    fclose(f);
    calcBarGraph(filename, num, myConsituencyNameString);
    return 0;
}
