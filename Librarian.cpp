#include "Librarian.h"

Librarian::Librarian(int staffID, string name, string address, string email, int salary) : 
	staffID(staffID), salary(salary) {
	setName(name);
	setAddress(address);
	setEmail(email);
}

int Librarian::getStaffID() {
	return this->staffID;
}

void Librarian::setStaffID(int staffID) {
	this->staffID = staffID;
}

int Librarian::getSalary() {
	return this->salary;
}

void Librarian::setSalary(int salary) {
	this->salary = salary;
}
