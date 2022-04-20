
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
    if (strlen(element)<1)
    {
        printf("ERROR:no string was given\n");
        fflush(stdout);
    }
    
    int size = strlen(element);
    node *temp=malloc(sizeof(node));
    memset(temp->string,0,MAX_SIZE);
    memcpy(temp->string,element,size);  
    temp -> next = head;
    printf("DEBUG: end of push command\n");
    return temp;
}


void show_top(node* head)
{
    //case of empty stack
    if (!head)
    {
        printf("ERROR:the stack is empty\n");
    }
    else
    {
        node *tmp = head;
        printf("OUTPUT:%s\n",tmp->string);
        printf("DEBUG: end of top display\n");
    }
    
}

node *pop(node * head)
{
    //case of empty stack
        if (!head)
    {
        printf("ERROR:the stack is empty\n");
        return NULL;
    }
    else
    {
        // create a new head
        node *newHead = head->next;
        // pop the element from stack
        free(head);
        return newHead;
        printf("DEBUG: end of pop command\n");
    }
    
}

void destroy_stack(node *p)
{
    if (!p)
    {
        printf("ERROR:the stack is empty\n");
    }
    else
    {
        while (p != NULL)
        {
            node *next = p->next;
            free(p);
            p = next;
        }
        printf("DEBUG: end of stack destruction\n");
    }
    
}

void display(node *head)
{
    if (!head)
    {
        printf("ERROR:the stack is empty\n");
    }
    else{
        node *tmp = head;
        int i = 1;
        while (tmp)
        {
            printf("%d-%s-\n",i,tmp->string);
            tmp = tmp->next;
            i++;
        }
        printf("DEBUG: end of display command\n");
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