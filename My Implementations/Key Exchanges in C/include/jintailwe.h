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

#define LATTICE_DIMENSION 728 // Matrix Dimension
#define MODULO_Q 2147483647 // q - The modulo factor

struct vector_params{
  int *secret_vector;
  int *public_vector;
};

struct matrix_params{
  int **secret_matrix;
  int **public_matrix;
};

/*-----------------------------Global Variables-------------------------------*/
int M[LATTICE_DIMENSION][LATTICE_DIMENSION]; //Public parameter M
int M_TRANSPOSE[LATTICE_DIMENSION][LATTICE_DIMENSION]; //M transpose
struct matrix_params Alice_params;
struct vector_params Alice1_params;
struct vector_params Bob_params;

//Alice Params
int **EA; //Alices Error Matrix
int *edashA; //Alices other error vector
int *KA;
int *SKA;

//Bob Params
int *eB; //Bobs Error vector
int *edashB; //Bobs Error Scalar
int *KB;
int *SKB;

//Signal generated
int *sig; //either 0 or 1 at any index

//-----DGS-----//
dgs_disc_gauss_dp_t *D;

//------TEST RESULTS-----//
int vector_mem = (LATTICE_DIMENSION*sizeof(int));
int matrix_mem = (LATTICE_DIMENSION*LATTICE_DIMENSION*sizeof(int));

int Alice0_mem_vector = 1; //edashA
int Alice0_mem_matrix = 3; //SA, EA, PA
int Bob_mem_vector = 6; //sB, eB, edashB, pB, KB, sigma
int Alice1_mem_vector = 2;
/*------------------------------Function Prototypes---------------------------*/
extern void run_key_exchange(int argc, char **argv); //Running the key exchange protocol based on public params

extern void generate_gaussian_matrix(); // Generate a matrix sampled from the Discrete Gaussian distribution

extern void generate_gaussian_vector(int gauss_vec[LATTICE_DIMENSION]); // Generate a vector sampled from the Discrete Gaussian distribution

extern int generate_gaussian_scalar(); // Generate a discrete gaussian scalar value

extern int robust_extractor(int x, int sigma); // Find the same shared key

extern bool check_robust_extractor(int x, int y); // Condition for the validity of the robust extractor

extern int signal_function(int y, int b); // A hint algorithm

//----- Printing functions-----
extern void pretty_print_vector(int vec[LATTICE_DIMENSION]); //Prints a vector
extern void pretty_print_matrix(int **matrix); //Prints a matrix

//----- Test Result Functions --
extern void memory_consumed();

//------------------ Public Parameters for the key Exchange -------------------
extern void generate_M();


//------------------ Generate gaussian numbers in C ----------------------------
extern inline long discrete_normal_distribution();

/*---------------------------End of Function Prototypes-----------------------*/

#endif
