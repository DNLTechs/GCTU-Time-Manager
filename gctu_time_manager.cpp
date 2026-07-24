#include <iostream>
#include <string>
using namespace std;

string toLower(string text) {
    for (int i = 0; i < text.length(); i++) {
        text[i] = tolower(text[i]);
    }
    return text;
}

int main() {

    string assignmentCourse[10];
    string assignmentDescription[10];
    int assignmentDays[10];
    int assignmentType[10];
    bool assignmentSubmitted[10];
    int wrongAttempts[10];

    int totalAssignments = 0;
    int choice;

    cout << "================================" << endl;
    cout << "   GCTU ASSIGNMENT TRACKER      " << endl;
    cout << "       By: Davidson             " << endl;
    cout << "================================" << endl;
    cout << "\nGood day, Davidson!" << endl;
    cout << "Checking your assignments..." << endl;
    cout << "-----------------------------------" << endl;

    if (totalAssignments == 0) {
        cout << "No assignments recorded yet." << endl;
        cout << "Select Option 1 to add todays assignments!" << endl;
    } else {

        bool hasUrgent = false;

        for (int i = 0; i < totalAssignments; i++) {

            if (!assignmentSubmitted[i] && assignmentDays[i] > 0) {
                assignmentDays[i]--;
            }

            if (!assignmentSubmitted[i]) {
                if (assignmentDays[i] == 0) {
                    cout << "!! DUE TODAY: " << assignmentCourse[i];
                    cout << " - " << assignmentDescription[i] << endl;
                    hasUrgent = true;
                } else if (assignmentDays[i] == 1) {
                    cout << "!! DUE TOMORROW: " << assignmentCourse[i];
                    cout << " - " << assignmentDescription[i] << endl;
                    hasUrgent = true;
                } else if (assignmentDays[i] <= 3) {
                    cout << "** REMINDER: " << assignmentCourse[i];
                    cout << " - " << assignmentDescription[i];
                    cout << " (" << assignmentDays[i] << " days left)" << endl;
                    hasUrgent = true;
                }
            }
        }

        if (!hasUrgent) {
            cout << "All assignments are on track!" << endl;
        }
    }

    cout << "-----------------------------------" << endl;

    do {
        cout << "\n================================" << endl;
        cout << "          MAIN MENU             " << endl;
        cout << "================================" << endl;
        cout << "1. Add todays assignments" << endl;
        cout << "2. Check all deadlines" << endl;
        cout << "3. Verify assignment submission" << endl;
        cout << "4. Exit" << endl;
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {

            int numToday;

            cout << "\n===================================" << endl;
            cout << "       ADD TODAYS ASSIGNMENTS      " << endl;
            cout << "===================================" << endl;
            cout << "Dear Student, how many assignments" << endl;
            cout << "were you given today? ";
            cin >> numToday;
            cin.ignore();

            if (numToday == 0) {
                cout << "Great! No assignments today!" << endl;
            }

            for (int i = 0; i < numToday; i++) {

                cout << "\n--- Assignment " << (totalAssignments + 1) << " ---" << endl;

                cout << "Which course is this for? ";
                getline(cin, assignmentCourse[totalAssignments]);

                cout << "What is the assignment about? ";
                getline(cin, assignmentDescription[totalAssignments]);

                cout << "How many days until deadline? ";
                cin >> assignmentDays[totalAssignments];
                cin.ignore();

                cout << "Submission type?" << endl;
                cout << "1. Soft copy (LMS or email)" << endl;
                cout << "2. Hard copy (handwritten or printed)" << endl;
                cout << "Choose: ";
                cin >> assignmentType[totalAssignments];
                cin.ignore();

                assignmentSubmitted[totalAssignments] = false;
                wrongAttempts[totalAssignments] = 0;
                totalAssignments++;

                cout << "Assignment recorded successfully!" << endl;
            }

        } else if (choice == 2) {

            cout << "\n===================================" << endl;
            cout << "        DEADLINE STATUS            " << endl;
            cout << "===================================" << endl;

            if (totalAssignments == 0) {
                cout << "No assignments recorded yet!" << endl;
            }

            for (int i = 0; i < totalAssignments; i++) {

                cout << "\nAssignment " << (i + 1) << endl;
                cout << "Course     : " << assignmentCourse[i] << endl;
                cout << "About      : " << assignmentDescription[i] << endl;

                if (assignmentType[i] == 1) {
                    cout << "Type       : Soft Copy (LMS/Email)" << endl;
                } else {
                    cout << "Type       : Hard Copy (Handwritten/Printed)" << endl;
                }

                if (assignmentSubmitted[i]) {
                    cout << "Status     : SUBMITTED" << endl;
                } else if (assignmentDays[i] == 0) {
                    cout << "Days Left  : Due TODAY!" << endl;
                    cout << "Status     : SUBMIT NOW!" << endl;
                } else if (assignmentDays[i] == 1) {
                    cout << "Days Left  : " << assignmentDays[i] << endl;
                    cout << "Status     : DUE TOMORROW - Submit today!" << endl;
                } else if (assignmentDays[i] <= 2) {
                    cout << "Days Left  : " << assignmentDays[i] << endl;
                    cout << "Status     : URGENT - Start now!" << endl;
                } else if (assignmentDays[i] <= 3) {
                    cout << "Days Left  : " << assignmentDays[i] << endl;
                    cout << "Status     : REMINDER - 3 days left!" << endl;
                } else {
                    cout << "Days Left  : " << assignmentDays[i] << endl;
                    cout << "Status     : OK - Stay on track" << endl;
                }
                cout << "-----------------------------------" << endl;
            }

        } else if (choice == 3) {

            cout << "\n===================================" << endl;
            cout << "      SUBMISSION VERIFICATION      " << endl;
            cout << "===================================" << endl;

            if (totalAssignments == 0) {
                cout << "No assignments recorded yet!" << endl;
            } else {

                cout << "Which assignment did you submit?" << endl;
                for (int i = 0; i < totalAssignments; i++) {
                    if (!assignmentSubmitted[i]) {
                        cout << (i + 1) << ". " << assignmentCourse[i];
                        cout << " - " << assignmentDescription[i] << endl;
                    }
                }

                int selected;
                cout << "Enter assignment number: ";
                cin >> selected;
                cin.ignore();
                selected--;

                cout << "\n--- Verification Questions ---" << endl;

                string answer1, answer2, answer3;

                if (assignmentType[selected] == 1) {

                    cout << "1. What is the title/topic of your assignment? ";
                    getline(cin, answer1);

                    if (toLower(answer1) == toLower(assignmentDescription[selected])) {

                        cout << "2. What was the file name you submitted? ";
                        getline(cin, answer2);

                        cout << "3. Enter your LMS confirmation number: ";
                        getline(cin, answer3);

                        cout << "\nVerification successful!" << endl;
                        cout << "Title      : " << answer1 << endl;
                        cout << "File name  : " << answer2 << endl;
                        cout << "LMS Ref    : " << answer3 << endl;

                        assignmentSubmitted[selected] = true;
                        cout << "\nAssignment marked as SUBMITTED!" << endl;

                    } else {
                        wrongAttempts[selected]++;
                        cout << "\n===================================" << endl;
                        cout << "       VERIFICATION FAILED         " << endl;
                        cout << "===================================" << endl;
                        cout << "The title you entered does not" << endl;
                        cout << "match your recorded assignment." << endl;
                        cout << "It appears you have NOT submitted." << endl;

                        if (wrongAttempts[selected] >= 2) {
                            cout << "\nWARNING: You have failed verification" << endl;
                            cout << wrongAttempts[selected] << " time(s)." << endl;
                            cout << "Continued dishonesty will cost you marks!" << endl;
                        }

                        cout << "\n===================================" << endl;
                        cout << "Please go and submit your" << endl;
                        cout << assignmentCourse[selected] << " assignment NOW!" << endl;
                        cout << "Return and verify after submission." << endl;
                        cout << "===================================" << endl;
                        cout << "\nProgram closing..." << endl;
                        cout << "Goodbye Davidson. Go submit!" << endl;
                        return 0;
                    }

                } else {

                    cout << "1. What is the title/topic of your assignment? ";
                    getline(cin, answer1);

                    if (toLower(answer1) == toLower(assignmentDescription[selected])) {

                        cout << "2. What time did you submit it? ";
                        getline(cin, answer2);

                        cout << "3. Which lecturer/TA did you hand it to? ";
                        getline(cin, answer3);

                        cout << "\nVerification successful!" << endl;
                        cout << "Title      : " << answer1 << endl;
                        cout << "Time       : " << answer2 << endl;
                        cout << "Handed to  : " << answer3 << endl;

                        assignmentSubmitted[selected] = true;
                        cout << "\nAssignment marked as SUBMITTED!" << endl;

                    } else {
                        wrongAttempts[selected]++;
                        cout << "\n===================================" << endl;
                        cout << "       VERIFICATION FAILED         " << endl;
                        cout << "===================================" << endl;
                        cout << "The title you entered does not" << endl;
                        cout << "match your recorded assignment." << endl;
                        cout << "It appears you have NOT submitted." << endl;

                        if (wrongAttempts[selected] >= 2) {
                            cout << "\nWARNING: You have failed verification" << endl;
                            cout << wrongAttempts[selected] << " time(s)." << endl;
                            cout << "Continued dishonesty will cost you marks!" << endl;
                        }

                        cout << "\n===================================" << endl;
                        cout << "Please go and hand in your" << endl;
                        cout << assignmentCourse[selected] << " assignment NOW!" << endl;
                        cout << "Return and verify after submission." << endl;
                        cout << "===================================" << endl;
                        cout << "\nProgram closing..." << endl;
                        cout << "Goodbye Davidson. Go submit!" << endl;
                        return 0;
                    }
                }
            }

        } else if (choice == 4) {
            cout << "\nGoodbye Davidson!" << endl;
            cout << "Keep grinding for that First Class!" << endl;
        } else {
            cout << "Invalid option! Choose 1 to 4." << endl;
        }

    } while (choice != 4);

    return 0;
}
