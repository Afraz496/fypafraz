/********************************************************************************************
 * A simple provably secure key exchange based on the learning with errors problem
 *
 *
 * Based on the paper:
 *     Jintai Ding, Xiang Xie and Xiaodong Ling - 2012
 *
 * Copyright (c) Jintai Ding, Xiang Xie and Xiaodong Ling for the theoretical key exchange
 *               Afraz Arif Khan for implementing the key exchange in C and TLS
 *
 * Released under the MIT License; see LICENSE.txt for details.
 ********************************************************************************************/

/** \file jintailwe.c
 * Key exchange between Alice and Bob.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#include "jintailwe.h"
#include "dgs.h"

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
      pA[i] = pA[i] + (M_TRANSPOSE[i][j]*sA[j] + 2*eA[j]);
    }
    pA[i] = (pA[i] < 0) ? pA[i]%MODULO_Q + MODULO_Q : pA[i]%MODULO_Q;
  }
  //------- Generate Bobs parameters ----------
  int *sB = generate_gaussian_vector();
  int *eB = generate_gaussian_vector();
  int pB[LATTICE_DIMENSION];

  for(i = 0; i < LATTICE_DIMENSION;i++){
    for(j = 0; j < LATTICE_DIMENSION; j++){
      pB[i] = pB[i] + (M_TRANSPOSE[i][j]*sB[j] + 2*eB[j]);
    }
    pB[i] = (pB[i] < 0) ? pB[i]%MODULO_Q + MODULO_Q : pB[i]%MODULO_Q;
  }

  int edashA = generate_gaussian_scalar();
  int edashB = generate_gaussian_scalar();

  //Find Alices Key

  int KA = 0;
  for(i = 0; i < LATTICE_DIMENSION; i++){
    KA = KA + (sA[i]*pB[i] + 2*edashA);
    KA = (KA < 0) ? KA % MODULO_Q + MODULO_Q : KA % MODULO_Q;
  }

  //Find Bobs Key

  int KB = 0;
  for(i = 0; i < LATTICE_DIMENSION; i++){
    KB = KB + (pA[i]*sB[i] + 2*edashB);
    KB = (KB < 0) ? KB % MODULO_Q + MODULO_Q : KB % MODULO_Q;
  }

  //-- Check the robust extractor condition till correct params generated ----
  /*
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

    //TODO: KA and KB are negative, revise logic

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
  */
  //Obtain a signal
  int sig = signal_function(KB, 0);
  //-- Obtain shared keys between Alice and Bob ----
  int SKA = robust_extractor(KA, sig);
  int SKB = robust_extractor(KB, sig);

  //--- Check if the keys are the same ---
  if(SKA == SKB){
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
  int mu = 0;

  int sigma = LATTICE_DIMENSION;

  for(i = 0; i < LATTICE_DIMENSION; i++){
    gauss_vec[i] = discrete_normal_distribution(mu,sigma);
    printf("gauss_vec[%i] = %i\n", i, gauss_vec[i]);
  }

  return gauss_vec;
}

int generate_gaussian_scalar(){
  int mu = 0;
  int sigma = 1;
  return discrete_normal_distribution(mu,sigma);
}

int robust_extractor(int x, int sigma){
  return ((((int)x)%MODULO_Q + (int64_t)(sigma * (MODULO_Q)/2)%MODULO_Q)%2);
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
  if(b == 0){
    if(y >= (double)-MODULO_Q/4 && y <= (double)MODULO_Q/4){
      return 0;
    }
    else{
      return 1;
    }
  }
  else if(b == 1){
    if(y >= (double)-MODULO_Q/4 + 1 && y <= (double)MODULO_Q/4 + 1){
      return 0;
    }
    else{
      return 1;
    }
  }
}

/*------------------- Generate Gaussian numbers in C -------------------------*/

//This is using the dgs library, all copyright is protected under the source files of the dgs library
long discrete_normal_distribution(int mean, int sigma){
  uint8_t tau = 6;
  dgs_disc_gauss_dp_t *D = dgs_disc_gauss_dp_init(sigma,mean,tau,DGS_DISC_GAUSS_UNIFORM_TABLE);
  return D->call(D);
}
