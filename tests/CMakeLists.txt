find_package(Catch2 3 REQUIRED)
add_executable(tests core/DependencyGraph.tests.cpp)
target_link_libraries(tests PRIVATE qmodsorter-core Qt::Core Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(tests)
