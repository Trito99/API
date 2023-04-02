#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



struct node {
    char *data;
    struct node *leftChild;
    struct node *rightChild;
};

struct vincoliPositivi {
    char character;
    int* positionsPresent;
    int* positionsNotPresent;
    int max;
    int number;
    int tempN;
};

bool search(char word[],char input[],int length,char target,char output[]) {

    bool ye=false;
    int count=0;

    for(int i=0;i<length;i++){
        if(word[i]==target) {
            count++;
        }
    }

    for(int i=0;i<length;i++){
        if(input[i]==target) {
            if(output[i]=='|' || output[i]=='+'){
                count--;
            }
        }
    }

    if(count>0) {
        ye = true;
    }

    return ye;
}

struct node* searchBst(char data[], struct node *root){
    if (root==NULL || strcmp(root->data,data)==0)
        return root;
    if (strcmp(root->data,data)>0)
        return searchBst(data,root->leftChild);
    else
        return searchBst(data,root->rightChild);
}

void createOutput(int length, char *chosenWord, char *input, char output[]){
    for(int i=0;i<length;i++){
        output[i]='0';
    }
    for(int i=0;i<length;i++){
        if(chosenWord[i]==input[i])
            output[i]='+';
    }
    for(int i=0;i<length;i++){
        if(output[i]!='+'){
            if(search(chosenWord,input,length,input[i],output))
                output[i]='|';
            else
                output[i]='/';
        }
    }
}

void insert(char data[], struct node *root,int number) {

    struct node * y = NULL;
    struct node * x = root;
    struct node * z = root + number;

    if (root->data==NULL){
        root->data=data;
    }else{
        while (x!=NULL) {
            y = x;
            if (strcmp(data, x->data) < 0)
                x = x->leftChild;
            else
                x = x->rightChild;
        }
        z->data = data;
        z->leftChild = NULL;
        z->rightChild = NULL;
        if (strcmp(data, y->data) < 0)
            y->leftChild = z;
        else
            y->rightChild = z;
    }
}

struct node* deleteNode(struct node* root,struct node* parent)
{
    struct node *x = NULL;
    struct node *y = NULL;

    struct node *father = parent;
    if (root->leftChild == NULL || root->rightChild == NULL) {
        y = root;
    } else {
        father=NULL;
        y = root->rightChild;
        while (y->leftChild != NULL) {
            father = y;
            y = y->leftChild;
        }
    }

    if(y->leftChild!=NULL)
        x=y->leftChild;
    else
        x=y->rightChild;

    if (father==NULL && y==root)
        return x;
    else if (father==NULL && y!=root)
        root->rightChild = x;
    else if (y == father->leftChild)
        father->leftChild=x;
    else
        father->rightChild=x;

    if (y!=root)
        root->data = y->data;

    return root;
}

struct node * delete(struct node *root, struct node *realRoot,struct node *parent,char* characterNotPresent,struct vincoliPositivi* charactersPresent,int number_of_found,int length,int * ind) {
    if (root == NULL) {
        return root;
    }
    delete(root->leftChild,realRoot,root,characterNotPresent,charactersPresent,number_of_found,length,ind);
    delete(root->rightChild,realRoot,root,characterNotPresent,charactersPresent,number_of_found,length,ind);



    while(*characterNotPresent!='*'){
        if(strchr(root->data,*characterNotPresent)!=NULL) {
            root = deleteNode(root, parent);
            return root;
        }
        characterNotPresent++;
    }
    for (int t = 0; t < number_of_found; t++) {
        struct vincoliPositivi *search = charactersPresent + t;

        for(int i=0;i<length;i++){
            if(search->positionsPresent!=NULL) {
                if (search->positionsPresent[i] != -1) {
                    if (root->data[search->positionsPresent[i]] != search->character) {
                        root = deleteNode(root, parent);
                        return root;
                    }
                }
            }
            if(search->positionsNotPresent!=NULL) {
                if (search->positionsNotPresent[i] != -1) {
                    if (root->data[search->positionsNotPresent[i]] == search->character) {
                        root = deleteNode(root, parent);
                        return root;
                    }
                }
            }
            if(search->max!=-1){
                int count=0;
                for(int s=0;s<length;s++){
                    if(root->data[s]==search->character){
                        count++;
                    }
                }
                if(count>search->max){
                    root = deleteNode(root, parent);
                    return root;
                }
            }
            if(search->number>0) {
                int count = 0;
                for (int s = 0; s < length; s++) {
                    if (root->data[s] == search->character) {
                        count++;
                    }
                }
                if (count < search->number) {
                    root = deleteNode(root, parent);
                    return root;
                }
            }
        }
    }
    *ind = *ind+1;
    return root;
}


