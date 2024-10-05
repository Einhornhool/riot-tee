set(LIBRARY_NAME "riot_tee_crypto")
set(CMSISDEVPATH "${PROJECT_SOURCE_DIR}/3rd-party/CMSIS_5/Device/ARM/ARMCM33/Include")

set(CC3XX_SOURCES
    ${CC3XXPATH}/low_level_driver/src/cc3xx_aes.c
    ${CC3XXPATH}/low_level_driver/src/cc3xx_dma.c
    ${CC3XXPATH}/low_level_driver/src/cc3xx_engine_state.c
    ${CC3XXPATH}/low_level_driver/src/cc3xx_hash.c
    ${CC3XXPATH}/low_level_driver/src/cc3xx_init.c
    ${CC3XXPATH}/low_level_driver/src/cc3xx_lcs.c
)

add_library(${LIBRARY_NAME}
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tee_cipher.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tee_ecc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tee_hashes.c
    ${CC3XX_SOURCES}
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
    ${OBERONPATH}/include
    ${CMSISPATH}/Core/Include
    ${CMSISDEVPATH}
    ${NRFXPATH}/mdk
    ${CMAKE_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/api
    ${PROJECT_SOURCE_DIR}/src/entry
    ${PROJECT_SOURCE_DIR}/src/random
    ${PROJECT_SOURCE_DIR}/src/rot
    ${CRYPTOSERVICE_PATH}/api
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/include
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
)

target_link_options(${LIBRARY_NAME}
    PUBLIC
    ${FLAGS}
)

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
    ${OBERONPATH}/lib/cortex-m33/hard-float/liboberon_3.0.15.a
)
