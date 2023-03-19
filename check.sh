#!/bin/bash

echo "" > $1/ans1.txt
chmod 777 $1/ans1.txt
gcc $1/main.c -o $1/a.out
echo "test1_diff:"
$1/a.out tests/01 $1/ans1.txt
sleep 0.5
diff -w -B -c tests/01.a $1/ans1.txt
sleep 0.5

echo "" > $1/ans2.txt
chmod 777 $1/ans2.txt
gcc $1/main.c -o $1/a.out
echo "test2_diff:"
$1/a.out tests/02 $1/ans2.txt
sleep 0.5
diff -w -B -c tests/02.a $1/ans2.txt
sleep 0.5

echo "" > $1/ans3.txt
chmod 777 $1/ans3.txt
gcc $1/main.c -o $1/a.out
echo "test3_diff:"
$1/a.out tests/03 $1/ans3.txt
sleep 0.5
diff -w -B -c tests/03.a $1/ans3.txt
sleep 0.5

echo "" > $1/ans4.txt
chmod 777 $1/ans4.txt
gcc $1/main.c -o $1/a.out
echo "test4_diff:"
$1/a.out tests/04 $1/ans4.txt
sleep 0.5
diff -w -B -c tests/04.a $1/ans4.txt
sleep 0.5

echo "" > $1/ans5.txt
chmod 777 $1/ans5.txt
gcc $1/main.c -o $1/a.out
echo "test5_diff:"
$1/a.out tests/05 $1/ans5.txt
sleep 0.5
diff -w -B -c tests/05.a $1/ans5.txt
sleep 0.5

rm $1/a.out
