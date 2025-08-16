#include "studentlogin.h"
#include "studentpanel.h"   // will open upon success

#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

StudentLoginDialog::StudentLoginDialog(StudentList* students, Course** courseHead, QWidget* parent)
    : QDialog(parent), m_students(students), m_courseHead(courseHead)
{
    setWindowTitle("Student Login");
    setMinimumSize(300,150);

    auto form = new QFormLayout(this);
    idEdit   = new QLineEdit; form->addRow("ID:",   idEdit);
    passEdit = new QLineEdit; passEdit->setEchoMode(QLineEdit::Password);
    form->addRow("Password:", passEdit);

    loginBtn = new QPushButton("Login");
    form->addRow("", loginBtn);

    connect(loginBtn, &QPushButton::clicked, this, &StudentLoginDialog::attemptLogin);
}

void StudentLoginDialog::attemptLogin()
{
    QString id   = idEdit->text().trimmed();
    QString pass = passEdit->text();
    if (id.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this,"Login","Enter both ID and password");
        return;
    }
    Student* s = m_students->findById(id);
    if (!s || s->getPassword() != pass) {
        QMessageBox::warning(this,"Login","Invalid credentials");
        return;
    }

    // success → open panel
    accept();   // close login dialog
    StudentPanelDialog panel(s, m_courseHead, parentWidget());
    panel.exec();
}
