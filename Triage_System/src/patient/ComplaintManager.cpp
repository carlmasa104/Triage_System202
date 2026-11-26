#include "ComplaintManager.h"
#include "Patient.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Fixed list of standardized clinic chief complaints
static const vector<string> STANDARD_COMPLAINTS = {
    "Fever",
    "Cough / Cold",
    "Headache",
    "Dizziness",
    "Chest Pain",
    "Shortness of Breath",
    "Abdominal Pain",
    "Nausea / Vomiting",
    "Injury / Wound"
};

struct ComplaintCount {
    string complaint;
    int count;
};

string promptStandardComplaint() {
// Lets the user pick a standardized complaint by number, or enter a custom one
    cout << "\nSelect Chief Complaint:\n";
    for (size_t i = 0; i < STANDARD_COMPLAINTS.size(); ++i) {
        cout << "  [" << (i + 1) << "] " << STANDARD_COMPLAINTS[i] << "\n";
    }
    cout << "  [" << (STANDARD_COMPLAINTS.size() + 1) << "] Other (Type custom complaint)\n";

    int maxOption = static_cast<int>(STANDARD_COMPLAINTS.size()) + 1;
    int choice = 0;

    while (true) {
        cout << "Enter choice [1-" << maxOption << "]: ";
        if (cin >> choice && choice >= 1 && choice <= maxOption) {
            cin.ignore(10000, '\n');
            if (choice == maxOption) {
                string custom;
                cout << "Enter Custom Chief Complaint: ";
                getline(cin, custom);
                return custom.empty() ? "Unspecified" : custom;
            }
            return STANDARD_COMPLAINTS[static_cast<size_t>(choice - 1)];
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << RED << "[!] Invalid selection. Enter a number from 1 to " << maxOption << "." << RESET << "\n";
    }
}

// Builds a quantitative complaint frequency list sorted from most to least frequent
void printSortedComplaintSummary() {
    map<string, int> counts;
    for (const auto& p : patients) {
        string label = p.complaint.empty() ? "Unspecified" : p.complaint;
        counts[label]++;
    }

    vector<ComplaintCount> ranked;
    ranked.reserve(counts.size());
    for (const auto& entry : counts) {
        ranked.push_back({entry.first, entry.second});
    }

    // Sort highest count first; break ties alphabetically
    sort(ranked.begin(), ranked.end(), [](const ComplaintCount& a, const ComplaintCount& b) {
        if (a.count != b.count) {
            return a.count > b.count;
        }
        return a.complaint < b.complaint;
    });

    cout << BOLD << "\nChief Complaints (Most to Least Frequent):\n" << RESET;
    if (ranked.empty()) {
        cout << "  No active patient complaints recorded.\n";
        return;
    }

    for (size_t i = 0; i < ranked.size(); ++i) {
        cout << "  " << (i + 1) << ". " << ranked[i].complaint
             << " — " << ranked[i].count
             << (ranked[i].count == 1 ? " patient\n" : " patients\n");
    }
}