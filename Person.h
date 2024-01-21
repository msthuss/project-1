#pragma once
#include <string>

using namespace std;

class Person
{
private:
	string name;
	string address;
	string email;

public:
	string getName();
	string getAddress();
	string getEmail();
	void setName(string name);
	void setAddress(string address);
	void setEmail(string email);
};

