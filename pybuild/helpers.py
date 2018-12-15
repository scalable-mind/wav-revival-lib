import os
import json
import shutil
import zipfile
from collections import namedtuple


def _json_object_hook(d):
    return namedtuple('X', list(d.keys()))(*list(d.values()))


def json2obj(data):
    return json.loads(data, object_hook=_json_object_hook)


def jsonf2obj(f):
    return json.load(f)


def unzip(filename, path_to=None, delete_zip=False, return_root=False):
    root_dir = ""
    with zipfile.ZipFile(filename, 'r') as zip_ref:
        root_dir += zip_ref.filelist[0].filename
        zip_ref.extractall(path=path_to)
    if delete_zip and os.path.exists(filename):
        os.remove(filename)
    if return_root:
        return root_dir
    else:
        return None


def delete_dir(path):
    if os.path.exists(path):
        shutil.rmtree(path)


def GetHashofDirs(directory, verbose=0):
    import hashlib
    import os
    SHAhash = hashlib.md5()
    if not os.path.exists(directory):
        return -1

    try:
        for root, _, files in os.walk(directory):
            for names in files:
                if verbose == 1:
                    print('Hashing', names)
                filepath = os.path.join(root, names)
                with open(filepath, 'rb') as f1:
                    while True:
                        buf = f1.read(4096)
                        if not buf:
                            break
                        kek = hashlib.md5(buf).hexdigest()
                        SHAhash.update(kek.encode())

    except:
        import traceback
        # Print the stack traceback
        traceback.print_exc()
        return -2

    return SHAhash.hexdigest()


def read_config(path):
    with open(path) as f:
        return jsonf2obj(f)
