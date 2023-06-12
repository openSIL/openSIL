#!/bin/bash
# This is script that parses input from http report feed variables into json formatted string
# It is used to populate file that Github actions read and populate review board screen
# for in line review of automated CI

arr=();
while read x y;
do
  arr=("${arr[@]}" $x $y)
done
vars=(${arr[@]})
len=${#arr[@]}
printf "{"
for (( i=0; i<len; i+=2 ))
  do
    if [[ ${vars[i+1]} =~ ^[0-9]+$ ]]
    then
      printf "\"${vars[i]}\": ${vars[i+1]}"
      if [ $i -lt $((len-2)) ] ; then
        printf ", "
      fi
    else
      printf "\"${vars[i]}\": \"${vars[i+1]}\""
      if [ $i -lt $((len-2)) ] ; then
        printf ", "
      fi
    fi
  done
printf "}"
echo
