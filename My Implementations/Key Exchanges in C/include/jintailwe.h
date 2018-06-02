/********************************************************************************************
 * A simple provably secure key exchange based on the learning with errors problem
 *
 *
 * Based on the paper:
 *     Jintai Ding, Xiang Xie and Xiaodong Ling
 *
 * Copyright (c) Jintai Ding, Xiang Xie and Xiaodong Ling for the theoretical key exchange
 *               Afraz Arif Khan for implementing the key exchange in C and TLS
 *
 * Released under the MIT License; see LICENSE.txt for details.
 ********************************************************************************************/

/** \file jintailwe.h
 * Core parameters and function prototypes for the PQC implementation
 */

#include <stdbool.h>

#include "dgs.h"

#ifndef HEADER_JINTAILWE_H

#define LATTICE_DIMENSION 512 // Matrix Dimension
#define MODULO_Q 2147483647 // q - The modulo factor

/*-----------------------------Global Variables-------------------------------*/
int M[LATTICE_DIMENSION][LATTICE_DIMENSION]; //Public parameter M
int M_TRANSPOSE[LATTICE_DIMENSION][LATTICE_DIMENSION]; //M transpose

/*------------------------------Function Prototypes---------------------------*/
void run_key_exchange(); //Running the key exchange protocol based on public params

int * generate_gaussian_vector(); // Generate a vector sampled from the Discrete Gaussian distribution

int generate_gaussian_scalar(); // Generate a discrete gaussian scalar value

int robust_extractor(int x, int sigma); // Find the same shared key

bool check_robust_extractor(int x, int y); // Condition for the validity of the robust extractor

int signal_function(int y, int b); // A hint algorithm

//------------------ Public Parameters for the key Exchange -------------------
void generate_M();

//------------------ Generate gaussian numbers in C ----------------------------

long discrete_normal_distribution(int mean, int sigma);

//*******Print functions****//
void pretty_print_vector(int *vec);
/*---------------------------End of Function Prototypes-----------------------*/

#endif
