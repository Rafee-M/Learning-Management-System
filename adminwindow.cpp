/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
#include "adminwindow.h"
#include "teacher.h"
#include "teacherlist.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>

#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>


// helper functions for larger dialogs
static QString largeGetText(QWidget* parent,
                            const QString& title,
                            const QString& label,
                            bool password = false)
{
    QInputDialog dlg(parent);
    dlg.setWindowTitle(title);
    dlg.setLabelText(label);
    if (password) dlg.setTextEchoMode(QLineEdit::Password);
    dlg.setOption(QInputDialog::NoButtons, false);
    dlg.setMinimumSize(400, 200);
    dlg.resize(500, 250);
    return (dlg.exec() == QDialog::Accepted) ? dlg.textValue() : QString();
}

static int largeGetInt(QWidget* parent,
                       const QString& title,
                       const QString& label,
                       int value, int min, int max, int step, bool& ok)
{
    QInputDialog dlg(parent);
    dlg.setWindowTitle(title);
    dlg.setLabelText(label);
    dlg.setIntRange(min, max);
    dlg.setIntValue(value);
    dlg.setIntStep(step);
    dlg.setMinimumSize(400, 200);
    dlg.resize(500, 250);
    ok = (dlg.exec() == QDialog::Accepted);
    return dlg.intValue();
}

AdminWindow::AdminWindow(StudentList* students,
                         Course** courseHead,
                         TeacherList* teachers,
                         QWidget* parent)
    : QDialog(parent)
    , m_students(students)
    , m_courseHead(courseHead)
    , m_teachers(teachers)
{
    setWindowTitle("Admin Panel");
    setMinimumSize(600, 400);
    resize(700, 500);

    m_students->addStudent(new Student("sam",    "S01", "sam@x","123"));
    m_students->addStudent(new Student("john",   "J02", "john@x","123"));
    m_students->addStudent(new Student("mark",   "M03", "mark@x","123"));
    m_students->addStudent(new Student("henry",  "H04", "henry@x","123"));
    m_students->addStudent(new Student("selena", "S05", "selena@x","123"));

    // ── seed 5 courses ─────────────────────────────────────
    struct C { const char* id; const char* title; const char* ts; };
    C initCourses[] = {
        {"CSE101","Intro to CSE","Mon-9:00-11:00"},
        {"EEE102","Basic EEE",    "Tue-9:00-11:00"},
        {"BIO103","Fundamentals of Biology","Wed-9:00-11:00"},
        {"CHEM104","General Chemistry","Fri-9:00-11:00"},
        {"MATH105","Calculus I",  "Fri-9:00-11:00"}
    };
    for (auto &cc : initCourses) {
        Course* c = new Course(cc.id, cc.title, 3, 1, cc.ts);
        c->next = *m_courseHead;
        *m_courseHead = c;
    }

    // ── seed 5 teachers ────────────────────────────────────
    struct T { const char* id; const char* name; };
    T initTeachers[] = {
        {"T01","Alice"}, {"T02","Bob"}, {"T03","Carol"},
        {"T04","Dave"},  {"T05","Eve"}
    };
    for (auto &tt : initTeachers) {
        Teacher* t = new Teacher(tt.id, tt.name, "123");
        t->next = m_teachers->head;
        m_teachers->head = t;
    }

    // ── build buttons ──────────────────────────────────────
    auto v = new QVBoxLayout(this);
    struct Btn { const char* txt; void (AdminWindow::*slot)(); };
    Btn buttons[] = {
        {"Add Course",                &AdminWindow::addCourse},
        {"Remove Course",             &AdminWindow::removeCourse},
        {"Add Student",               &AdminWindow::addStudent},
        {"Remove Student",            &AdminWindow::removeStudent},
        //{"Drop Student from Course",  &AdminWindow::dropStudentFromCourse},
        {"View All Students",         &AdminWindow::viewAllStudents},
        {"View Enrolled Students",    &AdminWindow::viewEnrolledStudents},
        {"View All Courses",          &AdminWindow::viewAllCourses},
        {"Add Course to Student",     &AdminWindow::addCourseToStudent},
        {"Remove Student from Course",&AdminWindow::removeStudentFromCourse},
        {"Add Teacher",               &AdminWindow::addTeacher},
        {"Remove Teacher",            &AdminWindow::removeTeacher},
        {"Assign Course to Teacher",  &AdminWindow::assignCourseToTeacher},
        {"View Teacher Courses",      &AdminWindow::viewTeacherCourses},
        {"Save Data",                 &AdminWindow::saveAllData},
        {"Load Data",                 &AdminWindow::loadAllData},


        {"Update Course",             &AdminWindow::updateCourse},
        {"Update Student Info",       &AdminWindow::updateStudentInfo},
        {"Update Teacher Info",       &AdminWindow::updateTeacherInfo}
    };

    QSize btnSize(140,30);
    for (auto &b : buttons) {
        QPushButton* btn = new QPushButton(b.txt);
        btn->setFixedSize(btnSize);
        v->addWidget(btn, 0, Qt::AlignHCenter);
        connect(btn, &QPushButton::clicked, this, b.slot);
    }
}


