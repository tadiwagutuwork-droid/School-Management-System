#include "Teacher.h"
#include <iostream>
#include <ctime>
#include <format>

enum class Subjects
{
    MATHS,
    SCIENCE,
    ENGLISH,
    BIOLOGY,
    FINANCE
};

Teacher::Teacher(string f_name, string l_name, string password, std::vector<int> sub, bool registered)
    : User(f_name, l_name, password)
{
    if (!registered)
    {
        teacher_number++;
        this->teacherID = generateTeacherID();
    }
    bool isEmptyVector = true;
    if (sub.empty())
    {
        std::cerr << "No subjects allocated" << std::endl;
        subjects = {};
    }
    else
    {
        for (int index : sub)
        {
            if (index >= 0 && index <= 4)
            {
                subjects.push_back(static_cast<Subjects>(index));
                isEmptyVector = false;
            }
        }
        if (isEmptyVector)
        {
            std::cerr << "No subjects allocated" << std::endl;
            subjects = {};
        }
    }
}

Teacher::Teacher(string f_name, string l_name, string teacher_id, string password_hash_in,
                 std::vector<int> sub)
    : Teacher(f_name, l_name, "", sub, true)
{
    teacherID = teacher_id;
    password_hash = password_hash_in;
}

void Teacher::displayInfo() const
{
    string info = std::format(R"(
Name: {}
Surname: {}
Teacher ID: {}
Subjects: {}
)",
                              name, surname, teacherID, formatSubjectsAsString());

    std::cout << info << std::endl;
}

string Teacher::generateTeacherID() const
{
    std::time_t now = std::time(nullptr);
    std::tm *local_time = std::localtime(&now);
    int year = local_time->tm_year + 1900;

    return std::format("EDU{}{:04d}", year, teacher_number);
}

string Teacher::getSubjectName(const Subjects &subject) const
{
    switch (subject)
    {
    case Subjects::MATHS:
        return "Maths";
    case Subjects::SCIENCE:
        return "Science";
    case Subjects::ENGLISH:
        return "English";
    case Subjects::BIOLOGY:
        return "Biology";
    case Subjects::FINANCE:
        return "Finance";
    default:
        throw string("Invalid Subjects type");
    }
}

string Teacher::formatSubjectsAsString() const
{
    if (subjects.empty())
        return "No Subjects";
    string sub = "";

    int size = subjects.size();

    for (int i = 0; i < size; i++)
    {
        if (i == size - 1)
        {
            sub += getSubjectName(subjects[i]);
            break;
        }
        sub += format("{}, ", getSubjectName(subjects[i]));
    }
    return sub;
}

string Teacher::getTeacherID() const
{
    return teacherID;
}

string Teacher::getName() const
{
    return name;
}

string Teacher::getSurname() const
{
    return surname;
}

string Teacher::getPasswordHash() const
{
    return password_hash;
}

std::vector<int> Teacher::getSubjectCodes() const
{
    std::vector<int> codes;
    codes.reserve(subjects.size());
    for (const Subjects &s : subjects)
        codes.push_back(static_cast<int>(s));
    return codes;
}

int Teacher::teacher_number = 0;

