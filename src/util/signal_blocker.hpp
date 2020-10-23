#pragma once

#include <QObject>

namespace tactile {

class signal_blocker final
{
 public:
  explicit signal_blocker(QObject* object) noexcept : m_object{object}
  {
    Q_ASSERT(m_object);
    m_object->blockSignals(true);
  }

  ~signal_blocker() noexcept
  {
    m_object->blockSignals(false);
  }

 private:
  QObject* m_object{};
};

}  // namespace tactile
