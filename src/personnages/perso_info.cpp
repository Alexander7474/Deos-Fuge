#include "perso_info.h"

// perso par default
const perso_info KNIGHT = {
  "assets/personnages/knight/",
  // nombre de frames par anim
  {10,  // stationnary
    10, // run
    2, // dash 
    3,  // jump 
    3,  // fall 
    4,  // light_attack
    6,  // attack
    1   // hit
  },
  
  // timing des frames de chaque anim
  {0.09,  // stationnary
    0.05, // run 
    0.02,  // dash 
    0.1,  // jump 
    0.1,  // fall 
    0.04, // light_attack 
    0.05, // attack 
    0.2   // hit
  },

  // timing total de chaque anim
  {0.0,  // stationnary
    0.0, // run 
    0.1,  // dash 
    0.5,  // jump 
    0.0,  // fall 
    0.16, // light_attack 
    0.3, // attack 
    0.2   // hit
  }

};

// perso par default
const perso_info WHITE_KNIGHT = {
  "assets/personnages/white_knight/",
  // nombre de frames par anim
  {10,  // stationnary
    10, // run
    2, // dash 
    3,  // jump 
    3,  // fall 
    4,  // light_attack
    6,  // attack
    1   // hit
  },
  
  // timing des frames de chaque anim
  {0.09,  // stationnary
    0.05, // run 
    0.02,  // dash 
    0.1,  // jump 
    0.1,  // fall 
    0.04, // light_attack 
    0.05, // attack 
    0.2   // hit
  },

  // timing total de chaque anim
  {0.0,  // stationnary
    0.0, // run 
    0.1,  // dash 
    0.5,  // jump 
    0.0,  // fall 
    0.16, // light_attack 
    0.3, // attack 
    0.2   // hit
  }

};
