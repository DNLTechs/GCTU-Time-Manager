#include <iostream>   // allows us to use cout and cin
#include <string>     // allows us to use string variables
#include <vector>     // allows us to use vectors (like arrays but no size limit)
#include <fstream>    // allows us to read and write files (fixes data loss problem)
#include <sstream>    // allows us to split text using a separator
#include <cctype>     // allows us to use tolower() for converting text to lowercase

using namespace std;

/* STRUCT — A struct is like a container that groups
*  related variables together under one name.
* Think of it like a form with multiple fields.
* Each Assignment has 6 fields inside it.*/

struct Assignment {
    string course;        // name of the course
    string description;   // what the assignment is about
    string dueDate;       // deadline in format YYYY-MM-DD
    int type;             // 1 = Soft Copy, 2 = Hard Copy
    bool submitted;       // true if submitted, false if not
    int wrongAttempts;    // counts how many times student failed verification
};

/* CONSTANT — FILE_NAME stores the name of the file
* where all assignments are saved on the computer.
* const means this value can never be changed.
*/
const string FILE_NAME = "assignments.txt";

/* FUNCTION 1: toLower
// Converts any text to all lowercase letters.
// So "Calculus" and "CALCULUS" both become "calculus"
* This makes our verification comparison fair.
*/

string toLower(string text) {
    for (size_t i = 0; i < text.length(); i++) {
        text[i] = tolower(text[i]); // convert each character to lowercase
    }
    return text; // return the fully lowercase version
}

/* FUNCTION 2: getValidInput
* Keeps asking the user for a number until they
* enter a valid one within the allowed range.
* This fixes the crash problem when wrong input is typed.
* min = lowest allowed number
* max = highest allowed number*/

int getValidInput(int min, int max) {
    int input;
    while (true) { // keep looping until valid input received
        cin >> input;
        if (cin.fail() || input < min || input > max) {
            // cin.fail() = true when letters typed instead of numbers
            cin.clear();                  // clears the error flag
            cin.ignore(10000, '\n');      // throws away the bad input
            cout << "Invalid input! Please enter a number between "
                 << min << " and " << max << ": ";
        } else {
            cin.ignore(10000, '\n'); // clears leftover newline after valid input
            return input;           // return the valid number
        }
    }
}

/* FUNCTION 3: isValidDate
* Checks if a date typed by the user is real and correctly formatted as YYYY-MM-DD.
* Returns true if valid, false if not.
*/
bool isValidDate(const string& date) {
    // date must be exactly 10 characters e.g. 2026-08-15
    if (date.length() != 10) return false;

    // position 4 and 7 must be dashes
    if (date[4] != '-' || date[7] != '-') return false;

    // all other positions must be digits (numbers)
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue; // skip the dash positions
        if (!isdigit(date[i])) return false;
    }

    // extract year, month, day from the string
    int year  = stoi(date.substr(0, 4)); // stoi converts string to int
    int month = stoi(date.substr(5, 2));
    int day   = stoi(date.substr(8, 2));

    // check year, month, day are within realistic ranges
    if (year < 2000 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    // days allowed in each month (index 0 unused, index 1=Jan, 2=Feb etc.)
    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // leap year check — February gets 29 days in a leap year
    // a leap year is divisible by 4 but not 100, OR divisible by 400
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth[2] = 29;
    }

    // check that the day doesn't exceed the days in that month
    if (day > daysInMonth[month]) return false;

    return true; // all checks passed — date is valid
}

/* FUNCTION 4: getValidDateInput
* Keeps asking the user to enter a date until they type one in the correct format YYYY-MM-DD.
*/
string getValidDateInput() {
    string inputDate;
    while (true) { // keep looping until valid date entered
        getline(cin, inputDate);
        if (isValidDate(inputDate)) {
            return inputDate; // valid date — return it
        }
        cout << "Invalid format or impossible date! Please use YYYY-MM-DD: ";
    }
}

/* FUNCTION 5: calculateDaysLeft
* Calculates how many days remain until the deadline.
* Uses a simple estimation based on year, month, day.
* Returns 0 if the deadline has already passed.
*/
int calculateDaysLeft(const string& dueDateStr) {
    int year, month, day;
    char dash1, dash2;
    stringstream ss(dueDateStr); // break the date string into parts

    // read year, dash, month, dash, day from the string
    if (!(ss >> year >> dash1 >> month >> dash2 >> day)) {
        return 0; // if reading fails, return 0
    }

    // today's date — hardcoded to current date
    int currentYear  = 2026;
    int currentMonth = 7;
    int currentDay   = 24;

    // convert both dates into a total number of days for easy subtraction
    int totalDueDays     = (year * 365) + (month * 30) + day;
    int totalCurrentDays = (currentYear * 365) + (currentMonth * 30) + currentDay;

    int diff = totalDueDays - totalCurrentDays; // calculate the difference

    return (diff < 0) ? 0 : diff; // if already passed return 0, otherwise return days left
}

