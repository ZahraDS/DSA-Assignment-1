// updated version for final submission
#include "polynomial.h"
#include <iostream>
#include <map>
#include <sstream>
#include <cmath>

// Structure to store each term
struct Term {
	int coeff;
	int exp;
	Term* next;
	Term(int c, int e) : coeff(c), exp(e), next(nullptr) {}
};

// Global map to store each Polynomial’s linked list head
static std::map<const Polynomial*, Term*> polyData;

// Helper function to insert a term in ascending exponent order
static void insertOrderedTerm(Term*& head, int coeff, int exp) {
	if (coeff == 0) return;

	Term* prev = nullptr;
	Term* curr = head;

	// Search for correct position or like term
	while (curr && curr->exp > exp) {
		prev = curr;
		curr = curr->next;
	}

	if (curr && curr->exp == exp) {
		curr->coeff += coeff;
		if (curr->coeff == 0) {
			// Delete node if coefficient becomes zero
			if (prev) prev->next = curr->next;
			else head = curr->next;
			delete curr;
		}
		return;
	}

	// Create new term node
	Term* newNode = new Term(coeff, exp);

	if (!prev) {
		newNode->next = head;
		head = newNode;
	}
	else {
		newNode->next = curr;
		prev->next = newNode;
	}
}

// Helper function to deep copy a polynomial
static Term* copyList(Term* head) {
	if (!head) return nullptr;
	Term* newHead = new Term(head->coeff, head->exp);
	Term* tail = newHead;
	Term* curr = head->next;
	while (curr) {
		tail->next = new Term(curr->coeff, curr->exp);
		tail = tail->next;
		curr = curr->next;
	}
	return newHead;
}

// Helper to delete polynomial data when needed
static void deleteList(Term*& head) {
	while (head) {
		Term* temp = head;
		head = head->next;
		delete temp;
	}
}

// === Member Function Definitions ===

void Polynomial::insertTerm(int coefficient, int exponent) {
	Term*& head = polyData[this];
	insertOrderedTerm(head, coefficient, exponent);
}

std::string Polynomial::toString() const {
	Term* head = nullptr;
	auto it = polyData.find(this);
	if (it != polyData.end()) head = it->second;
	if (!head) return "0";

	std::ostringstream out;
	Term* curr = head;
	bool first = true;

	while (curr) {
		int c = curr->coeff;
		int e = curr->exp;

		if (!first) {
			if (c > 0) out << " + ";
			else out << " - ";
		}
		else {
			if (c < 0) out << "-";
			first = false;
		}

		int absCoeff = std::abs(c);
		if (absCoeff != 1 || e == 0) out << absCoeff;
		if (e > 0) {
			out << "x";
			if (e > 1) out << "^" << e;
		}
		curr = curr->next;
	}

	return out.str();
}

Polynomial Polynomial::add(const Polynomial& other) const {
	Polynomial result;

	Term* p1 = nullptr;
	Term* p2 = nullptr;

	auto it1 = polyData.find(this);
	if (it1 != polyData.end()) p1 = it1->second;

	auto it2 = polyData.find(&other);
	if (it2 != polyData.end()) p2 = it2->second;

	Term* merged = nullptr;

	// Merge both lists
	while (p1 || p2) {
		if (!p2 || (p1 && p1->exp > p2->exp)) {
			insertOrderedTerm(merged, p1->coeff, p1->exp);
			p1 = p1->next;
		}
		else if (!p1 || (p2 && p2->exp > p1->exp)) {
			insertOrderedTerm(merged, p2->coeff, p2->exp);
			p2 = p2->next;
		}
		else {
			insertOrderedTerm(merged, p1->coeff + p2->coeff, p1->exp);
			p1 = p1->next;
			p2 = p2->next;
		}
	}

	polyData[&result] = merged;
	return result;
}

Polynomial Polynomial::multiply(const Polynomial& other) const {
	Polynomial result;
	Term* p1 = nullptr;
	Term* p2 = nullptr;

	auto it1 = polyData.find(this);
	if (it1 != polyData.end()) p1 = it1->second;

	auto it2 = polyData.find(&other);
	if (it2 != polyData.end()) p2 = it2->second;

	for (Term* t1 = p1; t1; t1 = t1->next) {
		for (Term* t2 = p2; t2; t2 = t2->next) {
			int newCoeff = t1->coeff * t2->coeff;
			int newExp = t1->exp + t2->exp;
			insertOrderedTerm(polyData[&result], newCoeff, newExp);
		}
	}

	return result;
}

Polynomial Polynomial::derivative() const {
	Polynomial result;
	Term* head = nullptr;

	auto it = polyData.find(this);
	if (it != polyData.end()) head = it->second;

	for (Term* curr = head; curr; curr = curr->next) {
		if (curr->exp > 0) {
			int newCoeff = curr->coeff * curr->exp;
			int newExp = curr->exp - 1;
			insertOrderedTerm(polyData[&result], newCoeff, newExp);
		}
	}
	return result;
}

