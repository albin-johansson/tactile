#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "app.hpp"

int main(int argc, char** argv)
{
  tactile::app app{argc, argv};
  return QGuiApplication::exec();
}