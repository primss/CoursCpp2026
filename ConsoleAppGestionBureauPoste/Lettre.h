#pragma once

#include "Courrier.h"

class Lettre : public Courrier
{
public:
	Lettre();
	Lettre(double poids, const std::string_view& modeExpedition, const std::string& adresseDestination, const std::string_view& format);
	~Lettre() override;
	// Renvoie le prix d'envoi
	double calculerAffranchissement() const override;
	// Afficher les détails du courrier
	std::string toString() const override;

	// Préférer constexpr + std::string_view à const std::string pour éviter 
	// les allocations de mémoire inutiles et améliorer les performances.
	static inline constexpr std::string_view FORMAT_A3 = "A3";
	static inline constexpr std::string_view FORMAT_A4 = "A4";

private:
	std::string format; // Format de la lettre (A3 ou A4)
};

