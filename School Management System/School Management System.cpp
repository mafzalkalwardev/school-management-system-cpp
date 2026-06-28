#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <windows.h>

using namespace std;

// ========== UTILITY FUNCTIONS ==========
void clearScreen() {
    system("cls");
}

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void printHeader(string title) {
    clearScreen();
    setColor(11);
    cout << "\n";
    cout << "";
    setColor(14);
    cout << "              SCHOOL MANAGEMENT SYSTEM               ";
    setColor(11);
    cout << "\n";
    cout << "\n";
    setColor(7);
}

void pressAnyKey() {
    setColor(8);
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

// ========== DATA STRUCTURES ==========
struct Subject {
    string name;
    string code;
    Subject* next;

    Subject(string n, string c) : name(n), code(c), next(nullptr) {}
};

struct Student {
    string name;
    int id;
    float fee;
    int attendance;
    Subject* subjects;
    Student* next;

    Student(string n, int i, float f, int a) :
        name(n), id(i), fee(f), attendance(a), subjects(nullptr), next(nullptr) {
    }
};

struct Teacher {
    string name;
    int id;
    string subject;
    Teacher* next;

    Teacher(string n, int i, string s) :
        name(n), id(i), subject(s), next(nullptr) {
    }
};

// ========== SCHOOL MANAGEMENT SYSTEM CLASS ==========
class SchoolManagementSystem {
private:
    Student* studentHead;
    Teacher* teacherHead;
    int studentCount;
    int teacherCount;

public:
    SchoolManagementSystem() : studentHead(nullptr), teacherHead(nullptr),
        studentCount(0), teacherCount(0) {
        loadAllData();  // Load data when program starts
    }

    // ========== FILE HANDLING FUNCTIONS ==========
    void saveAllData() {
        // Save students to file
        ofstream studentFile("students.txt");
        Student* s = studentHead;
        while (s != nullptr) {
            studentFile << s->name << "," << s->id << "," 
                       << s->fee << "," << s->attendance << "\n";
            s = s->next;
        }
        studentFile.close();
        
        // Save teachers to file  
        ofstream teacherFile("teachers.txt");
        Teacher* t = teacherHead;
        while (t != nullptr) {
            teacherFile << t->name << "," << t->id << "," 
                       << t->subject << "\n";
            t = t->next;
        }
        teacherFile.close();
        
        setColor(10);
        cout << "✓ All data saved to files!\n";
        setColor(7);
    }
    
    void loadAllData() {
        // Load students from file
        ifstream studentFile("students.txt");
        if (studentFile.is_open()) {
            string line;
            while (getline(studentFile, line)) {
                int pos1 = line.find(',');
                int pos2 = line.find(',', pos1 + 1);
                int pos3 = line.find(',', pos2 + 1);
                
                if (pos1 != -1) {
                    string name = line.substr(0, pos1);
                    int id = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
                    float fee = stof(line.substr(pos2 + 1, pos3 - pos2 - 1));
                    int attendance = stoi(line.substr(pos3 + 1));
                    
                    // Add to linked list
                    Student* newStudent = new Student(name, id, fee, attendance);
                    
                    if (studentHead == nullptr) {
                        studentHead = newStudent;
                    } else {
                        Student* temp = studentHead;
                        while (temp->next != nullptr) {
                            temp = temp->next;
                        }
                        temp->next = newStudent;
                    }
                    studentCount++;
                }
            }
            studentFile.close();
        }
        
        // Load teachers from file
        ifstream teacherFile("teachers.txt");
        if (teacherFile.is_open()) {
            string line;
            while (getline(teacherFile, line)) {
                int pos1 = line.find(',');
                int pos2 = line.find(',', pos1 + 1);
                
                if (pos1 != -1) {
                    string name = line.substr(0, pos1);
                    int id = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
                    string subject = line.substr(pos2 + 1);
                    
                    Teacher* newTeacher = new Teacher(name, id, subject);
                    
                    if (teacherHead == nullptr) {
                        teacherHead = newTeacher;
                    } else {
                        Teacher* temp = teacherHead;
                        while (temp->next != nullptr) {
                            temp = temp->next;
                        }
                        temp->next = newTeacher;
                    }
                    teacherCount++;
                }
            }
            teacherFile.close();
        }
    }

    // ========== STUDENT OPERATIONS ==========
    void addStudent(string name, int id, float fee, int attendance) {
        // Check if student ID already exists
        Student* check = studentHead;
        while (check != nullptr) {
            if (check->id == id) {
                setColor(12);
                cout << "Error: Student ID already exists!\n";
                setColor(7);
                return;
            }
            check = check->next;
        }

        Student* newStudent = new Student(name, id, fee, attendance);

        if (studentHead == nullptr) {
            studentHead = newStudent;
        }
        else {
            Student* temp = studentHead;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newStudent;
        }
        studentCount++;

        setColor(10);
        cout << "Student '" << name << "' added successfully!\n";
        setColor(7);
        
        saveAllData();  // Save to file
    }

    void addSubjectToStudent(int studentId, string subjectName, string subjectCode) {
        Student* temp = studentHead;
        while (temp != nullptr) {
            if (temp->id == studentId) {
                Subject* newSubject = new Subject(subjectName, subjectCode);

                if (temp->subjects == nullptr) {
                    temp->subjects = newSubject;
                }
                else {
                    Subject* subTemp = temp->subjects;
                    while (subTemp->next != nullptr) {
                        subTemp = subTemp->next;
                    }
                    subTemp->next = newSubject;
                }

                setColor(10);
                cout << "Subject '" << subjectName << "' added to student!\n";
                setColor(7);
                return;
            }
            temp = temp->next;
        }
        setColor(12);
        cout << "Student not found!\n";
        setColor(7);
    }

    void displayStudents() {
        printHeader("STUDENT LIST");

        if (studentHead == nullptr) {
            setColor(12);
            cout << "No students in the system!\n";
            setColor(7);
            return;
        }

        cout << "┌────┬────────────────────┬────────────┬──────────┬──────────────┐\n";
        cout << "│ No │        Name        │     ID     │   Fee    │ Attendance % │\n";
        cout << "├────┼────────────────────┼────────────┼──────────┼──────────────┤\n";

        Student* temp = studentHead;
        int count = 1;
        while (temp != nullptr) {
            setColor(15);
            cout << "│ " << setw(2) << count << " │ ";
            setColor(11);
            cout << setw(18) << temp->name << " │ ";
            setColor(14);
            cout << setw(10) << temp->id << " │ $";
            setColor(10);
            cout << setw(7) << fixed << setprecision(2) << temp->fee << " │ ";
            setColor(13);
            cout << setw(12) << temp->attendance << " │\n";
            setColor(7);

            temp = temp->next;
            count++;
        }

        cout << "└────┴────────────────────┴────────────┴──────────┴──────────────┘\n";
    }

    void searchStudent(int id) {
        printHeader("SEARCH STUDENT");

        Student* temp = studentHead;
        while (temp != nullptr) {
            if (temp->id == id) {
                setColor(10);
                cout << "STUDENT FOUND\n\n";
                setColor(7);

                cout << "Name       : " << temp->name << endl;
                cout << "ID         : " << temp->id << endl;
                cout << "Fee        : $" << temp->fee << endl;
                cout << "Attendance : " << temp->attendance << "%\n";

                cout << "\nSubjects:\n";
                Subject* subTemp = temp->subjects;
                if (subTemp == nullptr) {
                    cout << "No subjects assigned\n";
                }
                else {
                    while (subTemp != nullptr) {
                        cout << "• " << subTemp->name << " (" << subTemp->code << ")\n";
                        subTemp = subTemp->next;
                    }
                }
                return;
            }
            temp = temp->next;
        }

        setColor(12);
        cout << "Student not found!\n";
        setColor(7);
    }

    void deleteStudent(int id) {
        if (studentHead == nullptr) {
            setColor(12);
            cout << "No students to delete!\n";
            setColor(7);
            return;
        }

        Student* temp = studentHead;
        Student* prev = nullptr;

        // If head needs to be deleted
        if (temp != nullptr && temp->id == id) {
            studentHead = temp->next;
            delete temp;
            studentCount--;
            setColor(10);
            cout << "Student deleted successfully!\n";
            setColor(7);
            saveAllData();  // Save to file
            return;
        }

        // Search for student to delete
        while (temp != nullptr && temp->id != id) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) {
            setColor(12);
            cout << "Student not found!\n";
            setColor(7);
            return;
        }

        // Unlink the node
        prev->next = temp->next;
        delete temp;
        studentCount--;

        setColor(10);
        cout << "Student deleted successfully!\n";
        setColor(7);
        saveAllData();  // Save to file
    }

    // ========== TEACHER OPERATIONS ==========
    void addTeacher(string name, int id, string subject) {
        // Check if teacher ID already exists
        Teacher* check = teacherHead;
        while (check != nullptr) {
            if (check->id == id) {
                setColor(12);
                cout << "Error: Teacher ID already exists!\n";
                setColor(7);
                return;
            }
            check = check->next;
        }

        Teacher* newTeacher = new Teacher(name, id, subject);

        if (teacherHead == nullptr) {
            teacherHead = newTeacher;
        }
        else {
            Teacher* temp = teacherHead;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newTeacher;
        }
        teacherCount++;

        setColor(10);
        cout << "Teacher '" << name << "' added successfully!\n";
        setColor(7);
        
        saveAllData();  // Save to file
    }

    void displayTeachers() {
        printHeader("TEACHER LIST");

        if (teacherHead == nullptr) {
            setColor(12);
            cout << "No teachers in the system!\n";
            setColor(7);
            return;
        }

        cout << "┌────┬────────────────────┬────────────┬─────────────────────┐\n";
        cout << "│ No │        Name        │     ID     │       Subject       │\n";
        cout << "├────┼────────────────────┼────────────┼─────────────────────┤\n";

        Teacher* temp = teacherHead;
        int count = 1;
        while (temp != nullptr) {
            setColor(15);
            cout << "│ " << setw(2) << count << " │ ";
            setColor(11);
            cout << setw(18) << temp->name << " │ ";
            setColor(14);
            cout << setw(10) << temp->id << " │ ";
            setColor(10);
            cout << setw(19) << temp->subject << " │\n";
            setColor(7);

            temp = temp->next;
            count++;
        }

        cout << "└────┴────────────────────┴────────────┴─────────────────────┘\n";
    }

    void searchTeacher(int id) {
        printHeader("SEARCH TEACHER");

        Teacher* temp = teacherHead;
        while (temp != nullptr) {
            if (temp->id == id) {
                setColor(10);
                cout << "TEACHER FOUND\n\n";
                setColor(7);
                cout << "Name    : " << temp->name << endl;
                cout << "ID      : " << temp->id << endl;
                cout << "Subject : " << temp->subject << endl;
                return;
            }
            temp = temp->next;
        }

        setColor(12);
        cout << "Teacher not found!\n";
        setColor(7);
    }

    // ========== MENU SYSTEM ==========
    void showMenu() {
        int choice;
        do {
            printHeader("MAIN MENU");

            setColor(14);
            cout << "\n";
            cout << "                          MAIN MENU                            \n";
            cout << "\n";
            setColor(11);
            cout << "  1. Add New Student                   6. Search Student        \n";
            cout << "  2. Add New Teacher                   7. Search Teacher        \n";
            cout << "  3. Add Subject to Student            8. Delete Student        \n";
            cout << "  4. View All Students                 9. View All Teachers     \n";
            cout << "  10. Save Data                        0. Exit                  \n";
            setColor(14);
            cout << "\n\n";

            setColor(15);
            cout << "Enter your choice (0-10): ";
            setColor(7);
            cin >> choice;

            switch (choice) {
            case 1: {
                printHeader("ADD NEW STUDENT");
                string name;
                int id, attendance;
                float fee;
                cout << "Enter Student Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Student ID: ";
                cin >> id;
                cout << "Enter Fee: $";
                cin >> fee;
                cout << "Enter Attendance (%): ";
                cin >> attendance;
                addStudent(name, id, fee, attendance);
                pressAnyKey();
                break;
            }
            case 2: {
                printHeader("ADD NEW TEACHER");
                string name, subject;
                int id;
                cout << "Enter Teacher Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Teacher ID: ";
                cin >> id;
                cout << "Enter Subject: ";
                cin.ignore();
                getline(cin, subject);
                addTeacher(name, id, subject);
                pressAnyKey();
                break;
            }
            case 3: {
                printHeader("ADD SUBJECT TO STUDENT");
                int studentId;
                string subjectName, subjectCode;
                cout << "Enter Student ID: ";
                cin >> studentId;
                cout << "Enter Subject Name: ";
                cin.ignore();
                getline(cin, subjectName);
                cout << "Enter Subject Code: ";
                getline(cin, subjectCode);
                addSubjectToStudent(studentId, subjectName, subjectCode);
                pressAnyKey();
                break;
            }
            case 4:
                displayStudents();
                pressAnyKey();
                break;
            case 6: {
                printHeader("SEARCH STUDENT");
                int id;
                cout << "Enter Student ID to search: ";
                cin >> id;
                searchStudent(id);
                pressAnyKey();
                break;
            }
            case 7: {
                printHeader("SEARCH TEACHER");
                int id;
                cout << "Enter Teacher ID to search: ";
                cin >> id;
                searchTeacher(id);
                pressAnyKey();
                break;
            }
            case 8: {
                printHeader("DELETE STUDENT");
                int id;
                cout << "Enter Student ID to delete: ";
                cin >> id;
                deleteStudent(id);
                pressAnyKey();
                break;
            }
            case 9:
                displayTeachers();
                pressAnyKey();
                break;
            case 10: {
                printHeader("SAVE DATA");
                saveAllData();
                pressAnyKey();
                break;
            }
            case 0:
                printHeader("EXIT");
                setColor(14);
                saveAllData();  // Save before exit
                cout << "Thank you for using School Management System!\n";
                setColor(7);
                break;
            default:
                setColor(12);
                cout << "Invalid choice! Please try again.\n";
                setColor(7);
                pressAnyKey();
            }
        } while (choice != 0);
    }

    // ========== DESTRUCTOR ==========
    ~SchoolManagementSystem() {
        // Free student memory
        Student* tempStudent = studentHead;
        while (tempStudent != nullptr) {
            Student* nextStudent = tempStudent->next;

            // Free subject memory for each student
            Subject* tempSubject = tempStudent->subjects;
            while (tempSubject != nullptr) {
                Subject* nextSubject = tempSubject->next;
                delete tempSubject;
                tempSubject = nextSubject;
            }

            delete tempStudent;
            tempStudent = nextStudent;
        }

        // Free teacher memory
        Teacher* tempTeacher = teacherHead;
        while (tempTeacher != nullptr) {
            Teacher* nextTeacher = tempTeacher->next;
            delete tempTeacher;
            tempTeacher = nextTeacher;
        }
    }
};

// ========== MAIN FUNCTION ==========
int main() {
    // Enable UTF-8 for Windows console
    system("chcp 65001 > nul");

    SchoolManagementSystem school;

    printHeader("WELCOME");

    setColor(14);
    cout << "\n";
    cout << "            SCHOOL MANAGEMENT SYSTEM - DSA PROJECT             \n";
    cout << "                Using Single Linked List                        \n";
    cout << "\n\n";
    setColor(7);

    cout << "Features:\n";
    cout << "1. Student Management (Add, View, Search, Delete)\n";
    cout << "2. Teacher Management\n";
    cout << "3. Subject Assignment\n";
    cout << "4. File Handling (Auto-save & Load)\n\n";

    // Start the menu system
    school.showMenu();

    return 0;
}