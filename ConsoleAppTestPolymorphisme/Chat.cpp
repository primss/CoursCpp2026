#pragma once

#include "Chat.h"

#include <iostream>

Chat::Chat(const std::string& nom) : Animal(nom) 
{
}

Chat::~Chat() 
{
	std::cout << "Le chat " << nom << " a été adopté et quitte le refuge." << std::endl;
}

void Chat::faireDuBruit() const
{
	std::cout << nom << " dit : Miaou! Miaou!" << std::endl;
}

