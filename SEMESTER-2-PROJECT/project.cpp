#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// TRANSCRIPT
class TR {
private:
    float sgpa[8];

public:

    TR() {

        for (int i = 0; i < 8; i++) {
            sgpa[i] = -1;
        }
    }

    void addGPA(int sem, float g) {

        if (sem >= 1 && sem <= 8) {
            sgpa[sem - 1] = g;
        }
    }

    bool done() const {

        for (int i = 0; i < 8; i++) {

            if (sgpa[i] == -1)
                return false;
        }

        return true;
    }

    float cgpa() const {

        float t = 0;

        for (int i = 0; i < 8; i++) {
            t += sgpa[i];
        }

        return t / 8;
    }

    void show(string n, int id) const {

        if (!done()) {

            cout << "\nAll semesters not completed!\n";
            return;
        }

        cout << "\n=========================\n";
        cout << "      TRANSCRIPT\n";
        cout << "=========================\n";

        cout << "Name : " << n << endl;
        cout << "ID   : " << id << endl;

        for (int i = 0; i < 8; i++) {

            cout << "Semester "
                << i + 1
                << " GPA : "
                << sgpa[i]
                << endl;
        }

        cout << "\nCGPA : " << cgpa() << endl;
        cout << "=========================\n";
    }
};

// FINANCE

class FN {
private:
    bool paid;

public:

    FN(bool s = false) {
        paid = s;
    }

    void set(bool s) {
        paid = s;
    }

    bool get() const {
        return paid;
    }

    string status() const {

        if (paid)
            return "Paid";

        return "Unpaid";
    }
};


// PERSON

class PS {
protected:
    string name;
    int id;

public:

    PS(string n = "", int i = 0) {

        name = n;
        id = i;
    }

    virtual void show() const = 0;

    virtual string save() const = 0;

    int getID() const {
        return id;
    }

    virtual ~PS() {}
};

// ENROLLMENT

class EN {
private:
    int sem;

public:

    EN(int s = 1) {
        sem = s;
    }

    void next() {

        if (sem < 8) {

            sem++;

            cout << "\nEnrolled in Semester "
                << sem
                << endl;
        }

        else {

            cout << "\nDegree Completed!\n";
        }
    }

    void show() const {

        cout << "Current Semester : "
            << sem
            << endl;
    }
};


// STUDENT

class ST : public PS {
private:
    float gpa;

    FN fee;

    TR tr;

    EN en;

public:

    ST(string n = "", int i = 0,
        float g = 0.0,
        bool f = false)

        : PS(n, i), fee(f) {

        gpa = g;
    }

    void calc(float q, float a,
        float p, float m,
        float f) {

        float avg =
            (q + a + p + m + f) / 5;

        if (avg >= 90)
            gpa = 4.0;

        else if (avg >= 80)
            gpa = 3.7;

        else if (avg >= 70)
            gpa = 3.0;

        else if (avg >= 60)
            gpa = 2.0;

        else if (avg >= 50)
            gpa = 1.0;

        else
            gpa = 0.0;
    }

    void setFee(bool s) {
        fee.set(s);
    }

    void show() const override {

        cout << "\n======= STUDENT =======\n";

        cout << "ID    : " << id << endl;
        cout << "Name  : " << name << endl;
        cout << "GPA   : " << gpa << endl;

        cout << "Fee   : "
            << fee.status()
            << endl;

        en.show();

        cout << "=======================\n";
    }

    string save() const override {

        return "S," +
            to_string(id) + "," +
            name + "," +
            to_string(gpa) + "," +
            (fee.get() ? "1" : "0");
    }

    void nextSem() {
        en.next();
    }

    void addRes(int s) {
        tr.addGPA(s, gpa);
    }

    void printTR() const {
        tr.show(name, id);
    }
};

// PROFESSOR

class PR : public PS {
private:
    string dept;

public:

    PR(string n = "",
        int i = 0,
        string d = "")

        : PS(n, i) {

        dept = d;
    }

    void show() const override {

        cout << "\n====== PROFESSOR ======\n";

        cout << "ID    : " << id << endl;
        cout << "Name  : " << name << endl;
        cout << "Dept  : " << dept << endl;

        cout << "=======================\n";
    }

    string save() const override {

        return "P," +
            to_string(id) + "," +
            name + "," +
            dept;
    }
};

// MANAGER

class UM {
private:

    vector<PS*> rec;

    string file = "university_data.txt";

    string token(string& line) {

        int p = line.find(',');

        if (p == string::npos) {

            string t = line;

            line = "";

            return t;
        }

        string t = line.substr(0, p);

        line.erase(0, p + 1);

        return t;
    }

public:

