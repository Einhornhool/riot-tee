set(LIBRARY_NAME "riot_tee_api")

add_library(${LIBRARY_NAME}
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CYS_ecc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CYS_hashes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CYS_random.c
)

target_include_directories(${LIBRARY_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CRYPTOSERVICE_PATH}/api
)

target_compile_options(${LIBRARY_NAME}
    PUBLIC
    ${FLAGS}
)
