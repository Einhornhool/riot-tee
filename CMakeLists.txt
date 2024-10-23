cmake_minimum_required(VERSION 3.30)

project(riot-tee VERSION 0.1 LANGUAGES C ASM)

set(RIOT_TEE_CIPHER_AES_HW True)

if (NOT DEFINED TOOLCHAINPATH)
    message(WARNING "Using default toolchain, may lead to errors.")
    message(WARNING "Recommended: Arm GNU 11.2-2022.02")
    set(TOOLCHAINPATH       /usr/bin)
endif()

set(CMAKE_C_COMPILER    ${TOOLCHAINPATH}/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER  ${TOOLCHAINPATH}/arm-none-eabi-gcc)

set(DEPENDENCIES_PATH       "${CMAKE_BINARY_DIR}/_deps")
set(CRYPTOSERVICE_PATH      ${RIOT_CRYPTOSERVICE})
set(CRYPTOSERVICE_RIOT_LIBS ${CRYPTOSERVICE_PATH}/libs/RIOT)

# set(NRFXPATH            "${DEPENDENCIES_PATH}/nrfx-src")
# set(CMSISPATH           "${DEPENDENCIES_PATH}/cmsis_5-src/CMSIS")

set(TFM_PATH            "~/work/repos/tf-m-github")
set(CC3XXPATH           "${TFM_PATH}/platform/ext/target/arm/drivers/cc3xx")
set(NRFXPATH            "${PROJECT_SOURCE_DIR}/3rd-party/nrfx")
set(CMSISPATH           "${PROJECT_SOURCE_DIR}/3rd-party/CMSIS_5/CMSIS")
set(CMSISDEVPATH        "${PROJECT_SOURCE_DIR}/3rd-party/CMSIS_5/Device/ARM/ARMCM33/Include")

set(TARGET_DIR   "${PROJECT_SOURCE_DIR}/target/nrf9160")

set(LINKER_SCRIPT       "${CMAKE_SOURCE_DIR}/ld/common.ld")
set(NSC_LIB             "${CMAKE_BINARY_DIR}/nsc_lib.o")
set(MAP_FILE            "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map")
set(CMAKE_BUILD_TYPE    "MinSizeRel")
# set(CMAKE_BUILD_TYPE    "Debug")
set(FLAGS
    "-mcpu=cortex-m33"
    "-mthumb"
    "-mfloat-abi=hard"
    "-mabi=aapcs"
    "-mfpu=fpv5-sp-d16"
    "-Os"
)

set(SOURCES
    src/main.c
    src/entry/tee_secure_entry.c
    ${NRFXPATH}/mdk/system_nrf9160.c
)

set(ASM_SOURCES
    ${NRFXPATH}/mdk/gcc_startup_nrf9160.S
)

set_source_files_properties(${ASM_SOURCES}
    PROPERTIES
    COMPILE_FLAGS
    "-mcpu=cortex-m33"
    "-mthumb"
    "-mfloat-abi=hard"
    "-mabi=aapcs"
    "-mfpu=fpv5-sp-d16"
    "-x"
    "assembler-with-cpp"
)

# Always compile cc3xx_pka.c with -Ofast for optimized speed
set_source_files_properties(${CC3XXPATH}/low_level_driver/src/cc3xx_pka.c
    PROPERTIES COMPILE_FLAGS -Ofast
)

set(EXECUTABLE_NAME ${PROJECT_NAME}.elf)

include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)

# Add source files to executable
add_executable(${EXECUTABLE_NAME}
    ${SOURCES}
    ${ASM_SOURCES}
)

add_subdirectory(api)
add_subdirectory(src/crypto)
add_subdirectory(src/rot)
add_subdirectory(src/random)

# Add include dirs to executable
target_include_directories(${EXECUTABLE_NAME}
    PUBLIC
    ${TFM_PATH}/platform/include
    ${CC3XXPATH}/common
    ${CC3XXPATH}/low_level_driver/include
    ${CRYPTOSERVICE_PATH}/api
    ${CRYPTOSERVICE_PATH}/include
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/include
    api/include
    ${TARGET_DIR}
    src/crypto
    src/entry
    src/random
    src/rot
    ${CMSISDEVPATH}
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

target_compile_options(${EXECUTABLE_NAME} PUBLIC
    ${FLAGS}
    "-mcmse"
    "-Wall"
    "-Werror"
    "-Os"
)

target_link_options(${EXECUTABLE_NAME}
    PUBLIC
    ${FLAGS}
    "-T${PROJECT_SOURCE_DIR}/ld/common.ld"
    "-Wl,--cmse-implib"
    "-Wl,--out-implib=${NSC_LIB}"
    "-Wl,-Map=${MAP_FILE}"
    "--specs=nosys.specs"
    "-L${NRFXPATH}/mdk/"
    "-L${PROJECT_SOURCE_DIR}/target/nrf9160/"
)

target_link_libraries(${EXECUTABLE_NAME}
    PUBLIC
    riot_tee_api
    riot_tee_crypto
    riot_tee_rot
    riot_tee_random
    "-Wl,--start-group -lgcc -lc -lnosys"
    "-Wl,--end-group"
)

# RIOT can set RIOT_INSTALL_PREFIX to decide where API header files are installed
# Default will be the build directory
# if (NOT DEFINED RIOT_INSTALL_PREFIX)
#   set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
# else()
#   set(CMAKE_INSTALL_PREFIX ${RIOT_INSTALL_PREFIX})
# endif()

# # Declare an interface library to export only headers for linking with other implementations
# add_library(cys_interface INTERFACE)
# target_sources(cys_interface
#   PUBLIC
#   FILE_SET HEADERS
#   BASE_DIRS ${PROJECT_SOURCE_DIR}/api
#   FILES
#     ${PROJECT_SOURCE_DIR}/api/CYS/common.h
#     ${PROJECT_SOURCE_DIR}/api/CYS/sealed_key.h
#     ${PROJECT_SOURCE_DIR}/api/CYS/unprotected.h
# )

# # Add install target to install headers at a specified location
# install(
#   TARGETS cys_interface
#   FILE_SET HEADERS
#   DESTINATION api
# )
