#ifndef TEACHERLIST_H
#define TEACHERLIST_H

#include <QString>
#include "teacher.h"   // include Teacher definition

// ── TeacherList ──────────────────────────────────────────────────────────────
class TeacherList {
public:
    Teacher* head;

    TeacherList();
    ~TeacherList();

    void      addTeacher(Teacher* t);
    Teacher*  findById(const QString& i) const;
};

#endif // TEACHERLIST_H
