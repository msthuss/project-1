#pragma once
#include <string>
#include <iostream>
#include <chrono>

class Member;

using namespace std;

class Book
{
private:
	int bookID;
	string bookName;
	string authorFirstName;
	string authorLastName;
	string bookType;
	chrono::system_clock::time_point dueDate;
	Member* borrower;

public:
	Book(int bookID, string bookName, string authorFirstName, string authorLastName, string bookType);
	int getBookID();
	string getBookName();
	string getAuthorFirstName();
	string getAuthorLastName();
	string getBookType();
	chrono::system_clock::time_point getDueDate();
	void setDueDate(chrono::system_clock::time_point dueDate);
	void returnBook();
	void borrowBook(Member* borrower, chrono::system_clock::time_point dueDate);
};

