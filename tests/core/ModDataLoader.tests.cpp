#include <ModDataLoader.hpp>
#include <ModManifestList.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("ModDataLoader can load mod manifests", "[ModDataLoader]") {
  ModDataLoader modDataLoader;
  ModManifestList modManifestList;
  auto testModsFolder = fs::current_path() / "fixtures" / "mods";

  modDataLoader.loadMods(modManifestList, testModsFolder);

  REQUIRE(modManifestList.getMods().size() == 2);
  REQUIRE(modManifestList.getModByPackageId("just.a.packageid.is.fine")
              .getName()
              .toStdString() == "ModWithoutName");

  REQUIRE(modManifestList.getModByPackageId("now.thats.a.proper.mod")
              .getName()
              .toStdString() == "A Proper Mod");

  REQUIRE(modManifestList.getDependencies().contains(
      std::pair("a.dependency", "now.thats.a.proper.mod")));
  REQUIRE(modManifestList.getDependencies().contains(
      std::pair("now.thats.a.proper.mod", "a.dependent")));
}
