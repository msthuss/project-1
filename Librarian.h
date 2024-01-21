#pragma once
#include "Person.h"
#include <vector>

class Librarian :
    public Person
{
private:
    int staffID;
    int salary;

public:
    Librarian(int staffID, string name, string address, string email, int salary);
    void addMember();
    void issueBook(int memberID, int bookID);
    void returnBook(int memberID, int bookID);
    void displayBorrowedBooks(int memberID);
    int calcFine(int memberID);
    int getStaffID();
    void setStaffID(int staffID);
    int getSalary();
    void setSalary(int salary);
};

