#include "teacherwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>

// externals from main:
extern StudentList students;
extern Course*     courseHead;

TeacherWindow::TeacherWindow(TeacherList* teachers, QWidget* parent)
    : QDialog(parent), m_teachers(teachers)
{
    setWindowTitle("Teacher Panel");
    setMinimumSize(500,300);
    resize(600,350);

    auto v = new QVBoxLayout(this);

    btnViewCourses  = new QPushButton("View My Courses");
    btnViewStudents = new QPushButton("View Students in Course");
    btnViewCourses->setEnabled(false);
    btnViewStudents->setEnabled(false);

    v->addWidget(btnViewCourses);
    v->addWidget(btnViewStudents);

    connect(btnViewCourses,  &QPushButton::clicked, this, &TeacherWindow::viewMyCourses);
    connect(btnViewStudents, &QPushButton::clicked, this, &TeacherWindow::viewStudentsInCourse);
}

bool TeacherWindow::login()
{
    bool ok;
    QString tid = QInputDialog::getText(this,"Teacher Login","Teacher ID:",QLineEdit::Normal,{},&ok);
    if (!ok || tid.isEmpty()) return false;
    QString pw  = QInputDialog::getText(this,"Teacher Login","Password:",QLineEdit::Password,{},&ok);
    if (!ok) return false;

    Teacher* t = m_teachers->findById(tid);
    if (!t || !t->authenticate(pw)) {
        QMessageBox::warning(this,"Login Failed","Invalid credentials");
        return false;
    }
    m_currentId = tid;
    m_loggedIn  = true;
    btnViewCourses->setEnabled(true);
    btnViewStudents->setEnabled(true);
    return true;
}

void TeacherWindow::viewMyCourses() {
    if (!m_loggedIn) return;
    Teacher* t = m_teachers->findById(m_currentId);
    QString out;
    t->viewCourses(out);
    QMessageBox::information(this,"My Courses", out.isEmpty() ? "(none)" : out);
}

void TeacherWindow::viewStudentsInCourse() {
    if (!m_loggedIn) return;
    bool ok;
    QString cid = QInputDialog::getText(this,"Students In Course","Course ID:",QLineEdit::Normal,{},&ok);
    if (!ok || cid.isEmpty()) return;

    Teacher* t = m_teachers->findById(m_currentId);
    bool has=false;
    for (Course* c=t->courseHead; c; c=c->next)
        if (c->getId()==cid) { has=true; break; }
    if (!has) {
        QMessageBox::warning(this,"Error","You are not assigned that course");
        return;
    }

    QString out="ID | Name\n-------\n";
    bool any=false;
    for (Student* s=students.head; s; s=s->next) {
        for (Course* c=s->courseHead; c; c=c->next) {
            if (c->getId()==cid) {
                out += s->getId()+" | "+s->getName()+"\n";
                any=true;
                break;
            }
        }
    }
    QMessageBox::information(this,"Enrolled Students", any?out:"(none)");
}
/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
