#pragma once

#include <string>

#include "ContinentData.h"

class PaysData
{
public:
	PaysData();
	PaysData(const std::string& code, const std::string& libelle);
	PaysData(const std::string& code, const std::string& libelle, const std::string& codeContinent);
	virtual ~PaysData();

	std::string getCode() const;
	void setCode(const std::string& newCode);
	std::string getLibelle() const;
	void setLibelle(const std::string& newLibelle);
	std::string getCodeContinent() const;
	void setCodeContinent(const std::string& newCodeContinent);

	ContinentData getContinent() const;
	void setContinent(const ContinentData& newContinent);

protected:

private:
	std::string code;
	std::string libelle;
	std::string codeContinent;
	ContinentData continent;
};