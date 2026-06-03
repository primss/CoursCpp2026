/***************************************************************************************************
* PROJET        : Gestion Bureau de Poste (GBP)
* FICHIER       : Colis.cpp
* TYPE          : Code Source C++
* ARCHITECTURE  : x64
* OUTILS        : Microsoft Visual Studio 2026
*---------------------------------------------------------------------------------------------------
* DESCRIPTION   :
* Gestion d'un colis avec calcul d'affranchissement basé sur le poids, le volume et le mode d'expédition.
*---------------------------------------------------------------------------------------------------
* HISTORIQUE DES MODIFICATIONS :
* Version   Date        Auteur          Description / Correctifs
* -------   ----------  --------------  ------------------------------------------------------------
* 0.0.1     02/06/2026  A. Abdallah     Version initiale.
***************************************************************************************************/

#include "Colis.h"
#include "Utils.h"

Colis::Colis() : Courrier(), volume(0) 
{
}

Colis::Colis(double poids, const std::string_view& modeExpedition, const std::string& adresseDestination, double volume)
	: Courrier(poids, modeExpedition, adresseDestination), volume(volume)
{
}

Colis::~Colis() 
{
}

double Colis::calculerAffranchissement() const
/***************************************************************************************************
* PURPOSE : Calcule l'affranchissement d'un colis en fonction de son poids, de son volume et du mode d'expédition.
***************************************************************************************************/
{
	double tarifBase = 0.25; // Prix de base pour un colis
	double tarifTotal = tarifBase * volume + 1.5 * (poids / 100);

	if (modeExpedition == Courrier::EXPEDITION_EXPRESS) {
		tarifTotal *= 2; // Multiplier par 2 pour le mode express
	}

	return tarifTotal;
}

std::string Colis::toString() const
/***************************************************************************************************
* PURPOSE : Retourne une représentation sous forme de chaîne de caractères d'un colis.
***************************************************************************************************/
{
	return "Colis: " + Courrier::toString() + ", Volume: " + Utils::formatDouble(volume) + " litres";
}