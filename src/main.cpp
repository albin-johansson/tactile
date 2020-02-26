#include <QApplication>
#include <QMainWindow>

int main(int argc, char** argv)
{
  QApplication app{argc, argv};

  QMainWindow window;
  QApplication::setActiveWindow(&window);

  window.show();

  return QApplication::exec();
}