#ifndef NODE_HPP
#define NODE_HPP

#include <QString>
#include <vector>

/**
 * @brief Represents the discovery state of a node during a DF scan of the
 * graph.
 */
enum NodeDiscoveryState { Undiscovered, Discovered, Finished };

/**
 * @brief Represents a single node (mod) in a graph of mod dependency data.
 * The lifetime of nodes is tied to that of their parent graph.
 * @see DependencyGraph
 */
class Node {
 public:
  /**
   * @brief Construct a new Node object with the given package ID and index in
the graph.
   *
   * @param packageId
   * @param index index of this node in the graph
   */
  Node(const QString &packageId, const std::vector<Node *>::size_type index)
      : packageId(packageId),
        index(index),
        discoveryState(NodeDiscoveryState::Undiscovered),
        predecessor(nullptr) {}
  /**
   * @brief Get the package ID of the mod represented by this node.
   *
   * @return const QString&
   */
  const QString &getPackageId() const noexcept { return packageId; }

  /**
   * @brief Get the index of this node in its parent graph.
   *
   * @return const std::vector<Node *>::size_type
   */
  const std::vector<Node *>::size_type getIndex() const noexcept {
    return index;
  }

  /**
   * @brief Get the list of nodes this node has outgoing edges to (i.e. its
   * dependents).
   *
   * @return const std::vector<Node *>&
   */
  const std::vector<Node *> &getEdges() const noexcept { return edges; }
  /**
   * @brief Add an outgoing edge to this node pointing to the given node (i.e.
   * specifying that the other node depends on this node).
   *
   * @param to
   */
  void addEdge(Node *to) { edges.push_back(to); }

  /**
   * @brief Get the current discovery state of this node for a DF search.
   *
   * @return const NodeDiscoveryState&
   */
  const NodeDiscoveryState &getDiscoveryState() const noexcept {
    return discoveryState;
  }

  /**
   * @brief Update the discovery state of this node during a DF search.
   */
  void updateDiscoveryState(NodeDiscoveryState newState) noexcept {
    discoveryState = newState;
  }

  /**
   * Get the predecessor of this node, i.e. the node that was discovered before
   * this node during a depth-first scan of its graph.
   *
   * @return const Node*
   */
  Node *getPredecessor() const noexcept { return predecessor; }

  /**
   * Set the predecessor of this node, i.e. the node that was discovered before
   * this node during a depth-first scan of its graph.
   */
  void setPredecessor(Node *predecessor) noexcept {
    this->predecessor = predecessor;
  }

 private:
  const QString &packageId;
  const std::vector<Node *>::size_type index;
  std::vector<Node *> edges;

  NodeDiscoveryState discoveryState;
  Node *predecessor;
};

#endif
