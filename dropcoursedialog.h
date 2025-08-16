/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
#ifndef DROPCOURSEDIALOG_H
#define DROPCOURSEDIALOG_H

#include <QDialog>
#include "student.h"
#include "course.h"

class QVBoxLayout;

/// Lists only the courses the student is enrolled in, each with “Drop” button
class DropCourseDialog : public QDialog {
    Q_OBJECT
public:
    DropCourseDialog(Student* student, Course** courseHead, QWidget* parent=nullptr);

private slots:
    void onDropClicked();

private:
    Student* m_student;
    Course** m_courseHead;
    QVBoxLayout* m_layout;
};

#endif // DROPCOURSEDIALOG_H
/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
