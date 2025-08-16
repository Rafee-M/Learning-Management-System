#ifndef STUDENTPANEL_H
#define STUDENTPANEL_H

#include <QDialog>
#include "student.h"
#include "course.h"

class QPushButton;

/// After login, this dialog shows three centered buttons:
/// View My Courses, Add Course, Drop Course
class StudentPanelDialog : public QDialog {
    Q_OBJECT
public:
    StudentPanelDialog(Student* student, Course** courseHead, QWidget* parent = nullptr);

private slots:
    void viewMyCourses();
    void addCourse();
    void dropCourse();

private:
    Student* m_student;
    Course** m_courseHead;

    QPushButton* viewBtn;
    QPushButton* addCourseBtn;
    QPushButton* dropCourseBtn;
};

#endif // STUDENTPANEL_H
