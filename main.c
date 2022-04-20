#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "stack.h"


#define MAX_SIZE 1024

int main(int argc, char const *argv[])
{
    node* top = NULL;

    while (1)
    {
        int index,rest,cmd_size;
        // initialize the cmd buffer
        char command[MAX_SIZE];
        memset(command,0,MAX_SIZE);
        printf("wait for command ...> ");
        scanf("%[^\n]%*c",&command);
        cmd_size = strlen(command);
        index = firstWordI(cmd_size,command);
        rest = cmd_size-index;

        //get the command 
        char cmd[index];
        memset(cmd,0,index+1);
        strncpy(cmd , command , index);
        fflush(stdout);

        // if we there is not parameter to the command
        if (rest == 0)
        {
            // show the top of the stack
            if (strcmp(cmd,"TOP")==0)
            {
                show_top(top);
            }
            // delete the top element of the stack
            else if (strcmp(cmd , "POP")==0)
            {
                top = pop(top);
            }
            // show the entire stack
            else if (strcmp(cmd , "SHOW")==0)
            {
                display(top);
            }
            // destroy the entire stack
            else if (strcmp(cmd,"RESET")==0)
            {
                destroy_stack(top);
                top = NULL;
            }
            // exit the program
            else if (strcmp(cmd,"EXIT")==0)
            {
                exit(EXIT_SUCCESS);
            }
            // if a not valid/recognized command was typed
            else
            {
                printf("DEBUG:No command recognized\n");
            }
            
        }
        // the case of a push command 
        else
        {
        char data[rest];
        memset(data,0,rest);
        strncpy(data , &command[index+1] , rest-1);
        if (strcmp(cmd,"PUSH")==0)
        {
            top = push(data,top);
        }
        }


    }
    
}
