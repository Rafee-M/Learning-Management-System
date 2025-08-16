/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
#include "dropcoursedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QMessageBox>

DropCourseDialog::DropCourseDialog(Student* student, Course** courseHead, QWidget* parent)
    : QDialog(parent), m_student(student), m_courseHead(courseHead)
{
    setWindowTitle("Drop Course");
    setMinimumSize(300,400);

    QScrollArea* scroll = new QScrollArea(this);
    QWidget* container = new QWidget;
    m_layout = new QVBoxLayout(container);

    // for each course in student->courseHead
    for (Course* c = m_student->courseHead; c; c=c->next) {
        QHBoxLayout* row = new QHBoxLayout;
        row->addWidget(new QLabel(c->getId() + " : " + c->getTitle()));
        QPushButton* btn = new QPushButton("Drop");
        btn->setProperty("cid", c->getId());
        row->addWidget(btn);
        m_layout->addLayout(row);
        connect(btn, &QPushButton::clicked, this, &DropCourseDialog::onDropClicked);
    }
    m_layout->addStretch();
    container->setLayout(m_layout);
    scroll->setWidget(container);
    scroll->setWidgetResizable(true);

    QVBoxLayout* main = new QVBoxLayout(this);
    main->addWidget(scroll);
}
void DropCourseDialog::onDropClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    QString cid = btn->property("cid").toString();

    m_student->removeCourse(cid);
    QMessageBox::information(this,"Dropped","Course " + cid + " dropped");

    btn->setEnabled(false);
}
/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
