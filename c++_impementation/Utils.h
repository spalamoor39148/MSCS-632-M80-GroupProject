#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <ctime>

void displayCategories();
std::string getCategoryByNumber(int num);
time_t parseDate(const std::string &dateStr);
void displayMenu();

#endif
