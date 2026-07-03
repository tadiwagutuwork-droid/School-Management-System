#ifndef ADMIN_H
#define ADMIN_H
#include "User.h"
#include <string>
#include <vector>

using std::string;

enum class Role;
class Student;
class Teacher;
class Database;

class Admin : private User
{
private:
    static int admin_number;
    string adminID;

    Admin(string f_name, string l_name, string admin_id, string password_hash_in);
    friend class Database;

public:
    Admin(string f_name, string l_name, string password, bool registered);

    Student registerStudent(string f_name, string l_name, string password, int grade_level) const;
    Teacher registerTeacher(string f_name, string l_name, string password, std::vector<int> sub) const;
    virtual void displayInfo() const override;
    void changePassword(Student &student, string password) const;
    string generateReports(string studentID) const;
    string generateAdminID() const;
    Role getRole(string first_3) const;
    void verifyAdmin(string password) const;

    string getAdminID() const;
    string getName() const;
    string getSurname() const;
    string getPasswordHash() const;
};
#endif
