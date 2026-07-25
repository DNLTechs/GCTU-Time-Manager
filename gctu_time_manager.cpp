#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

// --- DATA STRUCTURE ---
struct Assignment {
    string course;
    string description;
    string dueDate; // Format: YYYY-MM-DD
    int type;       // 1 = Soft Copy, 2 = Hard Copy
    bool submitted;
    int wrongAttempts;
};

// --- CONSTANTS ---
const string FILE_NAME = "assignments.txt";

// --- HELPER FUNCTIONS ---

string toLower(string text) {
    for (size_t i = 0; i < text.length(); i++) {
        text[i] = tolower(text[i]);
    }
    return text;
}

// Robust input validation to prevent crashes
int getValidInput(int min, int max) {
    int input;
    while (true) {
        cin >> input;
        if (cin.fail() || input < min || input > max) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number between "
                 << min << " and " << max << ": ";
        } else {
            cin.ignore(10000, '\n');
            return input;
        }
    }
}

// Validates format (YYYY-MM-DD) and calendar correctness (including leap years)
bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (year < 2000 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Leap year adjustment
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth[2] = 29;
    }

    if (day > daysInMonth[month]) return false;

    return true;
}

// Prompts user repeatedly until a valid date is entered
string getValidDateInput() {
    string inputDate;
    while (true) {
        getline(cin, inputDate);
        if (isValidDate(inputDate)) {
            return inputDate;
        }
        cout << "Invalid format or impossible date! Please use YYYY-MM-DD: ";
    }
}

// Calculate remaining days from YYYY-MM-DD
int calculateDaysLeft(const string& dueDateStr) {
    int year, month, day;
    char dash1, dash2;
    stringstream ss(dueDateStr);
    if (!(ss >> year >> dash1 >> month >> dash2 >> day)) {
        return 0;
    }

    int currentYear = 2026;
    int currentMonth = 7;
    int currentDay = 24;

    int totalDueDays = (year * 365) + (month * 30) + day;
    int totalCurrentDays = (currentYear * 365) + (currentMonth * 30) + currentDay;

    int diff = totalDueDays - totalCurrentDays;
    return (diff < 0) ? 0 : diff;
}

// --- FILE PERSISTENCE ---

void saveAssignmentsToFile(const vector<Assignment>& list) {
    ofstream outFile(FILE_NAME);
    if (!outFile.is_open()) {
        cout << "[Warning: Could not save assignments to disk!]" << endl;
        return;
    }

    for (const auto& a : list) {
        outFile << a.course << "|"
                << a.description << "|"
                << a.dueDate << "|"
                << a.type << "|"
                << a.submitted << "|"
                << a.wrongAttempts << "\n";
    }
    outFile.close();
}

void loadAssignmentsFromFile(vector<Assignment>& list) {
    ifstream inFile(FILE_NAME);
    if (!inFile.is_open()) return;

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string course, desc, date, typeStr, subStr, wrongStr;

        if (getline(ss, course, '|') &&
            getline(ss, desc, '|') &&
            getline(ss, date, '|') &&
            getline(ss, typeStr, '|') &&
            getline(ss, subStr, '|') &&
            getline(ss, wrongStr, '|')) {

            Assignment a;
            a.course = course;
            a.description = desc;
            a.dueDate = date;
            a.type = stoi(typeStr);
            a.submitted = (stoi(subStr) == 1);
            a.wrongAttempts = stoi(wrongStr);

            list.push_back(a);
        }
    }
    inFile.close();
}

// --- FEATURE MODULES ---

void addAssignments(vector<Assignment>& assignments, const string& studentName) {
    cout << "\n===================================" << endl;
    cout << "       ADD TODAY'S ASSIGNMENTS     " << endl;
    cout << "===================================" << endl;
    cout << "Dear " << studentName << ", how many assignments were you given today? ";
    int numToday = getValidInput(0, 50);

    for (int i = 0; i < numToday; i++) {
        Assignment temp;
        cout << "\n--- Assignment " << (assignments.size() + 1) << " ---" << endl;

        cout << "Which course is this for? ";
        getline(cin, temp.course);

        cout << "What is the assignment about? ";
        getline(cin, temp.description);

        cout << "Due date (Format YYYY-MM-DD, e.g., 2026-08-15): ";
        temp.dueDate = getValidDateInput();

        cout << "Submission type?" << endl;
        cout << "1. Soft copy (LMS or email)" << endl;
        cout << "2. Hard copy (handwritten or printed)" << endl;
        cout << "Choose: ";
        temp.type = getValidInput(1, 2);

        temp.submitted = false;
        temp.wrongAttempts = 0;

        assignments.push_back(temp);
        cout << "Assignment recorded successfully!" << endl;
    }
    saveAssignmentsToFile(assignments);
}

