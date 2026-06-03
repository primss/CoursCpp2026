#pragma once

#include "Courrier.h"

class Colis : public Courrier
{
public:
	Colis();
	Colis(double poids, const std::string_view& modeExpedition, const std::string& adresseDestination, double volume);
	~Colis() override;
	// Renvoie le prix d'envoi
	double calculerAffranchissement() const override;
	// Afficher les détails du courrier
	std::string toString() const override;

private:
	double volume; // Volume du colis en litres
};