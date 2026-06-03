/***************************************************************************************************
* PROJET        : Gestion Bureau de Poste (GBP)
* FICHIER       : Utils.cpp
* TYPE          : Code Source C++
* ARCHITECTURE  : x64
* OUTILS        : Microsoft Visual Studio 2026
*---------------------------------------------------------------------------------------------------
* DESCRIPTION   :
* Fonctions utilitaires pour la gestion des courriers et colis.
*---------------------------------------------------------------------------------------------------
* HISTORIQUE DES MODIFICATIONS :
* Version   Date        Auteur          Description / Correctifs
* -------   ----------  --------------  ------------------------------------------------------------
* 0.0.1     02/06/2026  A. Abdallah     Version initiale.
***************************************************************************************************/

#include "Utils.h"
#include <sstream>
#include <iomanip>
#include <cmath>

std::string Utils::formatDouble(double value)
/***************************************************************************************************
* PURPOSE : Formate un nombre à virgule flottante en chaîne de caractères avec une précision adaptée.
***************************************************************************************************/
{
    std::stringstream ss;

    // Check if the number is essentially a whole number (e.g., 15.0)
    if (value == std::floor(value)) {
        ss << std::fixed << std::setprecision(0) << value;
    }
    else {
        // Otherwise, show exactly one decimal place (e.g., 15.4)
        ss << std::fixed << std::setprecision(1) << value;
    }

    return ss.str();
}