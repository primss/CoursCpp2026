#pragma once

#include "Chien.h"

#include <string>
#include <iostream>

Chien::Chien(const std::string& nom) : Animal(nom) {}
Chien::~Chien()
{
	std::cout << "Le chien " << nom << " a été adopté et quitte le refuge." << std::endl;
}

void Chien::faireDuBruit() const
{
	std::cout << nom << " dit : Wouf! Wouf!" << std::endl;
}
