#ifndef TEACHER_H
#define TEACHER_H
#include "User.h"
#include <string>
#include <vector>

using std::string;

enum class Subjects;
class Admin;
class Database;

class Teacher : private User
{
private:
    static int teacher_number;
    string teacherID;
    std::vector<Subjects> subjects;

    Teacher(string f_name, string l_name, string teacher_id, string password_hash_in,
            std::vector<int> sub);
    friend class Database;

public:
    Teacher(string f_name, string l_name, string password, std::vector<int> sub, bool registered);
    friend class Admin;

    void markAttendance();
    string generateTeacherID() const;
    string getSubjectName(const Subjects &subject) const;
    virtual void displayInfo() const override;
    string formatSubjectsAsString() const;
    string getTeacherID() const;

    string getName() const;
    string getSurname() const;
    string getPasswordHash() const;
    std::vector<int> getSubjectCodes() const;
};
#endif
