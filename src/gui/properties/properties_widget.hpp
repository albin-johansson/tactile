#pragma once

#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "property.hpp"
#include "smart_pointers.hpp"

TACTILE_DECLARE_UI(PropertiesWidget)

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)
TACTILE_FORWARD_DECLARE(tactile::core, IPropertyManager)
TACTILE_FORWARD_DECLARE(tactile::vm, PropertyModel)

namespace tactile {

class PropertyTreeView;
class PropertyContextMenu;

class PropertiesWidget final : public QWidget
{
  Q_OBJECT

 public:
  explicit PropertiesWidget(QWidget* parent = nullptr);

  ~PropertiesWidget() noexcept override;

 public slots:
  void ShowMap(not_null<core::IPropertyManager*> manager);

  void ShowLayer(not_null<core::IPropertyManager*> manager);

  void ShowTileset(not_null<core::IPropertyManager*> manager);

  void OnAddedProperty(const QString& name);

  void OnAboutToRemoveProperty(const QString& name);

  void OnUpdatedProperty(const QString& name);

  void OnChangedPropertyType(const QString& name);

  void OnRenamedProperty(const QString& oldName, const QString& newName);

 private:
  Unique<Ui::PropertiesWidget> mUi;
  PropertyTreeView* mView{};
  PropertyContextMenu* mContextMenu{};
  Unique<vm::PropertyModel> mModel;
  Maybe<QString> mNameCopy;
  Maybe<core::Property> mPropertyCopy;

  void ChangeModel(not_null<core::IPropertyManager*> manager);

  [[nodiscard]] auto PropertyName(const QModelIndex& index) const -> QString;

  [[nodiscard]] auto CurrentPropertyName() const -> QString;

 private slots:
  void OnSelectionChanged(Maybe<QModelIndex> index);

  void OnCopyPropertyRequested();

  void OnPastePropertyRequested();

  void OnNewPropertyRequested();

  void OnRemovePropertyRequested();

  void OnRenamePropertyRequested();

  void OnChangeTypeRequested(core::PropertyType type);

  void OnDoubleClicked();

  void OnSpawnContextMenu(const QPoint& pos);
};

}  // namespace tactile
