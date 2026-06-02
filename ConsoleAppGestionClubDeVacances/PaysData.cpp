#include "PaysData.h"

PaysData::PaysData()
{
}

PaysData::PaysData(const std::string& code, const std::string& libelle)
	: code(code), libelle(libelle), codeContinent("")
{
}

PaysData::PaysData(const std::string& code, const std::string& libelle, const std::string& codeContinent)
	: code(code), libelle(libelle), codeContinent(codeContinent)
{
}

PaysData::~PaysData()
{
}

std::string PaysData::getCode() const
{
	return code;
}
void PaysData::setCode(const std::string& newCode)
{
	code = newCode;
}

std::string PaysData::getLibelle() const
{
	return libelle;
}
void PaysData::setLibelle(const std::string& newLibelle)
{
	libelle = newLibelle;
}

std::string PaysData::getCodeContinent() const
{
	return codeContinent;
}
void PaysData::setCodeContinent(const std::string& newCodeContinent)
{
	codeContinent = newCodeContinent;
}

ContinentData PaysData::getContinent() const
{
	return continent;
}
void PaysData::setContinent(const ContinentData& newContinent)
{
	continent = newContinent;
}