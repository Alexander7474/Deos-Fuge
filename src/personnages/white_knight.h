#pragma once

#include "knight.h"

class White_knight : public Knight
{
public:
    White_knight();
    
    const perso_info &getInfo() override;
};
