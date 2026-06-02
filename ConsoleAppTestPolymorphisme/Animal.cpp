#include "Animal.h"

#include <iostream>

Animal::Animal(const std::string& nom) : nom(nom) 
{
}

Animal::~Animal() 
{
}

void Animal::faireDuBruit() const
{
	std::cout << nom << " fait du bruit!" << std::endl;
}