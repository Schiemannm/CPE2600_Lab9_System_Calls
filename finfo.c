/**
 * @file finfo.c
 * @assignment Lab 9
 * @section CPE2600 121
 * @brief This file contains the main function for the finfo program.
 * @author Matthew Schiemann
 * @date 11/4/2026
 * @version 1.0
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>       // localtime, strftime
#include <sys/stat.h>   // lstat, struct stat, S_IS*, S_I* macros

/*
 * Returns a string describing the type of file based on the mode.
 * @param mode The file mode from struct stat.
 * @return A string representing the file type.
 */
static const char *file_type(mode_t mode){
    if (S_ISREG(mode)) {
        return "Regular file";
    } else if (S_ISDIR(mode)) {
        return "Directory";
    } else if (S_ISLNK(mode)) {
        return "Symbolic link";
    } else if (S_ISCHR(mode)) {
        return "Character device";
    } else if (S_ISBLK(mode)) {
        return "Block device";
    } else if (S_ISFIFO(mode)) {
        return "FIFO (named pipe)";
    } else if (S_ISSOCK(mode)) {
        return "Socket";
    }
    return "Unknown";
}

/*
 * Converts a file mode to a string of permissions.
 * @param mode The file mode.
 * @param buf The buffer to store the permission string.
 */
static void permission_string(mode_t mode, char *buf) {
    // Owner (user) permissions
    buf[0] = (mode & S_IRUSR) ? 'r' : '-';
    buf[1] = (mode & S_IWUSR) ? 'w' : '-';
    buf[2] = (mode & S_IXUSR) ? 'x' : '-';
    // Group permissions
    buf[3] = (mode & S_IRGRP) ? 'r' : '-';
    buf[4] = (mode & S_IWGRP) ? 'w' : '-';
    buf[5] = (mode & S_IXGRP) ? 'x' : '-';
    // Other (everyone else) permissions
    buf[6] = (mode & S_IROTH) ? 'r' : '-';
    buf[7] = (mode & S_IWOTH) ? 'w' : '-';
    buf[8] = (mode & S_IXOTH) ? 'x' : '-';
    buf[9] = '\0';
}

int main(int argc, char *argv[]) {

    // The file name MUST come from the command line
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
 
    /* 
     * File instructions for lstat:
     * lstat() is like stat(), but it does not follow symbolic links.
     * I picked lstat over stat and fstat because it is the most general 
     * and works for all file types.
     */
    struct stat st;
    if (lstat(argv[1], &st) == -1) {
        perror(argv[1]);
        return 1;
    }
 
    // Type and permissions (both come from st_mode)
    char perms[10];
    permission_string(st.st_mode, perms);
 
    /*
     * Reading the last modification time:
     */
    struct tm *mod = localtime(&st.st_mtime);
    if (mod == NULL) {
        perror("localtime");
        return 1;
    }
    char timebuf[64];
    if (strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", mod) == 0) {
        fprintf(stderr, "Error: could not format modification time\n");
        return 1;
    }


    /*
     * Print the file information:
     */
    printf("File: %s\n", argv[1]);
    printf("Type: %s\n", file_type(st.st_mode));
    printf("Permissions: %s\n", perms);
    printf("Owner (UID): %u\n", (unsigned int)st.st_uid);
    printf("Size: %lld bytes\n", (long long)st.st_size);
    printf("Last modified: %s\n", timebuf);
 
    return 0;
}