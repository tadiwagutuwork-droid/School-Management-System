# School Management System

A C++20 School Management System that demonstrates object-oriented programming, inheritance, password hashing, SQLite database integration, and role-based user management.

Developed by **Tadiwa Gutu**.

## Features

### User Management

* Base `User` class
* Secure password hashing using BCrypt
* Password verification
* User authentication support

### Student Management

* Student registration
* Unique student ID generation
* GPA management
* Academic status tracking
* Report card support

### Teacher Management

* Teacher registration
* Subject assignment
* Subject code management
* Teacher ID generation

### Admin Management

* Admin registration
* Admin authentication
* Password management
* Student and teacher administration

### Database Integration

* SQLite3 persistent storage
* Save and load students
* Save and load teachers
* Save and load administrators
* Duplicate record protection
* CRUD-style operations

---

## Project Structure

```text
School_Management_System
│
├── include/
│   ├── User.h
│   ├── Student.h
│   ├── Teacher.h
│   ├── Admin.h
│   └── Database.h
│
├── src/
│   ├── User.cpp
│   ├── Student.cpp
│   ├── Teacher.cpp
│   ├── Admin.cpp
│   ├── Database.cpp
│   ├── bcrypt.cpp
│   ├── blowfish.cpp
│   └── main.cpp
│
├── Makefile
├── LICENSE
└── README.md
```

---

## Technologies Used

* C++20
* SQLite3
* BCrypt Password Hashing
* GNU Make
* Object-Oriented Programming

---

## Class Overview

### User

Base class containing:

* First name
* Last name
* Password hash

Provides:

* Password hashing
* Password verification
* Common user functionality

### Student

Derived from `User`.

Stores:

* Student ID
* Grade
* GPA
* Academic status
* Report card

### Teacher

Derived from `User`.

Stores:

* Teacher ID
* Assigned subjects
* Subject codes

### Admin

Derived from `User`.

Stores:

* Admin ID
* Administrative privileges

Provides:

* Password management
* Admin verification

### Database

Handles SQLite interactions.

Provides:

* Database creation
* Table creation
* Student persistence
* Teacher persistence
* Admin persistence
* Record retrieval

---

## Building the Project

### Linux

Install SQLite development libraries:

```bash
sudo apt update
sudo apt install libsqlite3-dev
```

Build:

```bash
make
```

Run:

```bash
./school
```

Clean build files:

```bash
make clean
```

---

## Example Workflow

1. Create an Admin account.
2. Register Students.
3. Register Teachers.
4. Store data in SQLite.
5. Reload data from the database.
6. Verify passwords securely using BCrypt.

---

## Security

Passwords are never stored in plain text.

The system uses BCrypt hashing to:

* Generate secure password hashes
* Verify user credentials
* Protect stored account information

---

## Learning Objectives

This project demonstrates:

* Classes and Objects
* Inheritance
* Encapsulation
* Friend Classes
* Constructor Delegation
* File Organization
* SQLite Integration
* Password Hashing
* Makefiles
* Modern C++20 Development

---

## License

This project is licensed under the MIT License.

Copyright (c) 2026 Tadiwa Gutu

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software.

See the `LICENSE` file for the full license text.

---

## Author

**Tadiwa Gutu**

Aspiring Software Engineer specializing in C++, Software Development, and AI/ML.
