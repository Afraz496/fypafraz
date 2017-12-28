# Post Quantum Key Exchange over the Internet

## Introduction

This README is a short introduction on the organisation of the software that comes with this package. All of the work available or work that has been done was done by Afraz Arif Khan on this repository. You can find numerous software implementations for Post Quantum Key Exchange methods on this repository. After February 2017, this repository will incrementally add software which is safe to use with TLS (Transport Layer Security). 


Current Post Quantum Cryptographic implementations:

- A Simple Provably Secure Key Exchange Scheme Based on the Learning with Errors Problem which can be found [here](https://eprint.iacr.org/2012/688)

## Usage

First start by git cloning the repository to a directory you are comfortable with. To git clone please type the following in Terminal and to ensure that the submodules are in this repository:

`$ git --recurse-submodules clone https://github.com/Afrazinator/fypafraz.git`

After you have git cloned the repository then enter the repository using `cd`

`$ cd fypafraz`

If you do a simple `ls` it will show all the files in the repository. To use any implementations I have written you must do:

To examine already existing implementations of PQC and how their performance has been optimised

To make use of implementations written in this repository:

`$ cd My\ Implementations/` NB: This is easier with Tab Completion in terminal

### Existing Implementationa

There are 2 existing implementations and one of the focuses of this repo involves modifying them to improve their current performance. The 2 implementations are:

- [Frodo](https://github.com/lwe-frodo/lwe-frodo)
- [NewHope](https://github.com/tpoeppelmann/newhope)

Further examinations on how these 2 pieces of software will be optimised will be found in the submodules in this repository. 

### My Implementations

#### Simple Key exchange

Before executing the program you must at least have python 2.x in your system. The following dependency is required for generating the matrices in the program:

[Numpy](http://www.numpy.org/)

To install the dependency please use `pip` : 

`$ python -m pip install --user numpy scipy matplotlib ipython jupyter pandas sympy nose` as seen on [this website](https://scipy.org/install.html)

As referred to in the introduction section this is based on a simple key exchange with the learning with errors problem. If you followed the above steps and are in _My Implementations_ then to run the simple key exchange please type the following in terminal:

`$ python simplekeyexchange.py`

24/12/17 - This should only output SKA and SKB, from simple inspection they should both be 0 or 1 (provided the protocol was run honestly). 
