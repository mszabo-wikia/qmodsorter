include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.1.1
)
FetchContent_MakeAvailable(catch2)

add_executable(tests
  graph/DependencyGraph.tests.cpp
  core/SettingsLoader.tests.cpp
  core/ModDataLoader.tests.cpp
  core/ModNameListModel.tests.cpp
)
target_compile_options(tests PRIVATE ${QMODSORTER_CXXFLAGS})
target_link_libraries(tests PRIVATE qmodsorter-core qmodsorter-graph Qt::Core Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(tests)

# Copy over the fixture directory to the build directory to make fixtures easily usable by tests.
add_custom_command(
  TARGET tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/tests/fixtures
    ${CMAKE_CURRENT_BINARY_DIR}/fixtures
)
