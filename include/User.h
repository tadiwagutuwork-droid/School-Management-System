#ifndef USER_H
#define USER_H
#include <string>
#include "bcrypt.h"

using std::string;

class User
{
    protected:
    string name, surname;
    string password_hash;

    public:
    User(string first_name, string last_name, string password);
    virtual void displayInfo() const = 0;
    void setPasswordHash(string password);
    virtual string getName() const = 0;
    virtual string getSurname() const = 0;
    virtual string getPasswordHash() const = 0;
};

#endif