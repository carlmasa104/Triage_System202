#include "StaffStorage.h"
#include <fstream>
#include <sstream>

using namespace std;

const string FIXED_ADMIN_USER = "admin";
const string FIXED_ADMIN_PASS = "wvsu1234";
vector<StaffAccount> staffAccounts;
string currentLoggedInUser = "SYSTEM";

static const string STAFF_FILE = "staff_accounts.txt";

void saveStaffAccounts() {
    ofstream outFile(STAFF_FILE);
    if (!outFile.is_open()) {
        return;
    }

    for (const auto& acc : staffAccounts) {
        outFile << acc.username << "|" << acc.password << "|" << acc.roleTitle << "\n";
    }

    outFile.close();
}

void loadStaffAccounts() {
    staffAccounts.clear();
    ifstream inFile(STAFF_FILE);

    if (!inFile.is_open()) {
        staffAccounts.push_back({"nurse1", "nurse123", "Nurse"});
        saveStaffAccounts();
        return;
    }

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        StaffAccount acc;
        getline(ss, acc.username, '|');
        getline(ss, acc.password, '|');
        getline(ss, acc.roleTitle, '|');

        if (!acc.username.empty() && !acc.password.empty()) {
            if (acc.roleTitle.empty()) acc.roleTitle = "Nurse";
            staffAccounts.push_back(acc);
        }
    }

    inFile.close();
}

bool removeStaffAccountByUsername(const string& username) {
    for (size_t i = 0; i < staffAccounts.size(); ++i) {
        if (staffAccounts[i].username == username) {
            staffAccounts.erase(staffAccounts.begin() + static_cast<long>(i));
            saveStaffAccounts();
            return true;
        }
    }
    return false;
}

bool isUsernameTaken(const string& username) {
    if (username == FIXED_ADMIN_USER) return true;
    for (const auto& acc : staffAccounts) {
        if (acc.username == username) return true;
    }
    return false;
}