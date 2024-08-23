set(LIBRARY_NAME "riot_tee_random")
set(OBERONPATH   "${PROJECT_SOURCE_DIR}/3rd-party/sdk-nrfxlib/crypto/nrf_oberon")

add_library(${LIBRARY_NAME}
    STATIC
    random.c
)

target_include_directories(${LIBRARY_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/ns_api/include
    ${PROJECT_SOURCE_DIR}/ns_api/include/key_storage
    ${NRFXPATH}/mdk
    ${CMSISPATH}/Core/Include
    ${OBERONPATH}/include
)

target_compile_options(${LIBRARY_NAME}
    PUBLIC
    ${FLAGS}
)
