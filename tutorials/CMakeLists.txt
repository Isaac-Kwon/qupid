# Tutorials, built through the same core / ROOT split as test/.
# Executables land in the build root and write their output to the CWD.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ROOT-free tutorials: always built, link the core library only.
set(CORE_TUTORIALS
  tutorial1_gaussian_signal
  tutorial2_cluster_size
  tutorial3_montecarlo_csv
)

foreach(t ${CORE_TUTORIALS})
  add_executable(${t} ${t}.cxx)
  target_link_libraries(${t} qupid)
endforeach()
message(STATUS "Target Executable TUTORIAL (core): ${CORE_TUTORIALS}")

# ROOT-dependent tutorials: built only when the ROOT visualization stack is available.
if(QUPID_ROOT_ENABLED)
  set(ROOT_TUTORIALS
    tutorial4_visualization
  )
  foreach(t ${ROOT_TUTORIALS})
    add_executable(${t} ${t}.cxx)
    # tutorial4 uses QPGraphFactory (qupidVis, which brings vis/include) and the
    # header-only QPRandomTRandom adapter with TRandom3; TCanvas needs Gpad and
    # TRandom3 needs MathCore.
    target_link_libraries(${t} qupid qupidVis ROOT::Core ROOT::Gpad ROOT::Hist ROOT::MathCore)
  endforeach()
  message(STATUS "Target Executable TUTORIAL (ROOT): ${ROOT_TUTORIALS}")
endif()
