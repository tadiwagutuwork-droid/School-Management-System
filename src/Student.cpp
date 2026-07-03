#include "../include/Student.h"
#include <ctime>
#include <format>
#include <iostream>

enum class Status
{
    ACTIVE,
    SUSPENDED,
    EXPELLED,
    TRANSFERRED,
    COMPLETED
};

Student::Student(string f_name, string l_name, string password, int grade_level, bool registered)
    : User(f_name, l_name, password), grade(grade_level)
{
    if (!registered)
    {
        student_number++;
        this->studentID = generateStudentID();
    }
};

Student::Student(string f_name, string l_name, string stu_id, string password_hash_in,
                 int grade_level, float gpa_in, int status_code, string report_card_in)
    : Student(f_name, l_name, "", grade_level, true)
{
    setStudentID(stu_id);
    password_hash = password_hash_in;
    setGPA(gpa_in);
    setStatus(status_code);
    setReportCard(report_card_in);
}

void Student::displayInfo() const
{
    string info = std::format(R"(
Name: {}
Surname: {}
Grade: {}
Student ID: {}
GPA: {:.2f}
Status: {}
)",
                              name, surname, grade, studentID, gpa, getStatus());

    std::cout << info << std::endl;
}

void Student::setStudentID(string stuID)
{
    studentID = stuID;
}
void Student::setReportCard(string report)
{
    reportCard = report;
}
void Student::setGPA(float GPA)
{
    if (GPA >= 0 && GPA <= 4)
    {
        gpa = GPA;
    }
    else
    {
        throw string("Outside bounds: GPA should be between 0-4\n");
    }
}
void Student::setStatus(int s)
{
    if (s >= 0 && s <= 4)
    {
        status = static_cast<Status>(s);
    }
    else
    {
        throw string("Outside bounds: Index should be between 0-4\n");
    }
}

string Student::generateStudentID() const
{
    std::time_t now = std::time(nullptr);
    std::tm local_time = *std::localtime(&now);

    int year = local_time.tm_year + 1900;


    return std::format("STU{}{:04d}", year, student_number);
}

string Student::getStatus() const
{
    switch (status)
    {
    case Status::ACTIVE:
        return "Active";
    case Status::SUSPENDED:
        return "Suspended";
    case Status::EXPELLED:
        return "Expelled";
    case Status::TRANSFERRED:
        return "Transferred";
    case Status::COMPLETED:
        return "Completed";
    }
    return "Error";
}

float Student::getGPA() const
{
    return gpa;
}

void Student::viewReportCard() const
{
    if (reportCard == "")
    {
        std::cout << "No Report Cards Available!\n";
    }
    std::cout << reportCard << std::endl;
}

string Student::getStudentID() const
{
    return studentID;
}

string Student::getPasswordHash() const
{
    return password_hash;
}

string Student::getName() const
{
    return name;
}

string Student::getSurname() const
{
    return surname;
}

int Student::getGrade() const
{
    return grade;
}

int Student::getStatusCode() const
{
    return static_cast<int>(status);
}

string Student::getReportCard() const
{
    return reportCard;
}

int Student::student_number = 0;

