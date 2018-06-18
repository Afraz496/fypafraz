success=0
while true
do
  python simplekeyexchange.py
  success=$((success+1))
  echo "Ran for: "$success" times."
done
