cmake_minimum_required(VERSION 3.10)

project(riot-tee VERSION 0.1 LANGUAGES C ASM)

# if (NOT DEFINED RIOT_BUILD)
set(TOOLCHAINPATH /home/lena/apps/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/bin)

set(CMAKE_C_COMPILER    ${TOOLCHAINPATH}/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER  ${TOOLCHAINPATH}/arm-none-eabi-gcc)
# endif()

if (NOT DEFINED NRFXPATH)
    set(NRFXPATH        "${CMAKE_SOURCE_DIR}/3rd-party/nrfx")
endif()

if (NOT DEFINED CMSISPATH)
    set(CMSISPATH       "${CMAKE_SOURCE_DIR}/3rd-party/CMSIS_5/CMSIS")
endif()

set(LINKER_SCRIPT   "${CMAKE_SOURCE_DIR}/ld/common.ld")
set(NSC_LIB         "${CMAKE_BINARY_DIR}/nsc_lib.o")
set(MAP_FILE        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map")
set(CMAKE_BUILD_TYPE "Debug")

set(FLAGS
    "-mcpu=cortex-m33"
    "-mthumb"
    "-mfloat-abi=hard"
    "-mabi=aapcs"
    "-mfpu=fpv5-sp-d16"
)

set(SOURCES
    src/main.c
    src/ns_entry.c
    # src/non_secure_entry.c
    ${NRFXPATH}/mdk/system_nrf9160.c
)

set(ASM_SOURCES
    # ${NRFXPATH}/mdk/gcc_startup_nrf9160.S
    target/nrf9160/startup.S
)

set_source_files_properties(${ASM_SOURCES}
    PROPERTIES
    COMPILE_FLAGS
    ${FLAGS}
    "-x"
    "assembler-with-cpp"
)

set(EXECUTABLE_NAME ${PROJECT_NAME}.elf)

# Add source files to executable
add_executable(${EXECUTABLE_NAME}
    ${SOURCES}
    ${ASM_SOURCES}
)

# Add include dirs to executable
target_include_directories(${EXECUTABLE_NAME}
    PUBLIC
    ns_api/include
    ns_api/include/crypto
    ns_api/include/rot
    target/nrf9160
    src/rot
    src/crypto
    src/riot_tee_random
    src
    ${CMSISPATH}/Core/Include
    ${NRFXPATH}
    ${NRFXPATH}/mdk
    ${NRFXPATH}/hal
    ${NRFXPATH}/templates
    ${NRFXPATH}/drivers/include
)

# Add compile definitions for NRFX
target_compile_definitions(${EXECUTABLE_NAME}
    PUBLIC
    NRF9160_XXAA
    FLOAT_ABI_HARD
)

# Add crypto code as a library
add_subdirectory(ns_api)
add_subdirectory(src/crypto)
add_subdirectory(src/rot)
add_subdirectory(src/random)
# add_subdirectory(extern)

target_compile_options(${EXECUTABLE_NAME} PUBLIC
    ${FLAGS}
    "-mcmse"
)

target_link_options(${EXECUTABLE_NAME}
    PUBLIC
    ${FLAGS}
    "-T${PROJECT_SOURCE_DIR}/ld/common.ld"
    "-Wl,--cmse-implib"
    "-Wl,--out-implib=${NSC_LIB}"
    "-Wl,--gc-sections"
    "-Wl,-Map=${MAP_FILE}"
    "--specs=nosys.specs"
    "-L${NRFXPATH}/mdk/"
    "-L${PROJECT_SOURCE_DIR}/target/nrf9160/"
)

target_link_libraries(${EXECUTABLE_NAME}
    PUBLIC
    riot_tee_ns_api
    riot_tee_crypto
    riot_tee_rot
    riot_tee_random
    "-Wl,--start-group -lgcc -lc -lnosys"
    "-Wl,--end-group"
)