void addWordsToSecondBst(int number,char *words,struct node *secondBst, int length, char* characterNotPresent,struct vincoliPositivi* charactersPresent,int number_of_found,int number_of_words) {
    for(int f=0;f<number;f++) {
        bool delete=false;
        char* temp = characterNotPresent;
        while(*temp!='*'){
            if(memchr(words,*temp,length)!=0)
                delete=true;
            temp++;
        }
        if(!delete) {
            for (int t = 0; t < number_of_found; t++) {
                struct vincoliPositivi *search = charactersPresent + t;
                for (int i = 0; i < length; i++) {
                    if (search->positionsPresent != NULL) {
                        if (search->positionsPresent[i] != -1) {
                            if (words[search->positionsPresent[i]] != search->character) {
                                delete = true;
                            }
                        }
                    }
                    if (search->positionsNotPresent != NULL) {
                        if (search->positionsNotPresent[i] != -1) {
                            if (words[search->positionsNotPresent[i]] == search->character) {
                                delete = true;
                            }
                        }
                    }
                    if (search->max != -1) {
                        int count = 0;
                        for (int s = 0; s < length; s++) {
                            if (words[s] == search->character) {
                                count++;
                            }
                        }
                        if (count > search->max) {
                            delete = true;
                        }
                    }
                    if (search->number > 0) {
                        int count = 0;
                        for (int s = 0; s < length; s++) {
                            if (words[s] == search->character) {
                                count++;
                            }
                        }
                        if (count < search->number) {
                            delete = true;
                        }
                    }
                }
            }
        }
        if (!delete)
            insert(words, secondBst,number_of_words-number+f);
        words += length + 1;
    }
}

struct node* copyBst(struct node* bst,struct node* secondBst,struct node* realRoot,int * ind){
    if (bst == NULL)
        return NULL;

    secondBst->data=bst->data;
    *ind = *ind+1;

    secondBst->leftChild = copyBst(bst->leftChild,realRoot+*ind,realRoot,ind);
    secondBst->rightChild = copyBst(bst->rightChild,realRoot+*ind,realRoot,ind);

    return secondBst;
}

struct node * updateSecondBst(int * ind, char temp[],struct node* secondBst, char* firstWord, int maxLength, int number_of_words,char* characterNotPresent,struct vincoliPositivi* charactersPresent,int number_of_found){
    *ind=0;
    secondBst=delete(secondBst,secondBst,NULL,characterNotPresent,charactersPresent,number_of_found,maxLength,ind);

    return secondBst;
}

void inOrder(struct node *root) {
    if (root == NULL) {
        return;
    }
    inOrder(root->leftChild);
    printf("%s\n", root->data);
    inOrder(root->rightChild);
}

