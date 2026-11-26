#ifndef STAFF_STORAGE_H
#define STAFF_STORAGE_H

#include <string>
#include <vector>

enum class UserRole {
    NONE,
    ADMIN,
    STAFF
};

struct StaffAccount {
    std::string username;
    std::string password;
    std::string roleTitle;
};

extern const std::string FIXED_ADMIN_USER;
extern const std::string FIXED_ADMIN_PASS;
extern std::vector<StaffAccount> staffAccounts;
extern std::string currentLoggedInUser;

void loadStaffAccounts();
void saveStaffAccounts();
bool isUsernameTaken(const std::string& username);
bool removeStaffAccountByUsername(const std::string& username);

#endif
