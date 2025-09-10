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

# Suppressions

## MISRA 2008
- `-clion-misra-cpp2008-5-0-11` The plain char type shall only be used for the storage and use of character values
  - Doesn't recognize `std::uint8_t` even in the cases where it's matching the underlying HW concept/register and treats `std::uint8_t` as plain `char`
- `-clion-misra-cpp2008-6-6-5` A function shall have a single point of exit at the end of function
  - Could make some code more nested code and harder to read as not always is possible to set the return value in a variable and keep changing until hitting single return

## MISRA 2023
- `-clion-misra-cpp2023-17-8-1` Function templates shall not be explicitly specialized
  - It's necessary for compile-time recursion-based for-loop-style iterations

## Clion spell checking

```xml
<component name="ProjectDictionaryState">
  <dictionary name="project">
    <words>
      <w>acdpre</w>
      <w>addr</w>
      <w>afio</w>
      <w>baudrate</w>
      <w>bshr</w>
      <w>cfghr</w>
      <w>cfglr</w>
      <w>cfgr</w>
      <w>cntr</w>
      <w>cssc</w>
      <w>cssf</w>
      <w>ctlr</w>
      <w>datar</w>
      <w>eabien</w>
      <w>extem</w>
      <w>hclk</w>
      <w>hseon</w>
      <w>hserdy</w>
      <w>hserdyc</w>
      <w>hsical</w>
      <w>hsirdyc</w>
      <w>htif</w>
      <w>indr</w>
      <w>inesten</w>
      <w>initfr</w>
      <w>intfcr</w>
      <w>intfr</w>
      <w>intr</w>
      <w>intsyscr</w>
      <w>lckr</w>
      <w>lsion</w>
      <w>lsirdy</w>
      <w>lsirdyc</w>
      <w>maddr</w>
      <w>mpie</w>
      <w>msize</w>
      <w>mstatus</w>
      <w>nrst</w>
      <w>outdr</w>
      <w>paddr</w>
      <w>pfic</w>
      <w>pllrdyc</w>
      <w>psize</w>
      <w>rmvf</w>
      <w>rstsckr</w>
      <w>statr</w>
      <w>swio</w>
      <w>sysclk</w>
      <w>tcif</w>
      <w>teif</w>
      <w>usart</w>
      <w>usartdiv</w>
    </words>
  </dictionary>
</component>
```

