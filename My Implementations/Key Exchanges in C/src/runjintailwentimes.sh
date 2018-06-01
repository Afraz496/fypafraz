#!/bin/bash
number="$1"
success=0
for((i=0; i<$number; i+=1)); do
  output=$(./jintailwe)
  size=${#output}
  if [ $size -gt 0 ]; then
    echo "SUCCESS"
    success=$((success+1))
  else
    echo "FAILED"
  fi
done
echo "The program succeeded for ""$success"" times out of ""$number"
echo
