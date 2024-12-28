#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct Account {
    int ID;
    string email;
    string password;
    string name;
};

struct StudentAcc {
    int ID;
    string email;
    string password;
    string name;
    char attendance;

    StudentAcc* next;
};

class AdminSystem {
    vector<Account> adminAccount;
    string adminFileAcc = "adminAccount.csv";
    string studentFileAcc = "studentAcoount.csv";
    string attendanceFile = "attendance.csv";
    StudentAcc* head;
    StudentAcc* tail;

    void saveAdminToFile() {
        fstream file;
        file.open(adminFileAcc, ios::app);
        for(int i = 0; i < adminAccount.size(); i++) {
            file << adminAccount[i].ID << "," << adminAccount[i].name << "," << adminAccount[i].email << "," << adminAccount[i].password << endl;
        }
        file.close();
    }

    void loadAdminFromFile() {
        fstream file;
        file.open(adminFileAcc, ios::in);
        if(!file.is_open()) {
            cout << "File not found!" <<endl;
        }
        string line;
        getline(file, line);
        while(getline(file, line)) {
            stringstream ss(line);
            int ID;
            string name, email, password , temp_id;
            getline(ss , temp_id , ',');
            ID = stoi(temp_id);
            ss.ignore();
            getline(ss, name, ',');
            getline(ss, email, ',');
            getline(ss, password, ',');
            adminAccount.push_back({ID, email, password, name});
        }
        file.close();
    }

public:
    // AdminSystem() {
    //         size = 0;
    //         head = nullptr;
    //         tail = nullptr;
    //     }

    AdminSystem() {
        loadAdminFromFile();
        if(adminAccount.empty()) {
            adminAccount.push_back({1,"Admin name","admin","password"});
            saveAdminToFile();
        }
    }

    bool adminLogin(string email, string password) {
        for(int i = 0; i < adminAccount.size(); i++) {
            if(adminAccount[i].email == email && adminAccount[i].password == password) {
                return true;
            }
        }
        return false;
    }

    void createAdminAccount() {
        int ID;
        string email, password, name;
        cout << "Enter the ID for new admin: ";
        cin >> ID;
        cout << "Enter the name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter the new Email: ";
        cin >> email;
        cout << "Enter the new password: ";
        cin >> password;
        adminAccount.push_back({ID, email, password, name});
        saveAdminToFile();
        cout << "Admin account created successfully!" << endl;
    }

    void addStudent(int studentID, string studentName, string studentEmail, string studentPassword) {
        StudentAcc* information = new StudentAcc{studentID, studentName, studentEmail, studentPassword, 'A', nullptr};

        if(head == nullptr) {
            head = tail = information;
        } 
        else {
            tail->next = information;
            tail = information;
        }
    }

    void viewStudentList() {
        StudentAcc* current = head;
        cout << "Student List" <<endl;
        cout << "ID \t\t\t Name \t\t\t Email \t\t\t Password \t\t\t" <<endl;
        while(current != nullptr) {
            cout << current->ID <<"\t\t\t"<< current->name << "\t\t\t" << current->email << "\t\t\t" << current->password <<endl;
            current = current->next;
        }
    }

    void editStudentAttendance(int studentID) {
        StudentAcc* current = head;
        while(current != nullptr) {
            if(current->ID == studentID) {
                char status;
                cout << "Enter the new attendance(P: present, L: Late, A: Absent): ";
                cin >> status;
                status = toupper(status);
                if(status == 'P' || status == 'L' || status == 'A') {
                    current->attendance = status;
                    cout << "Attendance updated successfully!" <<endl;
                }
                else {
                    cout << "Invaild input. Attendance not updated." <<endl;
                }
                return;
            }
            current = current->next;
        }
        cout << "Student ID not found." << endl;
    }

    void saveToFileStudent() {
        fstream file;
        file.open(studentFileAcc, ios::app);
        StudentAcc* current = head;
        while (current != nullptr) {
            file << current->ID << "," << current->name << "," << current->email << "," << current->password << endl;
            current = current->next;
        }
        file.close();
    }

    void loadFromFileStudent() {
        string line, tempID, name, email, password;
        fstream file;
        file.open(studentFileAcc, ios::in);
        if(!file.is_open()) {
            cout << "File Not Found!" <<endl;
        }

        while(getline(file, line)) {
            stringstream ss(line);

            getline(ss, tempID, ',');
            getline(ss, name, ',');
            getline(ss, email, ',');
            getline(ss, password);

            int ID = stoi(tempID);
            addStudent(ID, name, email, password);
        }
        file.close();
    }
};

class AttendanceSystem {
    int size;
    StudentAcc* head;
    StudentAcc* tail;
public:
    AttendanceSystem() {
        size = 0;
        head = nullptr;
        tail = nullptr;
    }

    bool isEmpty() {
            if(head != nullptr) {
            return false;
        }
        return true;
    }

};

// Funtion
void newStudent() {
    AdminSystem *data = new AdminSystem();
    int ID;
    string email, password, name;
    
    cout << "Enter the ID for new student: ";
    cin >> ID;
    cout << "Enter the name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter the new Email: ";
    cin >> email;
    cout << "Enter the new password: ";
    cin >> password;
    
    data->addStudent(ID, email, password, name);
    data->saveToFileStudent();
    cout << "Student account created successfully!" << endl;
}


int main() {
    // newAdmin();
    // newStudent();
    AdminSystem admin;
    admin.loadFromFileStudent();
    int userType;

    do {
        cout << "=== Attendance System ===" <<endl;
        cout << "1. Admin" <<endl;
        cout << "2. Student" <<endl;
        cout << "3. Exit the program" <<endl;
        cout << "Enter your choice: ";
        cin >> userType;

        switch (userType) {
        case 1: {
            string adminEmail, adminPassword;
            cout << "Enter admin email: ";
            cin >> adminEmail;
            cout << "Enter admin password: ";
            cin >> adminPassword;
            if(admin.adminLogin(adminEmail, adminPassword)) {
                int adminChoice;
                do {
                    cout << "\n=== Admin System ===" <<endl;
                    cout << "1) Add new student" <<endl;
                    cout << "2) Add new admin" <<endl;
                    cout << "3) View student list" <<endl;
                    cout << "4) Edit student attendance" <<endl;
                    cout << "5) View attendance" <<endl;
                    cout << "6) Logout" <<endl;
                    cout << "Enter your choice: ";
                    cin >> adminChoice;

                    switch(adminChoice) {
                        case 1: {
                            newStudent();
                            break;
                        }
                        case 2: {
                            admin.createAdminAccount();
                            break;
                        }
                        case 3: {
                            admin.viewStudentList();
                            break;
                        }
                        case 4: {
                            break;
                        }
                        case 5: {
                            break;
                        }
                        case 6: {
                            break;
                        }
                    }
                } while (adminChoice != 6);
                
            }
            else {
                cout << "Invalid email or password!" <<endl;
            }
            break;
        }
        default:
            break;
        }
    } while (userType != 3);

    return 0;
}