#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

// Structure to represent a file uploaded by a user
struct File {
    string filename;
    string uploaderRole;
};

// Student class encapsulates student data and file management
class Student {
public:
    string name;
    int rollNo;
    float cgpa;
    float attendance;
    vector<File> uploadedFiles;

    Student(string n, int r, float c, float a) : name(n), rollNo(r), cgpa(c), attendance(a) {}

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

// Faculty class encapsulates faculty data, authentication, and file management
class Faculty {
private:
    string passkey;

public:
    string name;
    vector<File> uploadedFiles;

    Faculty(string n, const string& key) : name(n), passkey(key) {}

    bool verifyPasskey(const string& key) const {
        return key == passkey;
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

    void viewStudentFiles(const vector<Student>& students) const {
        cout << "Files uploaded by students:\n";
        for (const auto& s : students) {
            for (const auto& f : s.uploadedFiles) {
                cout << "- " << f.filename << " (by " << s.name << ")\n";
            }
        }
    }

    void viewAttendanceReport() const {
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
};

// Helper function to get index by name from a list with prompt
int getIndexByName(const vector<string>& names, const string& prompt) {
    if (names.empty()) {
        cout << "No entries available.\n";
        return -1;
    }
    cout << prompt << endl;
    for (size_t i = 0; i < names.size(); ++i) {
        cout << i << ". " << names[i] << endl;
    }
    int choice;
    cout << "Enter choice: ";
    while (!(cin >> choice) || choice < 0 || choice >= (int)names.size()) {
        cout << "Invalid choice. Enter a number between 0 and " << names.size() - 1 << ": ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return choice;
}

// HOD class encapsulates administrative privileges and data viewing
class HOD {
private:
    string passkey;

public:
    string name;

    HOD(string n, const string& key) : name(n), passkey(key) {}

    bool verifyPasskey(const string& key) const {
        return key == passkey;
    }

    void viewAllStudentData(const vector<Student>& students) const {
        cout << "All student data:\n";
        for (const auto& s : students) {
            cout << "Name: " << s.name << ", Roll: " << s.rollNo
                 << ", CGPA: " << s.cgpa << ", Attendance: " << s.attendance << "%\n";
        }
    }

    void viewAllFacultyData(const vector<Faculty>& faculties) const {
        cout << "All faculty data:\n";
        for (const auto& f : faculties) {
            cout << "Name: " << f.name << endl;
        }
    }

    void viewAllFiles(const vector<Student>& students, const vector<Faculty>& faculties) const {
        cout << "All uploaded files:\n";
        for (const auto& s : students) {
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

    void hireFaculty(vector<Faculty>& faculties, const string& filename) {
        string name, passkey;
        cout << "Enter name of new faculty: ";
        cin.ignore(1000, '\n'); // flush input buffer
        getline(cin, name);
        cout << "Enter passkey for new faculty: ";
        cin >> passkey;
        faculties.emplace_back(name, passkey);
        ofstream out(filename, ios::app);
        if (out.is_open()) {
            out << name << "," << passkey << "\n";
            out.close();
            cout << "Faculty hired successfully.\n";
        } else {
            cout << "Error updating faculty file.\n";
        }
    }

    void fireFaculty(vector<Faculty>& faculties, const string& filename) {
        if (faculties.empty()) {
            cout << "No faculty to remove.\n";
            return;
        }
        vector<string> names;
        for (const auto& f : faculties) names.push_back(f.name);
        int index = getIndexByName(names, "Select a faculty to remove:");
        if (index < 0 || index >= (int)faculties.size()) {
            cout << "Invalid selection.\n";
            return;
        }
        faculties.erase(faculties.begin() + index);
        ofstream out(filename);
        if (out.is_open()) {
            for (const auto& f : faculties) {
                out << f.name << ",placeholder\n"; // Placeholder passkey
            }
            out.close();
            cout << "Faculty removed successfully.\n";
        } else {
            cout << "Error updating faculty file.\n";
        }
    }
};

// Writes attendance report to file
void writeAttendanceReport(const vector<Student>& students) {
    ofstream out("attendance_report.txt");
    if (!out) {
        cerr << "Failed to write attendance report." << endl;
        return;
    }
    out << "Name, RollNo, Attendance%\n";
    for (const auto& s : students) {
        out << s.name << ", " << s.rollNo << ", " << s.attendance << "%\n";
    }
    out.close();
}

// Marks attendance for students
void markAttendance(vector<Student>& students) {
    cout << "\n--- Mark Attendance (1 = Present, 0 = Absent) ---\n";
    for (auto& student : students) {
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
    writeAttendanceReport(students);
    cout << "Attendance marked and report updated.\n";
}

// Loads data from file "meow.txt"
void loadData(const string& filename, vector<HOD>& hods, vector<Faculty>& faculties, vector<Student>& students) {
    ifstream in(filename);
    if (!in) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    int hodCount, facCount, stuCount;
    in >> hodCount >> facCount >> stuCount;
    in.ignore(1000, '\n'); // flush newline

    string line;
    for (int i = 0; i < hodCount; ++i) {
        getline(in, line);
        size_t comma = line.find(',');
        hods.emplace_back(line.substr(0, comma), line.substr(comma + 1));
    }
    for (int i = 0; i < facCount; ++i) {
        getline(in, line);
        size_t comma = line.find(',');
        faculties.emplace_back(line.substr(0, comma), line.substr(comma + 1));
    }
    for (int i = 0; i < stuCount; ++i) {
        getline(in, line);
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        string name = line.substr(0, pos1);
        int rollNo = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        float cgpa = stof(line.substr(pos2 + 1, pos3 - pos2 - 1));
        int attendance = stoi(line.substr(pos3 + 1));
        students.emplace_back(name, rollNo, cgpa, attendance);
    }
    in.close();

    writeAttendanceReport(students);
}

int main() {
    vector<Student> students;
    vector<Faculty> faculties;
    vector<HOD> hods;

    loadData("meow.txt", hods, faculties, students);
    if (hods.empty()) {
        cout << "No HODs loaded. Exiting.\n";
        return 1;
    }
    HOD& hod = hods[0];

    while (true) {
        cout << "\n===== MENU =====\n";
        cout << "1. Student Upload File\n";
        cout << "2. Student Delete File\n";
        cout << "3. View Student Attendance\n";
        cout << "4. View Student CGPA\n";
        cout << "5. Faculty Upload File\n";
        cout << "6. Faculty Delete File\n";
        cout << "7. Faculty View Student Files\n";
        cout << "8. Faculty View Attendance Report\n";
        cout << "9. HOD View All Student Data\n";
        cout << "10. HOD View All Faculty Data\n";
        cout << "11. HOD View All Files\n";
        cout << "12. HOD Hire Faculty\n";
        cout << "13. HOD Fire Faculty\n";
        cout << "14. Mark Attendance\n";
        cout << "0. Exit\n";

        int choice;
        cin >> choice;
        int index;
        string key;
        vector<string> names;

        switch (choice) {
            case 1:
                names.clear();
                for (const auto& s : students) names.push_back(s.name);
                index = getIndexByName(names, "Choose a student:");
                if (index != -1) students[index].uploadFile();
                break;
            case 2:
                names.clear();
                for (const auto& s : students) names.push_back(s.name);
                index = getIndexByName(names, "Choose a student:");
                if (index != -1) students[index].deleteFile();
                break;
            case 3:
                names.clear();
                for (const auto& s : students) names.push_back(s.name);
                index = getIndexByName(names, "Choose a student:");
                if (index != -1) students[index].viewAttendance();
                break;
            case 4:
                names.clear();
                for (const auto& s : students) names.push_back(s.name);
                index = getIndexByName(names, "Choose a student:");
                if (index != -1) students[index].viewCGPA();
                break;
            case 5:
                names.clear();
                for (const auto& f : faculties) names.push_back(f.name);
                index = getIndexByName(names, "Choose a faculty:");
                if (index != -1) {
                    cout << "Enter passkey: ";
                    cin >> key;
                    if (faculties[index].verifyPasskey(key)) faculties[index].uploadFile();
                    else cout << "Invalid passkey!\n";
                }
                break;
            case 6:
                names.clear();
                for (const auto& f : faculties) names.push_back(f.name);
                index = getIndexByName(names, "Choose a faculty:");
                if (index != -1) {
                    cout << "Enter passkey: ";
                    cin >> key;
                    if (faculties[index].verifyPasskey(key)) faculties[index].deleteFile();
                    else cout << "Invalid passkey!\n";
                }
                break;
            case 7:
                names.clear();
                for (const auto& f : faculties) names.push_back(f.name);
                index = getIndexByName(names, "Choose a faculty:");
                if (index != -1) {
                    cout << "Enter passkey: ";
                    cin >> key;
                    if (faculties[index].verifyPasskey(key)) faculties[index].viewStudentFiles(students);
                    else cout << "Invalid passkey!\n";
                }
                break;
            case 8:
                names.clear();
                for (const auto& f : faculties) names.push_back(f.name);
                index = getIndexByName(names, "Choose a faculty:");
                if (index != -1) {
                    cout << "Enter passkey: ";
                    cin >> key;
                    if (faculties[index].verifyPasskey(key)) faculties[index].viewAttendanceReport();
                    else cout << "Invalid passkey!\n";
                }
                break;
            case 9:
                cout << "Enter HOD passkey: ";
                cin >> key;
                if (hod.verifyPasskey(key)) hod.viewAllStudentData(students);
                else cout << "Invalid passkey!\n";
                break;
            case 10:
                cout << "Enter HOD passkey: ";
                cin >> key;
                if (hod.verifyPasskey(key)) hod.viewAllFacultyData(faculties);
                else cout << "Invalid passkey!\n";
                break;
            case 11:
                cout << "Enter HOD passkey: ";
                cin >> key;
                if (hod.verifyPasskey(key)) hod.viewAllFiles(students, faculties);
                else cout << "Invalid passkey!\n";
                break;
            case 12:
                cout << "Enter HOD passkey: ";
                cin >> key;
                if (hod.verifyPasskey(key)) hod.hireFaculty(faculties, "meow.txt");
                else cout << "Invalid passkey!\n";
                break;
            case 13:
                cout << "Enter HOD passkey: ";
                cin >> key;
                if (hod.verifyPasskey(key)) hod.fireFaculty(faculties, "meow.txt");
                else cout << "Invalid passkey!\n";
                break;
            case 14:
                cout << "Enter HOD passkey: ";
                cin >> key;
                if (hod.verifyPasskey(key)) markAttendance(students);
                else cout << "Invalid passkey!\n";
                break;
            case 0:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid option. Please try again.\n";
                break;
        }
    }
}
