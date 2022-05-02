from vcs import VCS
from sys import argv, exit
from os import getcwd as pwd

def main():
    if sys.argv[1] == 'init' or os.path.exists(pwd() + '/.retrospect'):
        vcs = VCS(pwd(), pwd() + '/.retrospect')
    else:
        print('Retrospect repository does not exist here. Exiting.')
        exit(1)
    if sys.argv[1] == 'revert':
        vcs.
    elif sys.argv[1] == 'view':
        pass
    elif sys.argv[1] == 'head':
        pass
    elif sys.argv[1] == 'inspect':
        pass
    elif sys.argv[1] == 'log':
        pass

