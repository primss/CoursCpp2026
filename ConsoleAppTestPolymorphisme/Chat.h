#pragma once

#include "Animal.h"		

class Chat : public Animal
{
public:
	Chat(const std::string& nom);
	~Chat() override;

	void faireDuBruit() const override;
};