    ~UM() {

        for (int i = 0; i < rec.size(); i++) {
            delete rec[i];
        }
    }

    bool exist(int id) {

        for (int i = 0; i < rec.size(); i++) {

            if (rec[i]->getID() == id)
                return true;
        }

        return false;
    }

    void addStu() {

        string n;

        int id;

        cin.ignore();

        cout << "Enter Name: ";
        getline(cin, n);

        cout << "Enter ID: ";
        cin >> id;

        if (exist(id)) {

            cout << "ID Already Exists!\n";
            return;
        }

        ST* s = new ST(n, id);

        rec.push_back(s);

        cout << "Student Added!\n";
    }

    void addPro() {

        string n, d;

        int id;

        cin.ignore();

        cout << "Enter Name: ";
        getline(cin, n);

        cout << "Enter ID: ";
        cin >> id;

        if (exist(id)) {

            cout << "ID Already Exists!\n";
            return;
        }

        cin.ignore();

        cout << "Enter Department: ";
        getline(cin, d);

        PR* p = new PR(n, id, d);

        rec.push_back(p);

        cout << "Professor Added!\n";
    }

    void grades() {

        int id, sem;

        float q, a, p, m, f;

        cout << "Enter Student ID: ";
        cin >> id;

        bool ok = false;

        for (int i = 0; i < rec.size(); i++) {

            ST* s = dynamic_cast<ST*>(rec[i]);

            if (s != NULL &&
                s->getID() == id) {

                cout << "Semester: ";
                cin >> sem;

                cout << "Quiz: ";
                cin >> q;

                cout << "Assignment: ";
                cin >> a;

                cout << "Project: ";
                cin >> p;

                cout << "Mid: ";
                cin >> m;

                cout << "Final: ";
                cin >> f;

                s->calc(q, a, p, m, f);

                s->addRes(sem);

                cout << "GPA Updated!\n";

                ok = true;
            }
        }

        if (!ok) {
            cout << "Student Not Found!\n";
        }
    }

    void fees() {

        int id;

        char ch;

        cout << "Enter Student ID: ";
        cin >> id;

        bool ok = false;

        for (int i = 0; i < rec.size(); i++) {

            ST* s = dynamic_cast<ST*>(rec[i]);

            if (s != NULL &&
                s->getID() == id) {

                cout << "Paid? (y/n): ";
                cin >> ch;

                if (ch == 'y' || ch == 'Y')
                    s->setFee(true);

                else
                    s->setFee(false);

                cout << "Fee Updated!\n";

                ok = true;

                break;
            }
        }

        if (!ok) {
            cout << "Student Not Found!\n";
        }
    }

    void showAll() {

        if (rec.size() == 0) {

            cout << "No Records!\n";
            return;
        }

        for (int i = 0; i < rec.size(); i++) {
            rec[i]->show();
        }
    }

    void saveFile() {

        ofstream out(file);

        if (!out) {

            cout << "File Error!\n";
            return;
        }

        for (int i = 0; i < rec.size(); i++) {

            out << rec[i]->save()
                << endl;
        }

        out.close();

        cout << "Saved!\n";
    }

    void loadFile() {

        ifstream in(file);

        if (!in)
            return;

        string line;

        while (getline(in, line)) {

            if (line == "")
                continue;

            string t = token(line);

            string id = token(line);

            string n = token(line);

            string ex = token(line);

            if (t == "S") {

                string fs = token(line);

                ST* s =
                    new ST(n,
                        stoi(id),
                        stof(ex),
                        fs == "1");

                if (!exist(s->getID()))
                    rec.push_back(s);

                else
                    delete s;
            }

            else if (t == "P") {

                PR* p =
                    new PR(n,
                        stoi(id),
                        ex);

                if (!exist(p->getID()))
                    rec.push_back(p);

                else
                    delete p;
            }
        }

        in.close();
    }

    void printTR() {

        int id;

        cout << "Enter Student ID: ";
        cin >> id;

        bool ok = false;

        for (int i = 0; i < rec.size(); i++) {

            ST* s =
                dynamic_cast<ST*>(rec[i]);

            if (s != NULL &&
                s->getID() == id) {

                s->printTR();

                ok = true;

                break;
            }
        }

        if (!ok) {
            cout << "Student Not Found!\n";
        }
    }

