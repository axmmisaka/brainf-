#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define PREV -1
#define NEXT 1

// Node serves as an linked list than can be used in both data and code storage
struct node{
    int data;
    struct node* prev;
    struct node* next;
};

struct node* initNode(void){
    struct node* newptr = malloc(sizeof(struct node));
    newptr -> data = 0;
    newptr -> prev = NULL;
    newptr -> next = NULL;
    return newptr;
}

//Give a node, and returns a new pointer that is the previous/next node of the given one
struct node* move(struct node* pointer, int direction){
    if (direction == PREV){
        //see if there's previous node
        if (pointer->prev == NULL){
            struct node* newptr = initNode();
            newptr -> next = pointer;
            pointer -> prev = newptr;
            pointer = newptr;
        }
        else {
            pointer = pointer->prev;
        }
    }
    //same for this
    if (direction == NEXT){
        if (pointer->next == NULL){
            struct node* newptr = initNode();
            newptr -> prev = pointer;
            pointer -> next = newptr;
            pointer = newptr;
        }
        else {
            pointer = pointer->next;
        }
    }
    return pointer;
}

int change(struct node* ptr, int changeValue){
    ptr -> data += changeValue;
    return 0;
}

int inputVaule(struct node* ptr){
    scanf("%d",&(ptr -> data));
    return 0;
}

int outputValue(const struct node* ptr){
    printf("%d", ptr -> data);
    return 0;
}

//Returns the length of string from given '[' to corresponding ']'
int findSubProg(const char * programCode,
                int startPos, //This should be the position of '[',
                int* returnLength // a int pointer that contains the length of subprogram string
                ){
    int stack = 0;
    int position;
    const int totalLen = strlen(programCode);
    position = startPos;
    do{
        if (position == totalLen){//Already out of bound
            return EXIT_FAILURE;
        }
        if (programCode[position] == '['){
            stack += 1;
        }
        else if (programCode[position] == ']'){
            stack -= 1;
        }
        position += 1;
   } while (stack > 0);

    int length = position - startPos;
    *returnLength = length;

    return EXIT_SUCCESS;
}

char* substring(const char* originalString,
                int startPos,
                int length
                ){
    char* result;
    result = (char*)malloc(sizeof(char) * (length + 1));//Added one for the '\0'
    //strncpy(subCode, programCode + startPos, length); //Can't use strncpy - experience SIGSEGV for no reason
    sprintf(result, "%.*s", length, originalString + startPos);
    return result;
}

int printData(const struct node* dataNode){
    printf("%c", dataNode -> data);
    return EXIT_SUCCESS;
}

int inputData(struct node* dataNode){
    char temp;
    scanf("%c", &temp);
    dataNode -> data = temp;
    return EXIT_SUCCESS;
}

//Give code and code data, print the code, and the current position will be red
void visualizeCode(const char* code, int codePosition){
    int totalLength = strlen(code);
    for(int i = 0; i < totalLength; i++){
        if(!(codePosition - i)){
            printf(ANSI_COLOR_RED);
        }
        printf("%c", code[i]);
        if(!(codePosition - i)){
            printf(ANSI_COLOR_RESET);
        }
    }
    printf("\n");
}

//Give one of the data node, this function will print all data node with the given one in red
void visualizeData(struct node* data){
    struct node* ptr = data;
    //find the head of the linked list
    while(ptr -> prev != NULL){
        ptr = ptr -> prev;
    }
    //output
    while(ptr != NULL){
        if(ptr == data){
            printf(ANSI_COLOR_RED);
        }
        printf("%d ", ptr->data);
        if(ptr == data){
            printf(ANSI_COLOR_RESET);
        }
        ptr = ptr -> next;
    }
    printf("\n");
}

//give a program code and pointer pointing to a data node pointer
//pointer should be pointing a node (which is a struct node*) in the data linked list
int interprete(const char* programCode, struct node ** ptr){
    int codePosition = 0;
    while(programCode[codePosition] != '\0'){
        //visualizeCode(programCode, codePosition);
        //visualizeData(*ptr);
        //getchar();
        switch (programCode[codePosition]) {
        case '<':
            *ptr = move(*ptr, PREV);
            codePosition++;
            break;
        case '>':
            *ptr = move(*ptr, NEXT);
            codePosition++;
            break;
        case '+':
            change(*ptr, 1);
            codePosition++;
            break;
        case '-':
            change(*ptr, -1);
            codePosition++;
            break;
        case '.':
            printData(*ptr);
            codePosition++;
            break;
        case ',':
            printData(*ptr);
            codePosition++;
            break;
        case '[':{
            int length = 0;
            findSubProg(programCode, codePosition, &length);
            while((*ptr)->data){
                char* newProg;
                newProg = (char*)malloc(sizeof(char) * length);
                strcpy(newProg, substring(programCode, codePosition + 1, length - 2));//omit [ and ]
                interprete(newProg, ptr);
            }
            codePosition += length;
            break;
        }
        case ']':
            codePosition++;
            break;
        }
    }
    return 0;
}


int main(int argc, char * argv[]){
    char a[1000] = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    struct node* newptrptr = initNode();
    interprete(a, &newptrptr);
    visualizeData(newptrptr);
    return 0;
}
