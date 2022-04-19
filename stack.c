
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 1024

typedef struct node
{
    char string[MAX_SIZE];
    struct node *next;
} node;

node* push(char *element, node *head)
{
    int size = strlen(element);
    node *temp=malloc(sizeof(node));
    memset(temp->string,0,MAX_SIZE);
    memcpy(temp->string,element,size);  
    temp -> next = head;
return temp;
}


void show_top(node* head)
{
    node *tmp = head;
    printf("-->%s\n",tmp->string);
}

node *pop(node * head)
{
    // create a new head
    node *newHead = head->next;

    // pop the element from stack
    free(head);

    return newHead;

}

void destroy_stack(node *p)
{
    while (p != NULL)
    {
        node *next = p->next;
        free(p);
        p = next;
    }
}


int firstWordI(int size , const char* line)
{
    int i = 0;
    while (i<size)
    {
        if (line[i] == ' ')
        {
            break;
        }
        // printf("-%d-for letter %c\n",i,line[i]);
        i++;
    }
    
    return i;
}