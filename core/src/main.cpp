#include "core/inc/simservice.h"

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  SimService m_simService;

  if (!m_simService.LoadQML()) return -1;

  return app.exec();
}
