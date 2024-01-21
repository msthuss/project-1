#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include "Librarian.h"
#include "Book.h"
#include "Member.h"
#include <vector>
#include <algorithm>

using namespace std;

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

void printMemberDetails(Member* member) {
	cout << "\nDetails of member with ID: " << member->getMemberID() << " are: " << endl;
	cout << "Name: " << member->getName() << "\nAddress: " << member->getAddress() << "\nEmail: " << member->getEmail() << endl;
	cout << endl;
}

void Librarian::addMember() {
	cout << "Please Enter Member Details: " << endl;
	string name, address, email;
	cout << "Name: ";
	cin >> name;
	cout << "Address: ";
	cin >> address;
	cout << "Email: ";
	cin >> email;
	Member* member = new Member(membersID, name, address, email);
	members.push_back(member);
	membersID++;
	printMemberDetails(member);
}

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
	cout << "\nBook with ID: "<< bookID << " borrowed by member with ID: "<< memberID << endl;
	cout << endl;
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

void printBookDetails(Book* book) {
	cout << "\nDetails of Book with ID: " << book->getBookID() << " are: " << endl;
	cout << "Book Name: " << book->getBookName() << "\nAuthor First Name: " << book->getAuthorFirstName()
		<< "\nAuthor Last Name: " << book->getAuthorLastName() << "\nBook Type: " << book->getBookType() << endl;
	cout << endl;
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
		printBookDetails(book);
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

void printInstructions() {
	cout << "\nOptions:\n";
	cout << "1. Add a member.\n";
	cout << "2. Issue a book to member.\n";
	cout << "3. Return a book.\n";
	cout << "4. Display all books borrowed by a member.\n";
	cout << "5. Calculate a fine for a member for overdue book(s).\n";
	cout << "6. Exit\n";
	cout << "Enter your choice: ";
}

void driverFunction(string fileName) {
	cout << "............Library Management System...........\n" << endl;
	readFile(fileName);
	Librarian* librarian = new Librarian(1, "Admin", "Library", "admin@admin.com", 10000);
	int choice;
	do {
		printInstructions();
		cin >> choice;
		switch (choice) {
		case 1:
			librarian->addMember();
			break;
		case 2:
			int memberID, bookID;
			cout << "Please enter member ID: ";
			cin >> memberID;
			cout << "Please enter book ID: ";
			cin >> bookID;
			librarian->issueBook(memberID, bookID);
			cout << endl;
			break;
		case 3:
			int memberID1, bookID1;
			cout << "Please enter member ID: ";
			cin >> memberID1;
			cout << "Please enter book ID: ";
			cin >> bookID1;
			librarian->returnBook(memberID1, bookID1);
			cout << endl;
			break;
		case 4:
			int memberID2;
			cout << "Please enter member ID: ";
			cin >> memberID2;
			librarian->displayBorrowedBooks(memberID2);
			cout << endl;
			break;
		case 5:
			int memberID3;
			cout << "Please enter member ID: ";
			cin >> memberID3;
			cout << "Total fine for member for overdue book(s) is: " << librarian->calcFine(memberID3) << endl;
			cout << endl;
			break;
		case 6:
			cout << "Exiting the program.\n";
			cout << endl;
			break;
		default:
			cout << "Invalid choice. Please try again.\n";
			cout << endl;
			break;
		}

	} while (choice != 6);
	delete librarian;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cout << "Usage: Must provide file name from which the books should be read." << endl;
		exit(1);
	}
	driverFunction(argv[1]);
	return 0;
}
