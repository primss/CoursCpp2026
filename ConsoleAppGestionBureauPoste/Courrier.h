#pragma once

#include <string>
#include <string_view>

class Courrier
{
public:
	Courrier();
	Courrier(double poids, const std::string_view& modeExpedition, const std::string& adresseDestination);
	virtual	~Courrier();

	// Renvoie le prix d'envoi
	virtual double calculerAffranchissement() const;
	// Afficher les détails du courrier
	virtual std::string toString() const;

	// Préférer constexpr + std::string_view à const std::string pour éviter 
	// les allocations de mémoire inutiles et améliorer les performances.
	static inline constexpr std::string_view EXPEDITION_NORMAL = "normal";
	static inline constexpr std::string_view EXPEDITION_EXPRESS = "express";

protected:
	double poids;
	std::string modeExpedition;
	std::string adresseDestination;
};