void AdminWindow::showMessage(const QString& title, const QString& msg) {
    QMessageBox::information(this, title, msg);
}



void AdminWindow::updateCourse() {
    QString cid = largeGetText(this,"Update Course","Enter Course ID to update:");
    if (cid.isEmpty()) return;
    Course* c = nullptr;
    for (Course* p = *m_courseHead; p; p = p->next)
        if (p->getId() == cid) { c = p; break; }
    if (!c) { showMessage("Error","Course not found"); return; }

    // pre‑fill each field
    QString newId    = largeGetText(this,"Update Course","Course ID:", false).arg(c->getId());
    if (newId.isEmpty()) return;
    QString newTitle = largeGetText(this,"Update Course","Title:", false).arg(c->getTitle());
    if (newTitle.isEmpty()) return;
    QString newTS    = largeGetText(this,"Update Course","Time slot:", false).arg(c->getTimeSlot());

    bool ok;
    int newCH  = largeGetInt(this,"Update Course","Credit Hours:",   c->getCreditHour(), 0,100,1,ok);
    if (!ok) return;
    int newCap = largeGetInt(this,"Update Course","Max Capacity:",    c->getMaxCapacity(),0,500,1,ok);
    if (!ok) return;

    // apply changes
    c->setId(newId);
    c->setTitle(newTitle);
    c->setTimeSlot(newTS);
    c->setCreditHour(newCH);
    c->setMaxCapacity(newCap);

    showMessage("Update Course","Course updated.");
}

// — Update Student Info —
void AdminWindow::updateStudentInfo() {
    QString sid = largeGetText(this,"Update Student","Enter Student ID:");
    if (sid.isEmpty()) return;
    Student* s = m_students->findById(sid);
    if (!s) { showMessage("Error","Student not found"); return; }

    QString newName = largeGetText(this,"Update Student","Name:", false);
    if (newName.isEmpty()) return;
    QString newId   = largeGetText(this,"Update Student","ID:",   false);
    if (newId.isEmpty()) return;
    QString newEmail= largeGetText(this,"Update Student","Email:",false);
    if (newEmail.isEmpty()) return;
    QString newPass = largeGetText(this,"Update Student","Password:", true);
    if (newPass.isEmpty()) return;

    s->setName(newName);
    s->setId(newId);
    s->setEmail(newEmail);
    s->setPassword(newPass);

    showMessage("Update Student","Student info updated.");
}

// — Update Teacher Info —
void AdminWindow::updateTeacherInfo() {
    QString tid = largeGetText(this,"Update Teacher","Enter Teacher ID:");
    if (tid.isEmpty()) return;
    Teacher* t = m_teachers->findById(tid);
    if (!t) { showMessage("Error","Teacher not found"); return; }

    QString newId = largeGetText(this,"Update Teacher","Teacher ID:", false);
    if (newId.isEmpty()) return;
    QString newName = largeGetText(this, "Update Teacher", "Name:", false);
    if(newName.isEmpty()) return;
    QString newPass = largeGetText(this,"Update Teacher","Password:", true);
    if (newPass.isEmpty()) return;

    t->setName(newName);
    t->setId(newId);
    t->setPassword(newPass);

    showMessage("Update Teacher","Teacher info updated.");
}