int createVincoli(int number_of_found,struct vincoliPositivi* charactersPresent, int maxLength,char* input, char * charactersNotPresent, char * chosenWord,char* found){
    for (int t = 0; t < number_of_found; t++) {
        struct vincoliPositivi *search = charactersPresent + t;
        search->tempN=0;
    }
    for (int i = 0; i < maxLength; i++) {
        if (input[i] == '+') {
            struct vincoliPositivi *present = NULL;
            for (int t = 0; t < number_of_found; t++) {
                struct vincoliPositivi *search = charactersPresent + t;
                if (search->character == found[i]) {
                    present = search;
                    t = number_of_found;
                }
            }
            if (present == NULL) {
                struct vincoliPositivi *tempV = charactersPresent + number_of_found;
                tempV->character = found[i];
                tempV->max = -1;
                tempV->tempN++;
                int *p = (int *) malloc(maxLength * sizeof(int));
                memset(p, -1, maxLength * sizeof(int));
                p[i] = i;
                tempV->positionsPresent = p;
                tempV->positionsNotPresent = NULL;
                number_of_found++;
            } else {
                present->tempN++;
                if (present->positionsPresent == NULL) {
                    int *p = (int *) malloc(maxLength * sizeof(int));
                    memset(p, -1, maxLength * sizeof(int));
                    p[i] = i;
                    present->positionsPresent = p;
                } else {
                    if (present->positionsPresent[i] == -1) {
                        present->positionsPresent[i] = i;
                    }
                }
            }
        } else if (input[i] == '|') {
            struct vincoliPositivi *present = NULL;
            for (int t = 0; t < number_of_found; t++) {
                struct vincoliPositivi *search = charactersPresent + t;
                if (search->character == found[i]) {
                    present = search;
                    t = number_of_found;
                }
            }
            if (present == NULL) {
                struct vincoliPositivi *tempV = charactersPresent + number_of_found;
                tempV->character = found[i];
                tempV->max = -1;
                tempV->tempN++;
                int *p = (int *) malloc(maxLength * sizeof(int));
                memset(p, -1, maxLength * sizeof(int));
                p[i] = i;
                tempV->positionsNotPresent = p;
                tempV->positionsPresent = NULL;
                number_of_found++;
            } else {
                present->tempN++;
                if (present->positionsNotPresent == NULL) {
                    int *p = (int *) malloc(maxLength * sizeof(int));
                    memset(p, -1, maxLength * sizeof(int));
                    p[i] = i;
                    present->positionsNotPresent = p;
                } else {
                    if (present->positionsNotPresent[i] == -1) {
                        present->positionsNotPresent[i] = i;
                    }
                }
            }
        }
    }
    for (int t = 0; t < number_of_found; t++) {
        struct vincoliPositivi *search = charactersPresent + t;
        if(search->tempN>search->number)
            search->number=search->tempN;
    }
    for (int i = 0; i < maxLength; i++) {
        if (input[i] == '/') {
            struct vincoliPositivi *present = NULL;
            for (int t = 0; t < number_of_found; t++) {
                struct vincoliPositivi *search = charactersPresent + t;
                if (search->character == found[i]) {
                    present = search;
                    t = number_of_found;
                }
            }
            if (present == NULL) {
                if (memchr(charactersNotPresent, found[i], 64) == NULL) {
                    char *tempC = charactersNotPresent;
                    while (*tempC != '*')
                        tempC++;
                    *tempC = found[i];
                }
            } else {
                int count = 0;
                for (int p = 0; p < maxLength; p++) {
                    if (chosenWord[p] == found[i]) {
                        count++;
                    }
                }
                present->max = count;
                if (present->positionsNotPresent == NULL) {
                    int *p = (int *) malloc(maxLength * sizeof(int));
                    memset(p, -1, maxLength * sizeof(int));
                    p[i] = i;
                    present->positionsNotPresent = p;
                } else {
                    if (present->positionsNotPresent[i] == -1) {
                        present->positionsNotPresent[i] = i;
                    }
                }
            }
        }
    }
    return number_of_found;
}


