#ifndef STUDENTLOGIN_H
#define STUDENTLOGIN_H

#include <QDialog>
#include "student.h"
#include "course.h"

class QLineEdit;
class QPushButton;

/// Step 1: login dialog
class StudentLoginDialog : public QDialog {
    Q_OBJECT
public:
    /// students: the global student list; courseHead: pointer to global course list
    StudentLoginDialog(StudentList* students, Course** courseHead, QWidget* parent = nullptr);

private slots:
    void attemptLogin();

private:
    StudentList* m_students;
    Course**     m_courseHead;

    QLineEdit*   idEdit;
    QLineEdit*   passEdit;
    QPushButton* loginBtn;
};

#endif // STUDENTLOGIN_H
