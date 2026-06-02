#pragma once

#include "ContinentData.h"


ContinentData::ContinentData() 
{
}

ContinentData::ContinentData(const std::string& code, const std::string& libelle)
	: code(code), libelle(libelle)
{
}

ContinentData::~ContinentData()
{
}

std::string ContinentData::getCode() const 
{ 
	return code;
}
void ContinentData::setCode(const std::string& newCode) 
{ 
	code = newCode; 
}

std::string ContinentData::getLibelle() const 
{ 
	return libelle;
}
void ContinentData::setLibelle(const std::string& newLibelle) 
{ 
	libelle = newLibelle; 
}