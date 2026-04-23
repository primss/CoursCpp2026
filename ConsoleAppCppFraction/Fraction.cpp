#include "Fraction.h"

#include <iostream>
#include <sstream>

Fraction::Fraction() : Num(0), Den(1)
{
}

Fraction::Fraction(long n, long d) : Num(n)
{
	setDen(d);
}

void Fraction::setNum(long n) {
	Num = n;
}
long Fraction::getNum() const {
	return Num;
}

void Fraction::setDen(long d) {
	if (d != 0) {
		Den = d;
	} 
	else {
		std::cout << "Erreur : Le dénominateur ne peut pas être nul !" << std::endl;
	}
	if (Den < 0) {
		Num = -Num;
		Den = -Den;
	}
}
long Fraction::Fraction::getDen() const {
	return Den;
}

/****************************************
* Purpose : Créer une nouvelle fraction
*****************************************/
void Fraction::creer(int position) 

{
	long n, d;

	std::cout << "--- Saisie de la fraction " << position << " ---" << std::endl;
	
	while (true) {
		std::cout << "Numérateur: ";
		if (std::cin >> n) {
			setNum(n);
			break; // On sort de la boucle
		} 
		else {
			std::cout << "Erreur : Le numérateur n'est pas un nombre !" << std::endl;
			std::cin.clear(); // Réinitialise l'état de cin
			std::cin.ignore(10000, '\n'); // Supprime la mauvaise saisie du tampon
		}
	}

	while (true) {
		std::cout << "Dénominateur: ";
		if (std::cin >> d) {
			setDen(d);
			if (d != 0) {				
				break; // On sort de la boucle
			}
		}
		else {
			std::cout << "Erreur : Le dénominateur n'est pas un nombre !" << std::endl;
			std::cin.clear(); // Réinitialise l'état de cin
			std::cin.ignore(10000, '\n'); // Supprime la mauvaise saisie du tampon
		}
	}
} // void Fraction::creerFraction(long position) 

void Fraction::afficher(int position)
{
	if (position <= 0) {
		std::cout << toString() << std::endl;
	}
	else {
		std::cout << position << ". " << toString() << std::endl;;
	}
}

std::string const Fraction::toString()
{
	std::stringstream ss;
	ss << Num << "/" << Den;
	return ss.str();
}

bool Fraction::operator==(const Fraction& autre) const 
{
	return (this->Num * autre.Den == this->Den * autre.Num);
}

bool Fraction::operator<(const Fraction& autre) const
{
	return (this->Num * autre.Den < autre.Num * this->Den);
}

bool Fraction::operator<=(const Fraction& autre) const
{
	return (*this < autre) || (*this == autre);
}

/*a <= b : (a < b) || (a == b)
a > b : !(a <= b)
a >= b : !(a < b)
a != b : !(a == b)*/