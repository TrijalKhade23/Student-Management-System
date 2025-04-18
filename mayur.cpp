#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>

using namespace std;

// File structure for file management
struct File {
    string filename;
    string uploaderRole;
};

// Student class with all features from both programs
class Student {
public:
    string name;
    int rollNo;
    float cgpa;
    float attendance;
    string password;
    vector<File> uploadedFiles;

    Student(string n = "", int r = 0, float c = 0.0, float a = 0.0, string pass = "")
        : name(n), rollNo(r), cgpa(c), attendance(a), password(pass) {}

    void display() const {
        cout << "Name: " << name << "\nRoll No: " << rollNo 
             << "\nCGPA: " << cgpa << "\nAttendance: " << attendance << "%\n";
    }

    void uploadFile() {
        string fname;
        cout << "Enter file name to upload: ";
        cin >> fname;
        uploadedFiles.push_back({fname, "Student"});
        ofstream out("student_files.txt", ios::app);
        if (out.is_open()) {
            out << name << "," << fname << "\n";
            out.close();
            cout << "Student uploaded file: " << fname << endl;
        } else {
            cout << "Error opening student file storage." << endl;
        }
    }

    void deleteFile() {
        string fname;
        cout << "Enter file name to delete: ";
        cin >> fname;
        bool found = false;
        vector<File> newFiles;
        for (size_t i = 0; i < uploadedFiles.size(); ++i) {
            if (uploadedFiles[i].filename == fname) {
                found = true;
            } else {
                newFiles.push_back(uploadedFiles[i]);
            }
        }
        if (found) {
            uploadedFiles = newFiles;
            updateFileStorage("student_files.txt", uploadedFiles, name);
            cout << "Deleted file: " << fname << endl;
        } else {
            cout << "File not found!" << endl;
        }
    }

    void viewAttendance() const {
        cout << "Attendance for " << name << ": " << attendance << "%\n";
    }

    void viewCGPA() const {
        cout << "CGPA for " << name << ": " << cgpa << "\n";
    }

private:
    void updateFileStorage(const string& filename, const vector<File>& files, const string& owner) {
        ifstream in(filename);
        ofstream out("temp.txt");
        string line;
        while (getline(in, line)) {
            bool keep = true;
            if (line.substr(0, owner.size()) == owner) {
                keep = false;
                for (size_t i = 0; i < files.size(); ++i) {
                    if (line.find("," + files[i].filename) != string::npos) {
                        keep = true;
                        break;
                    }
                }
            }
            if (keep) out << line << "\n";
        }
        in.close();
        out.close();
        remove(filename.c_str());
        rename("temp.txt", filename.c_str());
    }
};

// Faculty class with all features
class Faculty {
private:
    string passkey;

public:
    string name;
    vector<File> uploadedFiles;

    Faculty(string n = "", const string& key = "") : name(n), passkey(key) {}

    bool verifyPasskey(const string& key) const {
        return key == passkey;
    }

    void markAttendance(map<int, Student>& students, int rollNo, int newAttendance) {
        if (students.find(rollNo) != students.end()) {
            students[rollNo].attendance = newAttendance;
        } else {
            throw runtime_error("Student not found.");
        }
    }

    void viewStudentData(const map<int, Student>& students, int rollNo) {
        if (students.find(rollNo) != students.end()) {
            students.at(rollNo).display();
        } else {
            throw runtime_error("Student not found.");
        }
    }

    void addStudent(map<int, Student>& students, Student s) {
        if (students.find(s.rollNo) == students.end()) {
            students[s.rollNo] = s;
            saveStudentToFile(s);
        } else {
            throw runtime_error("Student already exists.");
        }
    }

    void uploadFile() {
        string fname;
        cout << "Enter file name to upload: ";
        cin >> fname;
        uploadedFiles.push_back({fname, "Faculty"});
        ofstream out("faculty_files.txt", ios::app);
        if (out.is_open()) {
            out << name << "," << fname << "\n";
            out.close();
            cout << "Faculty uploaded file: " << fname << endl;
        } else {
            cout << "Error opening faculty file storage." << endl;
        }
    }

