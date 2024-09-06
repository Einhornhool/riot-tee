set(LIBRARY_NAME "riot_tee_crypto")

add_library(${LIBRARY_NAME}
    STATIC
    hashes.c
    ecc.c
)

target_include_directories(${LIBRARY_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/ns_api/include
    ${PROJECT_SOURCE_DIR}/ns_api/include/crypto
    ${PROJECT_SOURCE_DIR}/src/random
    ${PROJECT_SOURCE_DIR}/src/rot
    ${PROJECT_SOURCE_DIR}/src
    ${OBERONPATH}/include
    ${CRYPTOSERVICE_PATH}/api
)
target_compile_definitions(${LIBRARY_NAME}
    PUBLIC
    NRF9160_XXAA
    FLOAT_ABI_HARD
)

target_compile_options(${LIBRARY_NAME}
    PUBLIC
    ${FLAGS}
    "-Wall"
    "-Werror"
    "-Wpedantic"
)

target_link_options(${LIBRARY_NAME}
    PUBLIC
    ${FLAGS}
)

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
    ${OBERONPATH}/lib/cortex-m33/hard-float/liboberon_3.0.15.a
)
