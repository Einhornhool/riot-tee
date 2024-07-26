# set(OBERONPATH      "${FETCHCONTENT_BASE_DIR}/sdk-nrfxlib-src/crypto/nrf_oberon")
set(OBERONPATH      "${PROJECT_SOURCE_DIR}/3rd-party/sdk-nrfxlib/crypto/nrf_oberon")

set(LIBRARY_NAME "riot_tee_crypto")
set(CMAKE_BUILD_TYPE "Debug")

add_library(${LIBRARY_NAME}
    STATIC
    crypto.c
    hashes.c
)

target_include_directories(${LIBRARY_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/ns_api/include
    ${PROJECT_SOURCE_DIR}/ns_api/include/crypto
    ${OBERONPATH}/include
)
target_compile_definitions(${LIBRARY_NAME}
    PUBLIC
    NRF9160_XXAA
    FLOAT_ABI_HARD
)

target_compile_options(${LIBRARY_NAME}
    PUBLIC
    "-mcpu=cortex-m33"
    "-mthumb"
    "-mfloat-abi=hard"
    "-mabi=aapcs"
    "-mfpu=fpv5-sp-d16"
)

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
    riot_tee_ns_api
    ${OBERONPATH}/lib/cortex-m33/hard-float/liboberon_3.0.15.a
)
