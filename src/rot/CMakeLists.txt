set(LIBRARY_NAME "riot_tee_rot")

add_library(${LIBRARY_NAME}
    STATIC
    rot.c
    rot_puf_sram.c
    ${CRYPTOSERVICE_RIOT_LIBS}/sys/crypto/ciphers.c
    ${CRYPTOSERVICE_RIOT_LIBS}/sys/crypto/aes.c
    ${CRYPTOSERVICE_RIOT_LIBS}/sys/crypto/modes/ocb.c
)

target_include_directories(${LIBRARY_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/ns_api/include
    ${PROJECT_SOURCE_DIR}/ns_api/include/key_storage
    ${PROJECT_SOURCE_DIR}/src/random
    ${NRFXPATH}/mdk
    ${CMSISPATH}/Core/Include
    ${OBERONPATH}/include
    ${CRYPTOSERVICE_PATH}/api
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/include
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/include/crypto
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/include/crypto/modes
)

target_compile_options(${LIBRARY_NAME}
    PUBLIC
    ${FLAGS}
)