void checkDeadlines(const vector<Assignment>& assignments) {
    cout << "\n===================================" << endl;
    cout << "        DEADLINE STATUS            " << endl;
    cout << "===================================" << endl;

    if (assignments.empty()) {
        cout << "No assignments recorded yet!" << endl;
        return;
    }

    for (size_t i = 0; i < assignments.size(); i++) {
        int daysLeft = calculateDaysLeft(assignments[i].dueDate);

        cout << "\nAssignment " << (i + 1) << endl;
        cout << "Course     : " << assignments[i].course << endl;
        cout << "About      : " << assignments[i].description << endl;
        cout << "Due Date   : " << assignments[i].dueDate << endl;
        cout << "Type       : " << (assignments[i].type == 1 ? "Soft Copy (LMS/Email)" : "Hard Copy") << endl;

        if (assignments[i].submitted) {
            cout << "Status     : SUBMITTED" << endl;
        } else if (daysLeft == 0) {
            cout << "Status     : DUE TODAY / OVERDUE - Submit now!" << endl;
        } else if (daysLeft == 1) {
            cout << "Status     : DUE TOMORROW - Submit today!" << endl;
        } else if (daysLeft <= 3) {
            cout << "Status     : URGENT (" << daysLeft << " days left)" << endl;
        } else {
            cout << "Status     : OK (" << daysLeft << " days left)" << endl;
        }
        cout << "-----------------------------------" << endl;
    }
}

void verifySubmission(vector<Assignment>& assignments) {
    cout << "\n===================================" << endl;
    cout << "      SUBMISSION VERIFICATION      " << endl;
    cout << "===================================" << endl;

    if (assignments.empty()) {
        cout << "No assignments recorded yet!" << endl;
        return;
    }

    cout << "Unsubmitted Assignments:" << endl;
    vector<int> unsubmittedIndices;
    for (size_t i = 0; i < assignments.size(); i++) {
        if (!assignments[i].submitted) {
            unsubmittedIndices.push_back(i);
            cout << unsubmittedIndices.size() << ". " << assignments[i].course
                 << " - " << assignments[i].description << endl;
        }
    }

    if (unsubmittedIndices.empty()) {
        cout << "All assignments have already been submitted! Great job!" << endl;
        return;
    }

    cout << "Select assignment number to verify: ";
    int selChoice = getValidInput(1, unsubmittedIndices.size());
    int selected = unsubmittedIndices[selChoice - 1];

    cout << "\n--- Verification Questions ---" << endl;
    string answer1, answer2, answer3;

    cout << "1. What is the title/topic of your assignment? ";
    getline(cin, answer1);

    if (toLower(answer1) == toLower(assignments[selected].description)) {
        if (assignments[selected].type == 1) {
            cout << "2. What was the file name you submitted? ";
            getline(cin, answer2);
            cout << "3. Enter your LMS confirmation number: ";
            getline(cin, answer3);
        } else {
            cout << "2. What time did you submit it? ";
            getline(cin, answer2);
            cout << "3. Which lecturer/TA did you hand it to? ";
            getline(cin, answer3);
        }

        assignments[selected].submitted = true;
        saveAssignmentsToFile(assignments);
        cout << "\nVerification successful! Marked as SUBMITTED." << endl;

    } else {
        assignments[selected].wrongAttempts++;
        saveAssignmentsToFile(assignments);

        cout << "\n===================================" << endl;
        cout << "       VERIFICATION FAILED         " << endl;
        cout << "===================================" << endl;
        cout << "The title you entered does not match your recorded assignment." << endl;

        if (assignments[selected].wrongAttempts >= 2) {
            cout << "\nWARNING: You have failed verification "
                 << assignments[selected].wrongAttempts << " time(s)." << endl;
            cout << "Continued dishonesty will cost you marks!" << endl;
        }

        cout << "\nPlease go and submit your " << assignments[selected].course
             << " assignment NOW!" << endl;
    }
}

void editAssignment(vector<Assignment>& assignments) {
    cout << "\n===================================" << endl;
    cout << "        EDIT AN ASSIGNMENT         " << endl;
    cout << "===================================" << endl;

    if (assignments.empty()) {
        cout << "No assignments recorded yet!" << endl;
        return;
    }

    for (size_t i = 0; i < assignments.size(); i++) {
        cout << (i + 1) << ". " << assignments[i].course
             << " - " << assignments[i].description << endl;
    }

    cout << "Which assignment do you want to edit? ";
    int selected = getValidInput(1, assignments.size()) - 1;

    cout << "\nWhat do you want to edit?" << endl;
    cout << "1. Course name" << endl;
    cout << "2. Assignment description" << endl;
    cout << "3. Due date (YYYY-MM-DD)" << endl;
    cout << "4. Submission type" << endl;
    cout << "Choose: ";
    int editChoice = getValidInput(1, 4);

    if (editChoice == 1) {
        cout << "New course name: ";
        getline(cin, assignments[selected].course);
    } else if (editChoice == 2) {
        cout << "New description: ";
        getline(cin, assignments[selected].description);
    } else if (editChoice == 3) {
        cout << "New due date (YYYY-MM-DD): ";
        assignments[selected].dueDate = getValidDateInput();
    } else if (editChoice == 4) {
        cout << "Choose new submission type (1 = Soft, 2 = Hard): ";
        assignments[selected].type = getValidInput(1, 2);
    }
    saveAssignmentsToFile(assignments);
    cout << "Assignment updated and saved!" << endl;
}

