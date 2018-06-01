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

#include "omp.h"

#include "jintailwe.h"
#include "dgs.h"

int main(){
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
  run_key_exchange();
  return 0;
}

void run_key_exchange(){
  srand(time(NULL));
  generate_M();
  int i, j; // loop index
  //------- Generate Alices parameters --------
  time_t t;
  t = clock();
  generate_gaussian_matrix(Alice_params.secret_matrix);
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("Alice Matrix took: %f\n", time_taken);
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
    printf("%i\n", KB[i]);
  }





  //-- Check the robust extractor condition till correct params generated ----
  /*
  i = 0;
  bool Alice_gen = true;
  bool Bob_gen = false;

  while(i < LATTICE_DIMENSION){
    if(!check_robust_extractor(KA[i], KB[i])){
      //Redo single parameters

      //Alice's Params
      generate_gaussian_vector(Alice1_params.secret_vector);
      generate_gaussian_vector(eA);
      generate_public_vector(Alice1_params.secret_vector,eA,Alice1_params.public_vector,Alice_gen);
      edashA_single = generate_gaussian_scalar();


      //Bob's params
      generate_gaussian_vector(Bob_params.secret_vector);
      generate_gaussian_vector(eB);
      generate_public_vector(Bob_params.secret_vector,eB,Bob_params.public_vector,Bob_gen);
      edashB_single = generate_gaussian_scalar();

      //KA gen:
      for(j = 0; j < LATTICE_DIMENSION; j++){
        KA[i] = KA[i] + Alice1_params.secret_vector[j]*Bob_params.public_vector[j] + 2*edashA_single;
        KB[i] = KB[i] + Alice1_params.public_vector[j]*Bob_params.secret_vector[j] + 2*edashB_single;
      }

      KA[i] = (KA[i] < 0) ? KA[i] % MODULO_Q + MODULO_Q : KA[i] % MODULO_Q;
      KB[i] = (KB[i] < 0) ? KB[i] % MODULO_Q + MODULO_Q : KB[i] % MODULO_Q;
    }
    else{
      i = i+1;
    }
  }
  */

  //Shared Keys
  for(i = 0; i < LATTICE_DIMENSION; i++){
    sig[i] = signal_function(KB[i], rand()%2);
    SKA[i] = robust_extractor(KA[i], sig[i]);
    SKB[i] = robust_extractor(KB[i], sig[i]);
  }

  bool kex_success = true;
  //--- Check if the keys are the same ---
  for(i = 0;i < LATTICE_DIMENSION; i++){
    if(SKA[i] != SKB[i]){
      printf("%i\n", i);
      kex_success = false;
    }
  }

  if(kex_success){
    printf("Key Exchange worked, Alice and Bob Share the same key!\n");
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

void generate_public_vector(int* secret_vec, int*error_vec, int* public_vec, bool client){
  uint16_t i, j;

  if(!client){

    for(i = 0;i < LATTICE_DIMENSION; i++){
      for(j = 0; j < LATTICE_DIMENSION; j++){
          public_vec[i] = public_vec[i] + M_TRANSPOSE[i][j]*secret_vec[j] + 2*error_vec[j];
        }
        public_vec[i] = (public_vec[i] < 0) ? public_vec[i] % MODULO_Q + MODULO_Q : public_vec[i] % MODULO_Q;
      }
  }
  else{
    for(i = 0;i < LATTICE_DIMENSION; i++){
      for(j = 0; j < LATTICE_DIMENSION; j++){
          public_vec[i] = public_vec[i] + M[i][j]*secret_vec[j] + 2*error_vec[j];
        }
        public_vec[i] = (public_vec[i] < 0) ? public_vec[i] % MODULO_Q + MODULO_Q : public_vec[i] % MODULO_Q;
      }
  }
}


//This function is broken for the globals
void generate_gaussian_matrix(int **gauss_matrix){
  int mu = 0;

  int sigma = 512;
  #pragma omp parallel for num_threads(4) collapse(2)
  for(int i = 0; i < 512; i++){
    for(int j = 0; j < 512; j++){
      gauss_matrix[i][j] = discrete_normal_distribution(mu,sigma);
    }
  }
}

void generate_gaussian_vector(int gauss_vec[LATTICE_DIMENSION]){
  int i; //Loop index
  int mu = 0;
  int sigma = LATTICE_DIMENSION;

  for(i = 0; i < LATTICE_DIMENSION; i++){
    gauss_vec[i] = discrete_normal_distribution(mu,sigma);
  }
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
  return ((x-y)%2 == 0 && abs(x-y) <= delta);
}

int signal_function(int y, int b){
  return !(y >= (double)-MODULO_Q/4 + b && y <= (double)MODULO_Q/4 + b);
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
    printf("Vector[%i] = %i\n",i, vec[i]);
  }
}
/*------------------- Generate Gaussian numbers in C -------------------------*/

//Makes use of the dgs library
long discrete_normal_distribution(int mean, int sigma){
  uint8_t tau = 6;
  dgs_disc_gauss_dp_t *D = dgs_disc_gauss_dp_init(sigma,mean,tau,DGS_DISC_GAUSS_UNIFORM_TABLE);
  return D->call(D);
}
