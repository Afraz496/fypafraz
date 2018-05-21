#!usr/bin/env python

def parameter_selection(n,q):
    alpha = (1/float(n))**3
    print("SIDE1: " + str(8*((alpha*q)**2)*n))
    print("SIDE2: " + str((q/4) - 2))
    if 8*((alpha*q)**2)*n <= (q/4) - 2:
        return True
    else:
        return False


def main():

    n = 512
    q = 2**32 - 1
    if parameter_selection(n,q):
        print("These parameters work!")
    else:
        print("These paramters do not work, try again")


if __name__ == '__main__':
    main()
