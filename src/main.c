#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../include/utils.h"
#include "../include/constants.h"
#include "../include/config.h"
#include "../include/validator.h"

//===============================================//
//                                               //
//        1. Show welcome message                //
//                                               //
//===============================================//

void show_welcome(void)
{
    printf("\nSLAM v%s\n", SLAM_VERSION);
    printf("Made by %s %s\n\n", SLAM_AUTHOR, SLAM_YEAR);
    printf("Enter a command or type 'help' to see commands.\n\n");
}

//===============================================//
//                                               //
//        2. Show help menu                      //
//                                               //
//===============================================//

void show_help(void)
{
    printf("\nAvailable commands:\n");
    printf("help    - Show this help message\n");
    printf("info    - Show program information\n");
    printf("analyze - Analyze WAV files in input folder\n");
    printf("exit    - Quit the program\n\n");
}

//===============================================//
//                                               //
//        3. Main program loop                   //
//                                               //
//===============================================//

int main()
{

    /*================================*/
    /*        3.1 Initialize          */
    /*================================*/

    bool running = true;
    Config config;
    char command[MAX_COMMAND_LENGTH];

    show_welcome();

    // Error 3.1 main.c: Config file not found, using defaults
    if (load_config("slam.conf", &config) != 0)
    {
        print_warning("main.c", "Warning 3.1: Could not load slam.conf, using defaults");
        config.sample_rate = DEFAULT_SAMPLE_RATE;
        config.bit_depth = DEFAULT_BIT_DEPTH;
        strcpy(config.samples_folder, DEFAULT_SAMPLES_FOLDER);
    }

    /*================================*/
    /*  3.2 Main command loop         */
    /*================================*/

    while (running == true)
    {
        print_prompt();

        fgets(command, MAX_COMMAND_LENGTH, stdin);

        // Error 3.2 main.c: Command too long (buffer overflow protection)
        if (strchr(command, '\n') == NULL)
        {
            print_error("main.c", "Error 3.2: Command too long");

            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        // Process 3.2.1: Remove newline character
        command[strcspn(command, "\n")] = '\0';

        // Process 3.2.2: Skip empty input
        if (strlen(command) == 0)
        {
            continue;
        }

        /*================================*/
        /*     3.3 Process commands       */
        /*================================*/

        // Process 3.3.1: Exit command
        if (strcmp(command, "exit") == 0)
        {
            printf("Exiting...\n");
            running = false;
        }

        // Process 3.3.2: Help command
        else if (strcmp(command, "help") == 0)
        {
            show_help();
        }

        // Process 3.3.3: Info command
        else if (strcmp(command, "info") == 0)
        {
            printf("\nSLAM v%s - Sample Loop Analysis Module\n", SLAM_VERSION);
            printf("Author: %s\n", SLAM_AUTHOR);
            print_config(&config);
        }

        // Process 3.3.4: Analyze command
        else if (strcmp(command, "analyze") == 0)
        {
            analyze_samples_folder(&config);
        }

        // Process 3.3.5: Unknown command
        else 
        {
            printf("Entered command is not a valid command, kindly enter 'help' to see valid commands.\n\n");
        }
    }

    return 0;

}