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
 * Key exchange between Alice and Bob
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#include <omp.h>

#include "jintailwe.h"
#include "dgs.h"

dgs_disc_gauss_dp_t *D;

int main(){
  D = dgs_disc_gauss_dp_init(LATTICE_DIMENSION,0,6,DGS_DISC_GAUSS_UNIFORM_TABLE);
  /************ Allocate Temporary Memory on the Fly **************************/
  uint16_t i, j;
  //Alice Memory Allocation
  Alice_params.secret_matrix =      (int**)malloc(LATTICE_DIMENSION*sizeof(int*));
  for(i = 0; i < LATTICE_DIMENSION; i++){
    Alice_params.secret_matrix[i] = (int*)malloc(LATTICE_DIMENSION*sizeof(int));
  }
  Alice_params.public_matrix =      (int**)malloc(LATTICE_DIMENSION*sizeof(int*));
  for(i = 0; i < LATTICE_DIMENSION; i++){
    Alice_params.public_matrix[i] = (int*)malloc(LATTICE_DIMENSION*sizeof(int));
  }

  EA =                              (int**)malloc(LATTICE_DIMENSION*sizeof(int*));
  for(i = 0;i < LATTICE_DIMENSION;i++){
    EA[i] =                         (int*)malloc(LATTICE_DIMENSION*sizeof(int));
  }
  edashA =                          (int*)malloc(sizeof(int)*LATTICE_DIMENSION);

  KA =                              (int*)malloc(sizeof(int)*LATTICE_DIMENSION);

  SKA =                             (int*)malloc(sizeof(int)*LATTICE_DIMENSION);

  //Resampling for Alice:
  Alice1_params.secret_vector =     (int*)malloc(sizeof(int)*LATTICE_DIMENSION);
  Alice1_params.public_vector =     (int*)malloc(sizeof(int)*LATTICE_DIMENSION);
  eA =                              (int*)malloc(sizeof(int)*LATTICE_DIMENSION);

  //Bob Memory Allocation
  Bob_params.secret_vector = (int*)malloc(sizeof(int)*LATTICE_DIMENSION);
  eB =                       (int*)malloc(sizeof(int)*LATTICE_DIMENSION);
  Bob_params.public_vector = (int*)malloc(sizeof(int)*LATTICE_DIMENSION);
  edashB =                   (int*)malloc(sizeof(int)*LATTICE_DIMENSION);
  KB =                       (int*)malloc(sizeof(int)*LATTICE_DIMENSION);
  SKB =                      (int*)malloc(sizeof(int)*LATTICE_DIMENSION);

  //Signal Memory Allocation
  sig =                      (int*)malloc(sizeof(int)*LATTICE_DIMENSION);
  time_t t = clock();
  run_key_exchange();

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("The total time taken for the key exchange is: %f seconds\n",time_taken );

  return 0;
}

