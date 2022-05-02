from pathlib import Path
from datetime import datetime
from itertools import islice
import json
import os
import shutil

def keep_rule(_path):
    path = str(_path)
    return '.retrospect' not in path

class VCS:
    def __init__(self, path, save_path):
        self.path = Path(path)
        self.save_path = Path(save_path)
        try:
            self.save_path.mkdir()
            self.save_path.joinpath('head').mkdir()
        except FileExistsError:
            with open(self.save_path / 'data.json') as f:
                d = json.load(f)
            self.cid = d['current-cid']
            self.viewing = d['viewing']
            with open(self.save_path / 'marks.json') as f:
                self.marks = json.load(f)
        else:
            self.cid = 0
            self.marks = {}
            self.viewing = False
            self.update_state()

    def update_state(self):
        with open(self.save_path / 'data.json', 'w') as f:
            json.dump({
                'current-cid': self.cid,
                'viewing': self.viewing
            }, f)
        with open(self.save_path / 'marks.json', 'w') as f:
            json.dump(self.marks, f)

    def set_viewing_mode(self, v):
        self.viewing = v
        self.update_state()

    def snap(self, msg=""):
        if self.viewing: return
        wd = self.save_path / str(self.cid)
        wd.mkdir()
        with open(wd / 'meta.json', 'w') as f:
            json.dump({'msg': msg, 'date': datetime.now().strftime("%H:%M %h %d")}, f)
        for root, dirs, files in os.walk(self.path):
            print(root)
            wd.joinpath(root).mkdir()
            for i, x in enumerate(dirs.copy()):
                if not keep_rule(x):
                    del dirs[i]
            for x in files:
                a = os.path.join(root, x)
                if keep_rule(a):
                    shutil.copyfile(a, wd / root / x)
                    print(f"Copied {a}")
                else:
                    print(f"Ignoring {a}")
        self.cid += 1
        self.update_state()

    def restore_path(self, cid, viewing=True):
        if cid != 'head':
            try:
                shutil.rmtree(self.save_path.joinpath('head'))
            except FileNotFoundError:
                pass
            self.save_path.joinpath('head').mkdir()
            shutil.move(str(self.path), str(self.save_path / 'head'))
        else:
            shutil.rmtree(str(self.path))
        shutil.copytree(str(self.save_path / str(cid) / self.path.name), str(self.path))
        self.set_viewing_mode(viewing)

    def mark(self, name, cid=None):
        if cid is None:
            cid = self.cid
        self.marks[cid] = name
        self.update_state()

    def get_cid_data(self, cid):
        with open(self.save_path / str(cid) / 'meta.json') as f:
            return json.load(f)

    def _display_snap(self, cid):
        mark_str = self.marks.get(cid, "")
        data = self.get_cid_data(cid)
        change_str = data['msg']
        date_str = data['date']
        prefixlen = 1 + len(str(cid)) + len(mark_str) + 2
        width = max([
            prefixlen + len(change_str) + 1,
            1 + len(date_str) + 1
        ])
        side_str = width * "─"
        return f"""
│ ╭{side_str}╮
├─┤ \033[0;36m#{cid}\033[0;33m{mark_str}\033[0m: {change_str.ljust(width - prefixlen - 2)} │
│ │ \033[0;35m{date_str.ljust(width - 2)}\033[0m │
│ ╰{side_str}╯
""".strip()

    def display(self, n=None, verbose=False):
        if verbose:
            for cid in islice(range(self.cid - 1, -1, -1), n):
                print(self._display_snap(cid))
            if n is not None:
                print('.\n.\n.')
        else:
            for cid in islice(filter(self.marks.__contains__, range(self.cid - 1, -1, -1)), n):
                print(self._display_snap(cid))
            if n is not None:
                print('.\n.\n.')

if __name__ == '__main__':
    vcs = VCS('./test-directory', './test-directory/.retrospect')
    import sys
    if sys.argv[1] == 'save':
        if len(sys.argv) == 3:
            vcs.snap(msg=sys.argv[2])
        else:
            vcs.snap()
    elif sys.argv[1] == 'restore':  # Add revert and view instead of this
        vcs.restore_path(int(sys.argv[2]))
    elif sys.argv[1] == 'head':
        vcs.restore_path('head')
    elif sys.argv[1] == 'show':
        print(vcs._display_snap(int(sys.argv[2])))
    elif sys.argv[1] == 'showall':
        vcs.display(verbose=True)
