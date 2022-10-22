add_executable(qmodsorter
    PathSelectorPanel.cpp PathSelectorPanel.hpp
    MainWindow.cpp MainWindow.hpp
    main.cpp
)

target_link_libraries(qmodsorter PRIVATE qmodsorter-core Qt6::Widgets)

set_target_properties(qmodsorter PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