void run_key_exchange(){
  srand(time(NULL));
  generate_M();
  int i, j; // loop index
  //------- Generate Alices parameters --------
  generate_gaussian_matrix(Alice_params.secret_matrix);
  generate_gaussian_matrix(EA);

  /*
  Implement the following Algorithm:

  PA = (M.SA + 2*EA) mod q
  */

  //Generate Public Parameter
  for(i = 0; i < LATTICE_DIMENSION; i++){
    for(j = 0; j < LATTICE_DIMENSION; j++){
      Alice_params.public_matrix[i][j] = Alice_params.public_matrix[i][j] + (M[i][j]*Alice_params.secret_matrix[i][j] + 2*EA[i][j]);
      Alice_params.public_matrix[i][j] = (Alice_params.public_matrix[i][j] < 0) ? Alice_params.public_matrix[i][j] % MODULO_Q + MODULO_Q : Alice_params.public_matrix[i][j] % MODULO_Q;
    }
  }


  generate_gaussian_vector(edashA);

  //------- Generate Bobs parameters ----------

  generate_gaussian_vector(Bob_params.secret_vector);
  generate_gaussian_vector(eB);
  generate_gaussian_vector(edashB);

  //Generate Public Parameter
  for(i = 0; i < LATTICE_DIMENSION;i++){
    for(j = 0; j < LATTICE_DIMENSION; j++){
      Bob_params.public_vector[i] = Bob_params.public_vector[i] + (M_TRANSPOSE[i][j]*Bob_params.secret_vector[j] + 2*eB[j]);
    }
    Bob_params.public_vector[i] = (Bob_params.public_vector[i] < 0) ? Bob_params.public_vector[i] % MODULO_Q + MODULO_Q : Bob_params.public_vector[i] % MODULO_Q;
  }


  //Find Alices Key
  for(i = 0; i < LATTICE_DIMENSION; i++){
    for(j = 0; j < LATTICE_DIMENSION; j++){
      KA[i] = KA[i] + Alice_params.secret_matrix[j][i]*Bob_params.public_vector[j] + 2*edashA[j];
    }
    KA[i] = (KA[i] < 0) ? KA[i] % MODULO_Q + MODULO_Q : KA[i] % MODULO_Q;
  }


  //Find Bobs Key

  for(i = 0; i < LATTICE_DIMENSION; i++){
    for(j = 0; j < LATTICE_DIMENSION; j++){
      KB[i] = KB[i] + Alice_params.public_matrix[j][i]*Bob_params.secret_vector[j] + 2*edashB[j];
    }
    KB[i] = (KB[i] < 0) ? KB[i] % MODULO_Q + MODULO_Q : KB[i] % MODULO_Q;
  }





  //-- Check the robust extractor condition till correct params generated ----
  i = 0;
  bool Alice_gen = true;
  bool Bob_gen = false;
  double delta = MODULO_Q/4 - 2;

  while(i < LATTICE_DIMENSION){
    if(!check_robust_extractor(KA[i], KB[i])){
      //Redo single parameters
      long offset = discrete_normal_distribution();
      if((KA[i] - KB[i])%2 != 0){
        KA[i] = KA[i] - 1; //Make it even
      }
      if(abs(KA[i] - KB[i]) > delta){
        if(KA[i] > KB[i]){
          //reduce KA a bit
          KB[i] = KA[i] + offset;
        }
        else{
          //reduce KB a bit
          KB[i] = KA[i] + offset;
        }
      }

    }
    else{
      i = i+1;
    }
  }

  //Shared Keys
  for(i = 0; i < LATTICE_DIMENSION; i++){
    sig[i] = signal_function(KB[i], rand()%2);
    SKA[i] = robust_extractor(KA[i], sig[i]);
    SKB[i] = robust_extractor(KB[i], sig[i]);
  }

  /******* RESULTS **********/

  bool kex_success = true;
  //--- Check if the keys are the same ---
  for(i = 0;i < LATTICE_DIMENSION; i++){
    if(SKA[i] != SKB[i]){
      kex_success = false;
    }
  }

  if(kex_success){
    printf("Key Exchange worked, Alice and Bob Share the same key!\n");
    /*
    printf("Alice's key is:\n");
    pretty_print_vector(SKA);
    printf("\n");
    printf("Bob's key is:\n");
    pretty_print_vector(SKB);
    printf("\n");
    */
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

//This function is broken for the globals
void generate_gaussian_matrix(int **gauss_matrix){

  //#pragma omp parallel for collapse(2)
  for(int i = 0; i < LATTICE_DIMENSION; i++){
    for(int j = 0; j < LATTICE_DIMENSION; j++){
      gauss_matrix[i][j] = discrete_normal_distribution();
    }
  }
}

void generate_gaussian_vector(int gauss_vec[LATTICE_DIMENSION]){
  int i; //Loop index
  for(i = 0; i < LATTICE_DIMENSION; i++){
    gauss_vec[i] = discrete_normal_distribution();
  }
}

int generate_gaussian_scalar(){
  return discrete_normal_distribution();
}

int robust_extractor(int x, int sigma){
  return ((((int)x)%MODULO_Q + (int64_t)(sigma * (MODULO_Q - 1)/2)%MODULO_Q)%2);
}

bool check_robust_extractor(int x, int y){
  double delta = MODULO_Q/4 - 2;
  return ((x-y)%2 == 0 && abs(x-y) <= delta);
}

int signal_function(int y, int b){
  return !(y >= floor(-MODULO_Q/4) + b && y <= floor(MODULO_Q/4) + b);
}

void pretty_print_matrix(int **matrix){
  int i, j;
  for(i = 0; i < LATTICE_DIMENSION; i++){
    for(j = 0; j < LATTICE_DIMENSION; j++){
      printf("Matrix[%i][%i] = %i\n", i, j, matrix[i][j]);
    }
    printf("\n");
  }
}

void pretty_print_vector(int vec[LATTICE_DIMENSION]){
  int i;
  for(i = 0; i < LATTICE_DIMENSION; i++){
    printf("%i", vec[i]);
  }
}
/*------------------- Generate Gaussian numbers in C -------------------------*/

//Makes use of the dgs library
inline long discrete_normal_distribution(){
  long val = D->call(D);
  return val;
}
