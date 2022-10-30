#ifndef DEPENDENCY_GRAPH_HPP
#define DEPENDENCY_GRAPH_HPP

#include <QList>
#include <QString>
#include <set>
#include <vector>

#include "CircularDependencies.hpp"
#include "Node.hpp"

class DependencyGraph {
 public:
  DependencyGraph(const QStringList &activeModIds,
                  const QStringList &knownExpansionIds,
                  const std::set<std::pair<QString, QString>> &dependencies);
  ~DependencyGraph();

  /**
   * @brief Get the list of active mod package IDs sorted based on their
   * dependencies.
   *
   * @return QStringList
   */
  QStringList getSortedPackageIds();

 private:
  const QString CORE_PACKAGE_ID = "ludeon.rimworld";
  std::vector<Node *> nodes;

  CircularDependencies findCircles();
  void depthFirstTraverse(Node *root,
                          CircularDependencies &circularDependencies);
};

#endif
