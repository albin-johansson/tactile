#pragma once

#include "mouse_tool.hpp"

namespace Tactile {

class BucketTool final : public AMouseTool
{
 public:
  explicit BucketTool(NotNull<Model*> model);

  [[nodiscard]] auto GetType() const -> MouseToolType override
  {
    return MouseToolType::Bucket;
  }
};

}  // namespace Tactile
