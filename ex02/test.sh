
min=1
max=100000

green="\e[0;32m"
red="\e[0;31m"
reset="\e[0m"

total_tests=${1:-3000}
tests_passed=0
tests_failed=0

binary=$(mktemp)
cp ./PmergeMe "$binary"
chmod +x "$binary"
trap 'rm -f $binary' EXIT

for count in $(seq 1 "$total_tests");
do
  echo -n "Testing ${count} numbers... "
  shuf -i "$min"-"$max" -n "$count" | xargs "$binary" >/dev/null
  status=$?
  if [ $status -eq 0 ]; then
    echo -e "$green"sorted"$reset"
    ((tests_passed++))
  else
    echo -e "$red"unsorted"$reset"
    ((tests_failed++))
  fi
done


echo Passed "$tests_passed"/"$total_tests" tests, failed "$tests_failed"
