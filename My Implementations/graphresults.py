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

with open('simplekexresults.txt', 'r') as k:
    lines = k.read().splitlines()

indexes = [18, 24, 22, 15, 22, 24, 28, 18]
times = [""]*7
print(times)
for i in range(0, len(lines)):
    line = lines[i]
    times[i] = line[indexes[i]:indexes[i]+5]

print(times)
