set(LIBRARY_NAME "riot_tee_ns_api")
set(CMAKE_BUILD_TYPE "Debug")

add_library(${LIBRARY_NAME}
    STATIC
    src/tee_hashes.c
)

target_include_directories(${LIBRARY_NAME}
    PUBLIC
    include
    include/crypto
)

target_compile_options(${LIBRARY_NAME}
    PUBLIC
    "-mcpu=cortex-m33"
    "-mthumb"
    "-mfloat-abi=hard"
    "-mabi=aapcs"
    "-mfpu=fpv5-sp-d16"
)
