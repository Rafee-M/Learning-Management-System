#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include "course.h"

class Student {
public:
    QString name, id, email, password;
    Course* courseHead;   // student’s *own* list of course-clones
    Student* next;

    Student(const QString& n,
            const QString& i,
            const QString& e,
            const QString& p);

    QString getName() const;
    QString getId()   const;
    QString getPassword() const;
    QString getEmail() const;
    void addCourse(Course* c);           // enroll
    void removeCourse(const QString& cid);

    void setName(const QString& v)      { name = v; }
    void setId(const QString& v)        { id = v; }
    void setEmail(const QString& v)     { email = v; }
    void setPassword(const QString& v)  { password = v; }
    // drop
};

class StudentList {
public:
    Student* head;
    StudentList();
    ~StudentList();

    void addStudent(Student* s);
    Student* findById(const QString& sid) const;
    Student* findByIdEmail(const QString& sid, const QString& em) const;
    void removeStudent(const QString& sid);
};

#endif // STUDENT_H
/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
