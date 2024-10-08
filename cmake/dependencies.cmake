include(FetchContent)
include(ExternalProject)
set(FETCHCONTENT_QUIET OFF)

# ExternalProject_Add(
#     CryptoService
#     GIT_REPOSITORY      git@github.com:LP-HAW/CryptoService.git
#     GIT_TAG             feature/nrf9160dk-target-integration
#     PREFIX              ${CMAKE_BINARY_DIR}/_deps/crypto-service
#     INSTALL_COMMAND     ""
#     CMAKE_ARGS
#         -DTOOLCHAINPATH=${TOOLCHAINPATH}
#         -DCMAKE_TOOLCHAIN_FILE=cmake/armGNU.cmake
# )

# FetchContent_Declare(
#     cmsis_5
#     GIT_REPOSITORY  https://github.com/ARM-software/CMSIS_5.git
#     GIT_TAG         5.9.0
# )

# FetchContent_Declare(
#     nrfx
#     GIT_REPOSITORY  https://github.com/NordicSemiconductor/nrfx.git
#     GIT_TAG         v3.5.0
# )

# FetchContent_MakeAvailable(cmsis_5)
# FetchContent_MakeAvailable(nrfx)

