#include "Database.h"
#include "Student.h"
#include "Teacher.h"
#include "Admin.h"
#include <sstream>

namespace
{

    class Stmt
    {
    public:
        Stmt(sqlite3 *db, const std::string &sql) : db_(db)
        {
            if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt_, nullptr) != SQLITE_OK)
            {
                throw SQLiteException("prepare failed: " + std::string(sqlite3_errmsg(db_)) +
                                      "\nSQL: " + sql);
            }
        }
        ~Stmt()
        {
            if (stmt_)
                sqlite3_finalize(stmt_);
        }

        Stmt(const Stmt &) = delete;
        Stmt &operator=(const Stmt &) = delete;

        Stmt &bind(int idx, int value)
        {
            sqlite3_bind_int(stmt_, idx, value);
            return *this;
        }
        Stmt &bind(int idx, double value)
        {
            sqlite3_bind_double(stmt_, idx, value);
            return *this;
        }
        Stmt &bind(int idx, const std::string &value)
        {
            sqlite3_bind_text(stmt_, idx, value.c_str(), -1, SQLITE_TRANSIENT);
            return *this;
        }

        bool step()
        {
            int rc = sqlite3_step(stmt_);
            if (rc == SQLITE_ROW)
                return true;
            if (rc == SQLITE_DONE)
                return false;
            throw SQLiteException("step failed: " + std::string(sqlite3_errmsg(db_)));
        }

        int getInt(int col) const { return sqlite3_column_int(stmt_, col); }
        double getDouble(int col) const { return sqlite3_column_double(stmt_, col); }
        std::string getText(int col) const
        {
            const unsigned char *t = sqlite3_column_text(stmt_, col);
            return t ? reinterpret_cast<const char *>(t) : "";
        }

    private:
        sqlite3 *db_;
        sqlite3_stmt *stmt_ = nullptr;
    };

} // namespace

Database::Database(const std::string &path)
{
    if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK)
    {
        std::string msg = sqlite3_errmsg(db_);
        sqlite3_close(db_);
        throw SQLiteException("Cannot open database '" + path + "': " + msg);
    }
    execute("PRAGMA foreign_keys = ON;");
    createTables();
}

Database::~Database()
{
    if (db_)
        sqlite3_close(db_);
}

void Database::execute(const std::string &sql)
{
    char *errMsg = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::string msg = errMsg ? errMsg : "unknown error";
        sqlite3_free(errMsg);
        throw SQLiteException("execute() failed: " + msg + "\nSQL: " + sql);
    }
}

void Database::createTables()
{
    execute(
        "CREATE TABLE IF NOT EXISTS students ("
        "  student_id TEXT PRIMARY KEY,"
        "  name TEXT NOT NULL,"
        "  surname TEXT NOT NULL,"
        "  password_hash TEXT NOT NULL,"
        "  grade INTEGER,"
        "  gpa REAL,"
        "  status_code INTEGER,"
        "  report_card TEXT"
        ");");

    execute(
        "CREATE TABLE IF NOT EXISTS teachers ("
        "  teacher_id TEXT PRIMARY KEY,"
        "  name TEXT NOT NULL,"
        "  surname TEXT NOT NULL,"
        "  password_hash TEXT NOT NULL,"
        "  subject_codes TEXT"
        ");");

    execute(
        "CREATE TABLE IF NOT EXISTS admins ("
        "  admin_id TEXT PRIMARY KEY,"
        "  name TEXT NOT NULL,"
        "  surname TEXT NOT NULL,"
        "  password_hash TEXT NOT NULL"
        ");");
}

std::string Database::joinSubjectCodes(const std::vector<int> &codes)
{
    std::ostringstream oss;
    for (size_t i = 0; i < codes.size(); ++i)
    {
        if (i > 0)
            oss << ",";
        oss << codes[i];
    }
    return oss.str();
}

std::vector<int> Database::splitSubjectCodes(const std::string &joined)
{
    std::vector<int> result;
    if (joined.empty())
        return result;

    std::stringstream ss(joined);
    std::string item;
    while (std::getline(ss, item, ','))
    {
        result.push_back(std::stoi(item));
    }
    return result;
}




void Database::addStudent(const Student &s)
{
    Stmt stmt(db_,
              "INSERT INTO students "
              "(student_id, name, surname, password_hash, grade, gpa, status_code, report_card) "
              "VALUES (?, ?, ?, ?, ?, ?, ?, ?);");
    stmt.bind(1, s.getStudentID())
        .bind(2, s.getName())
        .bind(3, s.getSurname())
        .bind(4, s.getPasswordHash())
        .bind(5, s.getGrade())
        .bind(6, static_cast<double>(s.getGPA()))
        .bind(7, s.getStatusCode())
        .bind(8, s.getReportCard());
    stmt.step();
}

std::optional<Student> Database::getStudent(const std::string &studentID)
{
    Stmt stmt(db_,
              "SELECT name, surname, student_id, password_hash, grade, gpa, status_code, report_card "
              "FROM students WHERE student_id = ?;");
    stmt.bind(1, studentID);

    if (!stmt.step())
        return std::nullopt;

    return Student(
        stmt.getText(0),
        stmt.getText(1),
        stmt.getText(2),
        stmt.getText(3),
        stmt.getInt(4),
        static_cast<float>(stmt.getDouble(5)),
        stmt.getInt(6),
        stmt.getText(7)


    );
}

