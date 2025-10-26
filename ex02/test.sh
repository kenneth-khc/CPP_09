
min=1
max=100000

green="\e[0;32m"
red="\e[0;31m"
reset="\e[0m"

total_tests=${1:-3000}
tests_passed=0
tests_failed=0

while true; do
  read -p "View full details? y/n " response

  if [[ "$response" == "y" || "$response" == "n" ]]; then
    break
  fi
done

binary=$(mktemp)
cp ./PmergeMe "$binary"
chmod +x "$binary"
trap 'rm -f $binary' EXIT

for count in $(seq 1 "$total_tests");
do
  echo "Testing ${count} numbers..."
  output=$(shuf -i "$min"-"$max" -n "$count" | xargs "$binary")
  status=$?
  if [ $status -ne 0 ]; then
    echo -e "$red"SOMETHING WENT WRONG!"$reset"
    echo "Program output:"
    echo "$output"
  fi
  sorted_line=$(echo "$output" | head -n 2 | tail -n 1)
  if [[ "$response" == "y" ]]; then
    line="$output"
  else
    line="$sorted_line"
  fi
  echo "$line"
  echo "$sorted_line" | sed 's/^[^0-9]*//' | tr ' ' '\n' | sort -guC
  status=$?
  if [ $status -eq 0 ]; then
    echo -e "$green"SORTED!"$reset"
    ((tests_passed++))
  else
    echo -e "$red"UNSORTED!"$reset"
    ((tests_failed++))
  fi
  echo "..."
  sleep 0.5
done


echo Passed "$tests_passed"/"$total_tests" tests, failed "$tests_failed"
