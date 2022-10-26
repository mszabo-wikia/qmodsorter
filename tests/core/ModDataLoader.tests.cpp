#include <ModDataLoader.hpp>
#include <ModManifestList.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("ModDataLoader can load mod manifests", "[ModDataLoader]") {
  ModDataLoader modDataLoader;
  ModManifestList modManifestList;
  auto testModsFolder = fs::current_path() / "fixtures" / "mods";

  modDataLoader.loadMods(modManifestList, testModsFolder);

  REQUIRE(modManifestList.getMods().size() == 3);

  auto modWithJustPackageId =
      modManifestList.getModByPackageId("just.a.packageid.is.fine");
  REQUIRE(modWithJustPackageId.getName().toStdString() == "ModWithoutName");
  REQUIRE(modWithJustPackageId.getFolder() ==
          testModsFolder / "ModWithoutName");

  auto properMod = modManifestList.getModByPackageId("now.thats.a.proper.mod");
  REQUIRE(properMod.getName().toStdString() == "A Proper Mod");
  REQUIRE(properMod.getDescription().toStdString() == "A proper description");
  REQUIRE(properMod.getFolder() == testModsFolder / "ProperlyMadeMod");

  auto modWithEncodedNewlines =
      modManifestList.getModByPackageId("newlines.mod");
  REQUIRE(modWithEncodedNewlines.getDescription().toStdString() ==
          "A mod with some\nencoded newlines.");

  REQUIRE(modManifestList.getDependencies().contains(
      std::pair("a.dependency", "now.thats.a.proper.mod")));
  REQUIRE(modManifestList.getDependencies().contains(
      std::pair("now.thats.a.proper.mod", "a.dependent")));
}