// — Add Course —
void AdminWindow::addCourse() {
    QString id = largeGetText(this, "Add Course", "Course ID:");
    if (id.isEmpty()) return;
    for (Course* p = *m_courseHead; p; p = p->next)
        if (p->getId() == id) { showMessage("Error","Course exists"); return; }

    QString title = largeGetText(this, "Add Course", "Title:");
    if (title.isEmpty()) return;

    QString ts = largeGetText(this, "Add Course", "Time slot:");
    if (ts.isEmpty()) return;

    bool ok; //checks for user input or exit
    int ch  = largeGetInt(this, "Add Course", "Credit Hours:", 3, 0, 100, 1, ok);
    if (!ok) return;
    int cap = largeGetInt(this, "Add Course", "Max Capacity:", 30, 0, 500, 1, ok);
    if (!ok) return;

    Course* c = new Course(id, title, ch, cap, ts);
    c->next = *m_courseHead;
    *m_courseHead = c;
    showMessage("Add Course", "Course added.");
}

// — Remove Course —
void AdminWindow::removeCourse() {
    QString id = largeGetText(this, "Remove Course", "Course ID:");
    if (id.isEmpty()) return;

    Course* p = *m_courseHead; Course* prev = nullptr;
    while (p) {
        if (p->getId() == id) {
            if (prev) prev->next = p->next; else *m_courseHead = p->next;
            delete p;
            showMessage("Remove Course", "Deleted.");
            return;
        }
        prev = p; p = p->next;
    }
    showMessage("Remove Course", "Not found.");
}

// — Add Student —
void AdminWindow::addStudent() {
    QString n = largeGetText(this, "Add Student", "Name:");
    if (n.isEmpty()) return;
    QString i = largeGetText(this, "Add Student", "ID:");
    if (i.isEmpty()) return;
    if (m_students->findById(i)) { showMessage("Error","Student exists"); return; }

    QString e = largeGetText(this, "Add Student", "Email:");
    if (e.isEmpty()) return;
    QString p = largeGetText(this, "Add Student", "Password:", true);
    if (p.isEmpty()) return;

    m_students->addStudent(new Student(n,i,e,p));
    showMessage("Add Student", "Student added.");
}

// — Remove Student —
void AdminWindow::removeStudent() {
    QString sid = largeGetText(this, "Remove Student", "Student ID:");
    if (sid.isEmpty()) return;
    if (!m_students->findById(sid)) { showMessage("Error","No such student"); return; }
    m_students->removeStudent(sid);
    showMessage("Remove Student", "Done.");
}

// — Drop Student from Course —
void AdminWindow::dropStudentFromCourse() {
    QString sid = largeGetText(this, "Drop From Course", "Student ID:");
    if (sid.isEmpty()) return;
    Student* s = m_students->findById(sid);
    if (!s) { showMessage("Error","No such student"); return; }

    QString cid = largeGetText(this, "Drop From Course", "Course ID:");
    if (cid.isEmpty()) return;
    bool found=false;
    for (Course* c=s->courseHead; c; c=c->next) if (c->getId()==cid) { found=true; break; }
    if (!found) { showMessage("Error","Not enrolled"); return; }

    s->removeCourse(cid);
    showMessage("Drop From Course", "Done.");
}

// — View All Students —
void AdminWindow::viewAllStudents() {
    QString txt;
    for (Student* s=m_students->head; s; s=s->next)
        txt += s->getId() + " | " + s->getName() + "\n";
    showMessage("All Students", txt.isEmpty() ? "(none)" : txt);
}

// — View Enrolled Students —
void AdminWindow::viewEnrolledStudents() {
    QString cid = largeGetText(this, "View Enrolled", "Course ID:");
    if (cid.isEmpty()) return;
    Course* cp=nullptr;
    for (Course* c=*m_courseHead; c; c=c->next) if (c->getId()==cid) { cp=c; break; }
    if (!cp) { showMessage("Error","No such course"); return; }

    QString txt="ID | Name\n-------\n"; bool any=false;
    for (Student* s=m_students->head; s; s=s->next) {
        for (Course* c=s->courseHead; c; c=c->next)
            if (c->getId()==cid) { txt += s->getId()+" | "+s->getName()+"\n"; any=true; break; }
    }
    showMessage("Enrolled in "+cid, any?txt:"(none)");
}

// — View All Courses —
void AdminWindow::viewAllCourses() {
    QString txt;
    for (Course* c = *m_courseHead; c; c = c->next) {
        int count = 0;
        for (Student* s = m_students->head; s; s = s->next) {
            for (Course* sc = s->courseHead; sc; sc = sc->next) {
                if (sc->getId() == c->getId()) {
                    count++;
                    break;
                }
            }
        }
        txt += c->getId() + " | " + c->getTitle() + " | " + c->getTimeSlot() +
               " | Students Enrolled: " + QString::number(count) + "\n";
    }
    showMessage("All Courses", txt.isEmpty() ? "(none)" : txt);
}


