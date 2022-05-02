from time import sleep
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
from os.path import basename, join, abspath
from datetime import datetime
from shutil import make_archive, mkdir
from json import dump

def ignore(event):
    return not (not event.is_directory and basename(event.src_path) != '.')

class snapshot:
    def __init__(self, path):
        self.path = path
        self.time = datetime.now()
    
    def save(self, save_path, msg, all=False):
        make_archive(join(save_path, 'archive'), 'zip', self.path)    # Make sure archive file itself isn't included
        with open(join(save_path, 'meta.json'), 'w') as f:
            dump({
                'path': abspath(self.path),
                'time': str(self.time),
                'msg': msg
            }, f)

class StorageManager:
    def __init__(self, path, save_path=None):
        self.path = path
        if save_path is None:
            self.save_path = join(path, '.retro')
        else:
            self.save_path = save_path
        mkdir(self.save_path)
        self.cid = 0
        self.info = {'current': self.cid}

    def update_info(self):
        self.info['current'] = self.cid
        with open(join(self.save_path, 'info'), 'w') as f:
            dump(self.info, f)

    def snap(self, msg):
        snap_path = join(self.save_path, str(self.cid))
        mkdir(snap_path)
        snapshot(self.path).save(snap_path, msg)
        self.update_info()
        self.cid += 1

    def restore(self, cid):
        pass


class Handler(FileSystemEventHandler):
    @staticmethod
    def on_any_event(event):
        if not ignore(event):
            print(event)

def main():
    observer = Observer()
    handler = Handler()
    observer.schedule(handler, 'test-directory', recursive=True)
    observer.start()
    try:
        while True:
            sleep(1)
    except Exception as e:
        print(e)
    finally:
        observer.stop()
        observer.join()

if __name__ == '__main__':
    snapshot('.').save('.')
