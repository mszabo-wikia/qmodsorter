#include <CircularDependencies.hpp>
#include <DependencyGraph.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>

using namespace qmodsorter;

TEST_CASE("DependencyGraph can sort a basic modlist", "[DependencyGraph]") {
  QStringList mods;
  std::set<std::pair<QString, QString>> dependencies;

  for (auto i = 1; i <= 3; i++) {
    QString index;
    index.setNum(i);

    mods.push_back("test.mod" + index);
  }

  SECTION("sorts alphabetically without rules") {
    DependencyGraph graph(mods, {}, dependencies);
    auto sortedPackageIds = graph.getSortedPackageIds();

    REQUIRE(sortedPackageIds.count() == 3);

    for (auto i = 1; i <= 3; i++) {
      REQUIRE(sortedPackageIds.at(i - 1).toStdString() ==
              "test.mod" + std::to_string(i));
    }
  }

  SECTION("respects dependency rules") {
    dependencies.insert(std::make_pair("test.mod3", "test.mod2"));

    DependencyGraph graph(mods, {}, dependencies);
    auto sortedNames = graph.getSortedPackageIds();

    REQUIRE(sortedNames.count() == 3);

    REQUIRE(sortedNames.at(0).toStdString() == "test.mod1");
    REQUIRE(sortedNames.at(1).toStdString() == "test.mod3");
    REQUIRE(sortedNames.at(2).toStdString() == "test.mod2");
  }

  SECTION("reports circular dependencies") {
    dependencies.insert(std::make_pair("test.mod3", "test.mod2"));
    dependencies.insert(std::make_pair("test.mod2", "test.mod3"));

    DependencyGraph graph(mods, {}, dependencies);

    SECTION("fails when circular dependencies are detected") {
      REQUIRE_THROWS_AS(graph.getSortedPackageIds(), CircularDependencies);
    }

    SECTION("reports circular dependency info") {
      try {
        graph.getSortedPackageIds();
      } catch (const CircularDependencies& circularDependencies) {
        const auto& circles = circularDependencies.getCircles();

        REQUIRE(circles.size() == 1);

        REQUIRE(circles[0].size() == 3);

        REQUIRE(circles[0][0].toStdString() == "test.mod2");
        REQUIRE(circles[0][1].toStdString() == "test.mod3");
        REQUIRE(circles[0][2].toStdString() == "test.mod2");
      }
    }
  }
}
