#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "tactile_application.h"

int main(int argc, char** argv)
{
  tactile::TactileApplication app{argc, argv};
  return QGuiApplication::exec();
}