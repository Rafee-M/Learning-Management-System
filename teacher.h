#ifndef TEACHER_H
#define TEACHER_H

#include <QString>
#include "course.h"

class Teacher {
public:
    QString id;
    QString name;
    QString password;
    Course* courseHead;
    Teacher* next;

    // **declaration only**:
    Teacher(const QString& i, const QString n, const QString& pw);


    QString getId() const;
    QString getName() const;
    QString getPassword() const;
    bool    authenticate(const QString& pw) const;

    void setId(const QString& v)        { id = v; }
    void setName(const QString& v)      { name = v; }
    void setPassword(const QString& v)  { password = v; }


    void assignCourse(Course* orig);
    void viewCourses(QString& out) const;
};

#endif // TEACHER_H
