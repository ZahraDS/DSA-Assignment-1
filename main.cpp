#include <iostream>
#include "Polynomial.h"
using namespace std;

int main() {
	Polynomial p1, p2;

	cout << "Enter number of terms for first polynomial: ";
	int n1;
	cin >> n1;
	cout << "Enter each term as (coefficient exponent):\n";
	for (int i = 0; i < n1; i++) {
		int coeff, exp;
		cin >> coeff >> exp;
		p1.insertTerm(coeff, exp);
	}

	cout << "\nEnter number of terms for second polynomial: ";
	int n2;
	cin >> n2;
	cout << "Enter each term as (coefficient exponent):\n";
	for (int i = 0; i < n2; i++) {
		int coeff, exp;
		cin >> coeff >> exp;
		p2.insertTerm(coeff, exp);
	}

	cout << "\n--------------------------------------\n";
	cout << "First Polynomial: ";
	p1.display();
	cout << "\nSecond Polynomial: ";
	p2.display();

	Polynomial sum = p1.add(p2);
	Polynomial product = p1.multiply(p2);

	cout << "\n--------------------------------------\n";
	cout << "Sum: ";
	sum.display();

	cout << "\nProduct: ";
	product.display();
	cout << endl;

	return 0;
}
