#include "player.h"
#include "personnage.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

Player::Player(GLFWwindow*& window_, int joystick_, std::string name_)
  : name(name_),
    joystick(joystick_),
    window(window_)
{
  perso = new Knight();
}

void Player::update(double delta_time_, Map *map_)
{
  // si joystick < 0 alors il faut utiliser un clavier
  if(joystick >= 0){
    //gestion de la manette
    int present = glfwJoystickPresent(joystick);
    if (1 == present)
    {
      int n_axes;
      int n_buttons;
      const float *axes = glfwGetJoystickAxes(joystick,&n_axes);
      const unsigned char *buttons = glfwGetJoystickButtons(joystick, &n_buttons);
    
      if(axes[0] > 0.15){
        perso->goRight(delta_time_, axes[0]);
      }
      if(axes[0] < -0.15){
        perso->goLeft(delta_time_, axes[0]);
      }  
      //touche de la manette////////////////////////////////////////////////////////////////////////////////////////////

      //touche x de la manette
      if(buttons[2] == GLFW_PRESS){
        perso->calling_state = dash;
      }

      //touche a de la manette
      if(buttons[0] == GLFW_PRESS){
        perso->calling_state = jump;
      }

      //touche b de la manette
      if(buttons[1] == GLFW_PRESS){
        perso->calling_state = light_attack;
      }

      //touche y de la manette
      if(buttons[3] == GLFW_PRESS){
        perso->calling_state = attack;
      }

      //debuging/////////////////////////////////////////////
      if(false){
        std::cerr << "0:  " << axes[0] << std::endl;
        std::cerr << "1:  " << axes[1] << std::endl;
        std::cerr << "2:  " << axes[2] << std::endl;
        std::cerr << "3:  " << axes[3] << std::endl;
        std::cerr << "4:  " << axes[4] << std::endl;
        std::cerr << "5:  " << axes[5] << std::endl;
      }
      if(false){
        for(int i = 0; i < n_buttons; i++){
          if (buttons[i] == GLFW_PRESS){
            std::cerr << i << std::endl;
          }
        }
      }
      ////////////////////////////////////////////////////////
    }
  }else {
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
      perso->goRight(delta_time_, 1.f);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
      perso->goLeft(delta_time_, -1.f);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        perso->calling_state = jump;
    }
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        perso->calling_state = dash;
    }
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        perso->calling_state = light_attack;
    }
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        perso->calling_state = attack;
    }
  }
  //mettre a jour le personnage avec la map
  perso->updatePersonnage(delta_time_, map_);
 }

