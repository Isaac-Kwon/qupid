# for now put the executables in the build root
# for compatibility with existing scripts
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ROOT-free executables: always built, link the core library only.
set(CORE_EXE
)
foreach(exe ${CORE_EXE})
  add_executable(${exe} ${exe}.cpp)
  set_property(TARGET ${exe} PROPERTY POSITION_INDEPENDENT_CODE TRUE)
  target_link_libraries(${exe} qupid)
endforeach()
message(STATUS "Target Executable (core): ${CORE_EXE}")

# ROOT-dependent executables: built only when the ROOT visualization stack is available.
if(QUPID_ROOT_ENABLED)
  set(ROOT_EXE
  )
  foreach(exe ${ROOT_EXE})
    add_executable(${exe} ${exe}.cpp)
    set_property(TARGET ${exe} PROPERTY POSITION_INDEPENDENT_CODE TRUE)
    target_link_libraries(${exe} qupid qupidVis ROOT::Core ROOT::Gpad ROOT::Hist ROOT::Tree)
  endforeach()
  message(STATUS "Target Executable (ROOT): ${ROOT_EXE}")
endif()
