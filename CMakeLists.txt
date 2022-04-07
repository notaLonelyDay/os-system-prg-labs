cmake_minimum_required(VERSION 3.22)
project(osisp C)

set(CMAKE_C_STANDARD 17)

# lab2
add_executable(l2main1 lab2/main1.c)
add_executable(l2main2 lab2/main2.c)
add_executable(l2main3 lab2/main3.c)
add_executable(l2main4 lab2/main4.c)
add_executable(l2main5 lab2/main5.c)
add_executable(l2main lab2/main.c)

add_executable(l3main lab3/main.c)
add_executable(l3common lab3/common.c)
