#!/bin/bash
number="$1"
for((i=0; i<$number; i+=1)); do
  python simplekeyexchange.py
done
