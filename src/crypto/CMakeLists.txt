# set(OBERONPATH      "${FETCHCONTENT_BASE_DIR}/sdk-nrfxlib-src/crypto/nrf_oberon")
set(OBERONPATH      "${PROJECT_SOURCE_DIR}/3rd-party/sdk-nrfxlib/crypto/nrf_oberon")

set(RIOT_TEE_CRYPTO "riot-tee-crypto")

add_library(${RIOT_TEE_CRYPTO}
    STATIC
    crypto.c
    hashes.c
)

target_include_directories(${RIOT_TEE_CRYPTO}
    PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/ns_api
    ${PROJECT_SOURCE_DIR}/ns_api/crypto
    ${PROJECT_SOURCE_DIR}/src/crypto
    ${OBERONPATH}/include
)

target_compile_options(${RIOT_TEE_CRYPTO}
    PUBLIC
    "-mcpu=cortex-m33"
    "-mthumb"
    "-mfloat-abi=hard"
    "-mabi=aapcs"
    "-mfpu=fpv5-sp-d16"
    "-mcmse"
)

target_link_libraries(${RIOT_TEE_CRYPTO}
    PUBLIC
    ${OBERONPATH}/lib/cortex-m33/hard-float/liboberon_3.0.15.a
)
