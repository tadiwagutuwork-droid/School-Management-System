#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>
#include <optional>
#include <stdexcept>

class Admin;
class Teacher;
class Student;

class SQLiteException : public std::runtime_error
{
public:
    explicit SQLiteException(const std::string &message)
        : std::runtime_error(message) {}
};

class Database
{
public:
    explicit Database(const std::string &path);
    ~Database();

    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;

    void addStudent(const Student &s);
    std::optional<Student> getStudent(const std::string &studentID);
    std::vector<Student> getAllStudents();
    void updateStudent(const Student &s);
    void deleteStudent(const std::string &studentID);

    void addTeacher(const Teacher &t);
    std::optional<Teacher> getTeacher(const std::string &teacherID);
    std::vector<Teacher> getAllTeachers();
    void updateTeacher(const Teacher &t);
    void deleteTeacher(const std::string &teacherID);

    void addAdmin(const Admin &a);
    std::optional<Admin> getAdmin(const std::string &adminID);
    std::vector<Admin> getAllAdmins();
    void deleteAdmin(const std::string &adminID);

private:
    sqlite3 *db_ = nullptr;

    void createTables();
    void execute(const std::string &sql);

    static std::string joinSubjectCodes(const std::vector<int> &codes);
    static std::vector<int> splitSubjectCodes(const std::string &joined);
};
