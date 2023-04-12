#pragma once

/* 
 * Retrospect Daemon script
 *
 * This script performs one action only: updating the .retro folder of all
 * relevant folders. It receives instructions from the main, user-controlled
 * executable `retro`. Spec:
 *    S: shutdown
 *    M: moniter (a folder)
 *    D: demoniter
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "list.h"

#define PATH "/tmp/socket"
#define MAX_BACKLOG 3         // Really no reason for concurrent connections

void perror_and_exit(char* msg) {
  perror(msg);
  exit(1);
}

#define SAFE_CALL(call) do if ((call) < 0) perror_and_exit(#call); while (0)

LIST(Change);

struct Change {
    char* current_change_location;    // tar zip file
    struct Change* previous;
    LIST(struct Change)* next;
    int location;     /* Location is going to have semantics; may make long int */
};

DECLARE_LL(struct Change);

struct MoniteredFolder {
    char* name;
    struct Change* changelog;     // Starts out identical to the fresh repository
};

DECLARE_LL(struct MoniteredFolder);

LIST(struct MoniteredFolder) moniteredfolders;

int update_global_storage(void);  // Updating ~/.retro directory
int add_to_watchlist(char* foldername);
int remove_from_watchlist(char* foldername);

int stash_changes(struct MoniteredFolder);
int go_to(struct MoniteredFolder, int location);
int rollback(struct MoniteredFolder);

void moniter(void);      // Processes all file-change events and does appropriate actions