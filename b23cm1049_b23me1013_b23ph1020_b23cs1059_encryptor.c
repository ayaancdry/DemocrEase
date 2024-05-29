#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 62 // 26 letters (uppercase and lowercase) + 10 digits + symbols

// Function to encrypt a message using a Caesar cipher with a specified key
char * encrypt(char *message, int key) {
    char * tempMessage = (char *)malloc(strlen(message) * sizeof(char));
    strcpy(tempMessage, message);
    for (int i = 0; tempMessage[i] != '\0'; i++) {
        if (isalnum(tempMessage[i])) {
            if (isalpha(tempMessage[i])) {
                if (isupper(tempMessage[i])) {
                    tempMessage[i] = 'A' + (tempMessage[i] - 'A' + key) % 26;
                } else {
                    tempMessage[i] = 'a' + (tempMessage[i] - 'a' + key) % 26;
                }
            } else if (isdigit(tempMessage[i])) {
                tempMessage[i] = '0' + (tempMessage[i] - '0' + key) % 10;
            }
        }
    }
    return tempMessage;
}

// Function to decrypt a message encrypted with the Caesar cipher using the same key
char * decrypt(char *message, int key) {
    char * tempMessage = (char *)malloc(strlen(message) * sizeof(char));
    strcpy(tempMessage, message);
    for (int i = 0; tempMessage[i] != '\0'; i++) {
        if (isalnum(tempMessage[i])) {
            if (isalpha(tempMessage[i])) {
                if (isupper(tempMessage[i])) {
                    tempMessage[i] = 'A' + (tempMessage[i] - 'A' - key + 26 * 100) % 26;
                } else {
                    tempMessage[i] = 'a' + (tempMessage[i] - 'a' - key + 26 * 100) % 26;
                }
            } else if (isdigit(tempMessage[i])) {
                tempMessage[i] = '0' + (tempMessage[i] - '0' - key + 10 * 100) % 10;
            }
        }
    }
    return tempMessage;
}


/*
int main() {
    return 0;
}*/
