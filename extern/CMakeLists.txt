include(FetchContent)

FetchContent_Declare(
    cmsis_5
    GIT_REPOSITORY  https://github.com/ARM-software/CMSIS_5.git
    GIT_TAG         5.9.0
)

FetchContent_Declare(
    nrfx
    GIT_REPOSITORY  https://github.com/NordicSemiconductor/nrfx.git
    GIT_TAG         v3.5.0
)

FetchContent_Declare(
    sdk-nrfxlib
    GIT_REPOSITORY  https://github.com/nrfconnect/sdk-nrfxlib.git
    GIT_TAG         v2.7.0
)

FetchContent_MakeAvailable(cmsis_5)
FetchContent_MakeAvailable(nrfx)
FetchContent_MakeAvailable(sdk-nrfxlib)
