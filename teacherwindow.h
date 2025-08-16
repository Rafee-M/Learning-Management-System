#ifndef TEACHERWINDOW_H
#define TEACHERWINDOW_H

#include <QDialog>
#include "teacherlist.h"
#include "student.h"
#include "course.h"

class QPushButton;

class TeacherWindow : public QDialog {
    Q_OBJECT
public:
    explicit TeacherWindow(TeacherList* teachers, QWidget* parent = nullptr);

    /// Prompt for credentials.  Returns true if login succeeded.
    bool login();

private slots:
    void viewMyCourses();
    void viewStudentsInCourse();

private:
    TeacherList* m_teachers;
    QString       m_currentId;
    bool          m_loggedIn = false;
    QPushButton*  btnViewCourses;
    QPushButton*  btnViewStudents;
};

#endif // TEACHERWINDOW_H
