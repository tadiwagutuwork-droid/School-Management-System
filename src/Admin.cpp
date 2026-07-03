#include "bcrypt.h"
#include <ctime>
#include <format>
#include <iostream>
#include "../include/Admin.h"
#include "../include/Student.h"
#include "../include/Teacher.h"


enum class Role
{
    STUDENT,
    TEACHER,
    ADMIN
};

Admin::Admin(string f_name, string l_name, string password, bool registered) : User(f_name, l_name, password)
{
    if (!registered)
    {
        admin_number++;
        this->adminID = generateAdminID();
    }
}

Admin::Admin(string f_name, string l_name, string admin_id, string password_hash_in)
    : Admin(f_name, l_name, "", true)
{
    adminID = admin_id;
    password_hash = password_hash_in;
}

Student Admin::registerStudent(string f_name, string l_name, string password, int grade_level) const
{
    return Student(f_name, l_name, password, grade_level, false);
}

Teacher Admin::registerTeacher(string f_name, string l_name, string password, std::vector<int> sub) const
{
    return Teacher(f_name, l_name, password, sub, false);
}

void Admin::changePassword(Student &student, string password) const
{
    std::string admpass;
    std::cout << "Enter authorization password: ";
    std::cin >> admpass;

    verifyAdmin(admpass);
    student.setPasswordHash(password);
}

string Admin::generateReports(string studentID) const
{
    return "";
}

string Admin::generateAdminID() const
{
    std::time_t now = std::time(nullptr);
    std::tm *local_time = std::localtime(&now);
    int year = local_time->tm_year + 1900;

    return std::format("ADM{}{:04d}", year, admin_number);
}

Role Admin::getRole(string first_3) const
{
    if (first_3 == "STU")
        return Role::STUDENT;
    if (first_3 == "EDU")
        return Role::TEACHER;
    if (first_3 == "ADM")
        return Role::ADMIN;
    throw string("Role Error: Not Applicable");
}

void Admin::displayInfo() const
{
    string info = std::format(R"(
Name: {}
Surname: {}
Admin ID: {}
)",
                              name, surname, adminID);

    std::cout << info << std::endl;
}

void Admin::verifyAdmin(string password) const
{
    if (bcrypt::validatePassword(password, password_hash))
    {
        std::cout << "Correct Password, Admin Verified" << std::endl;
    }
    else
    {
        throw string("Incorrect Password. Not Admin");
    }
}

string Admin::getAdminID() const
{
    return adminID;
}

string Admin::getName() const
{
    return name;
}

string Admin::getSurname() const
{
    return surname;
}

string Admin::getPasswordHash() const
{
    return password_hash;
}

int Admin::admin_number = 0;

