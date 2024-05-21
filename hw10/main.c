#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char *isbn, *title, *author, status;
    int year;
}book;

typedef struct{
    char *name, **isbns;
    unsigned int id;
    int borrowed;
}student;

typedef struct list{
  book* data;
  student* sdata;
  struct list* next;
}list;

/*
    Homework descriptions werent specifying at all, so i've done everything as i wanted. I hope that doesnt arise any problems
    Because i had a lot of fun with this homework, being free to do as i want i had the opportunity try new things
*/


list* addBook(list* l, char* isbn, char* title, char* author, int publicationYear, int method, char status);
list* addStudent(list* l, char* name, unsigned int id, int borrowedCount, char** isbns);
list* deleteBook(list* l, char* isbn);
list* deleteStudent(list* l, list* books, unsigned int id);
void readFile(list** books, int* bookCount, list** students, int* studentCount);
void updateBook(list* l, char* isbn, char* feature, char* value);
list* last(list* l);
void filterAndSortBooks(list* l, int filterChoice, char* filter, int sortChoice);
void display(list* l);
void freeBook(list* l);//frees memory before exiting
void freeStudents(list* l);//frees memory before exiting
void swap(list* a, list* b);
void sort(list* start, int sortChoice);
list* reverseList(list* l);
list* findBook(list* l, char* isbn);
list* findStudent(list* l, unsigned int id);
void borrowBook(list* l, list* s, char* isbn, unsigned int id);
void returnBook(list* l, list* s, char* isbn, unsigned int id);
void writeFile(list* l, list* s);

int main(){//menu and function calls
    int bookCount = 0, studentCount = 0;
    list *l = NULL;
    list *stu = NULL;
    readFile(&l, &bookCount, &stu, 0);
    int selection, year, method;
    char isbn[20], title[40], author[40], name[40], feat[20], val[40];
    unsigned int id;
    do{
        printf("1 - Add Book\n2 - Remove Book\n3 - Update Book\n4 - Add Student\n5 - Remove Student\n6 - Filter and Sort\n7 - Reverse List\n8 - Borrow Book\n9 - Return Book\n10 - List of a students borrowed books\n11 - Display\n0 - Exit\n");
        scanf("%d", &selection);
        switch (selection){
        case 1://Add books
            printf("ISBN: "); scanf(" %[^\n]%*c", isbn);
            printf("Title: "); scanf(" %[^\n]%*c", title);
            printf("Author: "); scanf(" %[^\n]%*c", author);
            printf("Year: "); scanf(" %d", &year);
            printf("Method (FIFO: 0 LIFO: 1): "); scanf(" %d",&method);
            l = addBook(l, isbn, title, author, year, method, 'A');
            break;
        case 2://remove books
            printf("ISBN: ");
            scanf(" %[^\n]%*c", isbn);
            l = deleteBook(l, isbn);
            break;
        case 3://update book
            printf("ISBN: "); scanf(" %[^\n]%*c", isbn); 
            printf("Feature to change:"); scanf(" %[^\n]%*c", feat);
            printf("Changed to:");
            scanf(" %[^\n]%*c", val);  
            updateBook(l, isbn, feat, val);
            break;
        case 4://add student
            printf("Name: ");
            scanf(" %[^\n]%*c", name);
            printf("ID: "); scanf(" %u", &id);
            stu = addStudent(stu, name, id, 0, NULL);
            break;
        case 5://remove student
            printf("ID: "); scanf(" %u", &id);
            stu = deleteStudent(stu, l, id);
            break;
        case 6://filterandsortbooks
            int filterChoice, sortChoice;
            char filter[40];
            printf("Filter Choice (author: 0, year: 1): "); scanf(" %d", &filterChoice);
            printf("Filter:"); scanf(" %[^\n]%*c", filter);
            printf("Sort Choice (title: 0, author: 1, year: 2):"); scanf(" %d", &sortChoice);
            filterAndSortBooks(l, filterChoice, filter, sortChoice);
            break;
        case 7://reverse list of books
            l = reverseList(l);
            display(l);
            break;
        case 8://borrow book
            printf("ID: "); scanf(" %u", &id);
            printf("ISBN: "); scanf(" %[^\n]%*c", isbn);
            borrowBook(l, stu, isbn, id);
            break;
        case 9://return borrowed book
            printf("ID: "); scanf(" %u", &id);
            printf("ISBN: "); scanf(" %[^\n]%*c", isbn);
            returnBook(l, stu, isbn, id);
            break;
        case 10://display borrowed books of student
            list* temp;
            printf("ID: "); scanf(" %u", &id);
            temp = findStudent(stu, id);
            if(temp==NULL){
                printf("Couldnt find student!\n");
                break;
            }
            if(temp->sdata->borrowed == 0){ 
                printf("No borrowed books!\n");
                break;
            }
            printf("Books Borrowed by student: \n");
            for(int i=0; i<temp->sdata->borrowed; i++){
                printf("->%s - %s\n", temp->sdata->isbns[i], findBook(l, temp->sdata->isbns[i])->data->title);
            }
            //findStudent(l, id)->sdata->isbns
            break;
        case 11://print all
            display(l); printf("\n");
            display(stu); printf("\n");
            break;
        case 0://exit
            break;
        default:
            printf("Invalid selection\n"); while(getchar()!='\n');
            break;
        } writeFile(l, stu);//write changes to file
        
    }while(selection!=0);
    freeBook(l);
    freeStudents(stu);
}

