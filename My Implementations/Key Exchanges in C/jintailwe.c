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

/*-----------------------------Global Variables-------------------------------*/
int M[LATTICE_DIMENSION][LATTICE_DIMENSION]; //Public parameter M
int M_TRANSPOSE[LATTICE_DIMENSION][LATTICE_DIMENSION]; //M transpose

int main(){
  run_key_exchange();
  return 0;
}

void run_key_exchange(){
  srand(time(NULL));
  generate_M();

  //------- Generate Alices parameters --------
  int i, j; // loop index
  int *sA = generate_gaussian_vector();
  int *eA = generate_gaussian_vector();
  int pA[LATTICE_DIMENSION];

  for(i = 0; i < LATTICE_DIMENSION;i++){
    for(j = 0; j < LATTICE_DIMENSION; j++){
      pA[i] = pA[i] + (M[i][j]*sA[j] + 2*eA[j]);
    }
    if(pA[i] < 0){
      pA[i] = pA[i]%MODULO_Q + MODULO_Q;
    }
    else{
      pA[i] = pA[i]%MODULO_Q;
    }
  }
  //------- Generate Bobs parameters ----------
  int *sB = generate_gaussian_vector();
  int *eB = generate_gaussian_vector();
  int pB[LATTICE_DIMENSION];

  for(i = 0; i < LATTICE_DIMENSION;i++){
    for(j = 0; j < LATTICE_DIMENSION; j++){
      pB[i] = pB[i] + (M_TRANSPOSE[i][j]*sB[j] + 2*eB[j]);
    }
    if(pB[i] < 0){
      pB[i] = pB[i]%MODULO_Q + MODULO_Q;
    }
    else{
      pB[i] = pB[i]%MODULO_Q;
    }
  }

  int edashA = generate_gaussian_scalar();
  int edashB = generate_gaussian_scalar();

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
        pA[i] = pA[i] + (M_TRANSPOSE[i][j]*sA[j] + 2*eA[j]);
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

int *generate_gaussian_vector(){
  static int gauss_vec[LATTICE_DIMENSION];
  int i; //Loop index
  int q = pow(LATTICE_DIMENSION,4);
  double alpha = pow((1/(double)LATTICE_DIMENSION),3);
  int mu = 0;

  int sigma = alpha*q;

  for(i = 0; i < LATTICE_DIMENSION; i++){
    gauss_vec[i] = (int)normal_distribution(mu,sigma);
  }

  return gauss_vec;
}

int generate_gaussian_scalar(){
  int q = 1;
  int alpha = 1;
  int mu = 0;

  int sigma = alpha*q;

  return (int)normal_distribution(mu,sigma);
}

void generate_Alice_parameters(){

}

void generate_Bob_parameters(){

}

int robust_extractor(int x, int sigma){
  return (int)((((int)x)%MODULO_Q + sigma*((MODULO_Q-1)/2)%MODULO_Q)%2);
}

bool check_robust_extractor(int x, int y){
  double delta = MODULO_Q/4 - 2;

  if((x-y)%2 == 0 && abs(x-y) <= delta){
    return true;
  }
  else{
    return false;
  }

}

int signal_function(int y, int b){
  printf("The value of b is %i\n", b);
  if(b == 0){
    printf("KB is %i\n", y);
    if(y >= -MODULO_Q/4 && y <= MODULO_Q/4){
      return 0;
    }
    else{
      return 1;
    }
  }
  else if(b == 1){
    if(y >= -MODULO_Q/4 + 1 && y <= MODULO_Q/4 + 1){
      return 0;
    }
    else{
      return 1;
    }
  }
  return -1;
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
