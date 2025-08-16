#include "student.h"
#include "course.h"
#include <QString>
#include <QStringList>

// access the global list
extern Course* courseHead;

// — StudentList implementations —
StudentList::StudentList() : head(nullptr) {}
StudentList::~StudentList() {
    Student* p = head;
    while (p) { Student* t = p; p = p->next; delete t; }
}
void StudentList::addStudent(Student* s) {
    s->next = head; head = s;
}
Student* StudentList::findById(const QString& sid) const {
    for (Student* p=head; p; p=p->next)
        if (p->id == sid) return p;
    return nullptr;
}
Student* StudentList::findByIdEmail(const QString& sid, const QString& em) const {
    for (Student* p=head; p; p=p->next)
        if (p->id==sid && p->email==em) return p;
    return nullptr;
}
void StudentList::removeStudent(const QString& sid) {
    Student* p=head; Student* prev=nullptr;
    while (p) {
        if (p->id==sid) {
            if (prev) prev->next = p->next; else head = p->next;
            delete p;
            return;
        }
        prev = p; p = p->next;
    }
}

// — Student implementations —
Student::Student(const QString& n, const QString& i, const QString& e, const QString& p)
    : name(n), id(i), email(e), password(p), courseHead(nullptr), next(nullptr)
{}
QString Student::getName() const { return name; }
QString Student::getId()   const { return id; }

void Student::addCourse(Course* orig) {
    // time‐conflict & duplicate checks omitted for brevity—keep your logic here
    QString newTime = orig->getTimeSlot();
    QStringList newTimeFields = newTime.split("-");

    //check if input format is wrong
    if (newTimeFields.size() < 3) {
        throw QString("Invalid time slot format");
    }

    //check for time clash
    for(Course* temp = courseHead; temp; temp = temp->next){

        QString oldTime = temp->getTimeSlot();
        QStringList oldTimeFields = oldTime.split("-");


            if (newTimeFields[0] == oldTimeFields[0] &&  // Same day
                newTimeFields[1] == oldTimeFields[1]){   // Same time
                throw orig->getId() + "'s time clashes with " + courseHead->getId();
            }
    }

    // attempt to increment the ORIGINAL and check for max capacity reached
    if (!orig->incrementEnrolled()) {
        throw QString("Course ") + orig->getId() + " has reached max capacity";
    }

    // clone for student’s own list
    Course* clone = new Course(
        orig->getId(),
        orig->getTitle(),
        orig->getCreditHour(),
        orig->getMaxCapacity(),
        orig->getTimeSlot()
        );
    clone->next = courseHead;
    courseHead = clone;
}
QString Student::getPassword() const {
    return password;
}
QString Student::getEmail() const{
    return  email;
}
void Student::removeCourse(const QString& cid) {
    // remove from student’s clone-list
    Course* p = courseHead; Course* prev = nullptr;
    while (p) {
        if (p->getId()==cid) {
            if (prev) prev->next = p->next; else courseHead = p->next;
            delete p;
            break;
        }
        prev = p; p = p->next;
    }
    // decrement ORIGINAL’s count
    Course* g = ::courseHead;
    while (g) {
        if (g->getId()==cid) { g->decrementEnrolled(); break; }
        g = g->next;
    }
}
/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
