#!/bin/bash

if [[ -d "./list_copied/" ]]; then
  rm -r list_copied/
fi
cp -r list/ list_copied/
rm list_copied/list.o
./copy_types.sh

echo "Compilo"
my_gcc="gcc -Wall" || exit

cd list_copied
$my_gcc -c list.c -o list_tmp.o || exit

cd list_dynamic
for dir in */; do
  cd "$dir"
  ls *.c | xargs $my_gcc -c || exit
  cd ..
done
cd ..

cd list_table
for dir in */; do
  cd "$dir"
  ls *.c | xargs $my_gcc -c || exit
  cd ..
done
cd ..

find ./ -type f -name "*.o" | xargs ld -r -o ../list/list.o
cd ..
rm -r list_copied

cd test/
$my_gcc main.c ../list/list.o -o test || exit

# cd ../test/
# $my_gcc main.c ../lista/lista.o -o test
