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
#include <math.h>

#ifndef HEADER_JINTAILWE_H

#define LATTICE_DIMENSION 512 // Matrix Dimension
#define MODULO_Q 2^32 - 1 // q - The modulo factor

/*------------------------------Function Prototypes---------------------------*/
void run_key_exchange(); //Running the key exchange protocol based on public params

int * generate_gaussian_vector(); // Generate a vector sampled from the Discrete Gaussian distribution

int generate_gaussian_scalar(); // Generate a discrete gaussian scalar value

int robust_extractor(int x, int sigma); // Find the same shared key

bool check_robust_extractor(int x, int y); // Condition for the validity of the robust extractor

int signal_function(int y, int b); // A hint algorithm

//------------------ Public Parameters for the key Exchange -------------------
void generate_M();

void generate_Alice_parameters(); // Alices private parameters

void generate_Bob_parameters(); // Bobs private parameters

//------------------ Generate gaussian numbers in C ----------------------------
double RNG();

double normal_distribution_number();

double normal_distribution(double mean, double sigma);

/*---------------------------End of Function Prototypes-----------------------*/

#endif