std::vector<Student> Database::getAllStudents()
{
    Stmt stmt(db_,
              "SELECT name, surname, student_id, password_hash, grade, gpa, status_code, report_card "
              "FROM students ORDER BY student_id;");

    std::vector<Student> results;
    while (stmt.step())
    {
        Student s(
            stmt.getText(0),
            stmt.getText(1),
            stmt.getText(2),
            stmt.getText(3),
            stmt.getInt(4),
            static_cast<float>(stmt.getDouble(5)),
            stmt.getInt(6),
            stmt.getText(7));
        results.push_back(s);
    }
    return results;
}

void Database::updateStudent(const Student &s)
{
    Stmt stmt(db_,
              "UPDATE students SET name = ?, surname = ?, password_hash = ?, "
              "grade = ?, gpa = ?, status_code = ?, report_card = ? "
              "WHERE student_id = ?;");
    stmt.bind(1, s.getName())
        .bind(2, s.getSurname())
        .bind(3, s.getPasswordHash())
        .bind(4, s.getGrade())
        .bind(5, static_cast<double>(s.getGPA()))
        .bind(6, s.getStatusCode())
        .bind(7, s.getReportCard())
        .bind(8, s.getStudentID());
    stmt.step();
}

void Database::deleteStudent(const std::string &studentID)
{
    Stmt stmt(db_, "DELETE FROM students WHERE student_id = ?;");
    stmt.bind(1, studentID);
    stmt.step();
}




void Database::addTeacher(const Teacher &t)
{
    Stmt stmt(db_,
              "INSERT INTO teachers (teacher_id, name, surname, password_hash, subject_codes) "
              "VALUES (?, ?, ?, ?, ?);");
    stmt.bind(1, t.getTeacherID())
        .bind(2, t.getName())
        .bind(3, t.getSurname())
        .bind(4, t.getPasswordHash())
        .bind(5, joinSubjectCodes(t.getSubjectCodes()));
    stmt.step();
}

std::optional<Teacher> Database::getTeacher(const std::string &teacherID)
{
    Stmt stmt(db_,
              "SELECT name, surname, teacher_id, password_hash, subject_codes "
              "FROM teachers WHERE teacher_id = ?;");
    stmt.bind(1, teacherID);

    if (!stmt.step())
        return std::nullopt;

    return Teacher(
        stmt.getText(0),
        stmt.getText(1),
        stmt.getText(2),
        stmt.getText(3),
        splitSubjectCodes(stmt.getText(4))

    );
}

std::vector<Teacher> Database::getAllTeachers()
{
    Stmt stmt(db_,
              "SELECT name, surname, teacher_id, password_hash, subject_codes "
              "FROM teachers ORDER BY teacher_id;");

    std::vector<Teacher> results;
    while (stmt.step())
    {
        Teacher t(
            stmt.getText(0),
            stmt.getText(1),
            stmt.getText(2),
            stmt.getText(3),
            splitSubjectCodes(stmt.getText(4)));
        results.push_back(t);
    }
    return results;
}

void Database::updateTeacher(const Teacher &t)
{
    Stmt stmt(db_,
              "UPDATE teachers SET name = ?, surname = ?, password_hash = ?, subject_codes = ? "
              "WHERE teacher_id = ?;");
    stmt.bind(1, t.getName())
        .bind(2, t.getSurname())
        .bind(3, t.getPasswordHash())
        .bind(4, joinSubjectCodes(t.getSubjectCodes()))
        .bind(5, t.getTeacherID());
    stmt.step();
}

void Database::deleteTeacher(const std::string &teacherID)
{
    Stmt stmt(db_, "DELETE FROM teachers WHERE teacher_id = ?;");
    stmt.bind(1, teacherID);
    stmt.step();
}




void Database::addAdmin(const Admin &a)
{
    Stmt stmt(db_,
              "INSERT INTO admins (admin_id, name, surname, password_hash) VALUES (?, ?, ?, ?);");
    stmt.bind(1, a.getAdminID())
        .bind(2, a.getName())
        .bind(3, a.getSurname())
        .bind(4, a.getPasswordHash());
    stmt.step();
}

std::optional<Admin> Database::getAdmin(const std::string &adminID)
{
    Stmt stmt(db_,
              "SELECT name, surname, admin_id, password_hash FROM admins WHERE admin_id = ?;");
    stmt.bind(1, adminID);

    if (!stmt.step())
        return std::nullopt;

    return Admin(
        stmt.getText(0),
        stmt.getText(1),
        stmt.getText(2),
        stmt.getText(3)

    );
}

std::vector<Admin> Database::getAllAdmins()
{
    Stmt stmt(db_, "SELECT name, surname, admin_id, password_hash FROM admins ORDER BY admin_id;");

    std::vector<Admin> results;
    while (stmt.step())
    {
        Admin a(
            stmt.getText(0),
            stmt.getText(1),
            stmt.getText(2),
            stmt.getText(3));
        results.push_back(a);
    }
    return results;
}

void Database::deleteAdmin(const std::string &adminID)
{
    Stmt stmt(db_, "DELETE FROM admins WHERE admin_id = ?;");
    stmt.bind(1, adminID);
    stmt.step();
}