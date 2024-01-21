#pragma once
#include "Person.h"
#include <vector>

class Book;

class Member :
    public Person
{
private:
    int memberID;
    vector<Book*> booksLoaned;

public:
    Member(int memberID, string name, string address, string email);
    int getMemberID();
    vector<Book*>& getBooksBorrowed();
    void setBooksBorrowed(Book *book);
};

