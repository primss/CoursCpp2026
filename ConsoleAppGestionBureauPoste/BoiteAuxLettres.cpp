/***************************************************************************************************
* PROJET        : Gestion Bureau de Poste (GBP)
* FICHIER       : BoiteAuxLettres.cpp
* TYPE          : Code Source C++
* ARCHITECTURE  : x64
* OUTILS        : Microsoft Visual Studio 2026
*---------------------------------------------------------------------------------------------------
* DESCRIPTION   :
* Gestion d'une boîte aux lettres avec des lettres et des colis.
*---------------------------------------------------------------------------------------------------
* HISTORIQUE DES MODIFICATIONS :
* Version   Date        Auteur          Description / Correctifs
* -------   ----------  --------------  ------------------------------------------------------------
* 0.0.1     02/06/2026  A. Abdallah     Version initiale.
***************************************************************************************************/

#include "BoiteAuxLettres.h"

#include <iostream>

BoiteAuxLettres::BoiteAuxLettres() : courriers()
{
}

BoiteAuxLettres::~BoiteAuxLettres()
{
}

void BoiteAuxLettres::ajouterCourrier(Courrier* courrier)
/***************************************************************************************************
* PURPOSE : Ajoute un courrier à la boîte aux lettres.
***************************************************************************************************/
{
	courriers.push_back(courrier);
}

void BoiteAuxLettres::afficherTout() const
/***************************************************************************************************
* PURPOSE : Affiche le contenu de la boîte aux lettres.
***************************************************************************************************/
{
	for (const auto& courrier : courriers) {
		std::cout << courrier->toString() << std::endl;
	}
}

void BoiteAuxLettres::calculerAffranchissementTotal() const
/***************************************************************************************************
* PURPOSE : Calcule et affiche la somme des tarifs de tous les courriers de la boîte.
***************************************************************************************************/
{
	double total = 0;
	for (const auto& courrier : courriers) {
		total += courrier->calculerAffranchissement();
	}
	std::cout << "Affranchissement total: " << total << " €" << std::endl;
}