void display(list* l){//prints
    while(l!=NULL){
        if(l->data !=NULL){
            printf("%s - %s - %s - %d - %c  \n" ,l->data->isbn, l->data->title, l->data->author, l->data->year, l->data->status);
        } else if(l->sdata != NULL){
            printf("%d - %s\n", l->sdata->id, l->sdata->name);
        } l = l->next;
    }
}

list* last(list* l){
    while(l != NULL){
        if(l->next == NULL){
            break;
        }l=l->next;
    }
    return l;
}

void swap(list* a, list* b){
    book* temp = a->data;
    a->data = b->data;
    b->data = temp;
}
//in file i used | as its very unlikely to be in a books name
void readFile(list** books, int* bookCount, list** students, int* studentCount){
    FILE* fp = fopen("data.txt", "r");
    if (fp == NULL){
        printf("Error opening the file!\n");
        return;
    }
    char isbn[20], title[40], author[40], status, name[40], **isbns;
    int year, count;
    unsigned int id;
    char line[200];
    while (fgets(line, sizeof(line), fp) != NULL){
        if (line[0] == 'B'){
            sscanf(line, "%*c|%[^|]|%[^|]|%[^|]|%d|%c", isbn, title, author, &year, &status);
            *books = addBook(*books, isbn, title, author, year, 0, status);
        } else if (line[0] == 'S'){
            sscanf(line, "%*c|%[^|]|%d|%d|%[^\n]", name, &id, &count, line);
            isbns = malloc(count * sizeof(char*)); // Allocate memory for isbns
            for (int i = 0; i<count; i++){
                isbns[i] = malloc(20 * sizeof(char)); // Allocate memory for each isbn
                sscanf(line, "%[^|]|%[^\n]", isbns[i], line);
                isbns[i] = realloc(isbns[i], strlen((isbns[i])+1) * sizeof(char));//shrink array to make it memory efficient
            }//What im doing here is recursive-like sscan, i scan 2 elements at each time first is one isbn element other is whatever is left.
            //So when i scan whatever i left in the line back into line i get an effect just like in files, whatever i scanned is removed from line
            (*students) = addStudent(*students, name, id, count, isbns);
            //(*students)->sdata->borrowed = count;
        }
    }
    fclose(fp);
}

list* addBook(list* l, char* isbn, char* title, char* author, int publicationYear, int method, char status){
    list* newNode = malloc(sizeof(list));
    book* newData = malloc(sizeof(book));
    newData->isbn = malloc(strlen(isbn) + 1);
    strcpy(newData->isbn, isbn);
    newData->title = malloc(strlen(title) + 1);
    strcpy(newData->title, title);
    newData->author = malloc(strlen(author) + 1);
    strcpy(newData->author, author);
    newData->year = publicationYear;
    newData->status = status;
    newNode->data = newData;
    switch(method){
    
    case 0://insert end, fifo
        newNode->next=NULL;
        if(l==NULL){
            l=newNode;
        }
        last(l)->next = newNode;
        newNode->next = NULL;
        return l;
    case 1://insert beginning, lifo
        newNode->next = l;
        return newNode;
        break;
    default:
        return NULL;
        break;
    } 
}