int main() {
    int maxLength=0;
    char *words = (char *)malloc(100000000);
    memset(words,0,100000000);
    struct node *bst = (struct node *)malloc(6000000*sizeof(struct node));
    memset(bst,0,6000000*sizeof(struct node));
    int *index = (int *)malloc(4);
    memset(index,0,2);
    char *firstWord = words;
    char *charactersNotPresent = (char *)malloc(64);
    memset(charactersNotPresent,'*',64);
    struct vincoliPositivi *charactersPresent = (struct vincoliPositivi *)malloc(64*sizeof(struct vincoliPositivi));
    memset(charactersPresent,0,64*sizeof(struct vincoliPositivi));
    int number_of_words = 0,number_of_added_words = 0, number_of_found = 0;

    struct node *secondBst = (struct node *)malloc(6000000*sizeof(struct node));
    struct node *firstBST = secondBst;
    memset(secondBst,0,6000000*sizeof(struct node));//1562500; //100.000.000/32 * 0,5
    char s[100];
    int limit=-1;
    char phase ='B';
    int n=0;
    char *end;
    int tempo=0;

    end=fgets(s, 99, stdin);
    maxLength = atoi(s);
    char temp[maxLength+1];
    char* chosenWord = (char *)malloc(maxLength+1);
    char* output = (char *)malloc(maxLength+1);
    memset(output,0,maxLength+1);
    memset(chosenWord,0,maxLength+1);


    while(1) {
        end=fgets(s, 99, stdin);
        //printf("%i, %i",number_of_inputs,number_of_words);
        if(end==NULL) {
            free(bst);
            free(firstBST);
            free(firstWord);
            free(index);
            free(charactersPresent);
            free(charactersNotPresent);
            free(output);
            free(chosenWord);
            return 0;
        }

        if (s[0] == '+' && s[1] == 'n') {
            phase = 'M';
            n=0;
            number_of_found=0;
        }else if(s[0] == '+' && s[1] == 'i' && s[11]=='i'){
            tempo = n;
            if(phase=='M'){
                phase = 'A';
                n=-2;
            }else {
                phase = 'A';
                n = -3;
            }
        }else if(s[0] == '+' && s[1] == 'i' && s[11]=='f'){
            if(n==-2){
                phase='M';
            }else {
                phase = 'e';
            }

            addWordsToSecondBst(number_of_added_words, words-number_of_added_words*(maxLength+1), secondBst, maxLength,charactersNotPresent,charactersPresent,number_of_found,number_of_words);
            n=tempo;
            number_of_added_words=0;
        }

        if (phase == 'B') {
            strncpy(words, s, maxLength);
            insert(words,bst,number_of_words);
            words += (maxLength + 1);
            number_of_words++;
        } else if(phase =='M' && s[0] != '+') {
            if (n == 0) {
                strncpy(chosenWord, s, maxLength);
            } else if (n == 1) {
                limit = atoi(s);
                *index=0;
                secondBst = copyBst(bst,firstBST,firstBST,index);
            } else if(n>1) {
                s[maxLength] = '\0';
                strncpy(temp, s, maxLength + 1);
                struct node *found = searchBst(temp, bst);

                if (found != NULL) {
                    if (strcmp(chosenWord, temp) != 0) {
                        createOutput(maxLength,chosenWord,temp,output);
                        number_of_found = createVincoli(number_of_found,charactersPresent,maxLength,output,charactersNotPresent,chosenWord,temp);
                        secondBst = updateSecondBst(index, temp, secondBst, firstWord, maxLength, number_of_words,charactersNotPresent, charactersPresent,number_of_found);
                        printf("%s\n%i\n", output, *index);
                        limit--;
                    } else {
                        limit = -2;
                    }
                } else if (found == NULL && temp[0] != '+') {
                    printf("not_exists\n");
                }


                if (limit <=0) {
                    if (limit==0)
                        printf("ko\n");
                    else
                        printf("ok\n");
                    phase = 'e';
                    for(int i=0;i<number_of_found;i++){
                        struct vincoliPositivi* search = charactersPresent+i;
                        if (search->positionsPresent != NULL)
                            free(search->positionsPresent);
                        if (search->positionsNotPresent != NULL)
                            free(search->positionsNotPresent);
                    }

                    memset(charactersNotPresent,'*',64);
                    memset(charactersPresent,0,64*sizeof(struct vincoliPositivi));
                    memset(firstBST,0,number_of_words*sizeof(struct node));
                }
            }
            n++;
        }else if (phase == 'A'&& s[0] != '+'){
            strncpy(words, s, maxLength);
            insert(words,bst,number_of_words);
            words += (maxLength + 1);
            number_of_words++;
            number_of_added_words++;
        }else if (s[0] == '+' && s[1] == 's') {
            inOrder(secondBst);
        }
    }
}