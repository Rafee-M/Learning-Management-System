/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
#ifndef COURSE_H
#define COURSE_H

#include <QString>

// global head (defined in main.cpp)


class Course {
public:
    QString courseID;
    QString title;
    QString timeSlot;
    int creditHours;
    int maxCapacity;
    int enrolledCount;
    Course* next;

    Course(const QString& id,
           const QString& t,
           int ch,
           int mcap,
           const QString& ts)
        : courseID(id)
        , title(t)
        , timeSlot(ts)
        , creditHours(ch)
        , maxCapacity(mcap)
        , enrolledCount(0)
        , next(nullptr)
    {}

    QString getId() const       { return courseID; }
    QString getTitle() const    { return title; }
    QString getTimeSlot() const { return timeSlot; }
    int     getCreditHour() const   { return creditHours; }
    int     getMaxCapacity() const  { return maxCapacity; }
    int     getEnrolledCount() const{ return enrolledCount; }

    void setId(const QString& v)           { courseID = v; }
    void setTitle(const QString& v)        { title = v; }
    void setTimeSlot(const QString& v)     { timeSlot = v; }
    void setCreditHour(int v)              { creditHours = v; }
    void setMaxCapacity(int v)             { maxCapacity = v; }
    // if you really need it:
    void setEnrolledCount(int v)           { enrolledCount = v; }

    bool incrementEnrolled() {
        if (enrolledCount < maxCapacity) { ++enrolledCount; return true; }
        return false;
    }
    bool decrementEnrolled() {
        if (enrolledCount > 0) { --enrolledCount; return true; }
        return false;
    }
};
extern Course* courseHead;
#endif // COURSE_H
/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
