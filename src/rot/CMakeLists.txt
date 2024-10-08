set(LIBRARY_NAME "riot_tee_rot")

add_library(${LIBRARY_NAME}
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tee_rot.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tee_cipher_block.c
    ${CRYPTOSERVICE_RIOT_LIBS}/sys/crypto/ciphers.c
    ${CRYPTOSERVICE_RIOT_LIBS}/sys/crypto/modes/ocb.c
)

set(CC3XX_INCLUDES
    ${TFM_PATH}/platform/include
    ${CC3XXPATH}/low_level_driver/include
    ${CC3XXPATH}/low_level_driver/src
    ${CC3XXPATH}/common
)

target_include_directories(${LIBRARY_NAME}
    PRIVATE
    ${TARGET_DIR}
    ${CC3XX_INCLUDES}
    ${CMAKE_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/api
    ${PROJECT_SOURCE_DIR}/src/crypto
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
