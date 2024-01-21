#include "Member.h"

Member::Member(int memberID, string name, string address, string email) :
	memberID(memberID) {
	setName(name);
	setAddress(address);
	setEmail(email);
}

int Member::getMemberID() {
	return this->memberID;
}

vector<Book*>& Member::getBooksBorrowed() {
	return this->booksLoaned;
}
