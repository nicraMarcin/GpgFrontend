# GpgFrontend

![Language](https://img.shields.io/badge/language-C%2B%2B-green)
![License](https://img.shields.io/badge/License-GPL--3.0-orange)
![CodeSize](https://img.shields.io/github/languages/code-size/saturneric/GpgFrontend)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/d1750e052a85430a8f1f84e58a0fceda)](https://www.codacy.com/gh/saturneric/GpgFrontend/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=saturneric/GpgFrontend&amp;utm_campaign=Badge_Grade)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/saturneric/gpgfrontend)
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fsaturneric%2FGpgFrontend.svg?type=small)](https://app.fossa.com/projects/git%2Bgithub.com%2Fsaturneric%2FGpgFrontend?ref=badge_small)

GpgFrontend is a Modern, Easy-to-Use, Compact, Cross-Platform, and Installation-Free OpenPGP Frontend Tool.

By using GpgFrontend, you can quickly **encrypt and decrypt text or files**. Or at the same time as the above
operations, you can add **your own signature** to let others know that this document or this paragraph of text was
issued by you.

Furthermore, it visualizes most of the common operations of gpg commands. It aims to allow ordinary users to quickly use
gpg and make professional users more convenient. GpgFrontend supports new features of OpenPGP.

![Main ScreenShot](https://github.com/saturneric/Blob/blob/master/screenshots/main.png?raw=true)

Build Status: ![main](https://github.com/saturneric/gpgfrontend/actions/workflows/cmake.yml/badge.svg?branch=main)

## Table of Contents

- [Features](#features)
- [Usage](#usage)
- [Purpose](#purpose)
- [Build](#build)
- [Contract](#contract)
  - [Contributing](#contributing)
  - [Maintainers](#maintainers)
- [Licenses](#LICENSES)

## Features

- Open source, free, no need to install.
- Can run on **Windows, Linux and macOS**.
- Just double-click, and then you can use it freely.
- Before use, you only need to install gnupg. A tiny tool of a few MB.
- Supports multiple languages. Don’t forget to help me translate this software.

## Usage

0. Install gnupg. To check if it is installed, you can enter gpg on the command line.
    - For Windows User, Click [this link](https://gnupg.org/ftp/gcrypt/binary/gnupg-w32-2.3.1_20210420.exe) to download
      gnupg.
    - For macOS User, Use [Homebrew](https://brew.sh/) to install **gpg**.
    - For Linux User, Use the package manager(e.g., apt, yum, pacman) to install **gpg**.
1. Unzip GpgFrontend.zip. The compressed package can be downloaded in Release.
2. Start GpgFrontend with GpgFrontend.exe or GpgFrontend (depending on your os).

## Purpose

The GpgFrontend project inherit from a relatively mature but not maintained gpg4usb project. It inherits the stable,
easy-to-use, compact, and installation-free features of gpg4usb, and plans to support some new features of OpenPGP based
on it and continue to improve it.

GpgFrontend will add more functions in the future to improve the ease of use of GPG in end-to-end transmission. At the
same time, the addition of new functions does not affect the old basic functions. My personal strength is always
limited. GpgFrontend welcomes volunteers to join. You can use the GitHub platform to file an issue, or submit a pull
request.

The GpgFrontend project is as open source, and it also insists on using open source codes and libraries.

## Build

The tutorial for building the software will be released shortly.

## Contract

If you want to contact me individually, you can email [eric@bktus.com](mailto:eric@bktus.com).

### Contributing

Feel free to dive in! [Open an issue](https://github.com/saturneric/GpgFrontend/issues/new) or submit PRs.

### Maintainers

[@Saturneric](https://github.com/saturneric).

## LOGO

![logo](https://github.com/saturneric/Blob/blob/master/logos/gpgfrontend-logo.jpg?raw=true)

## LICENSES

GpgFrontend itself is licensed under the [GPLv3](COPYING).

There are some libraries, and the gpg-binary included in the zip-file / a static gpg-build which (may) have different
licenses, for more information check their homepages. You can also obtain the sources from there.

GPG: http://gnupg.org/
QT: http://trolltech.no/ , http://trolltech.no/downloads/opensource

The icons of this software use materials from Alibaba vector icon library. The Alibaba vector icon library is free to
use. The icons in the free library aren't registered as trademarks. There is no copyright issue involved and can be used
commercially.

