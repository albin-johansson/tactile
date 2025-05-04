#  Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

import os
import platform
import shutil


def get_platform_triplet(system_name: str):
    if system_name == "Windows":
        return "x64-windows-tactile.cmake"

    elif system_name == "Darwin":
        arch = platform.uname().machine
        if arch == "arm64":
            return "arm64-osx-tactile.cmake"
        else:
            return "x64-osx-tactile.cmake"

    elif system_name == "Linux":
        return "x64-linux-tactile.cmake"

    else:
        return None


def main():
    vcpkg_root = os.getenv("VCPKG_ROOT")
    if vcpkg_root is None:
        print("Error: VCPKG_ROOT is not defined")
        return

    system_name = platform.system()
    if system_name is None:
        print("Error: could not determine the platform")
        return

    triplet_filename = get_platform_triplet(system_name)
    if triplet_filename is None:
        print(f"Error: unsupported platform '{system_name}'")
        return

    triplet_src_path = f"../cmake/{triplet_filename}"
    triplet_dst_dir = f"{vcpkg_root}/triplets/community"
    triplet_dst_path = f"{triplet_dst_dir}/{triplet_filename}"

    if not os.path.isfile(triplet_dst_path):
        print(f"Adding '{triplet_filename}' to '{triplet_dst_dir}'")
        shutil.copy(triplet_src_path, triplet_dst_path)
    else:
        print(f"The triplet file '{triplet_filename}' already exists")


if __name__ == "__main__":
    main()
