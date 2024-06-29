#include "perso_info.h"

// perso par default
const perso_info DEFAULT_PERSO = {
  "img/personnages/knight/",
  {10,10,12,3,3,4,6,1}, // stationnary|run|dash|jump|fall|light_attack|attack|hit
  {0.09,0.05,0.02,0.1,0.1,0.08,0.12,2}, // stationnary|run|dash|jump|fall|light_attack|attack|hit
  250.f,
  650.f,
  100.f,
  Vector2f(200.f,200.f),
  {Vector2f(25.f,0.f),Vector2f(0.f,0.f)},
  {Vector2f(50.f,50.f),Vector2f(50.f,50.f)}
};
