include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.1.1
)
FetchContent_MakeAvailable(catch2)

add_executable(tests core/DependencyGraph.tests.cpp)
target_link_libraries(tests PRIVATE qmodsorter-core Qt::Core Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(tests)
