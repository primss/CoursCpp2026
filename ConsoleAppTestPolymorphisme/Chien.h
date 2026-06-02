#pragma once

#include "Animal.h"

class Chien : public Animal
{
public:
	Chien(const std::string& nom);
	~Chien() override;

	void faireDuBruit() const override;
};