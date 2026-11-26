#include "TriagePreview.h"
#include "Patient.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

static size_t visibleLength(const string& s) {
    size_t len = 0;
    bool inEscape = false;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\033') {
            inEscape = true;
        } else if (inEscape) {
            if (s[i] == 'm') {
                inEscape = false;
            }
        } else {
            ++len;
        }
    }
    return len;
}

static string padOrTrimAnsi(const string& text, size_t width) {
    size_t visLen = visibleLength(text);
    if (visLen == width) {
        return text;
    }
    if (visLen < width) {
        return text + string(width - visLen, ' ');
    }

    string result;
    size_t count = 0;
    bool inEscape = false;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\033') {
            inEscape = true;
            result += text[i];
        } else if (inEscape) {
            result += text[i];
            if (text[i] == 'm') {
                inEscape = false;
            }
        } else {
            if (count < width) {
                result += text[i];
                ++count;
            }
        }
    }
    result += RESET;
    return result;
}

static string getRowColor(const string& triage) {
    if (triage.find("RED") != string::npos) return RED;
    if (triage.find("YEL") != string::npos) return YELLOW;
    if (triage.find("AMB") != string::npos) return AMBER;
    if (triage.find("GRN") != string::npos) return GREEN;
    return RESET;
}

void printTriagePreview() {
    vector<Patient> sortedPatients = patients;
    sort(sortedPatients.begin(), sortedPatients.end(), [](const Patient& a, const Patient& b) {
        return a.deviationScore > b.deviationScore;
    });

    static const string border =
        "+--------+----------------------+-----+---------+-----+--------+--------------------------+---------------+";
    static const string header =
        "| ID     | PATIENT NAME         | AGE | BP      | HR  | TEMP   | CHIEF COMPLAINT          | TRIAGE        |";

    cout << BOLD << "Dashboard: [ ACTIVE PATIENT TRIAGE QUEUE ] " << RESET
         << "(Auto-Sorted by Age-Normal Deviation)\n";

    if (sortedPatients.empty()) {
        cout << border << "\n";
        cout << header << "\n";
        cout << border << "\n";
        cout << "|                               [!] No active patients in queue.                                       |\n";
        cout << border << "\n";
    } else {
        cout << border << "\n";
        cout << header << "\n";
        cout << border << "\n";

        for (const auto& p : sortedPatients) {
            ostringstream tempStream;
            tempStream << fixed << setprecision(1) << p.temp << "C";

            string rowColor = getRowColor(p.triage);
            string coloredTriage = rowColor + BOLD + p.triage + RESET;

            cout << "| " << padOrTrimAnsi(rowColor + p.id + RESET, 6)
                 << " | " << padOrTrimAnsi(rowColor + p.name + RESET, 20)
                 << " | " << padOrTrimAnsi(rowColor + to_string(p.age) + RESET, 3)
                 << " | " << padOrTrimAnsi(rowColor + p.bp + RESET, 7)
                 << " | " << padOrTrimAnsi(rowColor + to_string(p.hr) + RESET, 3)
                 << " | " << padOrTrimAnsi(rowColor + tempStream.str() + RESET, 6)
                 << " | " << padOrTrimAnsi(rowColor + p.complaint + RESET, 24)
                 << " | " << padOrTrimAnsi(coloredTriage, 13)
                 << " |\n";
        }

        cout << border << "\n";
    }

    cout << "(*" << RED << BOLD << "RED = HIGH DEVIATION" << RESET
         << " | " << YELLOW << BOLD << "YEL = URGENT" << RESET
         << " | " << AMBER << BOLD << "AMB = DELAYED" << RESET
         << " | " << GREEN << BOLD << "GRN = NEAR NORMAL" << RESET << ")\n\n";
}