#include "DependencyGraph.hpp"

#include <QDebug>
#include <QHash>
#include <queue>

#include "CircularDependencies.hpp"
#include "Node.hpp"

namespace qmodsorter {

DependencyGraph::DependencyGraph(
    const QStringList &activeModIds, const QStringList &knownExpansionIds,
    const std::set<std::pair<QString, QString>> &dependencies)
    : nodes(static_cast<size_t>(activeModIds.size())) {
  size_t i = 0;
  QHash<QString, Node *> nodesById;

  for (const auto &packageId : activeModIds) {
    auto node = new Node(packageId, i);
    nodes[i] = node;
    nodesById[packageId] = node;
    i++;
  }

  for (const auto &dependencyRule : dependencies) {
    auto dependencyNode = nodesById.value(dependencyRule.first, nullptr);
    auto dependentNode = nodesById.value(dependencyRule.second, nullptr);

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
    auto dlcNode = nodesById.value(officialPackageId, nullptr);
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
      // For simplicity and performance, don't consider transitive dependencies
      // here as they're unlikely in this scenario.
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
  for (auto node : nodes) {
    delete node;
  }
}

QStringList DependencyGraph::getSortedPackageIds() {
  CircularDependencies circularDependencies = findCircles();

  // Bail early if the current mod list contains circular dependencies
  if (!circularDependencies.getCircles().empty()) {
    throw circularDependencies;
  }

  QStringList sortedModNames;
  std::vector<int> dependencyCounts(nodes.size());
  std::queue<Node *> queue;

  for (const auto &node : nodes) {
    for (const auto &dependent : node->getEdges()) {
      dependencyCounts[dependent->getIndex()] += 1;
    }
  }

  for (const auto &node : nodes) {
    if (dependencyCounts[node->getIndex()] == 0) {
      queue.push(node);
    }
  }

  // Perform a topological sort on the set of active mods.
  for (auto node = queue.front(); !queue.empty();
       queue.pop(), node = queue.front()) {
    sortedModNames.push_back(node->getPackageId());

    for (const auto dependentNode : node->getEdges()) {
      dependencyCounts[dependentNode->getIndex()] -= 1;

      if (dependencyCounts[dependentNode->getIndex()] <= 0) {
        queue.push(dependentNode);
      }
    }
  }

  return sortedModNames;
}

CircularDependencies DependencyGraph::findCircles() {
  CircularDependencies circularDependencies;

  for (const auto &node : nodes) {
    node->updateDiscoveryState(NodeDiscoveryState::Undiscovered);
    node->setPredecessor(nullptr);
  }

  for (const auto &node : nodes) {
    if (node->getDiscoveryState() == NodeDiscoveryState::Undiscovered) {
      depthFirstTraverse(node, circularDependencies);
    }
  }

  return circularDependencies;
}

void DependencyGraph::depthFirstTraverse(
    Node *root, CircularDependencies &circularDependencies) {
  root->updateDiscoveryState(NodeDiscoveryState::Discovered);

  for (const auto &node : root->getEdges()) {
    const auto discoveryState = node->getDiscoveryState();
    if (discoveryState == NodeDiscoveryState::Undiscovered) {
      // We haven't yet visited this node as part of this DF scan,
      // so set its predecessor and recurse into it accordingly.
      node->setPredecessor(root);
      depthFirstTraverse(node, circularDependencies);
    } else if (discoveryState == NodeDiscoveryState::Discovered) {
      // We've visited this node as part of this DF scan, which implies a
      // circle - assemble all nodes on the circle for reporting.
      std::vector<QString> circularDependency;
      circularDependency.push_back(node->getPackageId());

      for (auto visitedNode = root; visitedNode != nullptr;
           visitedNode = visitedNode->getPredecessor()) {
        circularDependency.push_back(visitedNode->getPackageId());

        if (visitedNode->getIndex() == node->getIndex()) {
          break;  // we've come full circle
        }
      }

      std::reverse(circularDependency.begin(), circularDependency.end());

      circularDependencies.addCircularDependency(std::move(circularDependency));
    }
  }

  root->updateDiscoveryState(NodeDiscoveryState::Finished);
}

}  // namespace qmodsorter
