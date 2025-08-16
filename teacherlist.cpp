/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
#include "teacherlist.h"
#include "teacher.h"

// ——— TeacherList impl ———
TeacherList::TeacherList()
    : head(nullptr)
{}

TeacherList::~TeacherList() {
    Teacher* p = head;
    while (p) {
        Teacher* t = p;
        p = p->next;
        delete t;
    }
}

void TeacherList::addTeacher(Teacher* t) {
    t->next = head;
    head = t;
}

Teacher* TeacherList::findById(const QString& i) const {
    Teacher* p = head;
    while (p) {
        if (p->getId() == i) return p;
        p = p->next;
    }
    return nullptr;
}
