#include <DependencyGraph.hpp>
#include <ModManifestList.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("DependencyGraph can sort a basic modlist", "[DependencyGraph]") {
  ModManifestList modManifestList;
  for (auto i = 1; i <= 3; i++) {
    ModManifest manifest;
    QString index;
    index.setNum(i);
    manifest.setName("Mod" + index);
    manifest.setPackageId("test.mod" + index);
    modManifestList.addModManifest(manifest);
  }

  SECTION("sorts alphabetically without rules") {
    DependencyGraph graph(modManifestList.getPackageIds(), {}, modManifestList);
    auto sortedNames = graph.getSortedModNames();

    REQUIRE(sortedNames.count() == 3);

    for (auto i = 1; i <= 3; i++) {
      REQUIRE(sortedNames.at(i - 1).toStdString() == "Mod" + std::to_string(i));
    }
  }

  SECTION("respects dependency rules") {
    modManifestList.addModDependency("test.mod3", "test.mod2");
    DependencyGraph graph(modManifestList.getPackageIds(), {}, modManifestList);
    auto sortedNames = graph.getSortedModNames();

    REQUIRE(sortedNames.count() == 3);

    REQUIRE(sortedNames.at(0).toStdString() == "Mod1");
    REQUIRE(sortedNames.at(1).toStdString() == "Mod3");
    REQUIRE(sortedNames.at(2).toStdString() == "Mod2");
  }
}
