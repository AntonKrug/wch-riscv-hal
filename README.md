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