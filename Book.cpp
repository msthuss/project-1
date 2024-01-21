#include "Book.h"

Book::Book(int bookID, string bookName, string authorFirstName, string authorLastName, string bookType) :
	bookID(bookID), bookName(bookName), authorFirstName(authorFirstName), authorLastName(authorLastName),
	bookType(bookType) {
	this->borrower = NULL;
}

int Book::getBookID() {
	return this->bookID;
}

string Book::getBookName() {
	return this->bookName;
}

string Book::getAuthorFirstName() {
	return this->authorFirstName;
}

string Book::getAuthorLastName() {
	return this->authorLastName;
}

string Book::getBookType() {
	return this->bookType;
}

chrono::system_clock::time_point Book::getDueDate() {
	return this->dueDate;
}

void Book::setDueDate(chrono::system_clock::time_point dueDate) {
	this->dueDate = dueDate;
}

void Book::returnBook() {
	if (this->borrower == NULL) {
		return;
	}
	this->borrower = NULL;
	this->dueDate = std::chrono::system_clock::time_point();
}
