#include "studentpanel.h"
#include "addcoursedialog.h"
#include "dropcoursedialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QMessageBox>
#include <QTextEdit>

StudentPanelDialog::StudentPanelDialog(Student* student, Course** courseHead, QWidget* parent)
    : QDialog(parent), m_student(student), m_courseHead(courseHead)
{
    setWindowTitle("Student Panel: " + student->getId());
    setMinimumSize(500, 200);

    QSize btnSize(200, 50);
    viewBtn       = new QPushButton("View My Courses");  viewBtn->setFixedSize(btnSize);
    addCourseBtn  = new QPushButton("Add Course");       addCourseBtn->setFixedSize(btnSize);
    dropCourseBtn = new QPushButton("Drop Course");      dropCourseBtn->setFixedSize(btnSize);

    // — vertical layout of buttons —
    auto v = new QVBoxLayout(this);
    v->addStretch(1);
    v->addWidget(viewBtn,       0, Qt::AlignHCenter);
    v->addSpacing(20);
    v->addWidget(addCourseBtn,  0, Qt::AlignHCenter);
    v->addSpacing(20);
    v->addWidget(dropCourseBtn, 0, Qt::AlignHCenter);
    v->addStretch(2);

    // Connect
    connect(viewBtn,      &QPushButton::clicked, this, &StudentPanelDialog::viewMyCourses);
    connect(addCourseBtn, &QPushButton::clicked, this, &StudentPanelDialog::addCourse);
    connect(dropCourseBtn,&QPushButton::clicked, this, &StudentPanelDialog::dropCourse);
}

void StudentPanelDialog::viewMyCourses()
{
    if (!m_student) {
        QMessageBox::warning(this, "Error", "No student data available.");
        return;
    }

    QString html = "<table width=\"100%\" border=\"1\" style=\"border-collapse:collapse;\">"
                   "<tr><th>ID</th><th>Title</th><th>Time</th><th>Credits</th></tr>";

    Course* head = m_student->courseHead;
    if (head) {
        for (Course* c = head; c; c = c->next) {
            html += "<tr>"
                    "<td>" + c->getId() + "</td>"
                                   "<td>" + c->getTitle() + "</td>"
                                      "<td>" + c->getTimeSlot() + "</td>"
                                         "<td align=\"center\">" + QString::number(c->getCreditHour()) + "</td>"
                                                            "</tr>";
        }
    } else {
        html += "<tr><td colspan=\"4\" align=\"center\">No courses enrolled</td></tr>";
    }
    html += "</table>";

    QDialog dialog(this);
    dialog.setWindowTitle("My Courses");
    dialog.setMinimumSize(600, 400);

    auto layout = new QVBoxLayout(&dialog);
    QTextEdit* textEdit = new QTextEdit(&dialog);
    textEdit->setReadOnly(true);
    textEdit->setHtml(html);
    layout->addWidget(textEdit);

    dialog.exec();
}

void StudentPanelDialog::addCourse()
{
    AddCourseDialog dlg(m_student, m_courseHead, this);
    dlg.exec();
}

void StudentPanelDialog::dropCourse()
{
    DropCourseDialog dlg(m_student, m_courseHead, this);
    dlg.exec();
}