    void deleteFile() {
        string fname;
        cout << "Enter file name to delete: ";
        cin >> fname;
        bool found = false;
        vector<File> newFiles;
        for (size_t i = 0; i < uploadedFiles.size(); ++i) {
            if (uploadedFiles[i].filename == fname) {
                found = true;
            } else {
                newFiles.push_back(uploadedFiles[i]);
            }
        }
        if (found) {
            uploadedFiles = newFiles;
            updateFileStorage("faculty_files.txt", uploadedFiles, name);
            cout << "Deleted file: " << fname << endl;
        } else {
            cout << "File not found!" << endl;
        }
    }

    void viewStudentFiles(const map<int, Student>& students) const {
        cout << "Files uploaded by students:\n";
        for (const auto& pair : students) {
            const Student& s = pair.second;
            for (const auto& f : s.uploadedFiles) {
                cout << "- " << f.filename << " (by " << s.name << ")\n";
            }
        }
    }

    void viewAttendanceReport(const map<int, Student>& students) const {
        ofstream out("attendance_report.txt");
        if (!out) {
            cout << "Error generating attendance report.\n";
            return;
        }
        out << "Name, RollNo, Attendance%\n";
        for (const auto& pair : students) {
            const Student& s = pair.second;
            out << s.name << ", " << s.rollNo << ", " << s.attendance << "%\n";
        }
        out.close();
        
        ifstream in("attendance_report.txt");
        if (!in) {
            cout << "Attendance report not available.\n";
            return;
        }
        string line;
        cout << "\n--- Attendance Report ---\n";
        while (getline(in, line)) {
            cout << line << endl;
        }
        in.close();
    }

private:
    void updateFileStorage(const string& filename, const vector<File>& files, const string& owner) {
        ifstream in(filename);
        ofstream out("temp.txt");
        string line;
        while (getline(in, line)) {
            bool keep = true;
            if (line.substr(0, owner.size()) == owner) {
                keep = false;
                for (size_t i = 0; i < files.size(); ++i) {
                    if (line.find("," + files[i].filename) != string::npos) {
                        keep = true;
                        break;
                    }
                }
            }
            if (keep) out << line << "\n";
        }
        in.close();
        out.close();
        remove(filename.c_str());
        rename("temp.txt", filename.c_str());
    }

    void saveStudentToFile(const Student& s) {
        ofstream outFile("students_data.txt", ios::app);
        if (outFile.is_open()) {
            outFile << s.rollNo << "," << s.name << "," << s.cgpa << "," << s.attendance << "," << s.password << "\n";
            outFile.close();
        } else {
            throw runtime_error("Unable to open students_data.txt");
        }
    }
};

// HOD class with all features
class HOD : public Faculty {
private:
    vector<string> facultyList;

public:
    HOD(string n = "", const string& key = "") : Faculty(n, key) {}

    void saveFacultyToFile(const string& name) {
        ofstream outFile("faculties.txt", ios::app);
        if (outFile.is_open()) {
            outFile << name << "\n";
            outFile.close();
        } else {
            throw runtime_error("Unable to open faculties.txt");
        }
    }

    void removeFacultyFromFile(const string& name) {
        ifstream inFile("faculties.txt");
        ofstream tempFile("temp.txt");
        string line;

        if (!inFile.is_open() || !tempFile.is_open()) {
            throw runtime_error("Unable to modify faculties.txt");
        }

        while (getline(inFile, line)) {
            if (line != name) {
                tempFile << line << "\n";
            }
        }

        inFile.close();
        tempFile.close();
        remove("faculties.txt");
        rename("temp.txt", "faculties.txt");
    }

    void hireFaculty(string name) {
        facultyList.push_back(name);
        saveFacultyToFile(name);
        cout << "Faculty " << name << " hired.\n";
    }

    void fireFaculty(string name) {
        auto it = find(facultyList.begin(), facultyList.end(), name);
        if (it != facultyList.end()) {
            facultyList.erase(it);
            removeFacultyFromFile(name);
            cout << "Faculty " << name << " fired.\n";
        } else {
            throw runtime_error("Faculty not found.");
        }
    }

    void viewAllAttendance(const map<int, Student>& students) {
        for (const auto& pair : students) {
            cout << pair.second.name << " - Attendance: " << pair.second.attendance << "%\n";
        }
    }