list* addStudent(list* l, char* name, unsigned int id,  int borrowedCount, char** isbns){//always lifo this causes student list to reverse at each write was too lazy to do fifo for this
    list* newNode = malloc(sizeof(list));
    student* newData = malloc(sizeof(student));
    newData->id = id;
    newData->isbns = isbns;
    newData->name = malloc(strlen(name)+1);
    strcpy(newData->name, name);
    newData->borrowed = borrowedCount;
    newNode->sdata = newData;
    newNode->next = l;
    return newNode;
}

list* deleteBook(list* l, char* isbn){
    list *prev = l, *current = l;
    while(current != NULL && (strcmp(current->data->isbn, isbn))){
        prev = current;
        current = current->next;
    }
    if(current!=NULL){
        if(current->data->status=='R'){
            printf("Borrowed Books cant be deleted!\n");
            return l;
        }
        if(prev!=current) prev->next = current->next; 
        else l=l->next;
        free(current->data->author);
        free(current->data->title);
        free(current->data->isbn);
        free(current->data);
        free(current);
    }
    return l;
}

list* deleteStudent(list* l, list* books, unsigned int id){
    list *prev = l, *current = l;
    list* temp;
    while(current != NULL){
        if((current->sdata->id==id)) break; 
        prev = current;
        current = current->next;
    }
    if(current!=NULL){
        if(prev!=current) prev->next = current->next; 
        else l=l->next;
        free(current->sdata->name);
        for(int i = 0; i<current->sdata->borrowed; i++){
            findBook(books, current->sdata->isbns[i])->data->status = 'A';//when deleting a student i return books that student borrowed 
            free(current->sdata->isbns[i]);
        } free(current->sdata->isbns);
        free(current->sdata);
        free(current);
    }
    return l;
}

void freeBook(list* l){//free memory
    list* p = l;
    while (l != NULL){
        p = l;
        l = l->next;
        if (p->data != NULL){
            free(p->data->author);
            free(p->data->title);
            free(p->data->isbn);
            free(p->data);
        } free(p);
    }
}

void freeStudents(list* l){//free memory
    list* p = l;
    while (l != NULL){
        p = l;
        l = l->next;
        if (p->sdata != NULL){
            free(p->sdata->name);
            for (int i = 0; i < p->sdata->borrowed; i++){
                free(p->sdata->isbns[i]);
            } free(p->sdata->isbns);
            free(p->sdata);
        } free(p);
    }
}

list* findBook(list* l, char* isbn){
    if (l == NULL) return NULL;
    if(strcmp(l->data->isbn, isbn) == 0) return l;
    return findBook(l->next, isbn);
}

list* findStudent(list* l, unsigned int id){
    if (l == NULL || (l->sdata->id == id)){
        return l;
    } return findStudent(l->next, id);
}

void updateBook(list* l, char* isbn, char* feature, char* value){
    list* temp = findBook(l, isbn);
    if(temp==NULL){
        return;
    } else if(strcmp(feature, "title")==0){
        strcpy(temp->data->title, value);
    } else if(strcmp(feature, "author")==0){
        strcpy(temp->data->author, value);
    } else if(strcmp(feature, "year")==0){
        int year;
        sscanf(value,"%d", &year);
        temp->data->year = year;
    } return;
}

void filterAndSortBooks(list* l, int filterChoice, char* filter, int sortChoice){
    //firstly, i will create a new linked list that has different nodes but pointing to same datapoints original list points at
    //then i will sort this newly created list as per sortChoice
    list* filteredList = NULL;// = l->next;
    switch (filterChoice){
    case 0://by author
        while(l!=NULL){
            if(strcmp(l->data->author, filter)==0){
                list* new = malloc(sizeof(list));
                new->data = l->data;
                new->next = filteredList;
                filteredList = new;
            } l = l->next;
        } break;
    case 1://by year
        int year;
        sscanf(filter,"%d",&year);
        while(l!=NULL){
            if(l->data->year==year){
                list* new = malloc(sizeof(list));
                new->data = l->data;
                new->next = filteredList;
                filteredList = new;
            } l = l->next;
        } break;
    default:
        printf("Invalid filter!\n");
        while(getchar()!='\n');
        return;
    } if(filteredList==NULL){
        printf("No queries returned!\n");
        return;
    }//now sort 
    sort(filteredList, sortChoice);
    display(filteredList);
    free(filteredList);
    return;
}

