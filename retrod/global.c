#include "retrod.h"

/*
 * Serialization format:
 *      Global:
 *          ~/.retro/folders        List of all monitered folders (top-level)
*/

int update_global_storage(void) {
    int fd = open("~/.retro/folders", O_CREAT | O_WRONLY);
    if (fd < 0)
        perror_and_exit("fd");
    LOOP_THROUGH(struct MoniteredFolder, mf, moniteredfolders) {
        write(fd, mf.name, strlen(mf.name));
        write(fd, "\n", 1);
    }
}