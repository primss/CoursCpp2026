#pragma once

#include <string>

class ContinentData
{
public:
	ContinentData();
	ContinentData(const std::string& code, const std::string& libelle);
	virtual ~ContinentData();

	std::string getCode() const;
	void setCode(const std::string& newCode);
	std::string getLibelle() const;
	void setLibelle(const std::string& newLibelle);

protected:

private:
	std::string code;
	std::string libelle;
};