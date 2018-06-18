success=0
while true
do
  ./jintailwe
  success=$((success+1))
  echo "Ran for: "$success" times."
done
