/**
 * @file pinfo.c
 * @assignment Lab 9
 * @section CPE2600 121
 * @brief This file contains the main function for the pinfo program.
 * @author Matthew Schiemann
 * @date 11/4/2026
 * @version 1.0
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>        // strtol
#include <errno.h>         // errno
#include <unistd.h>        // getpid
#include <sched.h>         // sched_getscheduler, SCHED_* constants
#include <sys/resource.h>  // getpriority, PRIO_PROCESS
 

int main(int argc, char *argv[]) {
    pid_t pid;
 
    /*
     * This part Choose which process to inspect
     * argc == 1: no argument, so use our own pid from getpid().
     * argc == 2: convert argv[1] from text to a number with strtol.
     * strtol sets 'end' to the first character it could not
     * convert, so anything left over means invalid input.
     */
    if (argc == 1) {
        pid = getpid();
    } else if (argc == 2) {
        char *end;
        errno = 0; // Clear errno before calling strtol to detect errors
        long val = strtol(argv[1], &end, 10);
        // Check for errors: errno set, leftover characters, empty string, or non-positive value
        if (errno != 0 || *end != '\0' || end == argv[1] || val <= 0) {
            fprintf(stderr, "Error: invalid process identifier '%s'\n", argv[1]);
            return 1;
        }
        pid = (pid_t)val;
    } else {
        fprintf(stderr, "Usage: %s [pid]\n", argv[0]);
        return 1;
    }
 
    /*
     * getpriority(PRIO_PROCESS, pid) returns the nice value of the
     * process: -20 (highest priority) to 19 (lowest priority).
     * Because -1 is a VALID nice value, a -1 return alone does not mean
     * failure. So we clear errno first and check it afterwards.
     * If the pid does not exist, errno is set to ESRCH.
     */
    errno = 0;
    int prio = getpriority(PRIO_PROCESS, (id_t)pid);
    // Check for errors: if prio is -1 and errno is not 0, an error occurred
    if (prio == -1 && errno != 0) {
        if (errno == ESRCH) {
            fprintf(stderr, "Error: process %d does not exist\n", (int)pid);
        } else {
            perror("getpriority");
        }
        return 1;
    }
 
    /*
     * This is the Scheduling policy
     * sched_getscheduler returns the policy as an int, or -1 on error
     * (here -1 is never a valid policy, so it always means failure).
     */
    int policy = sched_getscheduler(pid);
    if (policy == -1) {
        perror("sched_getscheduler");
        return 1;
    }
 
    printf("Process ID: %d\n", (int)pid);
    printf("Priority (nice value): %d\n", prio);
    char *policy_str;
    switch (policy) {
    case SCHED_OTHER:
        policy_str = "SCHED_OTHER (standard time-sharing)";
        break;
    case SCHED_FIFO:
        policy_str = "SCHED_FIFO (real-time, first-in first-out)";
        break;
    case SCHED_RR:
        policy_str = "SCHED_RR (real-time, round robin)";
        break;  
    case SCHED_BATCH:
        policy_str = "SCHED_BATCH (batch / CPU-intensive)";
        break;
    case SCHED_IDLE:
        policy_str = "SCHED_IDLE (very low priority background)";
        break;
    default:
        policy_str = "Unknown";
        break;
    }

    printf("Scheduling method: %s\n", policy_str);
 

    return 0;
}