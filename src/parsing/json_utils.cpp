#include "json_utils.hpp"

#include <QFileInfo>
#include <QSaveFile>

#include "file_handle.hpp"
#include "preferences.hpp"

namespace tactile {

auto ReadJson(const QFileInfo& path) -> maybe<QJsonDocument>
{
  FileHandle file{path.absoluteFilePath()};
  if (!file.Open(QFile::ReadOnly | QFile::Text))
  {
    return std::nullopt;
  }

  auto json = QJsonDocument::fromJson(file.Read());

  if (json.isNull())
  {
    return std::nullopt;
  }

  return json;
}

auto WriteJson(const QFileInfo& path, const QJsonDocument& document) -> bool
{
  QSaveFile file{path.absoluteFilePath()};

  file.setDirectWriteFallback(true);
  file.open(QFile::WriteOnly | QFile::Text);

  constexpr auto def = prefs::readable_output_def;
  const auto format = prefs::UseReadableOutput().value_or(def)
                          ? QJsonDocument::Indented
                          : QJsonDocument::Compact;

  if (const auto result = file.write(document.toJson(format)); result == -1)
  {
    file.cancelWriting();
    return false;
  }
  else
  {
    file.commit();
    return true;
  }
}

}  // namespace tactile
