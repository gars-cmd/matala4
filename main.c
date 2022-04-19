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
        printf("wait for command ...> ");
        fflush(stdout);
        char command[1024];

        // getting the input command 
        scanf("%[^\n]%*c",&command);
        cmd_size = strlen(command);
        // printf("the size of the command is %d\n",cmd_size);
        //getting the index of the first word
        index = firstWordI(cmd_size,command);
        // printf("got the value index = %d\n",index);
        // getting the size of the rest of the input
        rest = cmd_size-index;

        //get the command 
        char cmd[index];
        memset(cmd,0,index+1);
        strncpy(cmd , command , index);
        // printf("we get the command %s\n",cmd);
        fflush(stdout);

        if (rest == 0)
        {
            if (strcmp(cmd,"TOP")==0)
            {
                show_top(top);
            }
            if (strcmp(cmd , "POP")==0)
            {
                top = pop(top);
            }
            // if (strcmp(cmd , "SHOW")==0)
            // {
            //     display(top);
            // }
            if (strcmp(cmd,"TEST")==0)
            {
                printf("test passed");
            }
            if (strcmp(cmd,"EXIT")==0)
            {
                exit(EXIT_SUCCESS);
            }
            
            
        }
        // the case of a push command 
        else
        {
        char data[rest];
        strncpy(data , &command[index] , rest);
        if (strcmp(cmd,"PUSH")==0)
        {
            top = push(data,top);
        }
        }


    }
    
}
