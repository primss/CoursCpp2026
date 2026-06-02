#include "VillageData.h"

VillageData::VillageData()
{
}

VillageData::VillageData(const std::string& code, const std::string& libelle, const std::string& codePays)
	: code(code), libelle(libelle), codePays(codePays)
{
}

VillageData::~VillageData()
{
}

std::string VillageData::getCode() const
{
	return code;
}
void VillageData::setCode(const std::string& newCode)
{
	code = newCode;
}

std::string VillageData::getLibelle() const
{
	return libelle;
}
void VillageData::setLibelle(const std::string& newLibelle)
{
	libelle = newLibelle;
}

std::string VillageData::getCodePays() const
{
	return codePays;
}
void VillageData::setCodePays(const std::string& newCodePays)
{
	codePays = newCodePays;
}
	
PaysData VillageData::getPays() const
{
	return pays;
}
void VillageData::setPays(const PaysData& newPays)
{
	pays = newPays;
}