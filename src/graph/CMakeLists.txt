add_library(qmodsorter-graph
    DependencyGraph.cpp DependencyGraph.hpp
    Node.hpp
    CircularDependencies.hpp
)

target_compile_options(qmodsorter-graph PRIVATE ${QMODSORTER_CXXFLAGS})
target_link_libraries(qmodsorter-graph PRIVATE Qt6::Core)
target_include_directories(qmodsorter-graph INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
