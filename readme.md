Simple Hasher
=============

# Summary
- [Description](#description)
- [Compilation](#compilation-requirements)
- [Install](#install)
- [Screenshots](#screenshots)
- [Repository information](#repository-information)

# Description
Tool to generate and check file signatures (hashes) using the following hash algorithms:
* Message Digest 5 (MD5)
* Secure Hash Algorithm (SHA): Variants SHA-1, SHA-224, SHA-256, SHA-384 and SHA-512.
* Tiger

The application can be used to check the SUM files that some websites provide to check for the integrity and authenticity of files. If invoked with a SUM file as parameter the application will check the signatures of the files present in the same folder as the SUM file. The computed hashes can be saved to disk (the whole table or only the selected ones) or to the clipboard using the contextual menu.

You can also drag files from the explorer and drop in the main dialog. If the file is a SUMS file (that contains hashes for some files) the application will automatically change to "check mode" and check them. Otherwise the files are added to the list of files to compute hashes.

The hash computations are done using multiple cores, either specified by the user or all available in the system.

## Options
From the configuration dialog several options can be modified:
* Hash representations (uppercase or lowercase, break with spaces or compact, in a single or several lines).
* Number of simultaneous threads to use when computing hashes. 
* Install or remove a link to this application in the Windows 'Send To' directory (useful to check SUMS files easier).

# Compilation requirements
## To build the tool:
* cross-platform build system: [CMake](http://www.cmake.org/cmake/resources/software.html).
* compiler: [Mingw64](http://sourceforge.net/projects/mingw-w64/) on Windows or [gcc](http://gcc.gnu.org/) on Linux.

## External dependencies:
The following libraries are required:
* [Qt opensource framework](http://www.qt.io/) version 6.

# Install

Simple Hasher is available for Windows 10 onwards. You can download the latest installer from the [releases page](https://github.com/FelixdelasPozas/SimpleHasher/releases). Neither the application or the installer are digitally signed so the system will ask for approval before running it the first time.

The last version compatible with Windows 7 & 8 is version 1.2.0 that can be downloaded [here](https://github.com/FelixdelasPozas/SimpleHasher/releases/tag/1.2.0).

# Screenshots
Simple main dialog, showing the progress while computing the hashes of some files.

![maindialog](https://github.com/user-attachments/assets/f7e927e2-7da4-4aaf-bccc-3768c5684481)

When checking hash files passed as a parameter (or dropped from the explorer) the green means success (hashes match), red indicates errors (hashes don't match) and yellow indicates that the files were not found (so hashes can't be checked).

![hashcheck](https://github.com/user-attachments/assets/197d2465-ed37-40b0-8133-c5561e8dcddc)

Configuration dialog

![configuration](https://github.com/user-attachments/assets/fe4dd902-8e5a-4ab5-bb4b-fc1448c80788)

# Repository information
**Version**: 1.5.0

**Status**: finished

**License**: GNU General Public License 3

**cloc statistics**

| Language                     |files          |blank        |comment           |code    |
|:-----------------------------|--------------:|------------:|-----------------:|-------:|
| C++                          |   15          |  546        |    437           |  2436  |
| C/C++ Header                 |   14          |  248        |    597           |   540  |
| CMake                        |    1          |   16        |      8           |    66  |
| **Total**                    |   **30**      |  **810**    |   **1042**       |**3042**|