    void viewAllStudentData(const map<int, Student>& students) const {
        cout << "All student data:\n";
        for (const auto& pair : students) {
            pair.second.display();
            cout << "-----------------\n";
        }
    }

    void viewAllFacultyData(const vector<Faculty>& faculties) const {
        cout << "All faculty data:\n";
        for (const auto& f : faculties) {
            cout << "Name: " << f.name << endl;
        }
    }

    void viewAllFiles(const map<int, Student>& students, const vector<Faculty>& faculties) const {
        cout << "All uploaded files:\n";
        for (const auto& pair : students) {
            const Student& s = pair.second;
            for (const auto& f : s.uploadedFiles) {
                cout << "- " << f.filename << " (by " << s.name << ")\n";
            }
        }
        for (const auto& f : faculties) {
            for (const auto& file : f.uploadedFiles) {
                cout << "- " << file.filename << " (by " << f.name << ")\n";
            }
        }
    }
};

// Helper functions
bool studentLogin(int rollNo, string password, const map<int, Student>& students) {
    ifstream inFile("students_data.txt");
    string line;
    while (getline(inFile, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        size_t pos4 = line.find(',', pos3 + 1);

        int storedRollNo = stoi(line.substr(0, pos1));
        string storedPassword = line.substr(pos4 + 1);

        if (storedRollNo == rollNo && storedPassword == password) {
            return true;
        }
    }
    return false;
}

void registerStudent(map<int, Student>& students) {
    string name, password;
    int rollNo;
    float cgpa;
    int attendance;

    cout << "Enter name: ";
    cin >> name;
    cout << "Enter roll number: ";
    cin >> rollNo;
    cout << "Enter CGPA: ";
    cin >> cgpa;
    cout << "Enter attendance: ";
    cin >> attendance;
    cout << "Set your password: ";
    cin >> password;

    Student newStudent(name, rollNo, cgpa, attendance, password);
    students[rollNo] = newStudent;
    
    ofstream outFile("students_data.txt", ios::app);
    if (outFile.is_open()) {
        outFile << rollNo << "," << name << "," << cgpa << "," << attendance << "," << password << "\n";
        outFile.close();
        cout << "Student registered successfully!\n";
    } else {
        throw runtime_error("Unable to open students_data.txt");
    }
}

void loadInitialData(map<int, Student>& students, vector<Faculty>& faculties, HOD& hod) {
    // Load HOD data
    hod = HOD("Admin", "hod123");
    
    // Load some initial faculty
    faculties.push_back(Faculty("Faculty1", "fac123"));
    
    // Try to load students from file
    ifstream inFile("students_data.txt");
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);
            size_t pos3 = line.find(',', pos2 + 1);
            size_t pos4 = line.find(',', pos3 + 1);

            int rollNo = stoi(line.substr(0, pos1));
            string name = line.substr(pos1 + 1, pos2 - pos1 - 1);
            float cgpa = stof(line.substr(pos2 + 1, pos3 - pos2 - 1));
            int attendance = stoi(line.substr(pos3 + 1, pos4 - pos3 - 1));
            string password = line.substr(pos4 + 1);

            students[rollNo] = Student(name, rollNo, cgpa, attendance, password);
        }
        inFile.close();
    }
}

void markAttendance(map<int, Student>& students) {
    cout << "\n--- Mark Attendance (1 = Present, 0 = Absent) ---\n";
    for (auto& pair : students) {
        Student& student = pair.second;
        int present;
        cout << "Is " << student.name << " present? (1/0): ";
        cin >> present;
        while (present != 0 && present != 1) {
            cout << "Invalid input. Enter 1 for present or 0 for absent: ";
            cin >> present;
        }
        if (present == 1) student.attendance = (float)((student.attendance + 1)/101)*100;
        else if (present == 0) student.attendance = (float)(student.attendance/101)*100;
    }
    
    // Update attendance report
    ofstream out("attendance_report.txt");
    if (out.is_open()) {
        out << "Name, RollNo, Attendance%\n";
        for (const auto& pair : students) {
            out << pair.second.name << ", " << pair.second.rollNo << ", " << pair.second.attendance << "%\n";
        }
        out.close();
    }
    cout << "Attendance marked and report updated.\n";
}

