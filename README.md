JPEG Decodeur
==================

**jpeg2ppm** is a program to decode jpeg images into ppm or pgm images, developed under Ubuntu and CentOS.

### Compiling the project

You can compile the project by typing in terminal:

> $ make

You can clean the .Object files, PPM & PGM images, documentation and the executables by typing in terminal:

> $ make clean


### Usage

To execute and run the program and tests simply open a terminal in its directory and type:

> **$ bin/jpeg2ppm image.jpeg** to lunch the main program.

> **$ bin/test_name_here** to lunch a test executable, some tests may need to pass an additional argument (an image or file name).

### Shell programs

To lunch the decodor on all images in /images directoy you can use the script **run_decoder** in shell directory by typing in terminal in main directoy:

> **$ scripts/run_decoder**

This shell program will preview a progress bar for each process in addition to the time of execution and after finishing converting the jpeg image to ppm or pgm it will preview the JPEG image and the PPM or PGM image.

To lunch all included tests you can use the script **run_tests** in shell directory by typing in terminal in main directoy:

> **$ scripts/run_tests**

In this script you need press any key in order to pass to next test.

To lunch valgrind for all images in /images you can use the script **run_valgrind** in shell directory by typing in terminal in main directoy:

> **$ scripts/run_valgrind**

In this script you need press any key in order to pass to next test.

### Documentation

The code is fully documented and the project directory already includes a file created by **Doxygen** to generate the documentation, you can find it in the **/doc** directory.

To re-generate the documentation just open terminal at project folder root and type:
>$ make doc

### Archive contents

> **/bin** contains the main executable + the tests executables.

> **/images** contains all images in .jpg/.jpeg formats.

> **/doc** contains project_doc.doxy file to re-generate the documentation, in addition to modules diagram image.

> **/include** contains libraries header files.

> **/obj** contains the object files generated while compiling the projects.

> **/obj-prof** contains the object files provided by profs.

> **/scripts** contains 4 shell scripts **run_decoder** to run the decoder on all JPEG images in /images directory, the second one **run_tests** to lunch all tests programs in terminal, the thrid one is **run_valgrind**, and the fourth script **run_decoder_without_preview**is the same as **run_decoder** but without preview for images.

> **/src** contains all the source code of the project.

> **Makefile** : to compile the project

> **README.md** : _current file._


### Built using
Atom


### Authors:
##### Lucas MORIN | Majd ODEH | Sylvain POUGET
Copyright © 2019 **Lucas MORIN**, **Majd ODEH** and **Sylvain POUGET**. All rights reserved.

École nationale supérieure d'informatique et de mathématiques appliquées
**Grenoble-INP ENSIMAG**