void deleteAssignment(vector<Assignment>& assignments) {
    cout << "\n===================================" << endl;
    cout << "       DELETE AN ASSIGNMENT        " << endl;
    cout << "===================================" << endl;

    if (assignments.empty()) {
        cout << "No assignments recorded yet!" << endl;
        return;
    }

    for (size_t i = 0; i < assignments.size(); i++) {
        cout << (i + 1) << ". " << assignments[i].course
             << " - " << assignments[i].description << endl;
    }

    cout << "Which assignment do you want to delete? ";
    int selected = getValidInput(1, assignments.size()) - 1;

    cout << "Are you sure you want to delete this assignment?" << endl;
    cout << "1. Yes, delete it" << endl;
    cout << "2. No, cancel" << endl;
    if (getValidInput(1, 2) == 1) {
        assignments.erase(assignments.begin() + selected);
        saveAssignmentsToFile(assignments);
        cout << "Assignment deleted successfully!" << endl;
    } else {
        cout << "Deletion cancelled." << endl;
    }
}

void refreshAssignments(vector<Assignment>& assignments) {
    cout << "\n===================================" << endl;
    cout << "       REFRESH / CLEAR ALL DATA    " << endl;
    cout << "===================================" << endl;

    if (assignments.empty()) {
        cout << "Your tracker is already empty!" << endl;
        return;
    }

    cout << "WARNING: This will permanently delete ALL assignments to start a new week." << endl;
    cout << "Are you absolutely sure?" << endl;
    cout << "1. Yes, clear everything" << endl;
    cout << "2. No, cancel" << endl;
    cout << "Choose: ";

    if (getValidInput(1, 2) == 1) {
        assignments.clear(); // Empties the vector
        saveAssignmentsToFile(assignments); // Overwrites the file with the empty vector
        cout << "\nTracker refreshed! All previous assignments have been cleared." << endl;
    } else {
        cout << "\nRefresh cancelled. Your data is safe." << endl;
    }
}

// --- MAIN PROGRAM ---

int main() {
    vector<Assignment> assignments;
    loadAssignmentsFromFile(assignments);

    string studentName, indexNumber;

    cout << "================================" << endl;
    cout << "   GCTU ASSIGNMENT TRACKER      " << endl;
    cout << "================================" << endl;
    cout << "\nWelcome! Please enter your details." << endl;

    cout << "Your Name : ";
    getline(cin, studentName);
    cout << "Your Index Number: ";
    getline(cin, indexNumber);

    cout << "\n================================" << endl;
    cout << "Welcome, " << studentName << "!" << endl;
    cout << "Index Number: " << indexNumber << endl;
    cout << "================================" << endl;
    cout << "\nChecking your assignments..." << endl;
    cout << "-----------------------------------" << endl;

    if (assignments.empty()) {
        cout << "No assignments recorded yet." << endl;
        cout << "Select Option 1 to add today's assignments!" << endl;
    } else {
        cout << "Loaded " << assignments.size() << " saved assignment(s) from memory." << endl;
    }
    cout << "-----------------------------------" << endl;

    int choice;
    do {
        cout << "\n================================" << endl;
        cout << "          MAIN MENU             " << endl;
        cout << "  Student: " << studentName << endl;
        cout << "================================" << endl;
        cout << "1. Add today's assignments" << endl;
        cout << "2. Check all deadlines" << endl;
        cout << "3. Verify assignment submission" << endl;
        cout << "4. Edit an assignment" << endl;
        cout << "5. Delete an assignment" << endl;
        cout << "6. Refresh/Clear all assignments" << endl;
        cout << "7. Exit" << endl;
        cout << "Choose an option: ";

        choice = getValidInput(1, 7); // Updated range to 7

        switch (choice) {
            case 1: addAssignments(assignments, studentName); break;
            case 2: checkDeadlines(assignments); break;
            case 3: verifySubmission(assignments); break;
            case 4: editAssignment(assignments); break;
            case 5: deleteAssignment(assignments); break;
            case 6: refreshAssignments(assignments); break; // Added Refresh function
            case 7:
                cout << "\nGoodbye " << studentName << "!" << endl;
                cout << "Keep grinding for that First Class!" << endl;
                break;
        }

    } while (choice != 7); // Updated exit condition to 7

    return 0;
}
