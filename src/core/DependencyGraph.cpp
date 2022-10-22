#include "DependencyGraph.hpp"

#include <QDebug>
#include <queue>

DependencyGraph::DependencyGraph(const QStringList &activeModIds,
                                 const QStringList &knownExpansionIds,
                                 const ModManifestList &modManifestList)
    : modManifestList(modManifestList) {
  for (const auto &packageId : activeModIds) {
    nodes[packageId] = new Node(packageId);
  }

  for (const auto dependencyRule : modManifestList.getDependencies()) {
    auto dependencyNode = nodes.value(dependencyRule.first, nullptr);
    auto dependentNode = nodes.value(dependencyRule.second, nullptr);

    // Exclude dependency rules that refer to mods not in the active list
    if (dependencyNode == nullptr || dependentNode == nullptr) {
      continue;
    }

    dependencyNode->addEdge(dependentNode);
  }

  // Hack: Add an implicit dependency to Ludeon DLCs to every mod that does not
  // explicitly need to load before Core or one of the DLCs.
  // This effectively ensures that Core and any enabled DLCs are sorted at the
  // top of the modlist, directly after Harmony.
  // Due to the lack of explicit dependencies on Core and Ludeon DLCs in both
  // mod load order rules and RimPy DB rules, they would otherwise appear
  // further down the list.
  for (const auto &officialPackageId : knownExpansionIds) {
    auto dlcNode = nodes.value(officialPackageId, nullptr);
    if (dlcNode == nullptr) {
      continue;  // this DLC is not enabled
    }

    for (const auto node : nodes) {
      // Don't add implicit dependencies to other DLCs or Core itself to avoid
      // cycles. The explicit DLC load order specified by community rules should
      // be sufficient here.
      if (node->getPackageId() == CORE_PACKAGE_ID ||
          knownExpansionIds.contains(node->getPackageId())) {
        continue;
      }

      // Avoid adding an implicit dependency to mods that explicitly want to
      // load before Core or a Ludeon DLC (e.g. Harmony).
      bool modWantsToLoadBeforeCoreOrDlc = false;

      for (const auto dependent : node->getEdges()) {
        if (dependent->getPackageId() == officialPackageId ||
            dependent->getPackageId() == CORE_PACKAGE_ID) {
          modWantsToLoadBeforeCoreOrDlc = true;
          break;
        }
      }

      if (!modWantsToLoadBeforeCoreOrDlc) {
        dlcNode->addEdge(node);
      }
    }
  }
}

DependencyGraph::~DependencyGraph() {
  for (auto node : nodes.values()) {
    delete node;
  }
}

QStringList DependencyGraph::getSortedModNames() {
  QStringList sortedModNames;
  QMap<QString, int> dependencyCountByNode;
  std::queue<Node *> queue;

  for (const auto node : nodes.values()) {
    for (auto dependent : node->getEdges()) {
      dependencyCountByNode[dependent->getPackageId()] =
          dependencyCountByNode.value(dependent->getPackageId(), 0) + 1;
    }
  }

  for (const auto &packageId : nodes.keys()) {
    if (dependencyCountByNode.value(packageId, 0) == 0) {
      queue.push(nodes[packageId]);
    }
  }

  // Perform a topological sort on the set of active mods.
  // TODO: Provide error reporting for cycles, missing dependencies etc. here.
  for (auto node = queue.front(); !queue.empty();
       queue.pop(), node = queue.front()) {
    auto modName =
        modManifestList.getModByPackageId(node->getPackageId()).getName();

    sortedModNames.push_back(modName);
    for (const auto dependentNode : node->getEdges()) {
      auto remainingDependencyCount =
          dependencyCountByNode.value(dependentNode->getPackageId(), 0) - 1;
      dependencyCountByNode[dependentNode->getPackageId()] =
          remainingDependencyCount;
      if (remainingDependencyCount <= 0) {
        queue.push(dependentNode);
      }
    }
  }

  return sortedModNames;
}
