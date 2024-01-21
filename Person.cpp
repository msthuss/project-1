#include "Person.h"

string Person::getName() {
	return this->name;
}

string Person::getAddress() {
	return this->address;
}

string Person::getEmail() {
	return this->email;
}

void Person::setName(string name) {
	this->name = name;
}

void Person::setAddress(string address) {
	this->address = address;
}

void Person::setEmail(string email) {
	this->email = email;
}