// — Add Course to Student —
void AdminWindow::addCourseToStudent() {
    QString sid = largeGetText(this, "Enroll Student", "Student ID:");
    if (sid.isEmpty()) return;
    Student* s = m_students->findById(sid);
    if (!s) { showMessage("Error","No such student"); return; }

    QString cid = largeGetText(this, "Enroll Student", "Course ID:");
    if (cid.isEmpty()) return;
    Course* c=nullptr;
    for (Course* p=*m_courseHead; p; p=p->next) if (p->getId()==cid) { c=p; break; }
    if (!c) { showMessage("Error","No such course"); return; }

    try{
        s->addCourse(c);
    }
    catch (QString errorMessage){
        showMessage("Error", errorMessage);
        return;
    }

    showMessage("Enroll Student", "Done.");
}

// — Remove Student from Course —
void AdminWindow::removeStudentFromCourse() {
    QString sid = largeGetText(this, "Unenroll Student", "Student ID:");
    if (sid.isEmpty()) return;
    Student* s = m_students->findById(sid);
    if (!s) { showMessage("Error","No such student"); return; }

    QString cid = largeGetText(this, "Unenroll Student", "Course ID:");
    if (cid.isEmpty()) return;
    bool enrolled=false;
    for (Course* c=s->courseHead; c; c=c->next) if (c->getId()==cid) { enrolled=true; break; }
    if (!enrolled) { showMessage("Error","Not enrolled"); return; }

    s->removeCourse(cid);
    showMessage("Unenroll Student", "Done.");
}

// — Saving All Data to File —
void AdminWindow::saveAllData(){
    QString suggestedFileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation); //uses windows documents folder
    QString filename = QFileDialog::getSaveFileName(this, "Save All Data", suggestedFileName
                                                                               + "/university_data.csv", "CSV Files (*.csv);; All Files (*)");
    //check if file is writeable
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(this, "Error", "Could not open file for writing");
        return;
    }

    QTextStream out(&file);

    //Headers for CSV
    out << "RECORD_TYPE,ID,NAME,EMAIL,PASSWORD,COURSE_ID,COURSE_TITLE,CREDITS,CAPACITY,TIME_SLOT\n";

    //Saving Course info
    for(Course* c = *m_courseHead; c; c = c->next){
        out << "COURSE,,,,,"
            << c->getId() << ","
            << c->getTitle() << ","
            << c->getCreditHour() << ","
            << c->getMaxCapacity() << ","
            << c->getTimeSlot() << "\n";
    }

    //Saving Teachers info
    for(Teacher* t = m_teachers->head; t; t = t->next){
        out << "TEACHER, "
            << t->getId() << ","
            << t->getName() << ",,"
            << t->getPassword () << ",,,,,\n";

        //save each teacher assignment
        for(Course* tc = t->courseHead; tc ; tc = tc->next){
            out << "TEACHER_ASSIGNMENT,"
                << t->getId() << ",,,," //Teacher ID
                << tc->getId() << ",,,," << "\n"; //Course ID
        }
    }

    //Saving student info
    for (Student* s = m_students->head; s; s = s->next){
        out << "STUDENT,"
            << s->getId() << ","
            << s->getName() << ","
            << s->getEmail() << ","
            << s->getPassword() << ",,,,,,\n";

        //Save each enrollmnent
        for(Course* sc = s->courseHead ; sc; sc = sc->next){
            out << "ENROLLMENT,"
                << s->id << ",,,,"
                << sc->getId() << ",,,,\n";

        }
    }
}

