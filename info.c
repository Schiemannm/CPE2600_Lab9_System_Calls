/**
 * @file info.c
 * @assignment Lab 9
 * @section CPE2600 121
 * @brief This file contains the main function for the info program.
 * @author Matthew Schiemann
 * @date 11/4/2026
 * @version 1.0
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>   // get_nprocs
#include <unistd.h>        // sysconf, getpagesize


// Prints "label: value" only if value is a non-empty string
static void print_if_set(const char *label, const char *value) {
      if (value != NULL && value[0] != '\0') {
          printf("%s: %s\n", label, value);
      }
  }

int main(void) {

    // Empty Container on the stack for the time to be stored into
    //struct timespec is defined in time.h
    // and contains two fields tv_sec and tv_nsec
    struct timespec tp;

    // Asks the OS what time it is. CLOCK_REALTIME 
    // is the system-wide clock that measures real (wall-clock) time.
    // Its counted from the Epoch (00:00:00 UTC, January 1, 1970).
    // &tp just passes the address of the struct from before
    // if it fails, clock_gettime returns -1 and sets errno to indicate the error
    if(clock_gettime(CLOCK_REALTIME, &tp) == -1) {
        // Learned this from google, but Perror, whe a system call fails, it stores an
        // error code in a global variable called errno. perror() prints a description of the error
        perror("clock_gettime");
        return 1; //Exits main with a non-zero value to indicate an error occurred
    } else {
        // Make the math happen in 64-bit space to avoid overflow, then print the result
        long long ns = (long long)tp.tv_sec * 1000000000LL + tp.tv_nsec;
        printf("Current time: %lld ns since the Epoch\n", ns);
    }


    // Declares a struct utsname variable u to hold system information
    struct utsname u;
    // Calls uname to fill the struct with system information. 
    // If it fails, it returns -1 and sets errno
    if (uname(&u) == -1) {
    perror("uname");
    return 1;
    } else { // Otherwise, it prints out the information for each of them
    print_if_set("System name", u.sysname);
    print_if_set("Node name", u.nodename);
    print_if_set("Release", u.release);
    print_if_set("Version", u.version);
    print_if_set("Machine", u.machine);
    }

    // Get the number of CPUs available to the system using get_nprocs() from sys/sysinfo.h
    int cpus = get_nprocs();
    // get_nprocs returns count directly and cannot fail, so no -1 check
    printf("Number of CPUs: %d\n", cpus);

    // get page size
    long pagesize = getpagesize();
    long total_pages = sysconf(_SC_PHYS_PAGES);
    long free_pages = sysconf(_SC_AVPHYS_PAGES);
    if (total_pages == -1 || free_pages == -1) {
        perror("sysconf");
        return 1;
    }
    // Cast to long long before multiplying to avoid overflow
    printf("Total memory: %lld bytes\n", (long long)total_pages * pagesize);
    printf("Free memory: %lld bytes\n", (long long)free_pages * pagesize);
   
return 0;
}


