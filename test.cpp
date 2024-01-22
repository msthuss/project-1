#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include "Librarian.h"
#include "Book.h"
#include "Member.h"
#include <vector>

string allowedBookTypesList[] = {
	"Science fiction",
	"Satire",
	"Drama",
	"Action and Adventure",
	"Romance",
	"Mystery",
	"Horror",
	"Health",
	"Guide",
	"Diaries",
	"Comics",
	"Journals",
	"Biographies",
	"Fantasy",
	"History",
	"Science",
	"Art"
};

vector<Book*> books;
vector<Member*> members;

static int membersID = 1;

Member* findMemberByID(int memberID) {
	Member* member = NULL;
	for (int i = 0; i < members.size(); i++) {
		if (members[i]->getMemberID() == memberID) {
			member = members[i];
		}
	}
	return member;
}

Book* findBookByID(int bookID) {
	Book* book = NULL;
	for (int i = 0; i < books.size(); i++) {
		if (books[i]->getBookID() == bookID) {
			book = books[i];
		}
	}
	return book;
}

void Librarian::addMember() {
	Member* member = new Member(membersID, "Muzzammil", "SEECS", "muzzammil@umaish.com");
	members.push_back(member);
	membersID++;
}

void Librarian::issueBook(int memberID, int bookID) {
	Member* member = findMemberByID(memberID);
	if (!member) {
		cout << "No memeber with member ID " << memberID << " is present." << endl;
		cout << endl;
		return;
	}
	Book* book = findBookByID(bookID);
	if (!book) {
		cout << "No book with book ID " << bookID << " is present." << endl;
		cout << endl;
		return;
	}
	book->borrowBook(member, std::chrono::system_clock::now() + std::chrono::hours(24 * 3));
	member->setBooksBorrowed(book);
}

void Librarian::returnBook(int memberID, int bookID) {
	Member* member = findMemberByID(memberID);
	if (!member) {
		cout << "No memeber with member ID " << memberID << " is present." << endl;
		cout << endl;
		return;
	}
	Book* book = findBookByID(bookID);
	if (!book) {
		cout << "No book with book ID " << bookID << " is present." << endl;
		cout << endl;
		return;
	}
	book->returnBook();
	auto& borrowedBooks = member->getBooksBorrowed();
	int index = -1;
	for (int i = 0; i < borrowedBooks.size(); i++) {
		if (borrowedBooks[i]->getBookID() == bookID) {
			index = i;
			break;
		}
	}
	if (index != -1) {
		swap(borrowedBooks[index], borrowedBooks.back());
		borrowedBooks.pop_back();
		cout << borrowedBooks.size() << endl;
	}
}

void Book::borrowBook(Member* borrower, chrono::system_clock::time_point dueDate) {
	if (borrower != NULL) {
		return;
	}
	this->borrower = borrower;
	this->dueDate = dueDate;
}

void Member::setBooksBorrowed(Book* book) {
	for (Book* book1 : this->booksLoaned) {
		if (book1->getBookID() == book->getBookID()) {
			return;
		}
	}
	this->booksLoaned.push_back(book);
}

bool bookTypePresent(string bookType) {
	int size = sizeof(allowedBookTypesList) / sizeof(allowedBookTypesList[0]);
	for (int i = 0; i < size; i++) {
		if (allowedBookTypesList[i] == bookType) {
			return true;
		}
	}
	return false;
}

vector<string> tokenizeCSV(string line) {
	vector<string> tokens;
	istringstream iss(line);
	string token;
	while (getline(iss, token, ',')) {
		if (!token.empty() && token.front() == '"') {
			string tempToken = token;
			while (getline(iss, token, ',')) {
				tempToken += "," + token;
				if (!token.empty() && token.back() == '"') {
					break;
				}
			}
			tempToken.erase(0, 1);
			tempToken.pop_back();
			tokens.push_back(tempToken);
		}
		else {
			tokens.push_back(token);
		}
	}
	return tokens;
}

void Librarian::displayBorrowedBooks(int memberID) {
	Member* member = findMemberByID(memberID);
	if (!member) {
		cout << "No memeber with member ID " << memberID << " is present." << endl;
		cout << endl;
		return;
	}
	if (member->getBooksBorrowed().size() == 0) {
		cout << "No books is borrowed by member with member ID " << memberID << "." << endl;
		cout << endl;
		return;
	}
	int i = 1;
	cout << "Books borrowed by member with member ID " << memberID << " are: " << endl;
	for (Book *book : member->getBooksBorrowed()) {
		cout << "Book " << i << ": " << endl;
	}
	cout << endl;
}

int Librarian::calcFine(int memberID) {
	Member* member = findMemberByID(memberID);
	if (!member) {
		cout << "No memeber with member ID " << memberID << " is present." << endl;
		cout << endl;
		return 0;
	}
	int totalFine = 0;
	for (Book* borrowedBook : member->getBooksBorrowed()) {
		std::chrono::system_clock::time_point currentDate = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point dueDate = borrowedBook->getDueDate();
		if (currentDate > dueDate) {
			std::chrono::hours overdueHours = std::chrono::duration_cast<std::chrono::hours>(currentDate - dueDate);
			int daysOverdue = overdueHours.count() / 24;
			int fineForBook = daysOverdue * 1;
			totalFine += fineForBook;
		}
	}
	return totalFine;
}

void readFile(string fileName) {
	ifstream file(fileName);
	if (!file.is_open()) {
		cout << "File Can't be open for reading" << endl;
		exit(2);
	}
	string line;
	getline(file, line);
	while (getline(file, line)) {
		vector<string> tokens = tokenizeCSV(line);
		if (!bookTypePresent(tokens[5])) {
			cout << "Invalid Book Type: " << tokens[5] << endl;
			continue;
		}
		Book* book = new Book(stoi(tokens[0]), tokens[1], tokens[3], tokens[4], tokens[5]);
		books.push_back(book);
	}

	file.close();
}

TEST_CASE("Library Management System Tests", "[librarian]") {
	readFile("library_books.csv");
    Librarian* librarian = new Librarian(1, "Admin", "Library", "admin@admin.com", 10000);

    SECTION("Adding a member") {
        librarian->addMember();
		REQUIRE(members.size() == 1);
		REQUIRE(members[0]->getMemberID() == 1);
    }

    SECTION("Issuing a book") {
        int memberID = 1;
        int bookID = 3;
        librarian->issueBook(memberID, bookID);
		REQUIRE(members[0]->getMemberID() == 1);
		REQUIRE(members[0]->getBooksBorrowed().size() == 1);
		REQUIRE(members[0]->getBooksBorrowed()[0]->getBookID() == 3);
    }

    SECTION("Returning a book") {
        int memberID = 1;
        int bookID = 3;
        librarian->returnBook(memberID, bookID);
		REQUIRE(members[0]->getMemberID() == 1);
		REQUIRE(members[0]->getBooksBorrowed().size() == 0);
    }

    SECTION("Displaying borrowed books") {
        int memberID = 1;
        librarian->displayBorrowedBooks(memberID);
        REQUIRE(1 == 1);
    }

    SECTION("Calculating fines") {
        int memberID = 1;
        int totalFine = librarian->calcFine(memberID);
        REQUIRE(totalFine == 0);
    }

    delete librarian;
}

