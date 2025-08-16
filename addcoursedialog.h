/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
#ifndef ADDCOURSEDIALOG_H
#define ADDCOURSEDIALOG_H

#include <QDialog>
#include "course.h"
#include "student.h"

class QVBoxLayout;
class QPushButton;

/// Lists all courses in a scrollable list, each with an “Add” button
class AddCourseDialog : public QDialog {
    Q_OBJECT
public:
    AddCourseDialog(Student* student, Course** courseHead, QWidget* parent = nullptr);

private slots:
    void onAddClicked();

private:
    Student* m_student;
    Course** m_courseHead;
    QVBoxLayout* m_layout;
};

#endif // ADDCOURSEDIALOG_H
/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
