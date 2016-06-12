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
The application can be used to check the SUM files that some websites provide to check for the integrity and authenticity of files. If invoked with a SUM file as parameter the application will check the signatures of the files present in the same folder as the SUM file. 
The computed hashes can be saved to disk (the whole table or only the selected ones) or to the clipboard using the contextual menu.

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
* [Qt opensource framework](http://www.qt.io/).

# Install
The only current option is build from source as binaries are not provided. 

# Screenshots
Simple main dialog, showing the progress while computing the hashes of some files.

![Main dialog]()

When checking hash files passed as a parameter the green means success (hashes match), red indicates errors (hashes don't match) and yellow indicates that the files were not found (so hashes can't be checked).

![Check dialog]()

Configuration dialog

![Main dialog]()

# Repository information
**Version**: 1.0.0

**Status**: finished

**cloc statistics**

| Language                     |files          |blank        |comment           |code    |
|:-----------------------------|--------------:|------------:|-----------------:|-------:|
| C++                          |   13          |  456        |    377           |  2000  |
| C/C++ Header                 |   12          |  199        |    493           |   383  |
| CMake                        |    1          |   16        |     10           |    63  |
| **Total**                    |   **26**      |  **671**    |   **880**        |**2446**|
