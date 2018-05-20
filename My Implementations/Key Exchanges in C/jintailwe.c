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

/** \file jintailwe.c
 * Key exchange between Alice and Bob
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#include "jintailwe.h"

int main(){
  run_key_exchange();
  return 0;
}

void run_key_exchange(){
  srand(time(NULL));
  generate_M();
  int i, j; // loop index
  //------- Generate Alices parameters --------


  //------- Generate Bobs parameters ----------
  Bob_params.secret_vector = (int*)malloc(sizeof(int)*LATTICE_DIMENSION);
  eB = (int*)malloc(sizeof(int)*LATTICE_DIMENSION);
  Bob_params.public_vector = (int*)malloc(sizeof(int)*LATTICE_DIMENSION);
  generate_gaussian_vector(Bob_params.secret_vector);
  generate_gaussian_vector(eB);

  for(i = 0; i < LATTICE_DIMENSION;i++){
    for(j = 0; j < LATTICE_DIMENSION; j++){
      Bob_params.public_vector[i] = Bob_params.public_vector[i] + (M_TRANSPOSE[i][j]*Bob_params.secret_vector[j] + 2*eB[j]);
    }
    Bob_params.public_vector[i] = (Bob_params.public_vector[i] < 0) ? Bob_params.public_vector[i] % MODULO_Q + MODULO_Q : Bob_params.public_vector[i] % MODULO_Q;
  }
  /*
  //Find Alices Key

  int KA = 0;
  for(i = 0; i < LATTICE_DIMENSION; i++){
    KA = abs((KA + (sA[i]*pB[i] + 2*edashA))%MODULO_Q);
  }

  //Find Bobs Key

  int KB = 0;
  for(i = 0; i < LATTICE_DIMENSION; i++){
    KB = abs((KB + (pA[i]*sB[i] + 2*edashB))%MODULO_Q);
  }

  //-- Check the robust extractor condition till correct params generated ----

  while(!check_robust_extractor(KA,KB)){
    sA = generate_gaussian_vector();
    eA = generate_gaussian_vector();

    for(i = 0; i < LATTICE_DIMENSION;i++){
      for(j = 0; j < LATTICE_DIMENSION; j++){
        pA[i] = pA[i] + (M[i][j]*sA[j] + 2*eA[j]);
      }
      pA[i] = pA[i]%MODULO_Q;
    }
    //------- Generate Bobs parameters ----------
    sB = generate_gaussian_vector();
    eB = generate_gaussian_vector();

    for(i = 0; i < LATTICE_DIMENSION;i++){
      for(j = 0; j < LATTICE_DIMENSION; j++){
        pB[i] = pB[i] + (M_TRANSPOSE[i][j]*sB[j] + 2*eB[j]);
      }
      pB[i] = pB[i]%MODULO_Q;
    }

    edashA = generate_gaussian_scalar();
    edashB = generate_gaussian_scalar();

    //Find Alices Key
    KA = 0;
    for(i = 0; i < LATTICE_DIMENSION; i++){
      KA = abs((KA + (sA[i]*pB[i] + 2*edashA))%MODULO_Q);
    }


    //Find Bobs Key
    KB = 0;
    for(i = 0; i < LATTICE_DIMENSION; i++){
      KB = abs((KB + (pA[i]*sB[i] + 2*edashB))%MODULO_Q);
    }

  }
  //Obtain a signal
  int sig = signal_function(KB, 0);
  //-- Obtain shared keys between Alice and Bob ----
  int SKA = robust_extractor(KA, sig);
  int SKB = robust_extractor(KB, sig);

  //--- Check if the keys are the same ---
  if(SKA == SKB){
    printf("SKA: %i\n", SKA);
    printf("SKB: %i\n", SKB);
    printf("SKA and SKB match!\n");
  }
  */
}

//Generating the public matrix M once and for all
void generate_M(){
  int i, j;

  for(i = 0; i < LATTICE_DIMENSION; i++){
    for(j = 0; j < LATTICE_DIMENSION; j++){
      M[i][j] = rand()%MODULO_Q;
      M_TRANSPOSE[j][i] = M[i][j];
    }
  }
}

void generate_gaussian_matrix(){

}

void generate_gaussian_vector(int gauss_vec[LATTICE_DIMENSION]){
  int i; //Loop index
  int q = pow(LATTICE_DIMENSION,4);
  double alpha = pow((1/(double)LATTICE_DIMENSION),3);
  int mu = 0;

  int sigma = alpha*q;

  for(i = 0; i < LATTICE_DIMENSION; i++){
    gauss_vec[i] = (int)normal_distribution(mu,sigma);
  }
}

int generate_gaussian_scalar(){
  int q = 1;
  int alpha = 1;
  int mu = 0;

  int sigma = alpha*q;

  return (int)normal_distribution(mu,sigma);
}

int robust_extractor(int x, int sigma){
  return ((((int)x)%MODULO_Q + (int64_t)(sigma * (MODULO_Q)/2)%MODULO_Q)%2);
}

bool check_robust_extractor(int x, int y){
  double delta = MODULO_Q/4 - 2;
  return ((x-y)%2 == 0 && abs(x-y) <= delta);
}

int signal_function(int y, int b){
  return !(y >= (double)-MODULO_Q/4 + b && y <= (double)MODULO_Q/4 + b);
}

void pretty_print_vector(){
  int i;
  for(i = 0; i < LATTICE_DIMENSION; i++){
    printf("Vector[%i] = %i\n",i, Bob_params.secret_vector[i]);
  }
}
/*------------------- Generate Gaussian numbers in C -------------------------*/

/*
This method makes use of the Box-Mueller implementation, Casting is required for Discrete Gaussian behaviour
*/
double RNG(){
  return ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
}
 // return a normally distributed random number (without variance and mean)
double normal_distribution_number(){
  double y1=RNG();
  double y2=RNG();
  return cos(2*3.14*y2)*sqrt(-2.*log(y1));
}

//Hub function
double normal_distribution(double mean, double sigma){
  return normal_distribution_number()*sigma + mean;
}
