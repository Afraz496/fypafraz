# Post Quantum Key Exchange over the Internet

## Introduction

This README is a short introduction on the organisation of the software that comes with this package. All of the work available or work that has been done was done by Afraz Arif Khan on this repository. This repository is organised in the following file based system:

- **My Implementations**: This is the main folder which includes the following subfolder + files:
  - **Key Exchanges in C**: This includes files organised as an include and src C program which can all be compiled using the Makefile in src.
    -**jintailwe.c**: This is the main key exchange file, it holds most of the relevant functions and simulates a key exchange between Alice and Bob, it also includes Key Construction and parameter generation with modifiable parameters available in the header file.
  - **simplekeyexchange.py**: This is a python script which runs the D-H like Key Exchange for multiple-bits.
  - **graphresults.py**: This file is a python script which delimits, segregates and averages the output obtained from a .txt file after running the key exchange from the python script in 'My Implementations.'
- **Parameter Selection**: This includes a python script to remark on the parameters used for the key exchange namely `n` and `q` based on Lemma 4 in the paper. 


This repository implemented the following Post Quantum Cryptographic protocol:

- A Simple Provably Secure Key Exchange Scheme Based on the Learning with Errors Problem which can be found [here](https://eprint.iacr.org/2012/688)

**Note:** This repository includes software that can be compiled on a Linux/Unix environemnt. The development for this software was done on a Mac OS X platform. Currently this software does not support usage on Windows. 

## Dependencies

### Python

Before running any of the software available on this repository you must install python 2 on your platform. To install python 2 [here](https://www.python.org/downloads/) is the official python distribution for all platforms. 

Since the implementations given in this repository make use of TLS/SSL it is important to have the library in its default configuration installed on your platform. There will also be some utilisation of TLS and SSL in the command line to run the program with adjustable parameters but the exact syntax of running SSL/TLS is not necessary and as a result will not require you to familiarise yourself with all of the SSL commands.

#### SSL for Python

To install SSL type the following command in terminal:

`$ pip install ssl`

#### OpenSSL for the command line

##### Linux

`$ sudo apt-get install openssl`

##### Mac OS X

To install packages in the command line of MAC OS X it is important to have [brew](https://brew.sh/) installed. 

`$ brew instal openssl`

### C

The C language requires multiple dependencies for the Discrete Gaussian library available on this [repository](https://github.com/malb/dgs). The following dependencies are:

- [Gmp](https://gmplib.org/)
- [Mpfr](https://www.mpfr.org/)
- [OpenMP](https://www.openmp.org/)

#### Mac OS X

The following commands will install the relevant software:

`$ brew install gmp`
`$ brew install mpfr`

Installing OpenMP on a Mac is available in the following [StackOverflow post](https://stackoverflow.com/questions/35134681/installing-openmp-on-mac-os-x-10-11). The full text is given as reference:

"On a mac, the command gcc is a symlink to Clang. So by calling gcc -fopenmp -o your_program your_program.c you are in fact using Clang, which until now has not had built-in support for OpenMP.

The newer versions of Clang do have support for OpenMP according to this post (where you can also find instructions on how to set it up).

On the other hand, if you still want to use gcc I can guide you through the steps that worked for me.

Install gcc with brew. The command you used should work: brew install gcc --without-multilib

Alternatively, if brew says that you already have gcc installed you can try brew reinstall gcc --without-multilib

As you may have noted, if you don't specify --without-multilib brew warns you that OpenMP may not work.

Find the location of the newly installed gcc. Brew appends the version number to gcc so that it does not conflict with the one installed by Command Line Tools. You will find the symlink in usr/local/bin. In my case it's usr/local/bin/gcc-5. If you right-click and chose "Show original" it should show the gcc-5 executable in /usr/local/Cellar/gcc/5.3.0/bin/gcc-5 (version numbers may differ).

Now you need to tell your system about it. When calling a compiler your bash will look into /usr/bin by default and not in /usr/local/bin. You need to add this directory to your $PATH. This can be easily done with the command:

PATH=/usr/local/bin:$PATH

Now you should be able to compile with OpenMP enabled using:

gcc-5 -fopenmp -o your_program your_program.c

Remark: gcc-5 is the version I have installed, yours might differ."

#### Linux

`$ sudo apt-get install gmp`
`$ sudo apt-get install mpfr`
`$ sudo apt install libomp-dev`

## Usage

First start by git cloning the repository to a directory you are comfortable with. To git clone please type the following in Terminal and to ensure that the submodules are in this repository:

`$ git clone https://github.com/Afrazinator/fypafraz.git`

After you have git cloned the repository then enter the repository using `cd`

`$ cd fypafraz`

If you do a simple `ls` it will show all the files in the repository.

To make use of implementations written in this repository:

`$ cd My\ Implementations/` NB: This is easier with Tab Completion in terminal

To use scripts to test the material available in this repository navigate to the Parameter Selection folder as:

`$ cd Paramter\ Selection/`

### My Implementations

#### Simple Key exchange in Python

Before executing the program you must at least have python 2.x in your system. The following dependency is required for generating the matrices in the program:

[Numpy](http://www.numpy.org/)

To install the dependency please use `pip` : 

`$ python -m pip install --user numpy scipy matplotlib ipython jupyter pandas sympy nose` as seen on [this website](https://scipy.org/install.html)

As referred to in the introduction section this is based on a simple key exchange with the learning with errors problem. If you followed the above steps and are in _My Implementations_ then to run the simple key exchange please type the following in terminal:

`$ python simplekeyexchange.py`

This protocol can now partake in n-bit security. In the python script the parameter 'n' in the main() function represents the lattice dimension, it can be changed to increase the brute force security. Currently the lattice dimension is set to 512. 

#### Key Exchanges in C

##### Prerequisites

If you followed the dependencies carefully and the gcc version turns out to be gcc-8 then you can skip this part:

"To make use of the software the OS of your computer must have a gcc compiler. The dependency on OpenMP for Mac OS X results in a version change of the gcc compiler used to run the program originally made for this repository. Please utilise the steps:

- Install gcc with brew. The command you used should work: brew install gcc --without-multilib

- Alternatively, if brew says that you already have gcc installed you can try brew reinstall gcc --without-multilib

- As you may have noted, if you don't specify --without-multilib brew warns you that OpenMP may not work.

- Find the location of the newly installed gcc. Brew appends the version number to gcc so that it does not conflict with the one installed by Command Line Tools. You will find the symlink in usr/local/bin. In my case it's usr/local/bin/gcc-5. If you right-click and chose "Show original" it should show the gcc-5 executable in /usr/local/Cellar/gcc/5.3.0/bin/gcc-5 (version numbers may differ).

- Now you need to tell your system about it. When calling a compiler your bash will look into /usr/bin by default and not in /usr/local/bin. You need to add this directory to your $PATH. This can be easily done with the command:

- PATH=/usr/local/bin:$PATH

To set gcc, the version at the time of this repository is gcc-8. Installing a new version may require changes to be made in certain files:

- **Makefile**: Here the `$(CC)=gcc-8` variable value can be changed to your version of gcc.
- **creatobjfiles.sh**: Here there are 2 files used to compile the object files with `gcc-8`. Modify this to your version of gcc."

Finally before you try to execute the software you must first create the object files. This can be done with the help of a simple shell script:

`$ ./createobjfiles.sh` (Again you might need to do `chmod u+x`)

After the obj folder is created and the object files are placed in the folder type:

`$ make`

This will compile the software and create an executable called `jintailwe`, in order to run this executable type:

`$ ./jintailwe`

This will print a successful message remarking that Alice and Bob share the same key and it displays execution time in seconds for the key exchange.

## Test Results

If you are interested in individual parameters and the total execution time of the program there is an already available script specifically designed to find the average of all the printed output of 'simplekeyexchange.py' and this script also plots a graph of the exeuction time for 'n' points. 

To make use of this test-suite first type:

`$ ./runsimplekexntimes.sh 100 > simplekexresults.txt` (if you want 100 results)

Then simply type:

`$ python graphresults.py`

Instead of running the key exchange once there is a shell script included to run the key exchange 'n' times. It can be used as:

`$ ./runsimplekexntimes n` <----- Here n specifies the number of times the key exchange runs before the program terminates.

If you are using a Unix based system you will need to make the shell script executable before running the above command, this can be achieved by:

`$ chmod u+x runsimplekexntimes`

If you wish to terminate execution on Linux or Unix you can use the Ctrl+X command to terminate the script.

