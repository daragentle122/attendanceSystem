#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
using namespace std;

struct Account {
    int ID;
    string name, email, password;

    Account* next;
};

class AdminSystem {
    Account* head;
    Account* tail;
    string adminfile = "adminAccount.csv";
    string studentfile = "studentAccount.csv";

public:
    AdminSystem() {
        int size = 0;
        head = tail = NULL;
    }

    bool login(string email, string password) {
        Account*current = head;
        while(current != NULL) {
            if(current->email == email && current->password == password) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool isEmpty() {
        if ( head == NULL ) {
            return true;
        }
        return false;
    }

    int studentID(string email, string password) {
        if (isEmpty()) {
            cout << "No accounts in the system. Please create an account first." << endl;
            return -1;
        }
        int id;
        Account *current = head;

        while (current != NULL) {
            if (current->email == email && current->password == password) {
                return current->ID;
            }
            current = current->next;
        }
        return 0;
    }

    void addAccount(int accID, string accName, string accEmail, string accPassword) {
        Account* information = new Account{accID, accName, accEmail, accPassword};
        if(isEmpty()) {
            head = tail = information;
        } 
        else {
            tail->next = information;
            tail = information;
        }
    }

    void createAccount(int choice) {
    int ID;
    string email, password, name;
    cout << "Enter the ID: ";
    cin >> ID;
    cout << "Enter the name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter the email: ";
    cin >> email;
    cout << "Enter the password: ";
    cin >> password;

    addAccount(ID, name, email, password);
    saveToFile(choice);
    cout << "Account created successfully!" << endl;
}

    void saveToFile(int choice) {
        fstream file;
        string fileName;
        if(choice == 1) {
            fileName = adminfile;
        }
        else if(choice == 2) {
            fileName = studentfile;
        }
        file.open(fileName, ios::app);
        Account* current = head;
        while (current != NULL) {
            file << current->ID << "," << current->name << "," << current->email << "," << current->password << endl;
            current = current->next;
        }
        file.close();
    }

    void loadFromFile(int choice) {
        fstream file;
        string fileName;
        if(choice == 1) {
            fileName = adminfile;
        }
        else if(choice == 2) {
            fileName = studentfile;
        }
        file.open(fileName, ios::in);
        if(!file.is_open()) {
            cout << "ERROR" <<endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            string tempID, name, email, password;
            stringstream ss(line);

            getline(ss, tempID, ',');
            int ID = stoi(tempID);
            getline(ss, name, ',');
            getline(ss, email, ',');
            getline(ss, password, ',');
            
            addAccount(ID, name, email, password);
        }
        file.close();
    }

    void displayAcc() {
        if (isEmpty()) {
            cout << "No accounts to display." << endl;
            return;
        }

        Account* current = head;
        while (current != NULL) {
            cout << "ID: " << current->ID << ", Name: " << current->name << ", Email: " << current->email << ", Password: " << current->password << endl;
            current = current->next;
        }
    }
};

struct Attendance {
    int ID;
    int present, late, absent;

    Attendance* next;
};

class AttendanceSystem {
    Attendance* head;
    Attendance* tail;
    string attendanceFile = "attendanceFile.csv";

public:
    AttendanceSystem() {
        int size = 0;
        head = NULL;
        tail = NULL;
    }

    bool isEmpty() {
        if (head == NULL) {
            return true;
        }
        return false;
    }

    void addAttendance(int studentID, int presentCount = 0, int lateCount = 0, int absentCount = 0) {
        Attendance* newAttendance = new Attendance{studentID, presentCount, lateCount, absentCount, NULL};
        if (isEmpty()) {
            head = tail = newAttendance;
        } else {
            tail->next = newAttendance;
            tail = newAttendance;
        }
    }

    void saveAttendanceFile() {
        fstream file;
        file.open(attendanceFile, ios::out);
        if (!file.is_open()) {
            cout << "ERROR: Could not open file." << endl;
            return;
        }
        Attendance* current = head;
        while (current != NULL) {
            file << current->ID << "," << current->present << "," << current->late << "," << current->absent << endl;
            current = current->next;
        }
        file.close();
    }

    void loadAttendanceFile() {
        fstream file;
        file.open(attendanceFile, ios::in);
        if (!file.is_open()) {
            cout << "ERROR: Could not open file." << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string tempID, tempPresent, tempLate, tempAbsent;

            getline(ss, tempID, ',');
            getline(ss, tempPresent, ',');
            getline(ss, tempLate, ',');
            getline(ss, tempAbsent);

            int ID = stoi(tempID);
            int present = stoi(tempPresent);
            int late = stoi(tempLate);
            int absent = stoi(tempAbsent);

            addAttendance(ID, present, late, absent);
        }
        file.close();
    }

    Attendance* findStudent(int studentID) {
        Attendance* current = head;
        while (current != NULL) {
            if (current->ID == studentID) {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    void checkAndCreateStudent(int studentID) {
        Attendance* existingStudent = findStudent(studentID);
        if (existingStudent == NULL) {
            addAttendance(studentID);
        }
    }

    void markAttendance(int studentID, const string& classStartTime) {
        // Parse class start time in 12-hour format (e.g., "08:00 AM" or "01:30 PM")
        int classHour, classMinute;
        string period;
        sscanf(classStartTime.c_str(), "%d:%d %s", &classHour, &classMinute, &period[0]);

        // Convert to 24-hour format for comparison
        if (period == "PM" && classHour != 12) {
            classHour += 12;
        }
        else if (period == "AM" && classHour == 12) {
            classHour = 0;
        }

        // Get current time
        auto now = chrono::system_clock::now();
        time_t currentTime = chrono::system_clock::to_time_t(now);
        tm* localTime = localtime(&currentTime);

        int currentHour = localTime->tm_hour;
        int currentMinute = localTime->tm_min;

        // Convert current time to AM/PM format for display
        string currentPeriod = (currentHour >= 12) ? "PM" : "AM";
        int displayHour = (currentHour % 12 == 0) ? 12 : currentHour % 12;

        cout << "Current Time: " << displayHour << ":" << (currentMinute < 10 ? "0" : "") << currentMinute << " " << currentPeriod << endl;

        // Calculate time difference in minutes
        int startInMinutes = classHour * 60 + classMinute;
        int currentInMinutes = currentHour * 60 + currentMinute;
        int diff = currentInMinutes - startInMinutes;

        // Find student and update attendance
        Attendance* student = findStudent(studentID);
        if (student) {
            if(diff >= 0 && diff <= 15) {
                student->present++;
                cout << "Student " << studentID << " marked as Present." << endl;
            }
            else if(diff > 15) {
                student->late++;
                cout << "Student " << studentID << " marked as Late." << endl;
            }
            else {
                student->absent++;
                cout << "Student " << studentID << " marked as Absent." << endl;
            }
        }
        else {
            // cout << "Student ID not found. Creating new record." << endl;
            addAttendance(studentID);
            markAttendance(studentID, classStartTime);
        }
    }
};


int main() {
    AdminSystem Account;
    int userType;
    string email, Password;
    AttendanceSystem system;
    // Account.display_acc();

    do {
        cout << "=== Attendance System ===" <<endl;
        cout << "1. Admin" <<endl;
        cout << "2. Student" <<endl;
        cout << "3. Exit the program" <<endl;
        cout << "Enter your choice: ";
        cin >> userType;

        Account.loadFromFile(userType);

        switch (userType) {
        case 1: {
            cout << "Enter admin email: ";
            cin >> email;
            cout << "Enter admin password: ";
            cin >> Password;
            if(Account.login(email, Password)) {
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
                            Account.createAccount(adminChoice);
                            break;
                        }
                        case 2: {
                            Account.createAccount(adminChoice);
                            break;
                        }
                        case 3: {

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
        case 2: {
            cout << "Enter student email: ";
            cin >> email;
            cout << "Enter student password: ";
            cin >> Password;
            if(Account.login(email, Password)) {
                system.loadAttendanceFile();
                int studentChoice;
                int ID = Account.studentID(email, Password);
                
                do {
                    cout << "\n=== Student System ===" <<endl;
                    cout << "1) Mark attendance" <<endl;
                    cout << "2) View attendance" <<endl;
                    cout << "3) Logout" <<endl;
                    cout << "Enter your choice: ";
                    cin >> studentChoice;
                    switch(studentChoice) {
                        case 1: {
                            system.markAttendance(ID, "08:00 AM");
                            system.saveAttendanceFile();
                            break;
                        }
                    }
                }while (studentChoice != 3);
            }
        }
            break;
        }
    } while (userType != 3);

    return 0;

}
