/***************************************************************************************************
* PROJET        : Gestion Bureau de Poste (GBP)
* FICHIER       : main.cpp
* TYPE          : Code Source C++
* ARCHITECTURE  : x64
* OUTILS        : Microsoft Visual Studio 2026
*---------------------------------------------------------------------------------------------------
* DESCRIPTION   :
* Point d'entrée principal de l'application GBP.
*---------------------------------------------------------------------------------------------------
* HISTORIQUE DES MODIFICATIONS :
* Version   Date        Auteur          Description / Correctifs
* -------   ----------  --------------  ------------------------------------------------------------
* 0.0.1     02/06/2026  A. Abdallah     Version initiale, création des classes.
* 0.0.2     10/06/2026  A. Abdallah     Implémentation des méthodes de calcul d'affranchissement.
***************************************************************************************************/

#include <iostream>
#include <windows.h>

#include "BoiteAuxLettres.h"
#include "Lettre.h"
#include "Colis.h"

int main()
/***************************************************************************************************
* PURPOSE : Démonstration de la gestion d'un bureau de poste avec des lettres et des colis.
***************************************************************************************************/
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

	BoiteAuxLettres boite;

    std::cout << "GESTION BUREAU POSTE" << std::endl;

	std::cout << "\nCréation d'une lettre A4 de 80g en mode normal..." << std::endl;
	Lettre lettreA4(80.0, Courrier::EXPEDITION_NORMAL, "Hountigomé", Lettre::FORMAT_A4);

	std::cout << "Création d'une lettre A3 de 250g en mode express..." << std::endl;
	Lettre lettreA3(250.0, Courrier::EXPEDITION_EXPRESS, "Hountigomé", Lettre::FORMAT_A3);

	std::cout << "Création d'un colis de 12 litres en mode normal..." << std::endl;
	Colis colis(1500, Courrier::EXPEDITION_NORMAL, "Hountigomé", 12);

	boite.ajouterCourrier(&lettreA4);
	boite.ajouterCourrier(&lettreA3);
	boite.ajouterCourrier(&colis);

	std::cout << "\nContenu de la boite aux lettres" << std::endl;
	boite.afficherTout();
	boite.calculerAffranchissementTotal();

	return 0;
}