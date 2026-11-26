#include "DischargeValidator.h"
#include <iostream>
#include <cctype>

using namespace std;

// Helper function to ask a Yes/No question and return true for 'Y'/'y'
static bool askCriteria(const string& question) {
    char answer;
    while (true) {
        cout << "  - " << question << " (Y/N): ";
        if (cin >> answer) {
            answer = static_cast<char>(tolower(static_cast<unsigned char>(answer)));
            if (answer == 'y') {
                return true;
            }
            if (answer == 'n') {
                return false;
            }
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "    " << RED << "[!] Please enter Y or N." << RESET << "\n";
    }
}

bool validateDischargeEligibility(const Patient& p) {
    cout << "\n" << CYAN << "Discharge Eligibility Checklist for " << p.name << " (" << p.id << "):" << RESET << "\n";

    bool stabilizedVitals = askCriteria("Stabilized vitals");
    bool noRedFlags = askCriteria("No red flags");
    bool lowResourceNeeds = askCriteria("Low resource needs");
    bool selfCareAbility = askCriteria("Ability to take care of one's self");
    bool clearFollowUp = askCriteria("Clear follow-up or referral plan");

    // Clear trailing newline from single-char cin >> answer before any getline calls
    cin.ignore(10000, '\n');

    bool eligible = stabilizedVitals && noRedFlags && lowResourceNeeds && selfCareAbility && clearFollowUp;

    if (!eligible) {
        cout << "\n" << RED << "[!] Discharge blocked: Patient did not meet all eligibility requirements." << RESET << "\n";
        if (!stabilizedVitals) cout << "    * Missing: Stabilized vitals\n";
        if (!noRedFlags)       cout << "    * Missing: No red flags\n";
        if (!lowResourceNeeds) cout << "    * Missing: Low resource needs\n";
        if (!selfCareAbility)  cout << "    * Missing: Ability to take care of one's self\n";
        if (!clearFollowUp)    cout << "    * Missing: Clear follow-up or referral plan\n";
    }

    return eligible;
}