void sort(list* head, int sortChoice){
    list* current = head;
    list* index = NULL;
    book* temp = NULL;
    switch (sortChoice){
    case 0:
        while (current != NULL){
        index = current->next;
        while (index != NULL){
            if (strcmp(current->data->title, index->data->title) > 0){
                swap(current, index);
            } index = index->next;
        } current = current->next;
    } break;
    case 1:
        while (current != NULL){
            index = current->next;
            while (index != NULL){
                if (strcmp(current->data->author, index->data->author) > 0){
                    swap(current, index);
                } index = index->next;
            } current = current->next;
        } break;
    case 2:
        while (current != NULL){
            index = current->next;
            while (index != NULL){
                if (current->data->year>index->data->year){
                    swap(current, index);
                } index = index->next;
            } current = current->next;
        } break;
    }
    
}

list* reverseList(list* l){
    if (l == NULL || l->next == NULL) return l;// empty or single
    list* nextElement = l->next;
    l->next = NULL;
    list* head = reverseList(nextElement);
    nextElement->next = l;
    return head;
}

void borrowBook(list* l, list* s, char* isbn, unsigned int id){
    list* temp = findBook(l, isbn);
    if(temp == NULL){
        printf("Couldnt find book in db\n");
        return;
    } if(temp->data->status=='R'){
        printf("Book is borrowed by someone else!\n");
        return;
    } temp->data->status='R';
    temp = findStudent(s, id);
    if(temp == NULL){
        printf("Couldnt find student in db\n");
        return;
    } temp->sdata->isbns = realloc(temp->sdata->isbns, (temp->sdata->borrowed + 1) * sizeof(char*)); 
    temp->sdata->isbns[temp->sdata->borrowed] = malloc((strlen(isbn) + 1)  * sizeof(char));
    strcpy(temp->sdata->isbns[temp->sdata->borrowed], isbn);
    temp->sdata->borrowed++;
}

void returnBook(list* l, list* s, char* isbn, unsigned int id){
    list* tbook = findBook(l, isbn);
    list* tstu = findStudent(s, id);
    int count = tstu->sdata->borrowed, flag = -1;
    for(int i=0; i<count; i++){
        if(strcmp(isbn, tstu->sdata->isbns[i])==0){
            flag = i;
            break;
        }
    } if(flag==-1){
        printf("Book is not borrowed by this student!\n");
        return;
    } free(tstu->sdata->isbns[flag]);//freeing isbn of returned book
    for(int i = flag; i<count-1; i++){//shifting books in the list after the subsequent one
        tstu->sdata->isbns[i] = tstu->sdata->isbns[i+1];
    } tbook->data->status = 'A';
    tstu->sdata->borrowed--;
    tstu->sdata->isbns = realloc(tstu->sdata->isbns, tstu->sdata->borrowed * sizeof(char*));//frees unnecessary pointer
}

void writeFile(list* l, list* s){
     FILE* fp = fopen("data.txt", "w");
    if (fp == NULL){
        printf("Error with the file.\n");
        return;
    }
    while(l!=NULL){
        if(l->data !=NULL){
            fprintf(fp, "B|%s|%s|%s|%d|%c|\n", l->data->isbn, l->data->title, l->data->author, l->data->year, l->data->status);
        } l = l->next;
    } while(s!=NULL){
        if(s->sdata !=NULL){
            fprintf(fp, "S|%s|%u|%d|", s->sdata->name, s->sdata->id, s->sdata->borrowed);
            for(int i=0; i<s->sdata->borrowed; i++){
                fprintf(fp,"%s|", s->sdata->isbns[i]);
            }fprintf(fp, "\n");
        } s = s->next;
    } fclose(fp);
}