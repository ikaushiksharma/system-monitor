#include "syslib.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <utmp.h>
// Main function from which the program is run.

int main(int argc, char **argv) {
    // Setting output mode based on command line arguments

    int visited_positional_args = 0;

    int system = 1;
    int user = 1;
    int graphics = 0;
    int sequential = 0;
    int samples = 10;
    int tdelay = 1;
    int i = 1;

    if (argc > 1 && atoi(argv[1]) != 0) {
        if (argc == 2) {
            fprintf(stderr, "exactly 2 digits can be used at the start, consecutively to denote 'samples tdelay'\n");
            return 0;
        }
        if (atoi(argv[2]) == 0) {
            fprintf(stderr, "error: positional arguments must be consecutive digits, no chars!\n");
            return 0;
        }
        visited_positional_args = 1;
        samples = atoi(argv[1]);
        tdelay = atoi(argv[2]);
        i = 3;
    }
    while (i < argc) {
        if (strncmp(argv[i], "--system", 8) == 0) {user = 0;}
        else if (strncmp(argv[i], "--user", 6) == 0) {system = 0;}
        else if (strncmp(argv[i], "--graphics", 10) == 0) {graphics = 1;}
        else if (strncmp(argv[i], "--sequential", 12) == 0) {sequential = 1;}
        else if (strncmp(argv[i], "--samples=", 10) == 0) {
            if (visited_positional_args == 1) {
                fprintf(stderr, "positional args and samples flag cannot be inputted at the same time\n");
                return 0;
            }
            char *samples_token = strtok(argv[i], "=");
            samples_token = strtok(NULL, "=");
            if (samples_token != NULL) {samples = atoi(samples_token);}
            else {printf("number of samples not detected in flag, running with default value 10");}
        }
        else if (strncmp(argv[i], "--tdelay=", 9) == 0) {
            if (visited_positional_args == 1) {
                fprintf(stderr, "positional args and tdelay flag cannot be inputted at the same time\n");
                return 0;
            }
            char *tdelay_token = strtok(argv[i], "=");
            tdelay_token = strtok(NULL, "=");
            if (tdelay_token != NULL) {tdelay = atoi(tdelay_token);}
            else {printf("tdelay value not detected in flag, running with default value 1");}
        }
        else {
            fprintf(stderr, "flag %s not recognized - valid flags are: --system --user --graphics --sequential --samples= --tdelay\n", argv[i]);
            return 0;
        }
        i++;
    }

    // Create arrays to keep history of memory, cpu usage

    float cpu_usage_history[samples];
    float mem_usage_history[samples];
    float vmem_usage_history[samples];

    // Print output

    printf("\e[1;1H\e[2J"); // escape code for clearing terminal

    // Print when sequential == 0 (not flagged)

    if (sequential == 0) {

        for (int sample_index = 0; sample_index < samples; sample_index++) {
            print_header_nonsequential(sample_index, samples, tdelay); // Prints header, which indicates # samples and tdelay
            print_ram_usage(); // Prints allocated ram for program
            if (system == 1 || user == 0) {
                print_memory_usage(mem_usage_history, vmem_usage_history, sample_index, samples, graphics, sequential);
                print_cpu_info(cpu_usage_history, sample_index, samples, graphics, sequential);
            }
            if (user == 1 || system == 0) {print_user_info();}
            print_sysinfo();

            // Printing of samples completed, waiting before refreshing and printing new sample

            if (sample_index != samples - 1) {
                sleep(tdelay);
                printf("\e[1;1H\e[2J");
            }
        }
    }

    // Print when sequential == 1 (flagged)
    /* Designed such that header will be printed, then memory, cpu usage at each ping will
    be printed out sequentially (with no spaces), before finishing with printing user data
    and system information. */

    if (sequential == 1) {
        print_header_sequential(samples, tdelay);
        print_ram_usage();
        for (int sample_index = 0; sample_index < samples && (system == 1 || user == 0); sample_index++) {
            print_memory_usage(mem_usage_history, vmem_usage_history, sample_index, samples, graphics, sequential);
            print_cpu_info(cpu_usage_history, sample_index, samples, graphics, sequential);
            sleep(tdelay);
        }
        if (user == 1 || system == 0) {print_user_info();}
        print_sysinfo();
    }

    return 0;
}
