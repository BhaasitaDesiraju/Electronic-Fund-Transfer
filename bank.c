#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "setup.h"

#define TRUE 1

struct UserAccount *userAccounts;
struct Transaction transactions[1000];
int userCount = 0, transactionCount = 0, transactionIndex = 0;
int userAccountIds[1000], userAccountIndex=0;

//Split each line into tokens
int getTokens(char *line, char tokenArray[5][10]) {
    int numOfTokens = 0, i, j = 0;
    for (i = 0; TRUE; i++) {
        if (line[i] != ' ') {
            tokenArray[numOfTokens][j++] = line[i];
        } else {
            tokenArray[numOfTokens][j++] = '\0';//addAndUpdateUserAccount NULL
            numOfTokens++;
            j = 0;
        }
        if (line[i] == '\0')
            break;
    }
    return numOfTokens;
}

//read the input file into two structures - userAccounts and transactions
void readInputFile(char *fileName) {
    FILE *file;
    int lineLen = 50, numOfTokens;
    char line[lineLen];
    char tokenArray[5][10];
    int compare;
    //open the file
    file = fopen(fileName, "r");
    //check for unsuccessful open
    if (file == NULL) {
        printf("Error opening file\n");
    } else {
        //File opened successfully
        while (fgets(line, lineLen, file) != NULL) {
            //split the token
            numOfTokens = getTokens(line, tokenArray);
            compare = strcasecmp(&tokenArray[0][0], "Transfer");

            //check if the line starts with "Transfer
            if (compare == 0) {
                //convert each line into tokens - split by space - add to transaction
                //assign work to threads
                transactions[transactionCount].sender = atoi(tokenArray[1]);
                transactions[transactionCount].receiver = atoi(tokenArray[2]);
                transactions[transactionCount].amountToTransfer = atoi(tokenArray[3]);
                transactionCount = transactionCount + 1;
            } else {
                //add to userAccount - handled by main thread!!!
                userAccountIds[userAccountIndex] = atoi(tokenArray[0]);
                addAndUpdateUserAccount(userAccounts, atoi(tokenArray[0]), atoi(tokenArray[1]));
                userCount = userCount + 1;
                userAccountIndex = userAccountIndex + 1;
            }
        }
    }
    fclose(file);
}

//Process transactions using multiple threads
void *processTransaction(void *arg) {
    pthread_mutex_lock(&lock);
    int index = transactionIndex;
    //lock each thread while performing the operation
    int sender = transactions[index].sender;
    int receiver = transactions[index].receiver;
    int amountToTransfer = transactions[index].amountToTransfer;
    //Look up users and gets their balances
    int senderAccountBalance = findUserAccountBalance(userAccounts, sender);
    int receiverAccountBalance = findUserAccountBalance(userAccounts, receiver);
    // updates the balances
    senderAccountBalance = senderAccountBalance - amountToTransfer;
    receiverAccountBalance = receiverAccountBalance + amountToTransfer;
    addAndUpdateUserAccount(userAccounts, sender, senderAccountBalance);
    addAndUpdateUserAccount(userAccounts, receiver, receiverAccountBalance);
    transactionIndex = transactionIndex + 1;
    //unlock
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

//Print the final output
void printUsers() {
    int iter, key, val;
    for(iter=0; iter<userAccountIndex; iter++) {
        key = userAccountIds[iter];
        printf("%d ",key);
        val = findUserAccountBalance(userAccounts, key);
        printf("%d\n", val);
    }
}

//MAIN
int main(int argc, char **argv) {
    if(argc != 3) {
        printf("Invalid input format\nGive arguments in the following form:\n./bank inputFile numberOfThreads\n");
        return 1;
    }
    char *fileName = argv[1];
    userAccounts = createUserAccounts(1000);
    int numOfThreads = atoi(argv[2]);
    readInputFile(fileName);
    int index;
    pthread_t threadID[numOfThreads];
    int mutexStatus = pthread_mutex_init(&lock, NULL);
    if (mutexStatus != 0) {
        printf("\n Mutex Init failed\n");
        return 1;
    }
    //one thread is created for each transaction
    for (index = 0; index < numOfThreads; index++) {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&threadID[index], &attr, processTransaction, (void *) &index);
    }

//    Ending the threads
    for (index = 0; index < numOfThreads; index++) {
        pthread_join(threadID[index], NULL);
    }
    pthread_mutex_destroy(&lock);
    printUsers();
    return 0;
}