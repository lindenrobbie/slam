#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../include/utils.h"
#include "../include/constants.h"
#include "../include/config.h"
#include "../include/validator.h"

void show_welcome(void)
{
    printf("\nSLAM v%s\n", SLAM_VERSION);
    printf("Made by %s %s\n\n", SLAM_AUTHOR, SLAM_YEAR);
    printf("Enter a command or type 'help' to see commands.\n\n");
}

void show_help(void)
{
    printf("\nAvailable commands:\n");
    printf("help    - Show this help message\n");
    printf("info    - Show program information\n");
    printf("analyze - Analyze WAV files in input folder\n");
    printf("exit    - Quit the program\n\n");
}

//maIN

int main()
{

    bool running = true;
    Config config;
    char command[MAX_COMMAND_LENGTH];

    show_welcome();

    if (load_config("slam.conf", &config) != 0)
    {
        printf("Warning: Could not load slam.conf, using defaults.\n\n");
        config.sample_rate = DEFAULT_SAMPLE_RATE;
        config.bit_depth = DEFAULT_BIT_DEPTH;
    }

    while (running == true)
    {
        print_prompt();

        fgets(command, MAX_COMMAND_LENGTH, stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strlen(command) == 0)
        {
            continue;
        }

        // available commands

        if (strcmp(command, "exit") == 0)
        {
            printf("Exiting...\n");
            running = false;
        }
        else if (strcmp(command, "help") == 0)
        {
            show_help();
        }
        else if (strcmp(command, "info")== 0)
        {
            printf("\nSLAM v%s - Sample Loop Analysis Module\n", SLAM_VERSION);
            printf("Author: %s\n", SLAM_AUTHOR);
            print_config(&config);
        }
        else if (strcmp(command, "analyze") == 0)
        {
            analyze_samples_folder(&config);
        }

        // end of available commands

        else 
        {
            printf("Entered command is not a valid command, kindly enter 'help' to see valid commands.\n\n");
        }
    }

    return 0;

}