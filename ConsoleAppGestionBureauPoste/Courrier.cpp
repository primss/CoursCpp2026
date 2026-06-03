/***************************************************************************************************
* PROJET        : Gestion Bureau de Poste (GBP)
* FICHIER       : Courrier.cpp
* TYPE          : Code Source C++
* ARCHITECTURE  : x64
* OUTILS        : Microsoft Visual Studio 2026
*---------------------------------------------------------------------------------------------------
* DESCRIPTION   :
* Gestion d'un courrier avec calcul d'affranchissement basé sur le poids et le mode d'expédition.
*---------------------------------------------------------------------------------------------------
* HISTORIQUE DES MODIFICATIONS :
* Version   Date        Auteur          Description / Correctifs
* -------   ----------  --------------  ------------------------------------------------------------
* 0.0.1     02/06/2026  A. Abdallah     Version initiale.
***************************************************************************************************/

#include "Courrier.h"
#include "Utils.h"

Courrier::Courrier() : poids(0), modeExpedition(EXPEDITION_NORMAL), adresseDestination("")
{
}	

Courrier::Courrier(double poids, const std::string_view& modeExpedition, const std::string& adresseDestination)
	: poids(poids), modeExpedition(modeExpedition), adresseDestination(adresseDestination)
{
}

Courrier::~Courrier()
{

}

double Courrier::calculerAffranchissement() const
/***************************************************************************************************
* PURPOSE : Calcule l'affranchissement d'un courrier.
***************************************************************************************************/
{
	return 0; 
}

std::string Courrier::toString() const
/***************************************************************************************************
* PURPOSE : Retourne une représentation sous forme de chaîne de caractères d'un courrier.
***************************************************************************************************/
{
	return "Poids: " + Utils::formatDouble(poids) + "g, Mode d'expedition: " + modeExpedition + ", Adresse de destination: " + adresseDestination;
}