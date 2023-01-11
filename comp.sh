#! /bin/bash
gcc -ansi -Wall -Wextra -Werror -pedantic-errors spkmeans.c -lm -o spkmeans
python3 setup.py build_ext --inplace
