#pragma once

#include <QWidget>  // QWidget

#include "fwd.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "property.hpp"
#include "smart_pointers.hpp"
#include "tactile_declare_ui.hpp"

TACTILE_DECLARE_UI(PropertiesWidget)

namespace tactile {

class PropertiesWidget final : public QWidget
{
  Q_OBJECT

 public:
  explicit PropertiesWidget(QWidget* parent = nullptr);

  ~PropertiesWidget() noexcept override;

 public slots:
  void OnAddedProperty(const QString& name);

  void OnAboutToRemoveProperty(const QString& name);

  void OnUpdatedProperty(const QString& name);

  void OnChangedPropertyType(const QString& name);

  void OnRenamedProperty(const QString& oldName, const QString& newName);

  void OnUpdatedPropertyContext(NotNull<core::IPropertyManager*> context,
                                const QString& name);

 private:
  Unique<Ui::PropertiesWidget> mUi;
  PropertyTreeView* mView{};
  PropertyContextMenu* mContextMenu{};
  Unique<vm::PropertyModel> mModel;
  Maybe<QString> mNameCopy;
  Maybe<core::Property> mPropertyCopy;

  void ChangeModel(NotNull<core::IPropertyManager*> manager);

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
