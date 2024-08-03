set(LIBRARY_NAME "riot_tee_ns_api")

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
    ${FLAGS}
)
