#  Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

import shutil
import sys
import urllib.request
import os.path

FONT_OUTPUT_DIR = "../assets/fonts"
IMAGE_OUTPUT_DIR = "../assets/images"
CACHE_DIR = "./cache"

FONTAWESOME_VERSION = "6.5.1"
FONTAWESOME_DIR_NAME = f"fontawesome-free-{FONTAWESOME_VERSION}-desktop"
FONTAWESOME_URL = f"https://use.fontawesome.com/releases/v{FONTAWESOME_VERSION}/{FONTAWESOME_DIR_NAME}.zip"

FONTAWESOME_CACHE_ARCHIVE = f"{CACHE_DIR}/{FONTAWESOME_DIR_NAME}.zip"
FONTAWESOME_CACHE_DIR = f"{CACHE_DIR}/{FONTAWESOME_DIR_NAME}"

FONTAWESOME_CACHE_FONT_FILE = f"{FONTAWESOME_CACHE_DIR}/otfs/Font Awesome 6 Free-Solid-900.otf"
FONTAWESOME_OUTPUT_FONT_FILE = f"{FONT_OUTPUT_DIR}/fa/fa-solid-900.ttf"

FONTAWESOME_CACHE_LICENSE_FILE = f"{FONTAWESOME_CACHE_DIR}/LICENSE.txt"
FONTAWESOME_OUTPUT_LICENSE_FILE = f"{FONT_OUTPUT_DIR}/fa/LICENSE.txt"

ROBOTO_FLAVOR = "roboto-unhinted"
ROBOTO_URL = f"https://github.com/googlefonts/roboto/releases/download/v2.138/{ROBOTO_FLAVOR}.zip"

ROBOTO_CACHE_ARCHIVE = f"{CACHE_DIR}/{ROBOTO_FLAVOR}.zip"
ROBOTO_CACHE_DIR = f"{CACHE_DIR}/{ROBOTO_FLAVOR}"

ROBOTO_CACHE_FONT_FILE = f"{ROBOTO_CACHE_DIR}/Roboto-Regular.ttf"
ROBOTO_OUTPUT_FONT_FILE = f"{FONT_OUTPUT_DIR}/roboto/Roboto-Regular.ttf"

ROBOTO_CACHE_LICENSE_FILE = f"{ROBOTO_CACHE_DIR}/LICENSE"
ROBOTO_OUTPUT_LICENSE_FILE = f"{FONT_OUTPUT_DIR}/roboto/LICENSE.txt"

DUMMY_IMAGE_URL = "https://dummyimage.com/96x64.png"
DUMMY_IMAGE_CACHE_FILE = f"{CACHE_DIR}/dummy.png"
DUMMY_IMAGE_OUTPUT_FILE = f"{IMAGE_OUTPUT_DIR}/dummy.png"


def download_file(url, outfile):
    print(f"Downloading '{url}' to '{outfile}'...")

    opener = urllib.request.build_opener()
    opener.addheaders = [("User-agent", "Mozilla/5.0")]

    urllib.request.install_opener(opener)
    urllib.request.urlretrieve(url, outfile)


def install_font_awesome_font():
    print("Installing FontAwesome font...")

    if os.path.isfile(FONTAWESOME_CACHE_ARCHIVE):
        print(f"  '{FONTAWESOME_CACHE_ARCHIVE}' already exists")
    else:
        download_file(FONTAWESOME_URL, FONTAWESOME_CACHE_ARCHIVE)

    if not os.path.isdir(FONTAWESOME_CACHE_DIR):
        print("  Unpacking FontAwesome archive...")
        shutil.unpack_archive(FONTAWESOME_CACHE_ARCHIVE, CACHE_DIR)

    shutil.copy(FONTAWESOME_CACHE_FONT_FILE, FONTAWESOME_OUTPUT_FONT_FILE)
    shutil.copy(FONTAWESOME_CACHE_LICENSE_FILE, FONTAWESOME_OUTPUT_LICENSE_FILE)


def install_roboto_font():
    print("Installing Roboto font...")

    if os.path.isfile(ROBOTO_CACHE_ARCHIVE):
        print(f"  '{ROBOTO_CACHE_ARCHIVE}' already exists")
    else:
        download_file(ROBOTO_URL, ROBOTO_CACHE_ARCHIVE)

    if not os.path.isdir(ROBOTO_CACHE_ARCHIVE):
        print("  Unpacking Roboto archive...")
        shutil.unpack_archive(ROBOTO_CACHE_ARCHIVE, ROBOTO_CACHE_DIR)

    shutil.copy(ROBOTO_CACHE_FONT_FILE, ROBOTO_OUTPUT_FONT_FILE)
    shutil.copy(ROBOTO_CACHE_LICENSE_FILE, ROBOTO_OUTPUT_LICENSE_FILE)


def install_dummy_image():
    print("Installing dummy image...")

    if os.path.isfile(DUMMY_IMAGE_CACHE_FILE):
        print(f"  '{DUMMY_IMAGE_CACHE_FILE}' already exists")
    else:
        download_file(DUMMY_IMAGE_URL, DUMMY_IMAGE_CACHE_FILE)

    shutil.copy(DUMMY_IMAGE_CACHE_FILE, DUMMY_IMAGE_OUTPUT_FILE)


def main():
    if not os.path.isdir(CACHE_DIR):
        os.mkdir(CACHE_DIR)

    if "--skip-fonts" not in sys.argv:
        install_font_awesome_font()
        install_roboto_font()

    install_dummy_image()


if __name__ == "__main__":
    main()