/* FUNCTION 6: saveAssignmentsToFile
* Saves ALL assignments to a text file on the computer.
* This fixes the data loss problem — data survives
* even after the program is closed.
* Each assignment is saved on one line separated by 1*/

void saveAssignmentsToFile(const vector<Assignment>& list) {
    ofstream outFile(FILE_NAME); // open file for writing

    if (!outFile.is_open()) { // check if file opened successfully
        cout << "[Warning: Could not save assignments to disk!]" << endl;
        return;
    }

    // loop through every assignment and write it to the file
    for (const auto& a : list) {
        outFile << a.course       << "|"
                << a.description  << "|"
                << a.dueDate      << "|"
                << a.type         << "|"
                << a.submitted    << "|"
                << a.wrongAttempts << "\n"; // each assignment on its own line
    }
    outFile.close(); // always close the file after writing
}

/* FUNCTION 7: loadAssignmentsFromFile
* Reads assignments saved in the text file and
* loads them back into the program when it opens.
* This is what makes data persist between sessions.
*/

void loadAssignmentsFromFile(vector<Assignment>& list) {
    ifstream inFile(FILE_NAME); // open file for reading

    if (!inFile.is_open()) return; // if no file exists yet, just return

    string line;
    while (getline(inFile, line)) { // read one line at a time
        if (line.empty()) continue; // skip empty lines

        // split the line by | separator to get each field
        stringstream ss(line);
        string course, desc, date, typeStr, subStr, wrongStr;

        if (getline(ss, course,   '|') &&
            getline(ss, desc,     '|') &&
            getline(ss, date,     '|') &&
            getline(ss, typeStr,  '|') &&
            getline(ss, subStr,   '|') &&
            getline(ss, wrongStr, '|')) {

            Assignment a;               // create a new assignment
            a.course        = course;
            a.description   = desc;
            a.dueDate       = date;
            a.type          = stoi(typeStr);          // convert string to int
            a.submitted     = (stoi(subStr) == 1);    // convert to bool
            a.wrongAttempts = stoi(wrongStr);

            list.push_back(a); // add the assignment to the vector
        }
    }
    inFile.close(); // close file after reading
}

/* FUNCTION 8: addAssignments
* Asks the student how many assignments they got today
* and records each one with all its details.*/

void addAssignments(vector<Assignment>& assignments, const string& studentName) {

    cout << "\n\033[1m       ADD TODAY'S ASSIGNMENTS     \033[0m\n";
    cout << "_____________________________________\n" << endl;
    cout << "Dear " << studentName << ", how many assignments were you given today? ";
    int numToday = getValidInput(0, 50); // validated — must be 0 to 50

    for (int i = 0; i < numToday; i++) { // loop once per assignment
        Assignment temp; // create a temporary assignment to fill in

        cout << "\n--- Assignment " << (assignments.size() + 1) << " ---" << endl;

        cout << "Which course is this for? ";
        getline(cin, temp.course); // read full course name including spaces

        cout << "What is the assignment about? ";
        getline(cin, temp.description); // this is what verification checks against

        cout << "Due date (Format YYYY-MM-DD, e.g., 2026-08-15): ";
        temp.dueDate = getValidDateInput(); // validated real calendar date

        cout << "Submission type?" << endl;
        cout << "1. Soft copy (LMS or email)" << endl;
        cout << "2. Hard copy (handwritten or printed)" << endl;
        cout << "Choose: ";
        temp.type = getValidInput(1, 2);

        temp.submitted     = false; // not submitted yet by default
        temp.wrongAttempts = 0;     // no failed verifications yet

        assignments.push_back(temp); // add to the list
        cout << "Assignment recorded successfully!" << endl;
    }
    saveAssignmentsToFile(assignments); // save immediately after adding
}

/* FUNCTION 9: checkDeadlines
* Shows all assignments with their deadline status.
* Uses real calendar date comparison to show how many actual days are left.*/

