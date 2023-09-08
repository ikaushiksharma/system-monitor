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

void print_dashes();
void print_header_nonsequential(int samples_index, int num_samples, int t_delay);
void print_header_sequential(int num_samples, int t_delay);
void print_ram_usage();
void print_memory_usage(float *mem_usage_history, float *vmem_usage_history, int sample_index, int samples, int graphics, int sequential);
void print_cpu_info(float *cpu_usage_history, int sample_index, int num_samples, int graphics, int sequential);
void print_user_info();
void print_sysinfo();