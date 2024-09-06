# Required by dependency sdk-nrfxlib
function(add_subdirectory_ifdef feature_toggle source_dir)
  if(${${feature_toggle}})
    add_subdirectory(${source_dir} ${ARGN})
  endif()
endfunction()
