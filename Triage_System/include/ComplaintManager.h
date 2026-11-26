#ifndef COMPLAINT_MANAGER_H
#define COMPLAINT_MANAGER_H

#include <string>

// Displays numbered standard complaints (plus custom option) and returns the chosen complaint
std::string promptStandardComplaint();

// Counts active patients per complaint and prints them sorted from most to least common
void printSortedComplaintSummary();

#endif