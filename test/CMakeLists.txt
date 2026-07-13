# for now put the executables in the build root
# for compatibility with existing scripts
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ROOT-free tests: always built, link the core library only.
set(CORE_TESTS
  test0_VectorMatrix
  test3_digitizer
  test7_montecarlo_csv
)

foreach(t ${CORE_TESTS})
  add_executable(${t} ${t}.cxx)
  target_link_libraries(${t} qupid)
endforeach()
message(STATUS "Target Executable TEST (core): ${CORE_TESTS}")

# ROOT-dependent tests: built only when the ROOT visualization stack is available.
if(QUPID_ROOT_ENABLED)
  set(ROOT_TESTS
    test1_spanGaussian
    test2_gaussianSignal
    test4_graphfactory
    test5_puremontecarlo1
    test6_puremontecarlo2
  )
  foreach(t ${ROOT_TESTS})
    add_executable(${t} ${t}.cxx)
    target_link_libraries(${t} qupid ROOT::Core ROOT::Gpad ROOT::Hist ROOT::Tree)
  endforeach()

  # test4 uses QPGraphFactory (vis module); linking qupidVis also brings vis/include.
  target_link_libraries(test4_graphfactory qupidVis)

  # test5/test6 include the header-only TRandom adapter from vis/include and
  # use TRandom3 directly: they need the vis include path and MathCore, but
  # not qupidVis itself.
  foreach(t test5_puremontecarlo1 test6_puremontecarlo2)
    target_include_directories(${t} PRIVATE ${PROJECT_SOURCE_DIR}/vis/include)
    target_link_libraries(${t} ROOT::MathCore)
  endforeach()

  message(STATUS "Target Executable TEST (ROOT): ${ROOT_TESTS}")
endif()
