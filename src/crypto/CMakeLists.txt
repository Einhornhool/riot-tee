set(OBERONPATH      "${PROJECT_SOURCE_DIR}/3rd-party/sdk-nrfxlib/crypto/nrf_oberon")

set(LIBRARY_NAME "riot_tee_crypto")

add_library(${LIBRARY_NAME}
    STATIC
    hashes.c
)

target_include_directories(${LIBRARY_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/ns_api/include
    ${PROJECT_SOURCE_DIR}/ns_api/include/crypto
    ${PROJECT_SOURCE_DIR}/src/random
    ${OBERONPATH}/include
)
target_compile_definitions(${LIBRARY_NAME}
    PUBLIC
    NRF9160_XXAA
    FLOAT_ABI_HARD
)

target_compile_options(${LIBRARY_NAME}
    PUBLIC
    ${FLAGS}
)

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
    riot_tee_ns_api
    ${OBERONPATH}/lib/cortex-m33/hard-float/liboberon_3.0.15.a
)
