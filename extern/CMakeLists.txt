include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

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

FetchContent_GetProperties(cmsis_5)
if(NOT cmsis_5_POPULATED)
    FetchContent_Populate(cmsis_5)
endif()

FetchContent_GetProperties(nrfx)
if(NOT nrfx_POPULATED)
    FetchContent_Populate(nrfx)
endif()

FetchContent_GetProperties(sdk-nrfxlib)
if(NOT sdk-nrfxlib_POPULATED)
    FetchContent_Populate(sdk-nrfxlib)
endif()
