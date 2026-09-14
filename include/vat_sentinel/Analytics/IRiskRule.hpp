#pragma once
#include "vat_sentinel/Core/Company.hpp"
#include "vat_sentinel/Graph/GraphRepository.hpp"

namespace Analytics {

class IRiskRule {
  public:
    virtual ~IRiskRule() = default;

    virtual int evaluate(const Core::Company& company,
                         const Graph::GraphRepository& graph) const = 0;
};

} // namespace Analytics