void checkDeadlines(const vector<Assignment>& assignments) {

    cout << "\n\033[1m        DEADLINE STATUS            \033[0m\n";
    cout << "___________________________________" << endl;

    if (assignments.empty()) { // check if no assignments exist
        cout << "No assignments recorded yet!" << endl;
        return;
    }

    for (size_t i = 0; i < assignments.size(); i++) { // loop through all assignments
        int daysLeft = calculateDaysLeft(assignments[i].dueDate); // calculate real days left

        cout << "\nAssignment " << (i + 1) << endl;
        cout << "Course     : " << assignments[i].course << endl;
        cout << "About      : " << assignments[i].description << endl;
        cout << "Due Date   : " << assignments[i].dueDate << endl;

        // ternary operator: condition ? value_if_true : value_if_false
        cout << "Type       : " << (assignments[i].type == 1 ? "Soft Copy (LMS/Email)" : "Hard Copy") << endl;

        // show status based on days left
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

/* FUNCTION 10: verifySubmission
* Asks the student verification questions to confirm
* they actually submitted their assignment.
* Secretly compares their answer to the recorded
* description — student does not know this is happening.
*/

void verifySubmission(vector<Assignment>& assignments) {

    cout << "\n\033[1m      SUBMISSION VERIFICATION      \033[0m\n";
    cout << "__________________________________________\n";

    if (assignments.empty()) {
        cout << "No assignments recorded yet!\n";
        return;
    }

    // collect only unsubmitted assignments into a separate list
    cout << "Unsubmitted Assignments:\n";
    vector<int> unsubmittedIndices; // stores the index numbers of unsubmitted ones

    for (size_t i = 0; i < assignments.size(); i++) {
        if (!assignments[i].submitted) { // ! means NOT submitted
            unsubmittedIndices.push_back(i); // remember this index
            cout << unsubmittedIndices.size() << ". "
                 << assignments[i].course
                 << " - " << assignments[i].description << endl;
        }
    }

    if (unsubmittedIndices.empty()) {
        cout << "All assignments have already been submitted! Great job!\n";
        return;
    }

    cout << "Select assignment number to verify: ";
    int selChoice = getValidInput(1, unsubmittedIndices.size());
    int selected  = unsubmittedIndices[selChoice - 1]; // get real index

    cout << "\n--- Verification Questions ---" << endl;
    string answer1, answer2, answer3;

    // Question 1 — the secret comparison question
    cout << "1. What is the title/topic of your assignment? ";
    getline(cin, answer1);

    // SECRET COMPARISON — student does not know this is happening
    if (toLower(answer1) == toLower(assignments[selected].description)) {

        // title matched — ask remaining questions based on submission type
        if (assignments[selected].type == 1) {
            // soft copy questions
            cout << "2. What was the file name you submitted? ";
            getline(cin, answer2);
            cout << "3. Enter your LMS confirmation number: ";
            getline(cin, answer3);
        } else {
            // hard copy questions
            cout << "2. What time did you submit it? ";
            getline(cin, answer2);
            cout << "3. Which lecturer/TA did you hand it to? ";
            getline(cin, answer3);
        }

        assignments[selected].submitted = true; // mark as submitted
        saveAssignmentsToFile(assignments);      // save the update
        cout << "\nVerification successful! Marked as SUBMITTED." << endl;

    } else {
        // title did not match — student likely lying
        assignments[selected].wrongAttempts++;  // increase fail count
        saveAssignmentsToFile(assignments);     // save the updated count

        cout << "\n\033[1m       VERIFICATION FAILED         \033[0m\n";
        cout << "__________________________________________\n";
        cout << "The title you entered does not match your recorded assignment.\n";

        if (assignments[selected].wrongAttempts >= 2) {
            cout << "\nWARNING: You have failed verification "
                 << assignments[selected].wrongAttempts << " time(s)." << endl;
            cout << "Continued dishonesty will cost you marks!\n";
        }

        cout << "\nPlease go and submit your "
             << assignments[selected].course << " assignment NOW!" << endl;
    }
}

/* FUNCTION 11: editAssignment
* Lets the student fix mistakes in a recorded
* assignment — course name, description, date or type.
*/

void editAssignment(vector<Assignment>& assignments) {
    cout << "\n\033[1m        EDIT AN ASSIGNMENT         \033[0m\n";
    cout << "_____________________________________\n";

    if (assignments.empty()) {
        cout << "No assignments recorded yet!" << endl;
        return;
    }

    // show all assignments with numbers
    for (size_t i = 0; i < assignments.size(); i++) {
        cout << (i + 1) << ". " << assignments[i].course
             << " - " << assignments[i].description << endl;
    }

    cout << "Which assignment do you want to edit? ";
    int selected = getValidInput(1, assignments.size()) - 1; // -1 converts to array index

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

    saveAssignmentsToFile(assignments); // save changes immediately
    cout << "Assignment updated and saved!" << endl;
}

/* FUNCTION 12: deleteAssignment
* Removes an assignment from the list permanently.
* Asks for confirmation first to prevent accidents.
*/

void deleteAssignment(vector<Assignment>& assignments) {

    cout << "\n\033[1m       DELETE AN ASSIGNMENT        \033[0m\n";
    cout << "________________________________________";

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
        // erase removes the item at the chosen position from the vector
        assignments.erase(assignments.begin() + selected);
        saveAssignmentsToFile(assignments); // save after deletion
        cout << "Assignment deleted successfully!" << endl;
    } else {
        cout << "Deletion cancelled." << endl;
    }
}


/* FUNCTION 13: refreshAssignments
* Clears ALL assignments to start fresh.
* Useful at the start of a new week or semester.
* Asks for double confirmation before clearing.
*/
void refreshAssignments(vector<Assignment>& assignments) {

    cout << "\n\033[1m       REFRESH / CLEAR ALL DATA    \033[0m\n";
    cout << "_____________________________________\n";

    if (assignments.empty()) {
        cout << "Your tracker is already empty!\n";
        return;
    }

    cout << "WARNING: This will permanently delete ALL assignments." << endl;
    cout << "Are you absolutely sure?" << endl;
    cout << "1. Yes, clear everything" << endl;
    cout << "2. No, cancel" << endl;
    cout << "Choose: ";

    if (getValidInput(1, 2) == 1) {
        assignments.clear();               // empties the entire vector
        saveAssignmentsToFile(assignments); // overwrites file with empty data
        cout << "\nTracker refreshed! All assignments have been cleared.\n";
    } else {
        cout << "\nRefresh cancelled. Your data is safe.\n";
    }
}


/* MAIN FUNCTION
* This is where the program starts.
* It sets everything up and runs the main menu loop.
*/
int main() {

    vector<Assignment> assignments; // create empty list of assignments
    loadAssignmentsFromFile(assignments); // load any saved assignments from file

    string studentName;
    string indexNumber;

    // this is the welcome screen

    cout << "\n\033[1m   GCTU ASSIGNMENT TRACKER      \033[0m\n";
    cout << "____________________________________\n";
    cout << "\nWelcome! Please enter your details.\n";

    // ask for student details — fixes hardcoded name problem
    cout << "Your Name: ";
    getline(cin, studentName);
    cout << "Your Index Number: ";
    getline(cin, indexNumber);

    cout << "\n***********************************";
    cout << "\n\033[1mWelcome, " << studentName << "!\033[0m" << endl;
    cout << "\033[1mIndex Number: " << indexNumber << "\033[0m\n";
    cout << "___________________________________" << endl;
    cout << "\nChecking your assignments..." << endl;
    cout << "-----------------------------------" << endl;

    // show how many assignments were loaded from file
    if (assignments.empty()) {
        cout << "No assignments recorded yet." << endl;
        cout << "Select Option 1 to add today's assignments!" << endl;
    } else {
        cout << "Loaded " << assignments.size()
             << " saved assignment(s) from memory." << endl;
    }
    cout << "-----------------------------------" << endl;

    // main menu loop — keeps running until student exits
    int choice;
    do {
        cout << "\n*******************************\n";
        cout << "\033[1m          MAIN MENU             \033[0m" << endl;
        cout << "  Student: " << studentName << endl;
        cout << "**********************************\n";
        cout << "1. Add today's assignments" << endl;
        cout << "2. Check all deadlines" << endl;
        cout << "3. Verify assignment submission" << endl;
        cout << "4. Edit an assignment" << endl;
        cout << "5. Delete an assignment" << endl;
        cout << "6. Refresh/Clear all assignments" << endl;
        cout << "7. Exit" << endl;
        cout << "Choose an option: ";

        choice = getValidInput(1, 7); // validated — must be 1 to 7

        // switch routes each choice to the correct function
        switch (choice) {
            case 1: addAssignments(assignments, studentName); break;
            case 2: checkDeadlines(assignments);              break;
            case 3: verifySubmission(assignments);            break;
            case 4: editAssignment(assignments);              break;
            case 5: deleteAssignment(assignments);            break;
            case 6: refreshAssignments(assignments);          break;
            case 7:
                cout << "\nGoodbye " << studentName << "!" << endl;
                cout << "Keep grinding for that First Class!" << endl;
                break;
        }

    } while (choice != 7); // keep looping until student chooses 7 to exit

    return 0; // program ends successfully
}
