// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

namespace tactile {

class StampTool;
class EraserTool;
class BucketTool;
class ObjectSelectionTool;
class RectangleTool;
class PointTool;
class EllipseTool;

class ToolVisitor {
 public:
  virtual ~ToolVisitor() noexcept = default;

  virtual void visit([[maybe_unused]] const StampTool& tool) {}

  virtual void visit([[maybe_unused]] const EraserTool& tool) {}

  virtual void visit([[maybe_unused]] const BucketTool& tool) {}

  virtual void visit([[maybe_unused]] const ObjectSelectionTool& tool) {}

  virtual void visit([[maybe_unused]] const RectangleTool& tool) {}

  virtual void visit([[maybe_unused]] const PointTool& tool) {}

  virtual void visit([[maybe_unused]] const EllipseTool& tool) {}
};

}  // namespace tactile
