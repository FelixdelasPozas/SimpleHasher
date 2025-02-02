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

Simple Hasher is available for Windows 7 onwards. You can download the latest installer from the [releases page](https://github.com/FelixdelasPozas/SimpleHasher/releases). Neither the application or the installer are digitally signed so the system will ask for approval before running it the first time.

# Screenshots
Simple main dialog, showing the progress while computing the hashes of some files.

![maindialog](https://user-images.githubusercontent.com/12167134/56167612-ecaf5900-5fd8-11e9-9a3f-f90cf26c8b86.jpg)

When checking hash files passed as a parameter (or dropped from the explorer) the green means success (hashes match), red indicates errors (hashes don't match) and yellow indicates that the files were not found (so hashes can't be checked).

![hashcheck](https://cloud.githubusercontent.com/assets/12167134/15993541/772701cc-30e9-11e6-9dee-b5a288412e1b.jpg)

Configuration dialog

![configuration](https://cloud.githubusercontent.com/assets/12167134/15993540/76f68e16-30e9-11e6-852f-328f1455117a.jpg)

# Repository information
**Version**: 1.3.0

**Status**: finished

**License**: GNU General Public License 3

**cloc statistics**

| Language                     |files          |blank        |comment           |code    |
|:-----------------------------|--------------:|------------:|-----------------:|-------:|
| C++                          |   14          |  518        |    419           |  2293  |
| C/C++ Header                 |   13          |  241        |    576           |   496  |
| CMake                        |    1          |   15        |      8           |    63  |
| **Total**                    |   **28**      |  **774**    |   **1003**       |**2852**|
