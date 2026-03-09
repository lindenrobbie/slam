#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../include/utils.h"
#include "../include/constants.h"

int main()
{

    bool running = true;
    char command[MAX_COMMAND_LENGTH];

    printf("\nSLAM v%s\n", SLAM_VERSION);
    printf("Made by %s %s\n\n", SLAM_AUTHOR, SLAM_YEAR);
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