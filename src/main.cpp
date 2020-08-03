#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "tactile_application.hpp"

int main(int argc, char** argv)
{
  tactile::app app{argc, argv};
  return QGuiApplication::exec();
}