    void enroll() {

        int id;

        cout << "Enter Student ID: ";
        cin >> id;

        bool ok = false;

        for (int i = 0; i < rec.size(); i++) {

            ST* s =
                dynamic_cast<ST*>(rec[i]);

            if (s != NULL &&
                s->getID() == id) {

                s->nextSem();

                ok = true;

                break;
            }
        }

        if (!ok) {
            cout << "Student Not Found!\n";
        }
    }
};

// MAIN

int main() {

    UM m;

    m.loadFile();

    int ch;

    do {

        cout << "\n========================\n";
        cout << " UNIVERSITY SYSTEM\n";
        cout << "========================\n";

        cout << "1.  Add Student\n";
        cout << "2.  Add Professor\n";
        cout << "3.  Enter Grades\n";
        cout << "4.  Update Fees\n";
        cout << "5.  Show All\n";
        cout << "6.  Save Data\n";
        cout << "7.  Print Transcript\n";
        cout << "8.  subject details\n";
        cout << "9.  Enroll Semester\n";
        cout << "10. Exit\n";

        cout << "\nChoice: ";
        cin >> ch;

        switch (ch) {

        case 1:
            m.addStu();
            break;

        case 2:
            m.addPro();
            break;

        case 3:
            m.grades();
            break;

        case 4:
            m.fees();
            break;

        case 5:
            m.showAll();
            break;

        case 6:
            m.saveFile();
            break;

        case 7:
            m.printTR();
            break;

        case 8: 
            cout << "\n=========================================\n"; 
            cout << " BS COMPUTER SCIENCE SEMESTER PLAN\n"; 
            cout << "=========================================\n"; 
            cout << "\nSemester 1\n"; cout << "1. Programming Fundamentals\n"; 
            cout << "2. Calculus\n"; 
            cout << "3. English Composition\n"; 
            cout << "4. ICT\n"; 
            cout << "5. Applied Physics\n"; 
            cout << "Semester Fee : 85,000 PKR\n"; 
            cout << "\nSemester 2\n"; 
            cout << "1. Object Oriented Programming\n"; 
            cout << "2. Discrete Structures\n"; 
            cout << "3. Communication Skills\n"; 
            cout << "4. Digital Logic Design\n"; 
            cout << "5. Linear Algebra\n"; 
            cout << "Semester Fee : 88,000 PKR\n"; 
            cout << "\nSemester 3\n"; 
            cout << "1. Data Structures\n"; 
            cout << "2. Computer Organization\n"; 
            cout << "3. Differential Equations\n"; 
            cout << "4. Database Systems\n"; 
            cout << "5. Islamic Studies\n"; 
            cout << "Semester Fee : 90,000 PKR\n"; 
            cout << "\nSemester 4\n"; 
            cout << "1. Operating Systems\n"; 
            cout << "2. Design and Analysis of Algorithms\n"; 
            cout << "3. Software Engineering\n";
            cout << "4. Probability & Statistics\n";
            cout << "5. Pakistan Studies\n";
            cout << "Semester Fee : 92,000 PKR\n"; 
            cout << "\nSemester 5\n"; 
            cout << "1. Computer Networks\n"; 
            cout << "2. Web Engineering\n"; 
            cout << "3. Artificial Intelligence\n"; 
            cout << "4. Theory of Automata\n";
            cout << "5. Technical Writing\n"; 
            cout << "Semester Fee : 95,000 PKR\n"; 
            cout << "\nSemester 6\n"; 
            cout << "1. Mobile Application Development\n"; 
            cout << "2. Compiler Construction\n"; 
            cout << "3. Information Security\n"; 
            cout << "4. Human Computer Interaction\n"; 
            cout << "5. Entrepreneurship\n";
            cout << "Semester Fee : 98,000 PKR\n"; 
            cout << "\nSemester 7\n"; 
            cout << "1. Final Year Project-I\n"; 
            cout << "2. Machine Learning\n"; 
            cout << "3. Cloud Computing\n"; 
            cout << "4. Data Mining\n";
            cout << "5. Professional Practices\n"; 
            cout << "Semester Fee : 100,000 PKR\n"; 
            cout << "\nSemester 8\n"; 
            cout << "1. Final Year Project-II\n"; 
            cout << "2. Deep Learning\n"; 
            cout << "3. Big Data Analytics\n"; 
            cout << "4. Cyber Security\n"; 
            cout << "5. Internship\n"; 
            cout << "Semester Fee : 105,000 PKR\n"; 
            cout << "\n=========================================\n"; 
        break;
        
        case 9:
            m.enroll();
            break;

        case 10:

            m.saveFile();

            cout << "Program Closed!\n";

            break;

        default:
            cout << "Invalid Choice!\n";
        }

    } while (ch != 10);

    return 0;
}