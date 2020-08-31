#include "Chipbit.h"

int main() {
  auto& app = Chipbit::Chipbit::Get();
  app.Initialize();
  app.Run();
  return 0;
}
