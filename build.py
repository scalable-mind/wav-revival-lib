import os
import json
import shutil
import argparse
import subprocess


def read_config(path):
    with open(path) as f:
        return json.load(f)


def delete_dir(path):
    if os.path.exists(path):
        shutil.rmtree(path)


cfg = read_config("build_config.json")


def build(arch="x86", build_type="Debug", target="tests_catch2"):
    install_deps()
    if not os.path.exists(cfg["build"][build_type]["buildDir"]):
        os.mkdir(cfg["build"][build_type]["buildDir"])

    p = subprocess.call(_build_cmd(arch, build_type, target), shell=False,
                        cwd=os.path.realpath(cfg["build"][build_type]["buildDir"]))


def clean(build_type):
    delete_dir(cfg["build"][build_type]["buildDir"])


def install_deps():
    if os.name == 'nt':
        for cmd in cfg["build"]["conan"]["msvc"]:
            subprocess.call(cmd)
    elif os.name == 'posix':
        for cmd in cfg["build"]["conan"]["gcc"]:
            subprocess.call(cmd)


def _msvc_cmd(arch):
    return cfg["build"]["commands"]["msvc"] + [cfg["build"]["arch"][arch]]


def _cmake_config_cmd(build_type):
    return cfg["build"]["commands"]["cmakeConf"] + [cfg["build"][build_type]["cmakeBuildType"]]


def _cmake_build_cmd(target):
    return cfg["build"]["commands"]["cmakeBuild"] + [target]


def _build_cmd(arch, build_type, target):
    cmd = ["&&"] + _cmake_config_cmd(build_type) + ["&&"] + _cmake_build_cmd(target)
    if os.name == "nt":
        cmd = _msvc_cmd(arch) + cmd
    elif os.name == "posix":
        cmd = cmd
    return cmd


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('--type', choices=['debug', 'release'])
    parser.add_argument('--arch', choices=['x86', 'x64'])
    parser.add_argument('--target', type=str)
    parser.add_argument('--clean', action='store_true')
    args = parser.parse_args()
    if args.clean:
        clean('Debug')
        clean('Release')
    else:
        build(build_type=args.type.capitalize(), arch=args.arch, target=args.target)
