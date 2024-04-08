#include "../include/player.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

Player::Player(int joystick_, std::string name_, perso_info personnage_info_)
  : personnage(personnage_info_),
    name(name_),
    joystick(joystick_)
{}

void Player::update(Map &map_)
{
 //gestion de la manette
  int present = glfwJoystickPresent(joystick);
  if (1 == present)
  {
    int n_axes;
    int n_buttons;
    const float *axes = glfwGetJoystickAxes(joystick,&n_axes);
    const unsigned char *buttons = glfwGetJoystickButtons(joystick, &n_buttons);
  
    //touche de la manette////////////////////////////////////////////////////////////////////////////////////////////
    //touche x de la manette
    if(buttons[2] == GLFW_PRESS){
      personnage.doDash();
    }

    //touche a de la manette
    if(buttons[0] == GLFW_PRESS){
      personnage.doJump();
    }

    if(axes[0] > 0.15){
      personnage.goRight(axes[0]);
    }
    if(axes[0] < -0.15){
      personnage.goLeft(axes[0]);
    }

    personnage.update(map_);
      
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
}

void Player::Draw(GLint renderModeLoc) const
{
  personnage.Draw(renderModeLoc);  
}
