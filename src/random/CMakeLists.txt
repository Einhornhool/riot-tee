set(LIBRARY_NAME "riot_tee_random")

add_library(${LIBRARY_NAME}
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tee_random.c
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/random/shaxprng.c
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/hashes/sha2xx_common.c
    ${CRYPTOSERVICE_PATH}/libs/RIOT/sys/hashes/sha256.c
)

target_include_directories(${LIBRARY_NAME}
    PRIVATE
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
