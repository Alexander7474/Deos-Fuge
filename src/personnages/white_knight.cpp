#include "white_knight.h"
#include "perso_info.h"

White_knight::White_knight()
{
  buildAnimCache(WHITE_KNIGHT);
}

const perso_info &White_knight::getInfo()
{
  return WHITE_KNIGHT;
}