int main() {
    map<int, Student> students;
    vector<Faculty> faculties;
    HOD hod;
    
    // Load initial data
    loadInitialData(students, faculties, hod);

    int mainChoice;
    bool loggedIn = false;
    string role;
    int currentStudentRoll = -1;

    while (true) {
        cout << "\n===== MAIN MENU =====\n";
        cout << "1. HOD Login\n";
        cout << "2. Faculty Login\n";
        cout << "3. Student Login\n";
        cout << "4. Register New Student\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> mainChoice;

        if (mainChoice == 0) {
            cout << "Exiting the program.\n";
            break;
        }

        try {
            if (mainChoice == 1) {
                string passkey;
                cout << "Enter HOD passkey: ";
                cin >> passkey;
                if (hod.verifyPasskey(passkey)) {
                    role = "HOD";
                    loggedIn = true;
                } else {
                    throw runtime_error("Invalid passkey.");
                }
            } 
            else if (mainChoice == 2) {
                if (faculties.empty()) {
                    throw runtime_error("No faculty registered.");
                }
                cout << "Select Faculty:\n";
                for (size_t i = 0; i < faculties.size(); i++) {
                    cout << i+1 << ". " << faculties[i].name << endl;
                }
                int facChoice;
                cin >> facChoice;
                if (facChoice < 1 || facChoice > (int)faculties.size()) {
                    throw runtime_error("Invalid faculty selection.");
                }
                string passkey;
                cout << "Enter passkey for " << faculties[facChoice-1].name << ": ";
                cin >> passkey;
                if (faculties[facChoice-1].verifyPasskey(passkey)) {
                    role = "FACULTY";
                    loggedIn = true;
                } else {
                    throw runtime_error("Invalid passkey.");
                }
            } 
            else if (mainChoice == 3) {
                int rollNo;
                string password;
                cout << "Enter your roll number: ";
                cin >> rollNo;
                cout << "Enter your password: ";
                cin >> password;
                if (studentLogin(rollNo, password, students)) {
                    role = "STUDENT";
                    loggedIn = true;
                    currentStudentRoll = rollNo;
                } else {
                    throw runtime_error("Invalid login credentials.");
                }
            } 
            else if (mainChoice == 4) {
                registerStudent(students);
                continue;
            } 
            else {
                throw runtime_error("Invalid choice.");
            }

            // Role-specific menu
            while (loggedIn) {
                if (role == "HOD") {
                    cout << "\n===== HOD MENU =====\n";
                    cout << "1. Hire Faculty\n";
                    cout << "2. Fire Faculty\n";
                    cout << "3. Add Student\n";
                    cout << "4. View All Student Data\n";
                    cout << "5. View All Faculty Data\n";
                    cout << "6. View All Files\n";
                    cout << "7. View All Attendance\n";
                    cout << "8. Mark Attendance\n";
                    cout << "9. Logout\n";
                    cout << "Enter choice: ";
                    int hodChoice;
                    cin >> hodChoice;

                    try {
                        if (hodChoice == 1) {
                            string name;
                            cout << "Enter faculty name to hire: ";
                            cin >> name;
                            hod.hireFaculty(name);
                            faculties.push_back(Faculty(name, "fac123")); // Default passkey
                        } 
                        else if (hodChoice == 2) {
                            if (faculties.empty()) {
                                throw runtime_error("No faculty to remove.");
                            }
                            cout << "Select faculty to fire:\n";
                            for (size_t i = 0; i < faculties.size(); i++) {
                                cout << i+1 << ". " << faculties[i].name << endl;
                            }
                            int fireChoice;
                            cin >> fireChoice;
                            if (fireChoice < 1 || fireChoice > (int)faculties.size()) {
                                throw runtime_error("Invalid selection.");
                            }
                            hod.fireFaculty(faculties[fireChoice-1].name);
                            faculties.erase(faculties.begin() + fireChoice - 1);
                        } 
                        else if (hodChoice == 3) {
                            registerStudent(students);
                        } 
                        else if (hodChoice == 4) {
                            hod.viewAllStudentData(students);
                        } 
                        else if (hodChoice == 5) {
                            hod.viewAllFacultyData(faculties);
                        } 
                        else if (hodChoice == 6) {
                            hod.viewAllFiles(students, faculties);
                        } 
                        else if (hodChoice == 7) {
                            hod.viewAllAttendance(students);
                        } 
                        else if (hodChoice == 8) {
                            markAttendance(students);
                        } 
                        else if (hodChoice == 9) {
                            loggedIn = false;
                            cout << "Logged out as HOD.\n";
                        } 
                        else {
                            throw runtime_error("Invalid choice.");
                        }
                    } catch (exception& e) {
                        cerr << "Error: " << e.what() << endl;
                    }
                } 
                else if (role == "FACULTY") {
                    Faculty& currentFaculty = faculties[0]; // Simplified for demo
                    
                    cout << "\n===== FACULTY MENU =====\n";
                    cout << "1. Add Student\n";
                    cout << "2. Mark Attendance\n";
                    cout << "3. View Student Data\n";
                    cout << "4. Upload File\n";
                    cout << "5. Delete File\n";
                    cout << "6. View Student Files\n";
                    cout << "7. View Attendance Report\n";
                    cout << "8. Logout\n";
                    cout << "Enter choice: ";
                    int facChoice;
                    cin >> facChoice;

                    try {
                        if (facChoice == 1) {
                            string name;
                            int roll;
                            float cgpa;
                            int attendance;
                            cout << "Enter name, roll no, cgpa, attendance: ";
                            cin >> name >> roll >> cgpa >> attendance;
                            currentFaculty.addStudent(students, Student(name, roll, cgpa, attendance, "default"));
                        } 
                        else if (facChoice == 2) {
                            int roll, att;
                            cout << "Enter roll no and new attendance: ";
                            cin >> roll >> att;
                            currentFaculty.markAttendance(students, roll, att);
                        } 
                        else if (facChoice == 3) {
                            int roll;
                            cout << "Enter roll no: ";
                            cin >> roll;
                            currentFaculty.viewStudentData(students, roll);
                        } 
                        else if (facChoice == 4) {
                            currentFaculty.uploadFile();
                        } 
                        else if (facChoice == 5) {
                            currentFaculty.deleteFile();
                        } 
                        else if (facChoice == 6) {
                            currentFaculty.viewStudentFiles(students);
                        } 
                        else if (facChoice == 7) {
                            currentFaculty.viewAttendanceReport(students);
                        } 
                        else if (facChoice == 8) {
                            loggedIn = false;
                            cout << "Logged out as Faculty.\n";
                        } 
                        else {
                            throw runtime_error("Invalid choice.");
                        }
                    } catch (exception& e) {
                        cerr << "Error: " << e.what() << endl;
                    }
                } 
                else if (role == "STUDENT") {
                    Student& currentStudent = students[currentStudentRoll];
                    
                    cout << "\n===== STUDENT MENU =====\n";
                    cout << "1. View Your Data\n";
                    cout << "2. View Your Attendance\n";
                    cout << "3. View Your CGPA\n";
                    cout << "4. Upload File\n";
                    cout << "5. Delete File\n";
                    cout << "6. Logout\n";
                    cout << "Enter choice: ";
                    int stuChoice;
                    cin >> stuChoice;

                    try {
                        if (stuChoice == 1) {
                            currentStudent.display();
                        } 
                        else if (stuChoice == 2) {
                            currentStudent.viewAttendance();
                        } 
                        else if (stuChoice == 3) {
                            currentStudent.viewCGPA();
                        } 
                        else if (stuChoice == 4) {
                            currentStudent.uploadFile();
                        } 
                        else if (stuChoice == 5) {
                            currentStudent.deleteFile();
                        } 
                        else if (stuChoice == 6) {
                            loggedIn = false;
                            currentStudentRoll = -1;
                            cout << "Logged out as Student.\n";
                        } 
                        else {
                            throw runtime_error("Invalid choice.");
                        }
                    } catch (exception& e) {
                        cerr << "Error: " << e.what() << endl;
                    }
                }
            }
        } catch (exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    return 0;
}