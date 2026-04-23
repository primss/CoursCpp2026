#pragma once

#include "GenreEnum.h"

#include <string>
#include <chrono>

class Etudiant
{
	public:
		Etudiant();
		virtual ~Etudiant();

		void renseigner(int position);
		void afficher(int position);

	protected:

	private:
		long NumeroCarte;
		std::string Nom;
		std::string Prenom;
		GenreEnum Genre;
		std::chrono::year_month_day DateNaissance;

		GenreEnum saisirGenre();
		std::chrono::year_month_day saisirDateValide();
};