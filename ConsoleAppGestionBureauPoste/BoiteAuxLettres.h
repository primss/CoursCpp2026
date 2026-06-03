#pragma once

#include "Courrier.h"

#include <vector>

class BoiteAuxLettres
{
public:
	BoiteAuxLettres();
	virtual ~BoiteAuxLettres();
	// Ajouter un courrier à la boîte aux lettres
	virtual void ajouterCourrier(Courrier* courrier);
	// Afficher les détails de tous les courriers présents
	virtual void afficherTout() const;
	// Calculer et afficher la somme des tarifs de tous les courriers de la boîte.
	virtual void calculerAffranchissementTotal() const;

private:
	std::vector<Courrier*> courriers;
};