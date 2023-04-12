"""Autosaving VCS.

Usage:
    retro init
        initializes current directory to have a .git and
        .retro directory
    retro start
        starts watching current directory
    retro show
        show tree of changes
    retro back
        reverts tree to previous stage
    retro forwards
        reverts tree to next stage
    retro to LOCATION
        reverts tree to specified location
    retro commit [ -m MESSAGE ]
        saves all changes in .retro to .git and deletes all
        program data (restarting the process). There's no
        way back from a commit; you're commiting to your
        future self that there's no need for a revert then

Daemon:
    written in C and shouldn't be used directly
"""

import sys
import socket