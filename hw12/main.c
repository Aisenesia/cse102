#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Node {
void* data;
struct Node* next;
} Node;

typedef struct {
char type[20];
int ivals[1];
double svals[1];
} Asset1;

typedef struct {
char type[20];
double svals[2];
int ivals[2];
}Asset2;

typedef struct {
char type[20];
char string1[50];
char string2[50];
}Asset3;

typedef struct {
char type[20];
double value1;
float value2;
double value3;
}Asset4;

void fillLinkedList(Node** head);//create ll
void serializeLinkedList(Node* head);//write ll to fp
void deserializeLinkedList(Node** head);//read ll from fp to memory
void print_ll(Node* head);
Node* ll_last(Node* head);
void ll_free(Node* head);

int main(){
    srand(time(NULL));
    Node* ll = NULL;
    
    fillLinkedList(&ll);//generate ll
    serializeLinkedList(ll);//write to file 
    print_ll(ll);

    Node* newll = NULL;
    deserializeLinkedList(&newll);//read ll
    print_ll(newll);

    ll_free(ll);
    ll_free(newll);
}

void fillLinkedList(Node** head){
    int count = rand()%11+10;
    int type;
    Asset1* new1;
    Asset2* new2;
    Asset3* new3;
    Asset4* new4;
    for(int i = 0; i<count; i++){
        //printf("t");
        Node* newNode = malloc(sizeof(Node));
        type = rand()%4;
        switch (type){
        case 0:
            new1 = malloc(sizeof(Asset1));
            strcpy(new1->type, "Asset1");
            new1->ivals[0] = rand()%50;//dont know what ivals is
            new1->svals[0] =  ((double)(rand()%500))/10;
            newNode->data = new1;
            break;
        case 1:
            new2 = malloc(sizeof(Asset2));
            strcpy(new2->type, "Asset2");
            new2->ivals[0] = rand()%50; new2->ivals[1] = rand()%50;
            new2->svals[0] = ((double)(rand()%500))/10; new2->svals[1] = ((double)(rand()%500))/10;
            newNode->data = new2;
            break;
        case 2:
            new3 = malloc(sizeof(Asset3));
            strcpy(new3->type, "Asset3");
            strcpy(new3->string1, "tstring1");
            strcpy(new3->string2, "tstring2");
            newNode->data = new3;
            break;
        case 3:
            new4 = malloc(sizeof(Asset4));
            strcpy(new4->type, "Asset4");
            new4->value1 = ((double)(rand()%500))/10;
            new4->value2 = ((double)(rand()%100))/100;
            new4->value3 = ((double)(rand()%500))/10;
            newNode->data = new4;
            break;
        } newNode->next = NULL;
        if(*head==NULL) *head = newNode;
        else ll_last((*head))->next = newNode;
    }
}

void serializeLinkedList(struct Node* head) {
    FILE* fp = fopen("linkedlist.bin", "wb");
    struct Node* current = head;
    while (current != NULL) {
        Asset1* tmp1 = (Asset1*)(current->data);
        Asset2* tmp2 = (Asset2*)(current->data);
        Asset3* tmp3 = (Asset3*)(current->data);
        Asset4* tmp4 = (Asset4*)(current->data);
        if(strcmp(tmp1->type, "Asset1")==0){//others have switch except this because it crossed my mind later
            fwrite(tmp1->type, sizeof(tmp1->type), 1, fp);
            fwrite(tmp1->ivals, sizeof(tmp1->ivals), 1, fp);
            fwrite(tmp1->svals, sizeof(tmp1->svals), 1, fp);
        }
        else if(strcmp(tmp2->type, "Asset2")==0){
            fwrite(tmp2->type, sizeof(tmp2->type), 1, fp);
            fwrite(tmp2->ivals, sizeof(tmp2->ivals), 1, fp);
            fwrite(tmp2->svals, sizeof(tmp2->svals), 1, fp);
        }
        else if(strcmp(tmp3->type, "Asset3")==0){
            fwrite(tmp3->type, sizeof(tmp3->type), 1, fp);
            fwrite(tmp3->string1, sizeof(tmp3->string1), 1, fp);
            fwrite(tmp3->string2, sizeof(tmp3->string2), 1, fp);
        }
        else if(strcmp(tmp4->type, "Asset4")==0){
            fwrite(&tmp4->type, sizeof(tmp4->type), 1, fp);
            fwrite(&tmp4->value1, sizeof(tmp4->value1), 1, fp);
            fwrite(&tmp4->value2, sizeof(tmp4->value2), 1, fp);
            fwrite(&tmp4->value3, sizeof(tmp4->value3), 1, fp);            
        }
        else{
            printf("Invalid data type!\n");
            return;
        } current = current->next;
    } 
    fclose(fp);
}

