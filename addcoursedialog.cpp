/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
**/
#include "addcoursedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QMessageBox>

AddCourseDialog::AddCourseDialog(Student* student, Course** courseHead, QWidget* parent)
    : QDialog(parent), m_student(student), m_courseHead(courseHead)
{
    setWindowTitle("Add Course");
    setMinimumSize(300,400);

    // scroll area to hold list
    QScrollArea* scroll = new QScrollArea(this);
    QWidget* container = new QWidget;
    m_layout = new QVBoxLayout(container);

    // for each global course, add row
    for (Course* c = *m_courseHead; c; c=c->next) {
        QHBoxLayout* row = new QHBoxLayout;
        row->addWidget(new QLabel(c->getId() + " : " + c->getTitle()));
        QPushButton* btn = new QPushButton("Add");
        btn->setProperty("cid", c->getId());
        row->addWidget(btn);
        m_layout->addLayout(row);
        connect(btn, &QPushButton::clicked, this, &AddCourseDialog::onAddClicked);
    }
    m_layout->addStretch();
    container->setLayout(m_layout);
    scroll->setWidget(container);
    scroll->setWidgetResizable(true);

    QVBoxLayout* main = new QVBoxLayout(this);
    main->addWidget(scroll);
}
void AddCourseDialog::onAddClicked()
{
    // find which button was pressed
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    QString cid = btn->property("cid").toString();

    // find Course*
    Course* course = nullptr;
    for (Course* c = *m_courseHead; c; c=c->next)
        if (c->getId()==cid) { course=c; break; }

    if (!course) {
        QMessageBox::warning(this,"Error","Course not found");
        return;
    }

    // attempt to add
    try{
        m_student->addCourse(course);
    }
    catch (QString errorMessage){
        QMessageBox::warning(this, "Error", errorMessage);
        return;
    }
    QMessageBox::information(this,"Added","Course " + cid + " added");

    // optionally disable this button so you can't add again
    btn->setEnabled(false);
}
/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
