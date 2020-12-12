#!/bin/sh 

if [ "$#" -eq "2" ] 
then
   python 2019201073_1.py $1 $2
else
   python 2019201073_2.py $1
fi

