#pragma once

#include <string>

#include "PaysData.h"

class VillageData
{
public:
	VillageData();
	VillageData(const std::string& code, const std::string& libelle, const std::string& codePays);
	virtual ~VillageData();

	std::string getCode() const;
	void setCode(const std::string& newCode);
	std::string getLibelle() const;
	void setLibelle(const std::string& newLibelle);
	std::string getCodePays() const;
	void setCodePays(const std::string& newCodePays);

	PaysData getPays() const;
	void setPays(const PaysData& newPays);

protected:

private:
	std::string code;
	std::string libelle;
	std::string codePays;
	PaysData pays;
};