#ifndef SETUP_H
#define SETUP_H

//Declaring pthreads
pthread_mutex_t lock;

//structure 1 - userAccount
struct UserMap{
    int key;
    int val;
    struct UserMap *next;
};

struct UserAccount{
    int size;
    struct UserMap **list;
};

struct UserAccount *createUserAccounts(int size){
    struct UserAccount *userTable = (struct UserAccount*)malloc(sizeof(struct UserAccount));
    userTable->size = size;
    userTable->list = (struct UserMap**)malloc(sizeof(struct UserMap*) * size);
    int i;
    for(i = 0; i<size; i++)
        userTable->list[i] = NULL;
    return userTable;
}

int hashCode(struct UserAccount *t, int key){
    if(key<0)
        return -(key%t->size);
    return key%t->size;
}

void addAndUpdateUserAccount(struct UserAccount *userTable, int accountNum, int accountBalance){
    int index = hashCode(userTable, accountNum);
    struct UserMap *list = userTable->list[index];
    struct UserMap *newUser = (struct UserMap*)malloc(sizeof(struct UserMap));
    struct UserMap *tempUser = list;
    while(tempUser){
        if(tempUser->key == accountNum){
            tempUser->val = accountBalance;
            return;
        }
        tempUser = tempUser->next;
    }
    newUser->key = accountNum;
    newUser->val = accountBalance;
    newUser->next = list;
    userTable->list[index] = newUser;
}

int findUserAccountBalance(struct UserAccount *t, int key){
    int pos = hashCode(t,key);
    struct UserMap *list = t->list[pos];
    struct UserMap *temp = list;
    while(temp){
        if(temp->key == key){
            return temp->val;
        }
        temp = temp->next;
    }
    return -1;
}

//structure 2 - transaction
struct Transaction {
    int sender;
    int receiver;
    int amountToTransfer;
};

#endif //SETUP_H
