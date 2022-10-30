add_library(qmodsorter-core
    ModManifest.hpp ModNameListModel.hpp
    Settings.hpp
    SettingsLoader.cpp SettingsLoader.hpp
    ModDataLoader.cpp ModDataLoader.hpp
    ModsConfigLoader.cpp ModsConfigLoader.hpp
    ModManifestList.cpp ModManifestList.hpp
    DependencyDatabaseLoader.cpp DependencyDatabaseLoader.hpp
)

target_link_libraries(qmodsorter-core PRIVATE Qt6::Core)
target_include_directories(qmodsorter-core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