// - Load All Data -
void AdminWindow::loadAllData(){

    // Show confirmation dialog
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Warning",
                                  "All current data will be deleted!\nDo you want to continue?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return; // User clicked No or closed the dialog
    }


    //File Dialog
    QString filename = QFileDialog::getOpenFileName(
        this, "Load All Data", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "CSV Files (*.csv);;All Files (*)");

    //no file
    if(filename.isEmpty()) return;

    //Open file
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, "Error", "Could not open file for reading");
    }

    // -- Deleting data --

    // Clear existing student data
    while(m_students -> head){ //all local and external link list head will point to nullptr
        Student* temp =  m_students -> head;
        m_students -> head = m_students->head->next;
        delete temp;
    }

    // Clear existing course data
    while(*m_courseHead){ //we dereferncing a double pointer, thus get access to original pointer
        Course* temp = *m_courseHead;
        *m_courseHead = (*m_courseHead) -> next;
        delete temp;
    }

    //Clear exisitng teacher data
    while(m_teachers -> head){
        Teacher* temp = m_teachers -> head;
        m_teachers -> head = m_teachers -> head -> next;
        delete temp;
    }

    QTextStream in(&file);
    QString header = in.readLine(); //skip header line

    while(!in.atEnd()){
        QString line = in.readLine();
        QStringList fields = line.split(",");

        if (fields[0] == "COURSE") {
            Course* c = new Course(
                fields[5], // courseID
                fields[6], // title
                fields[7].toInt(), // credits
                fields[8].toInt(), // capacity
                fields[9]  // timeSlot
                );
            c->next = *m_courseHead;
            *m_courseHead = c;
        }

        else if (fields[0] == "TEACHER") {
            Teacher* t = new Teacher(fields[1], fields[2], fields[4]); // ID and password
            t->next = m_teachers->head;
            m_teachers->head = t;
        }

        else if(fields[0] == "TEACHER_ASSIGNMENT"){

            for(Course* p = *m_courseHead; p; p = p->next){
                if((p -> getId()) == fields[5]){
                    m_teachers -> head -> assignCourse(p);
                    break;
                }
            }
        }

        else if (fields[0] == "STUDENT") {
            Student* s = new Student(
                fields[2], // name
                fields[1], // id
                fields[3], // email
                fields[4]  // password
                );
            m_students->addStudent(s);
        }

        else if(fields[0] == "ENROLLMENT"){

            for(Course* p = *m_courseHead; p; p = p->next){
                if((p -> getId()) == fields[5]){
                    m_students -> head -> addCourse(p);
                    break;
                }
            }
        }

    }

    file.close();
    QMessageBox::information(this, "Success", "All data loaded successfully");

}


// — Add Teacher —
void AdminWindow::addTeacher() {
    QString id = largeGetText(this,"Add Teacher","Teacher ID:");
    if (id.isEmpty()) return;
    if (m_teachers->findById(id)) { showMessage("Error","Teacher exists"); return; }
    QString n = largeGetText(this, "Add Teacher", "Name:");
    if (n.isEmpty()) return;
    QString pw = largeGetText(this,"Add Teacher","Password:", true);
    if (pw.isEmpty()) return;
    m_teachers->addTeacher(new Teacher(id, n, pw));
    showMessage("Add Teacher","Teacher added.");
}



// — Remove Teacher —
void AdminWindow::removeTeacher() {
    QString id = largeGetText(this,"Remove Teacher","Teacher ID:");
    if (id.isEmpty()) return;
    Teacher* p = m_teachers->head; Teacher* prev=nullptr;
    while(p) {
        if (p->getId()==id) {
            if(prev) prev->next=p->next; else m_teachers->head=p->next;
            delete p;
            showMessage("Remove Teacher","Teacher removed.");
            return;
        }
        prev=p; p=p->next;
    }
    showMessage("Remove Teacher","Not found.");
}


// — Assign Course to Teacher —
void AdminWindow::assignCourseToTeacher() {
    QString tid = largeGetText(this,"Assign to Teacher","Teacher ID:");
    if (tid.isEmpty()) return;
    Teacher* t = m_teachers->findById(tid);
    if (!t) { showMessage("Error","Teacher not found"); return; }

    QString cid = largeGetText(this,"Assign to Teacher","Course ID:");
    if (cid.isEmpty()) return;
    Course* c=nullptr;
    for (Course* p=*m_courseHead; p; p=p->next) if(p->getId()==cid){c=p;break;}
    if(!c){ showMessage("Error","Course not found"); return; }

    t->assignCourse(c);
    showMessage("Assign Course","Done.");
}

// — View Teacher Courses —
void AdminWindow::viewTeacherCourses() {
    QString tid = largeGetText(this,"View Teacher Courses","Teacher ID:");
    if (tid.isEmpty()) return;
    Teacher* t = m_teachers->findById(tid);
    if (!t) { showMessage("Error","Teacher not found"); return; }

    QString out; t->viewCourses(out);
    showMessage("Courses of "+tid, out.isEmpty()? "(none)" : out);
}
/**
 * Developed by Rafee-M
 * Repo: https://github.com/Rafee-M/Learning-Management-System
 * GPL-3.0 license
 **/
