#!usr/env/python
"""
This program is specifically designed for simplekeyexchange.py. It requires use of
the print_time function and specifically printed outputs on the git hash:
This program, graphs the results from a file which has recorded 'n' samples of output
from simplekeyexchange.py. This program then parses the appropriate data to build graphs and record averages
for testing purposes:

---------SAMPLE OUTPUT----------
Generating M took 3.875 milliseconds.
Alice's parameters took 29.209 milliseconds.
Bob's parameters took 1.619 milliseconds.
KA and KB took 5.323 milliseconds.
This while loop took 13.337 milliseconds.
Alice and Bob share the same key!
This program took 58.392 milliseconds.
----END OF SAMPLE OUTPUT--------

Based on the sample, noticeable values (the milliseconds values are located at and 'same' for line 6):
1) INDEX 18 to INDEX 22
2) INDEX 24 to INDEX 29
3) INDEX 22 to INDEX 27
4) INDEX 15 to INDEX 19
5) INDEX 22 to INDEX 27
6) INDEX 24 to INDEX 27 (Modify this to 29 for easier code logic)
7) INDEX 18 to INDEX 23
"""

import sys
import matplotlib.pyplot as plt

def calculate_length_without_correctness(length):
    done = False
    new_length = length
    i = 5
    while(not done):
        new_length -= 1
        i += 7
        if i > new_length:
            done = True
    return new_length

def avg(l):
    return sum(l)/len(l)

def correctness(same):
    if same == "same ":
        return True
    else:
        return False

with open('simplekexresults.txt', 'r') as k:
    lines = k.read().splitlines()

indexes = [18, 24, 22, 15, 21, 24, 18]
times = [""]*len(lines)
for i in range(0, len(lines)):
    line = lines[i]
    times[i] = line[indexes[i%7]:indexes[i%7]+5]

del(times[5::7]) #Remove same

#Convert to numbers
M_gen = []
Alice_params = []
Bob_params = []
Key_gen = []
While_loop = []
total_execution = []

OFFSET_AFTER_DELETION = 6
#Conversion to floats and appending invidiual params
for i in range(0, len(times)):
    times[i] = float(times[i])
    if i%OFFSET_AFTER_DELETION == 0:
        M_gen.append(times[i])
    elif i%OFFSET_AFTER_DELETION == 1:
        Alice_params.append(times[i])
    elif i%OFFSET_AFTER_DELETION == 2:
        Bob_params.append(times[i])
    elif i%OFFSET_AFTER_DELETION == 3:
        Key_gen.append(times[i])
    elif i%OFFSET_AFTER_DELETION == 4:
        While_loop.append(times[i])
    elif i%OFFSET_AFTER_DELETION == 5:
        total_execution.append(times[i])

plt.plot(total_execution)
plt.ylabel('Total program execution time/ms')
plt.show()
print("Average run time for M is " + str(avg(M_gen)))
print("Average run time for Alice Params is " + str(avg(Alice_params)))
print("Average run time for Bob Params is " + str(avg(Bob_params)))
print("Average run time for Key_gen is " + str(avg(Key_gen)))
print("Average run time for While Loop is " + str(avg(While_loop)))
print("Average run time for total key exchange is " + str(avg(total_execution)))
