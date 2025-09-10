# Notes

- Tested on Windows 10
- Cmake at this point depends on c:\progs\busybox.exe
- Clion 2024.1.4 & 2024.3.5 settings
  - Build, Execution, Deployment
    - Toolchains (gcc12 taken from Mounriver 2.1.0)
      - Cmake - bundled one - C:\Program Files\JetBrains\CLion 2024.3.5\bin\cmake\win\x64\bin\cmake.exe
      - Ninja - bundled one - C:\Program Files\JetBrains\CLion 2024.3.5\bin\ninja\win\x64\ninja.exe
      - C and C++ compiler, needs to be provided in Clion as well in the Cmake (`C` in path, is case sensisitve, some stuff will work, some will break) 
        - C:\progs\wch-gcc-12\bin\riscv-wch-elf-gcc.exe
        - C:\progs\wch-gcc-12\bin\riscv-wch-elf-g++.exe
    - Cmake
      - Create DEBUG profile and add -DCMAKE_VERBOSE_MAKEFILE=ON for troubleshooting

# Suppressed checks

## MISRA 2008
- `-clion-misra-cpp2008-5-0-11` The plain char type shall only be used for the storage and use of character values
  - Doesn't recognize `std::uint8_t` even in the cases where it's matching the underlying HW concept/register and treats `std::uint8_t` as plain `char`
- `-clion-misra-cpp2008-6-6-5` A function shall have a single point of exit at the end of function
  - Could make some code more nested code and harder to read as not always is possible to set the return value in a variable and keep changing until hitting single return

## MISRA 2023
- `-clion-misra-cpp2023-17-8-1` Function templates shall not be explicitly specialized
  - It's necessary for compile-time recursion-based for-loop-style iterations 