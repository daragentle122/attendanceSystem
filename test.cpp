#include <iostream>
#include <cstring>
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

    ~AdminSystem() {
        Account* current = head;
        while (current) {
            Account* temp = current;
            current = current->next;
            delete temp;
        }
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

    string getStudentNamebyID(int studentID) {
        Account* current = head;
        while (current != NULL) {
            if (current->ID == studentID) {
                string name = current->name;
                return name;
            }
            current = current->next;
        }
        return "Unknown";
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

    void displayStudent() {
        Account* current = head;
        cout << "\t=== Student List ===" << endl;
        cout << "ID\tName\t\tEmail" <<endl;
        while(current != NULL) {
            cout << current->ID << "\t" << current->name << "\t\t" << current->email <<endl;
            current = current->next;
        }
    }

    // void displayAcc() {
    //     Account* current = head;
    //     cout << "=== Student List ===" << endl;
    //     while (current != NULL) {
    //         if (current->userType == 2) {
    //             cout << "Name: " << current->name << ", ID: " << current->ID << ", Email: " << current->email << endl;
    //         }
    //         current = current->next;
    //     }
    // }
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
    AdminSystem* accountSystem = NULL;

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
    int classHour, classMinute;
    char period[3];
    sscanf(classStartTime.c_str(), "%d:%d %2s", &classHour, &classMinute, period);

    // Convert class start time to 24-hour format
    if (strcmp(period, "PM") == 0 && classHour != 12) {
        classHour += 12;
    } else if (strcmp(period, "AM") == 0 && classHour == 12) {
        classHour = 0;
    }

    // Get current time
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm* localTime = localtime(&currentTime);

    int currentHour = localTime->tm_hour;
    int currentMinute = localTime->tm_min;

    // Calculate time difference in minutes
    int startInMinutes = classHour * 60 + classMinute;
    int currentInMinutes = currentHour * 60 + currentMinute;
    int diff = currentInMinutes - startInMinutes;

    // Find student and update attendance
    Attendance* student = findStudent(studentID);
    if (student) {
        if (diff >= 0 && diff <= 15) {
            student->present++;
            cout << "Student " << studentID << " marked as Present." << endl;
        } else if (diff > 15) {
            student->late++;
            cout << "Student " << studentID << " marked as Late." << endl;
        } else {
            student->absent++;
            cout << "Student " << studentID << " marked as Absent." << endl;
        }
    } else {
        cout << "Student ID not found. Creating a new record." << endl;
        addAttendance(studentID);
        markAttendance(studentID, classStartTime);
    }
}

    void editAttendance(int studentID) {
        Attendance* student = findStudent(studentID);
        if (student == NULL) {
            cout << "Student with ID " << studentID << " not found." << endl;
            return;
        }

        cout << "Current Attendance for Student " << studentID << ":\n";
        cout << "Present: " << student->present << ", Late: " << student->late << ", Absent: " << student->absent << endl;

        int adminInput;
        bool validInput = false;

        while (!validInput) {
            cout << "Enter new attendance (1 for present, 2 for late, 3 for absent): ";
            cin >> adminInput;

            if (adminInput == 1) {
                student->present += 1;
                validInput = true;
            }
            else if (adminInput == 2) {
                student->late += 1;
                validInput = true;
            }
            else if (adminInput == 3) {
                student->absent += 1;
                validInput = true;
            }
            else {
                cout << "Input invalid. Please try again." << endl;
            }
        }

        saveAttendanceFile();
        cout << "Attendance updated successfully." << endl;
    }

    void editAttendanceInterface() {
        int studentID;
        cout << "Enter the student ID to edit attendance: ";
        cin >> studentID;

        editAttendance(studentID);
    }

    void viewAttendance() {
        if (isEmpty()) {
            cout << "No attendance records available." << endl;
            return;
        }

        cout << "=== Attendance Records ===" << endl;
        Attendance* current = head;
        string studentName = accountSystem->getStudentNamebyID(current->ID);
        while (current != NULL) {
            cout << "Student ID: " << current->ID << endl;
            cout << "Name: " << studentName <<endl;
            cout << "Present: " << current->present << endl;
            cout << "Late: " << current->late << endl;
            cout << "Absent: " << current->absent << endl;
            cout << "-------------------------" << endl;
            current = current->next;
        }
    }

    void displayWeeklyAttendance() {
        
        if (isEmpty()) {
            cout << "No attendance data available." << endl;
            return;
        }

        cout << "=== Weekly Attendance for All Students ===" << endl;
        cout << "----------------------------------------------------------" << endl;
        cout << "| Student ID | Student Name     | Monday  | Tuesday | Wednesday | Thursday | Friday  |" << endl;
        cout << "----------------------------------------------------------" << endl;

        Attendance* current = head;
        while (current != NULL) {
            string studentName = accountSystem->getStudentNamebyID(current->ID); // Replace with actual logic for fetching name
            
            string mondayStatus = (current->present > 0) ? "Present" : (current->late > 0) ? "Late" : "Absent";
            string tuesdayStatus = (current->present > 1) ? "Present" : (current->late > 1) ? "Late" : "Absent";
            string wednesdayStatus = (current->present > 2) ? "Present" : (current->late > 2) ? "Late" : "Absent";
            string thursdayStatus = (current->present > 3) ? "Present" : (current->late > 3) ? "Late" : "Absent";
            string fridayStatus = (current->present > 4) ? "Present" : (current->late > 4) ? "Late" : "Absent";

            cout << "| " << current->ID << "        | " << studentName << "       | " 
                << mondayStatus << " | " << tuesdayStatus << " | " 
                << wednesdayStatus << " | " << thursdayStatus << " | " 
                << fridayStatus << " |" << endl;

            current = current->next;
        }
    }

    ~AttendanceSystem() {
        Attendance* current = head;
        while (current) {
            Attendance* temp = current;
            current = current->next;
            delete temp;
        }
    }
};


int main() {
    AdminSystem Account, studentAccount;
    AttendanceSystem system;
    int userType;
    string email, Password;

    do {
        cout << "=== Attendance System ===" <<endl;
        cout << "1. Admin" <<endl;
        cout << "2. Student" <<endl;
        cout << "3. Exit the program" <<endl;
        cout << "Enter your choice: ";
        cin >> userType;

        Account.loadFromFile(userType);
        studentAccount.loadFromFile(2);
        system.loadAttendanceFile();
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
                    cout << "1) Add new admin" <<endl;
                    cout << "2) Add new student" <<endl;
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
                            studentAccount.displayStudent();
                            break;
                        }
                        case 4: {
                            system.editAttendanceInterface();
                            break;
                        }
                        case 5: {
                            system.viewAttendance();
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
                        case 2: {
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
