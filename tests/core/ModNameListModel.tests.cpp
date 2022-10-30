#include <ModNameListModel.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("ModNameListModel sets appropriate flags", "[ModNameListModel]") {
  ModNameListModel model;
  model.setStringList({"test", "foo"});

  SECTION("is droppable at list front/end") {
    const auto flags = model.flags(QModelIndex());

    REQUIRE((flags & Qt::ItemIsDropEnabled) == Qt::ItemIsDropEnabled);
  }

  SECTION("is not droppable within the list") {
    const auto middleIndex = model.index(1);
    const auto middleFlags = model.flags(middleIndex);

    REQUIRE((middleFlags & Qt::ItemIsEditable) == 0);
    REQUIRE((middleFlags & Qt::ItemIsDropEnabled) == 0);
  }
}
