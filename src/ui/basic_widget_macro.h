#pragma once

// A macro that can be used for declaring a simple UI-based Qt widget
#define TACTILE_BASIC_WIDGET_DECL(Name, Base, Ui) \
  class Name : public Base { /* NOLINT */         \
   public:                                        \
    explicit Name(QWidget* parent = nullptr);     \
                                                  \
    ~Name() noexcept override;                    \
                                                  \
   private:                                       \
    Ui* m_ui; /* NOLINT */                        \
  };

// A macro that can be used for defining a simple UI-based Qt widget
#define TACTILE_BASIC_WIDGET_DEF(Name, Base, Ui)                          \
  Name::Name(QWidget* parent) : Base{parent}, m_ui{new Ui{}} /* NOLINT */ \
  {                                                                       \
    m_ui->setupUi(this);                                                  \
  }                                                                       \
                                                                          \
  Name::~Name() noexcept { delete m_ui; }
