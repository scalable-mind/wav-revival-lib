import argparse
import os
import subprocess


def configure(gen, arch):
    configure_command = ["cmake", "-S", ".", "-B", "build"]
    if gen:
        configure_command += ["-G", gen]
    if arch:
        configure_command += ["-A", arch]
    subprocess.call(configure_command, shell=False,
                    cwd=os.path.realpath("./"))

def build_type(debug):
    if debug:
        return "Debug"
    else:
        return "Release"

def build_command(debug, target):
    cmd = ["cmake", "--build", "build"]
    if target:
        cmd += ["--target", target]
    cmd += ["--config", build_type(debug)]
    return cmd


def build(debug, target):
    subprocess.call(build_command(debug, target), shell=False,
                    cwd=os.path.realpath("./"))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Simple cmake build script')
    parser.add_argument('--debug', action="store_true")
    parser.add_argument('--target', type=str)
    parser.add_argument('--gen', type=str)
    parser.add_argument('--arch', type=str)
    args = parser.parse_args()

    configure(args.gen, args.arch)
    build(debug=args.debug, target=args.target)
