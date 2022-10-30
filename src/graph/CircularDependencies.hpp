#ifndef CIRCULAR_DEPENDENCIES_HPP
#define CIRCULAR_DEPENDENCIES_HPP

#include <QString>
#include <vector>

namespace qmodsorter {

/**
 * @brief Holds a set of circular dependencies discovered in a dependency graph
 */
class CircularDependencies : public std::exception {
  using CircularDependency = std::vector<QString>;

 public:
  const std::vector<CircularDependency>& getCircles() const noexcept {
    return circles;
  }

  void addCircularDependency(const CircularDependency&& circle) {
    circles.push_back(circle);
  }

  const char* what() const noexcept override {
    return "Circular dependencies found";
  }

 private:
  std::vector<CircularDependency> circles;
};

}  // namespace qmodsorter

#endif
