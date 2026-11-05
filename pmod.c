/**
 * @file pmod.c
 * @assignment Lab 9
 * @section CPE2600 121
 * @brief This file contains the main function for the pmod program.
 * @author Matthew Schiemann
 * @date 11/4/2026
 * @version 1.0
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>    // errno
#include <unistd.h>   // nice
#include <time.h>     // nanosleep, struct timespec

int main(void)
{
    /*
     * Reduce priority by 10
     * adds int 10 to the nice value and returns the new val
     * negative value increaes priority and requires root
     * need to use errno again becaue a -1 is valid
     */
    errno = 0;
    int new_nice = nice(10);
    if (new_nice == -1 && errno != 0) {
        perror("nice");
        return 1;
    }
    printf("Priority reduced. New nice value: %d\n", new_nice);
 
    /*
     * Sleep for 1,837,272,638 nanoseconds
     * takes a struct to specify the time to sleep
     * and a struct to store the remaining time if interrupted
     * Returns 0 on success, -1 on error.
     */
    struct timespec req;
    req.tv_sec = 1;
    req.tv_nsec = 837272638L;
 
    if (nanosleep(&req, NULL) == -1) {
        perror("nanosleep");
        return 1;
    }
 
    printf("Goodbye!\n");
    return 0;
}