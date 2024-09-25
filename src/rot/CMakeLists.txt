set(LIBRARY_NAME "riot_tee_rot")

add_library(${LIBRARY_NAME}
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tee_rot.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cipher_block.c
    ${TARGET_DIR}/cc310_driver/cc310_aes_128.c
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/hashes/sha2xx_common.c
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/hashes/sha256.c
    ${CRYPTOSERVICE_RIOT_LIBS}/sys/crypto/ciphers.c
    # ${CRYPTOSERVICE_RIOT_LIBS}/sys/crypto/aes.c
    ${CRYPTOSERVICE_RIOT_LIBS}/sys/crypto/modes/ocb.c
    ${CRYPTOSERVICE_PATH}/src/puf.c
)

target_include_directories(${LIBRARY_NAME}
    PRIVATE
    ${TARGET_DIR}
    ${CMAKE_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/api
    ${PROJECT_SOURCE_DIR}/src/random
    ${NRFXPATH}/mdk
    ${CMSISPATH}/Core/Include
    ${CMSISDEVPATH}
    ${OBERONPATH}/include
    ${CRYPTOSERVICE_PATH}/api
    ${CRYPTOSERVICE_PATH}/include
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/include
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/include/crypto
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/include/crypto/modes
)

target_compile_options(${LIBRARY_NAME}
    PUBLIC
    ${FLAGS}
)
