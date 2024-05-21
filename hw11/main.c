//incomplete

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    char name[20];
    int age;
    struct Node* father;
    struct Node* mother;
}Node;

typedef struct Family{
    int count;
    Node* cArr[20];
}Family;

char* loadMenu();
void loadFamilyTree(char* filename);
void addPerson(Family* root, char* name, int age, char* motherName, char* fatherName);
Node* createNode(char* name, int age, Node* mother, Node* father);
Node* findPerson(Node* root, char* name);

int main(){
    FILE *fp;
    Family* fam = NULL;
    char selection, filename[20], selection2, name[20], mother[20], father[20];
    int age;
    do{
        printf("Family Tree System Menu\n");
        printf("1. Create a Family Tree\n");
        printf("2. Remove a Family Tree\n");
        printf("3. Load a Family Tree\n");
        printf("4. Exit\n");
        scanf(" %c", &selection);
        switch (selection){
        case '1'://add
            do{
                printf("1-Add Node\n");
                printf("2-Save Family Tree\n");
                printf("3-Exit\n");
                scanf(" %c", &selection2);
                switch (selection2){
                case '1':
                    printf("Enter the name of the person: "); scanf(" %[^\n]%*c", name);
                    printf("Enter the age of the person: "); scanf(" %d", &age);
                    printf("Enter the name of mother: "); scanf("%[^\n]%*c", mother);
                    printf("Enter the name of the father: "); scanf("%[^\n]%*c", father);
                    //function to add person to family, will create new node in function
                    addPerson(fam, name, age, mother, father);

                    break;
                case '2':
                    printf("Enter filename: ");
                    scanf("%[^\n]%*c", filename);
                    fp = fopen(filename, "w");
                    //traverse and write to file
                    break;
                case '3':
                    break;                
                default:
                    while(getchar()!='\n');
                    break;
                }
            }while(selection2 != '3');
            //printf("")
            //scanf("%[^\n]%*c", filename);
            //FILE *fp;
            //fp = fopen(filename, "w");
            break;
        case '2'://remove
            printf("Enter the name of the txt file of the family tree: ");
            scanf(" %[^\n]%*c", filename);
            remove(filename);
            break;
        case '3'://load
            loadFamilyTree(loadMenu());
            break;
        case '4':
            break;
        default:
            while(getchar()!='\n');
            break;
        }
    }while(selection != '4');
}

char* loadMenu(){
    printf("Listing family trees:\n");
    char* filename = malloc(20 * sizeof(char));
    FILE *fp;
    int i = 1, ctr = 0;
    int selection;
    while(i<20){
        sprintf(filename, "Family%d.txt", i);
        fp = fopen(filename, "r");
        if(fp){
            ctr++;
            printf("%d-%s\n", ctr, filename);
        } i++;
    } 
    if (ctr == 0) printf("No family tree file is present!\n");
    else{
        scanf(" %d", &selection);
        return filename;
    } return NULL;
}

void loadFamilyTree(char* filename){
    FILE *fp;
    if(filename==NULL) return;
    fp = fopen(filename, "r");
    free(filename);
    if(!fp) return;
    //file read
    char selection, name[20], selection2;
    do{
        printf("1-Add a new person\n");
        printf("2-Remove a person\n");
        printf("3-Print a Person's nuclear family\n");
        printf("4-Search Relatives of Given Person\n");
        printf("5-Return to the main menu\n"); 
        switch (selection){
        case '1'://add person
            break;
        case '2'://remove person
            break;
        case '3'://nuclear family
            printf("Enter the name of the person: "); scanf("%[^\n]%*c", name);
            printf("Printing Nuclear family of %s:\n", name);
            //function to print nuclear family
            break;
        case '4'://relative search
            printf("Select type of relatives:\n");
            printf("1-Parents\n");
            printf("2-Spouse\n");
            printf("3-Children\n");
            printf("4-Grandparents\n");
            printf("5-Cousins\n");
            scanf(" %c", &selection2);
            //Another switch or function
            break;
        case '5':
            break;
        default:
            while (getchar()!='\n');
            break;
        }
    }while(selection != 5);
}

Node* createNode(char* name, int age, Node* mother, Node* father) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->name, name);
    newNode->age = age;
    newNode->mother = mother;
    newNode->father = father;
    return newNode;
}

Node* findPerson(Node* root, char* name) {
    if (root == NULL) return NULL;
    if (strcmp(root->name, name) == 0) return root;
    Node* foundInMother = findPerson(root->mother, name);
    if (foundInMother != NULL) return foundInMother;
    return findPerson(root->father, name);
}

void addPerson(Family* root, char* name, int age, char* motherName, char* fatherName) {
    int i = 0;
    Node* mother = NULL;
    Node* father = NULL;
    while(i < root->count){
        if(mother==NULL) mother = findPerson(root->cArr[i], motherName);
        if(father == NULL) father = findPerson(root->cArr[i], fatherName);
        i++;
    } root->count++;    
    Node* newNode = createNode(name, age, mother, father);
    root->cArr[root->count] = newNode;
}
