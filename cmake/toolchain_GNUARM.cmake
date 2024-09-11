set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m33)

set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)

set(TOOLCHAINPATH /home/lena/apps/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/bin)

set(CMAKE_C_COMPILER    ${TOOLCHAINPATH}/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER  ${CMAKE_C_COMPILER})
set(CMAKE_OBJCOPY       ${TOOLCHAINPATH}/arm-none-eabi-objcopy)
set(CMAKE_LINKER        ${TOOLCHAINPATH}/arm-none-eabi-ld)
set(CMAKE_AR            ${TOOLCHAINPATH}/arm-none-eabi-ar)

set(LINKER_VENEER_OUTPUT_FLAG -Wl,--cmse-implib,--out-implib=)
set(COMPILER_CMSE_FLAG -mcmse)

set(CMAKE_USER_MAKE_RULES_OVERRIDE set_extensions.cmake)
