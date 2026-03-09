#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../include/utils.h"

#define MAX_COMMAND_LENGTH 50

int main()
{

    bool running = true;
    char command[MAX_COMMAND_LENGTH];

    printf("\nSLAM v1.0\n");
    printf("Made by Robbie Linden 2026\n\n");
    printf("Enter a command or type 'help' to see commands.\n\n");

    while (running == true)
    {
        printf("[SLAM]> ");

        fgets(command, MAX_COMMAND_LENGTH, stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strlen(command) == 0)
        {
            continue;
        }

        if (strcmp(command, "exit") == 0)
        {
            running = false;
        }
        else if (strcmp(command, "help") == 0)
        {
            printf("\nAvailable commands:\n\n");
            printf("help - Show this help message.\n");
            printf("exit - Quit the program\n\n");
        }
        else 
        {
            printf("Entered command is not a valid command, kindly enter 'help' to see valid commands.\n\n");
        }
    }

    return 0;
}