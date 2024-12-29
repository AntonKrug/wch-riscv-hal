cmake_minimum_required(VERSION 3.28)
project(wch-riscv-hal)

set(CMAKE_CXX_STANDARD 20)

add_executable(wch-riscv-hal main.cpp
        peripheral/ch32v00x/gpio.h
        system/ch32v003/soc.h)
