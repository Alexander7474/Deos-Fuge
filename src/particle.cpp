#include "particle.h"
#include <BBOP/Graphics/bbopMathClass.h>
#include <BBOP/Graphics/textureClass.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

Particle::Particle()
  : Particle("assets/particles/Smoke-Sheet.png", Vector2i(4,5), 0.1f, 1)
{}

Particle::Particle(std::string _particle_sheet, Vector2i _size, float _frame_t)
  : Particle(_particle_sheet, _size, _frame_t, 0)
{}

Particle::Particle(std::string _particle_sheet, Vector2i _size, float _frame_t, int _dead_frame)
  : Sprite(Texture("assets/default.png"))
{
  particle_texture = bbopLoadSpriteSheet(_particle_sheet.c_str(), _size.y, _size.x);
  frame_t = _frame_t;
  n_frame = _size.x * _size.y - _dead_frame;
  frame_cpt = rand() % n_frame;
  total_t = n_frame * frame_t;
  anim_start = glfwGetTime();
  last_frame_t = glfwGetTime();
}

bool Particle::update()
{
  float actual_time = glfwGetTime();

  if(actual_time - last_frame_t >= frame_t){
    frame_cpt++;
    last_frame_t = glfwGetTime();
    if(frame_cpt >= n_frame){
      frame_cpt = 0;
    }
    setTexture(particle_texture[frame_cpt]);
  }

  if(actual_time - anim_start >= total_t)
    return true;

  return false;
}
