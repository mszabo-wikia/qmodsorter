#ifndef DEPENDENCY_GRAPH_HPP
#define DEPENDENCY_GRAPH_HPP

#include <QList>
#include <QMap>
#include <QString>
#include <vector>

#include "ModManifestList.hpp"

/**
 * @brief Represents a single node (mod) in a graph of mod dependency data.
 * The lifetime of nodes is tied to that of their parent graph.
 * @see DependencyGraph
 */
class Node {
 public:
  /**
   * @brief Construct a new Node object with the given package ID.
   *
   * @param packageId
   */
  Node(const QString &packageId) : packageId(packageId) {}
  /**
   * @brief Get the package ID of the mod represented by this node.
   *
   * @return const QString&
   */
  const QString &getPackageId() const { return packageId; }
  /**
   * @brief Get the list of nodes this node has outgoing edges to (i.e. its
   * dependents).
   *
   * @return const std::vector<Node *>&
   */
  const std::vector<Node *> &getEdges() const { return edges; }
  /**
   * @brief Add an outgoing edge to this node pointing to the given node (i.e.
   * specifying that the other node depends on this node).
   *
   * @param to
   */
  void addEdge(Node *to) { edges.push_back(to); }

 private:
  const QString &packageId;
  std::vector<Node *> edges;
};

class DependencyGraph {
 public:
  DependencyGraph(const QStringList &activeModIds,
                  const QStringList &knownExpansionIds,
                  const ModManifestList &modManifestList);
  ~DependencyGraph();

  /**
   * @brief Get the list of active mod names sorted based on their dependencies.
   *
   * @return QStringList
   */
  QStringList getSortedModNames();

 private:
  const QString CORE_PACKAGE_ID = "ludeon.rimworld";
  const ModManifestList &modManifestList;
  QMap<QString, Node *> nodes;
};

#endif
