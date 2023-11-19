#include "Acutator.h"
#include "IO.h"

Acutator acutator;
IO io;

void loop() {

  acutator.checkShots();  

  io.check();
}
