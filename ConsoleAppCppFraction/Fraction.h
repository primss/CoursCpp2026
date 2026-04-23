#pragma once

#include <string>

class Fraction
{
	public:
		Fraction();                // Déclaration du constructeur par défaut
		Fraction(long n, long d);    // Déclaration du constructeur avec paramètres

		void setNum(long n);
		long getNum() const;
		void setDen(long d);
		long getDen() const;

		void creer(int position);
		void afficher(int position);
		std::string const toString();

		// Surcharge de l'operateur '=='
		bool operator==(const Fraction& autre) const;
		// Surcharge de l'operateur '<'
		bool operator<(const Fraction& autre) const;
		// Surcharge de l'operateur '<='
		bool operator<=(const Fraction& autre) const;

	private:
		long Num;
		long Den;
};

