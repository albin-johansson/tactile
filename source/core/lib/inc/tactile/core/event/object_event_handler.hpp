#pragma once

namespace tactile::core {

class Model;
class EventDispatcher;

struct CreateObjectEvent;
struct RemoveObjectEvent;
struct MoveObjectEvent;
struct SetObjectTagEvent;
struct SetObjectVisibleEvent;

class ObjectEventHandler final
{
 public:
  explicit ObjectEventHandler(Model* model);

  void install(EventDispatcher& dispatcher);

  void on_create_object(const CreateObjectEvent& event) const;

  void on_remove_object(const RemoveObjectEvent& event) const;

  void on_move_object(const MoveObjectEvent& event) const;

  void on_set_object_tag(const SetObjectTagEvent& event) const;

  void on_set_object_visible(const SetObjectVisibleEvent& event) const;

 private:
  Model* m_model;
};

}  // namespace tactile::core
