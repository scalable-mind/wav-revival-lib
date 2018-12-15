import subprocess
from urllib import request

from pynt import task

from pybuild.helpers import *

cfg = read_config("pybuild/build.config.json")


@task()
def build(arch="x86", build_type="Debug", target="tests_catch2"):
    if not os.path.exists(cfg["build"][build_type]["buildDir"]):
        os.mkdir(cfg["build"][build_type]["buildDir"])

    p = subprocess.call(_build_cmd(arch, build_type, target), shell=False,
                        cwd=os.path.realpath(cfg["build"][build_type]["buildDir"]))


@task()
def clean(build_type):
    delete_dir(cfg["build"][build_type]["buildDir"])


@task()
def install_deps():
    install_catch2()


@task()
def install_catch2():
    catch = cfg["installCatch"]

    if _check_hash(catch["catchInstallPrefix"], catch["catchInstallPrefix"]):
        print("Already installed.")
    else:
        delete_dir(catch["catchInstallPrefix"])
        request.urlretrieve(catch["catchZipUrl"], catch["catchZipTmpFileName"])

        src_dir = unzip(catch["catchZipTmpFileName"],
                        delete_zip=True, return_root=True)

        for cmd in catch["commands"]:
            p = subprocess.Popen(cmd, cwd=os.path.realpath(src_dir))
            p.wait()

        delete_dir(src_dir)

        _save_hash(catch["catchInstallPrefix"], catch["catchInstallPrefix"])


def _msvc_cmd(arch):
    return cfg["build"]["commands"]["msvc"] + [cfg["build"]["arch"][arch]]


def _cmake_config_cmd(build_type):
    return cfg["build"]["commands"]["cmakeConf"] + [cfg["build"][build_type]["cmakeBuildType"]]


def _cmake_build_cmd(target):
    return cfg["build"]["commands"]["cmakeBuild"] + [target]


def _build_cmd(arch, build_type, target):
    return _msvc_cmd(arch) + ["&&"] + _cmake_config_cmd(build_type) + ["&&"] + _cmake_build_cmd(target)


def _check_hash(dirname, name) -> bool:
    if os.path.exists(cfg["cache"]):
        with open(cfg["cache"], 'r') as f:
            cache = json.load(f)
        if name in cache:
            return GetHashofDirs(dirname) == cache[name]
    return False


def _save_hash(dirname, name):
    cache = {}
    try:
        new_hash = GetHashofDirs(dirname)
        if os.path.exists(cfg["cache"]):
            with open(cfg["cache"], 'r') as f:
                cache = json.load(f)
        cache[name] = new_hash
        with open(cfg["cache"], 'w') as f:
            json.dump(cache, f)
    except:
        import traceback
        traceback.print_exc()
