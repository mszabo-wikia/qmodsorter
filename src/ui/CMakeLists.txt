add_executable(qmodsorter
    PathSelectorPanel.cpp PathSelectorPanel.hpp
    MainWindow.cpp MainWindow.hpp
    ModDetailsPanel.cpp ModDetailsPanel.hpp
    CircularDependenciesDialog.cpp CircularDependenciesDialog.hpp
    main.cpp
)

target_compile_options(qmodsorter PRIVATE ${QMODSORTER_CXXFLAGS})
target_link_libraries(qmodsorter PRIVATE qmodsorter-core qmodsorter-graph Qt6::Widgets)

set_target_properties(qmodsorter PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
