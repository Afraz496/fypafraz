#!usr/bin/env python

# Please note that where used % denotes the modulo operator

#As defined in section 2 of the paper a robust extractor takes in input
#an integer modulo q, a signal sigma defined in the 'signal' function and a prime number q

from random import *
import numpy
from numpy import *
import scipy.stats as ss



#NB: x is a numpy array so conversion required, x is KA or KB
def robust_extractor(x, sigma, q):
    return int((int(x)%q + sigma*((q-1)/2)%q)%2)

#b will determine what type of signal function you require, inputs are x as an integer mod q, q as a prime number and b (signal case)
def signal_functions(y, b, q):
    if b == 0:
        if y >= -q/4 or y <= q/4:
            return 0
        else:
            return 1
    elif b == 1:
        if y >= -q/4 + 1 or y <= q/4 + 1:
            return 0
        else:
            return 1

def check_robust_extractor(x,y,q):
    delta = q/4 - 2
    if (x-y)%2 == 0 and abs(x-y) <= delta:
        return True
    else:
        return False

#This function makes an n x n matrix of integers mod q
def generate_matrix_M(n, q):

    #Declare a matrix here:
    M = numpy.zeros((n,n))

    #Build the matrix here:
    for i in range(0,n):
        for j in range(0,n):
            M[i][j] = randint(0,q-1)
    return M

def generate_gaussian_vector(n):
    # parameter selection, n = lambda, q = lambda ^ 4, alpha = (lambda)^(-3)
    q = n**4
    alpha = (1/float(n))**3
    mu = 0 #page 5 of the paper
    sigma = alpha*q
    return numpy.random.normal(mu,sigma,n).astype(int)

def generate_gaussian_scalar():
    # parameter selection, n = lambda, q = lambda ^ 4.
    q = 1
    alpha = 1
    mu = 0
    sigma = alpha*q
    return numpy.random.normal(mu,sigma,1).astype(int)

def generate_alice_params(M,n,q):
    sA = generate_gaussian_vector(n)
    eA = generate_gaussian_vector(n)
    pA = M.dot(sA) + 2*eA%q
    return pA,sA

def generate_bob_params(M,n,q):
    sB = generate_gaussian_vector(n)
    eB = generate_gaussian_vector(n)
    pB = numpy.transpose(M).dot(sB) + 2*eB%q
    return pB,sB

def run_key_exchange(n,q):
    #M is a matrix of integers mod q and has dimensions n x n
    M = generate_matrix_M(n,q)
    pA,sA = generate_alice_params(M,n,q)
    pB,sB = generate_bob_params(M,n,q)

    edashA = generate_gaussian_scalar()
    edashB = generate_gaussian_scalar()

    KA = numpy.transpose(sA).dot(pB) + 2*edashA%q
    KB = numpy.transpose(pA).dot(sB) + 2*edashB%q

    signal = signal_functions(KB,0,q)
    #---------ensuring Robust extractor property is preserved--------

    while(not check_robust_extractor(KA,KB,q)):
        pA,sA = generate_alice_params(M,n,q)
        pB,sB = generate_bob_params(M,n,q)
        edashA = generate_gaussian_scalar()
        edashB = generate_gaussian_scalar()
        KA = numpy.transpose(sA).dot(pB) + 2*edashA%q
        KB = numpy.transpose(pA).dot(sB) + 2*edashB%q
        signal = signal_functions(KB,0,q)
    
    #---------------Generating shared keys------
    SKA = robust_extractor(KA,signal,q)
    SKB = robust_extractor(KB,signal,q)
    #-----------------Results-------------------
    print("KA: " + str(KA))
    print("KB: " + str(KB))

    print("q/4: " + str(q/4))
    print("KA/4: " + str(KA/4))
    print("KB/4: " + str(KB/4))

    print("SKA: " + str(SKA))
    print("SKB: " + str(SKB))

    if(check_robust_extractor(KA,KB,q)):
        print("Robust Extractor worked")
    else:
        print("Robust Extractor failed")

    if SKA == SKB:
        print("Alice and Bob share the same key!")


def main():

    n = 100
    q = 1024

    run_key_exchange(n,q)

#initialisation
if __name__ == "__main__":
    main()
