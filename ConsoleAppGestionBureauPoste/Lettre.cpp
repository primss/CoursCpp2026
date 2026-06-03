/***************************************************************************************************
* PROJET        : Gestion Bureau de Poste (GBP)
* FICHIER       : Lettre.cpp
* TYPE          : Code Source C++
* ARCHITECTURE  : x64
* OUTILS        : Microsoft Visual Studio 2026
*---------------------------------------------------------------------------------------------------
* DESCRIPTION   :
* Gestion d'une lettre avec calcul d'affranchissement basé sur le poids, le format et le mode d'expédition.
*---------------------------------------------------------------------------------------------------
* HISTORIQUE DES MODIFICATIONS :
* Version   Date        Auteur          Description / Correctifs
* -------   ----------  --------------  ------------------------------------------------------------
* 0.0.1     02/06/2026  A. Abdallah     Version initiale.
***************************************************************************************************/

#include "Lettre.h"

Lettre::Lettre() : Courrier(), format(Lettre::FORMAT_A4) 
{
}

Lettre::Lettre(double poids, const std::string_view& modeExpedition, const std::string& adresseDestination, const std::string_view& format)
	: Courrier(poids, modeExpedition, adresseDestination), format(format) 
{
}

Lettre::~Lettre() 
{
}

double Lettre::calculerAffranchissement() const
/***************************************************************************************************
* PURPOSE : Calcule l'affranchissement d'une lettre en fonction de son poids, de son format et du mode d'expédition.
***************************************************************************************************/
{
	double tarifBase = 2.5; // Prix de base pour une lettre A4
	if (format == Lettre::FORMAT_A3) {
		tarifBase = 3.5; // Prix de base pour une lettre A3
	}

	// Chaque tranche de 100 g
	int tranches = static_cast<int>(poids / 100);
	if (tranches * 100 < poids) {
		tranches++; // Ajouter une tranche supplémentaire pour les poids non multiples de 100 g
	}

	double tarifTotal = tarifBase + (tranches); // Ajouter 1 € par tranche de 100 g

	if (modeExpedition == Courrier::EXPEDITION_EXPRESS) {
		tarifTotal *= 2; // Multiplier par 2 pour le mode express
	}

	return tarifTotal;
}

std::string Lettre::toString() const
/***************************************************************************************************
* PURPOSE : Retourne une représentation sous forme de chaîne de caractères d'une lettre.
***************************************************************************************************/
{
	return "Lettre: " + Courrier::toString() + ", Format: " + format;
}