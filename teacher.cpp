#include "teacher.h"

// Constructor definition
Teacher::Teacher(const QString& i, const QString n, const QString& pw)
    : id(i), name(n), password(pw), courseHead(nullptr), next(nullptr){}

// getId definition
QString Teacher::getId() const {
    return id;
}

QString Teacher::getName() const{
    return name;
}

QString Teacher::getPassword() const{
    return password;
}

// authenticate definition
bool Teacher::authenticate(const QString& pw) const {
    return password == pw;
}

// assignCourse definition
void Teacher::assignCourse(Course* orig) {
    Course* c = new Course(
        orig->getId(),
        orig->getTitle(),
        orig->getCreditHour(),
        orig->getMaxCapacity(),
        orig->getTimeSlot()
        );
    c->next = courseHead;
    courseHead = c;
}

// viewCourses definition
void Teacher::viewCourses(QString& out) const {
    out.clear();
    for (Course* c = courseHead; c; c = c->next) {
        out += c->getId() + " | " + c->getTitle() + "\n";
    }
}
