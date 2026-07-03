#include <iostream>
#include <vector>
#include <optional>
#include <string>

// Include necessary headers based on your project structure
#include "Database.h"
#include "Admin.h"
#include "Student.h"
#include "Teacher.h"
#include "bcrypt.h"

int main()
{
    try
    {
        std::cout << "===== STARTING SCHOOL MANAGEMENT SYSTEM TEST =====\n"
                  << std::endl;


        std::cout << "[Step 1] Initializing Database..." << std::endl;
        Database db("../data/school_test.db");
        std::cout << "Database initialized successfully.\n"
                  << std::endl;

        // 3. Test Admin Creation and Business Logic
        std::cout << "[Step 2] Testing Admin Class..." << std::endl;
        Admin rootAdmin("Tadiwa", "Gutu", "SuperSecureAdmin123", false);
        rootAdmin.displayInfo();


        db.addAdmin(rootAdmin);
        std::cout << "Admin added to database.\n"
                  << std::endl;


        std::cout << "[Step 3] Registering and Saving a Student..." << std::endl;

        Student newStudent = rootAdmin.registerStudent("Alex", "Morgan", "studentPass2026", 11);


        newStudent.setGPA(3.85f);
        newStudent.setStatus(0);

        newStudent.setReportCard("Maths: A\nScience: B+\nEnglish: A-");

        std::cout << "Generated Student Info (Before DB Insert):";
        newStudent.displayInfo();

        // Save to database
        db.addStudent(newStudent);
        std::cout << "Student successfully inserted into SQL database.\n"
                  << std::endl;

        // 5. Register a Teacher via Admin and Save to Database
        std::cout << "[Step 4] Registering and Saving a Teacher..." << std::endl;
        // Subject codes: 0 = MATHS, 1 = SCIENCE, 4 = FINANCE
        std::vector<int> teacherSubjects = {0, 1, 4};
        Teacher newTeacher = rootAdmin.registerTeacher("Jane", "Doe", "teacherPassword", teacherSubjects);

        std::cout << "Generated Teacher Info (Before DB Insert):";
        newTeacher.displayInfo();

        // Save to database
        db.addTeacher(newTeacher);
        std::cout << "Teacher successfully inserted into SQL database.\n"
                  << std::endl;

        // 6. Test Database Retrieval and Updates
        std::cout << "[Step 5] Testing Database Retrieval Queries..." << std::endl;
        std::string searchID = newStudent.getStudentID();
        std::cout << "Searching for student ID: " << searchID << std::endl;

        std::optional<Student> retrievedStudent = db.getStudent(searchID);
        if (retrievedStudent.has_value())
        {
            std::cout << "\n--- Successfully Retrieved Student From DB ---";
            retrievedStudent->displayInfo();

            // View report card logic
            std::cout << "Report Card Contents:";
            retrievedStudent->viewReportCard();
        }
        else
        {
            std::cout << "Error: Student not found in database!" << std::endl;
        }

        // 7. Testing Admin Password Modification and Bcrypt Authorization
        std::cout << "\n[Step 6] Testing Admin Password Change Authorization..." << std::endl;
        std::cout << "Note: This will prompt you to type the Admin's authorization password." << std::endl;
        std::cout << "--> Enter: SuperSecureAdmin123" << std::endl;

        // This will call verifyAdmin internally using bcrypt
        rootAdmin.changePassword(newStudent, "BrandNewStudentPassword55");

        // Update the updated student state back to the database
        db.updateStudent(newStudent);
        std::cout << "Student password updated in memory and synchronized to DB.\n"
                  << std::endl;

        // 8. Test Data List Retrievals (GetAll)
        std::cout << "[Step 7] Testing Bulk Retrieval Queries..." << std::endl;
        std::vector<Student> allStudents = db.getAllStudents();
        std::cout << "Total students in system: " << allStudents.size() << std::endl;
        for (const auto &s : allStudents)
        {
            std::cout << " - " << s.getStudentID() << ": " << "Grade " << s.getGPA() << " GPA" << std::endl;
        }

        std::vector<Teacher> allTeachers = db.getAllTeachers();
        std::cout << "Total teachers in system: " << allTeachers.size() << std::endl;

        std::cout << "\n===== ALL TESTS COMPLETED SUCCESSFULLY =====" << std::endl;
    }
    catch (const SQLiteException &e)
    {
        std::cerr << "\n[DATABASE ERROR OCCURRED]: " << e.what() << std::endl;
    }
    catch (const std::string &errorMsg)
    {
        std::cerr << "\n[APPLICATION ERROR OCCURRED]: " << errorMsg << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "\n[SYSTEM ERROR OCCURRED]: " << e.what() << std::endl;
    }

    return 0;
}