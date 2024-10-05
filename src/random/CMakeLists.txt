set(LIBRARY_NAME "riot_tee_random")

set(CC3XX_SOURCES
    ${CC3XXPATH}/common/cc3xx_stdlib.c
    ${CC3XXPATH}/low_level_driver/src/cc3xx_rng.c
    ${CC3XXPATH}/low_level_driver/src/cc3xx_hmac.c
    ${CC3XXPATH}/low_level_driver/src/cc3xx_drbg.c
    ${CC3XXPATH}/low_level_driver/src/cc3xx_drbg_hmac.c
)

set(CC3XX_INCLUDES
    ${TARGET_DIR}
    ${TFM_PATH}/platform/include
    ${CC3XXPATH}/low_level_driver/include
    ${CC3XXPATH}/low_level_driver/src
    ${CC3XXPATH}/common
)

add_library(${LIBRARY_NAME}
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tee_random.c
    ${CC3XX_SOURCES}
)

target_include_directories(${LIBRARY_NAME}
    PRIVATE
    ${CC3XX_INCLUDES}
    ${PROJECT_SOURCE_DIR}/api
    ${PROJECT_SOURCE_DIR}/src/entry
    ${NRFXPATH}/mdk
    ${CMSISPATH}/Core/Include
    ${CRYPTOSERVICE_PATH}/api
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/include
)

target_compile_options(${LIBRARY_NAME}
    PUBLIC
    ${FLAGS}
)
