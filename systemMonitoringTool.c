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

// Prints the dashes that separate each category of System monitor.

void print_dashes() {
    printf("---------------------------------------PID: %ld\n", (long)getpid());
}

// Prints the header at the top of the program.
// TODO: Change number of samples and tdelay to user input values

void print_header_nonsequential(int samples_index, int num_samples, int t_delay) {
    printf("Iteration %d / Nbr of samples: %d -- every %d secs\n", samples_index + 1, num_samples, t_delay);
    return;
}

void print_header_sequential(int num_samples, int t_delay) {
    printf("Sequential - Nbr. of samples: %d -- every %d secs\n", num_samples, t_delay);
    return;
}

// Prints the memory usage of the program.

void print_ram_usage() {
    struct rusage ram_usage;
    if (getrusage(RUSAGE_SELF, &ram_usage) < 0) {
        fprintf(stderr, "Error opening ram usage file, returning.");
        return;
    }
    printf(" Memory usage: %ld kilobytes\n", ram_usage.ru_maxrss);
    print_dashes();
    return;
}

// Prints the memory usage of the system.

void print_memory_usage(float *mem_usage_history, float *vmem_usage_history, int sample_index, int samples, int graphics, int sequential) {
    struct sysinfo sysinfoData;
    if (sysinfo(&sysinfoData) < 0) {
        fprintf(stderr, "program failed when attempting to create sysinfo struct, returning.");
        return;
    }
    printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
    float total_ram = sysinfoData.totalram/pow(1024,3);
    float total_vram = total_ram + sysinfoData.totalswap/pow(1024, 3);
    float phys_ram = sysinfoData.totalram/pow(1024,3) - sysinfoData.freeram/pow(1024,3);
    float swap_used = sysinfoData.totalswap - sysinfoData.freeswap;
    float virtual_ram = phys_ram + swap_used/pow(1024,3);
    mem_usage_history[sample_index] = phys_ram;
    vmem_usage_history[sample_index] = virtual_ram;
    if (sequential == 0) {
        for (int i = 0; i < sample_index; i++) {
            printf("%.2f GB / %.2f GB -- %.2f GB / %.2f GB", mem_usage_history[i], total_ram, vmem_usage_history[i], total_vram);
            if (graphics == 1) {
                float difference;
                if (i == 0) {difference = 0;}
                else {difference = vmem_usage_history[i] - vmem_usage_history[i - 1];}
                printf ("   |");
                if (difference > 0) {
                    for (int bar = 0; bar < (int) (difference * 100); bar++){
                        printf("#");
                    }
                }
                if (difference < 0) {
                    for (int bar = 0; bar < (int) -(difference * 100); bar++) {
                        printf(":");
                    }
                }
                printf(" %.3f (%.2f)", difference, vmem_usage_history[i]);
            }
            printf("\n");
        }
    }
    printf("%.2f GB / %.2f GB -- %.2f GB / %.2f GB", mem_usage_history[sample_index], total_ram, vmem_usage_history[sample_index], total_vram);
    if (graphics == 1) {
        float difference;
        if (sample_index == 0) {difference = 0;}
        else {
            difference = vmem_usage_history[sample_index] - vmem_usage_history[sample_index - 1];
        }
        printf ("   |");
        if (difference > 0) {
            for (int bar = 0; bar < (int) (difference * 100); bar++){
                printf("#");
            }
        }
        if (difference < 0) {
            for (int bar = 0; bar < (int) -(difference * 100); bar++) {
                printf(":");
            }
        }
        printf(" %.3f (%.2f)", difference, vmem_usage_history[sample_index]);
    }
    printf("\n");
    if (sequential == 0) {
        for (int j = sample_index + 1; j < samples; j++) {
            printf("\n");
        }
    }
    print_dashes();
    return;
}

// Prints system CPU information, number of cores and total cpu usage

void print_cpu_info(float *cpu_usage_history, int sample_index, int num_samples, int graphics, int sequential) {
    FILE *fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening /proc/stat, returning.");
        return;
    }
    char cpu[10];
    long user, nice, system, idle, iowait, irq, softirq, a, b, c;
    fscanf(fp, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld", cpu, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &a, &b, &c);
    float cpu_usage = (float) 100 * ((float) (user + nice + system + iowait + irq + softirq + a + b + c) / (float) (user + nice + system + idle + iowait + irq + softirq + a + b + c));
    fscanf(fp, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld", cpu, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &a, &b, &c);
    int cpu_count = 0;
    while (strncmp(cpu, "cpu", 3) == 0) {
        cpu_count++;
        fscanf(fp, "%s  %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld", cpu, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &a, &b, &c);
    }
    if (fclose(fp)) {perror("fclose cpu info");}

    cpu_usage_history[sample_index] = cpu_usage;

    printf("Number of cores: %d\n", cpu_count);
    printf(" total cpu use = %.2f%%\n", cpu_usage_history[sample_index]);

    if (graphics == 1) {
        if (sequential == 1) {
            printf("     ");
            for (int i = 0; i < (int)cpu_usage_history[sample_index]; i++) {
                printf("|");
            }
            printf(" %.2f\n", cpu_usage_history[sample_index]);
            print_dashes();
            return;
        }
        for (int j = 0; j < sample_index + 1; j++) {
            printf("     ");
            for (int i = 0; i < (int)cpu_usage_history[j]; i++) {
                printf("|");
            }
            printf(" %.2f\n", cpu_usage_history[j]);
        }
        for (int j = sample_index + 1; j < num_samples; j++) {
            printf("\n");
        }
    }

    print_dashes();
    return;
}

// Prints the information on current users on the system.

void print_user_info() {
    printf("### Sessions/users ###\n");
    struct utmp *user;
    setutent();
    user = getutent();
    while (user != NULL) {
        if (user -> ut_type == USER_PROCESS) {
            printf(" %s       %s (%s)\n", user -> ut_name, user -> ut_line, user -> ut_host);
        }
        user = getutent();
    }
    print_dashes();
    return;
}

// Prints the system information category.

void print_sysinfo() {
    struct utsname sysinfo;
    if (uname(&sysinfo) < 0) {
        fprintf(stderr, "program failed when attempting to create uname struct, returning.");
        return;
    }
    printf("### System Information ###\n");
    printf(" System Name = %s\n", sysinfo.sysname);
    printf(" Machine Name = %s\n", sysinfo.nodename);
    printf(" Version = %s\n", sysinfo.version);
    printf(" Release = %s\n", sysinfo.release);
    printf(" Architecture = %s\n", sysinfo.machine);
    print_dashes();
    return;
}

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
