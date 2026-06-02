#pragma once

#include <string>

class Animal
{
protected:
	std::string nom;

public:
	Animal();
	Animal(const std::string& nom);
	virtual ~Animal();

	virtual void faireDuBruit() const;
};