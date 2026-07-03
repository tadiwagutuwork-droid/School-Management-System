#include "../include/User.h"
#include "bcrypt.h"

User::User(string first_name, string last_name, string password) : name(first_name), surname(last_name)
{
    password_hash = bcrypt::generateHash(password);
}

void User::setPasswordHash(string password)
{
    password_hash = bcrypt::generateHash(password);
}
