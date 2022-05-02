from time import sleep
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
from vcs import VCS
import sys

class Handler(FileSystemEventHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.vcs = VCS(sys.argv[1], sys.argv[2])

    @staticmethod
    def on_any_event(event):
        vcs.snap(str(event))

def main():
    observer = Observer()
    handler = Handler()
    observer.schedule(handler, 'test-directory', recursive=True)
    observer.start()
    try:   # Error is invoked by user (KeyboardInterrupt)
        while True:
            sleep(1)
    finally:
        observer.stop()
        observer.join()

if __name__ == '__main__':
    snapshot('.').save('.')