void deserializeLinkedList(struct Node** head) {
    FILE* fp = fopen("linkedlist.bin", "rb");
    if (fp == NULL) {
        printf("Error, file couldn't be opened!\n"); 
        return;
    } Asset1* new1; Asset2* new2; Asset3* new3; Asset4* new4;
    char type[20];
    while (1) {
        if (fread(type, sizeof(type), 1, fp) != 1) {
            break; //end of file, break the loop
        } Node* newNode = (Node*)malloc(sizeof(Node));
        switch (type[5]){
            case '1':
                new1 = malloc(sizeof(Asset1));
                strcpy(new1->type, type);
                fread(new1->ivals, sizeof(new1->ivals), 1, fp);
                fread(new1->svals, sizeof(new1->svals), 1, fp);
                newNode->data = new1;
                break;
            case '2':
                new2 = malloc(sizeof(Asset2));
                strcpy(new2->type, type);
                fread(new2->ivals, sizeof(new2->ivals), 1, fp);
                fread(new2->svals, sizeof(new2->svals), 1, fp);
                newNode->data = new2;
                break;
            case '3':
                new3 = malloc(sizeof(Asset3));
                strcpy(new3->type, type);
                fread(new3->string1, sizeof(new3->string1), 1, fp);
                fread(new3->string2, sizeof(new3->string2), 1, fp);
                newNode->data = new3;
                break;
            case '4':
                new4 = malloc(sizeof(Asset4));
                strcpy(new4->type, type);
                fread(&new4->value1, sizeof(new4->value1), 1, fp);
                fread(&new4->value2, sizeof(new4->value2), 1, fp);
                fread(&new4->value3, sizeof(new4->value3), 1, fp);
                newNode->data = new4;
                break;
            default:
                printf("Invalid data type!\n");
                break;
        } newNode->next = NULL;
        if (*head == NULL) *head = newNode; 
        else ll_last(*head)->next = newNode;
    }
    
    fclose(fp);
}

void print_ll(Node* head) {
    char* type;
    Asset1* a1 = head->data;
    Asset2* a2 = head->data;
    Asset3* a3 = head->data;
    Asset4* a4 = head->data;
    while (head!=NULL) {
        type = ((Asset1*)(head->data))->type;
        switch (type[5]){
        case '1':
            a1 = head->data;
            printf("type: %s, ival: %d, sval: %.1lf\n", a1->type, a1->ivals[0], a1->svals[0]);
            break;
        case '2':
            a2 = head->data;
            printf("type: %s, ivals: %d %d, svals: %.1lf %.1lf\n", a2->type, a2->ivals[0], a2->ivals[1], a2->svals[0], a2->svals[1]);
            break;
        case '3':
            a3 = head->data;
            printf("type: %s, string1: %s, string2: %s\n", a3->type, a3->string1, a3->string2);
            break;
        case '4':
            a4 = head->data;
            printf("type: %s, value1: %.1lf, value2: %.2f, value3: %.1lf\n",a4->type, a4->value1, a4->value2, a4->value3);
            break;
        } head = head->next;
    }
    printf("\n");
}

Node* ll_last(Node* head){
    while(head != NULL){
        if(head->next == NULL){
            break;
        }head=head->next;
    } return head;
}

void ll_free(Node* head){
     while(head!=NULL){
        Node* temp = head;
        head=head->next;
        free(temp->data);
        free(temp);
    }
}