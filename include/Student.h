#ifndef STUDENT_H
#define STUDENT_H
#include <string>
#include "User.h"

using std::string;

enum class Status;
class Admin;
class Database;

class Student : private User
{
private:
    static int student_number;
    string studentID, reportCard;
    int grade;
    float gpa;
    Status status;

    Student(string f_name, string l_name, string stu_id, string password_hash_in,
            int grade_level, float gpa_in, int status_code, string report_card_in);
    friend class Database;

public:
    Student(string f_name, string l_name, string password, int grade_level, bool registered);
    virtual void displayInfo() const override;
    friend class Admin;

    void setStudentID(string stuID);
    void setReportCard(string report);
    void setGPA(float GPA);
    void setStatus(int s);
    string getStudentID() const;
    float getGPA() const;
    string getStatus() const;
    void viewReportCard() const;
    string generateStudentID() const;
    string getPasswordHash() const;

    string getName() const;
    string getSurname() const;
    int getGrade() const;
    int getStatusCode() const;
    string getReportCard() const;
};

#endif
