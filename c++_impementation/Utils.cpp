#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>

/**
 * @brief Displays the list of available expense categories with their corresponding numbers.
 *
 * This function prints all supported categories to the standard output, allowing users
 * to select a category by number when adding or filtering expenses.
 */
void displayCategories() {
    std::cout << "\nAvailable Categories:\n";
    std::cout << "1. Housing\n";
    std::cout << "2. Transportation\n";
    std::cout << "3. Food & Dining\n";
    std::cout << "4. Utilities & Communication\n";
    std::cout << "5. Healthcare & Insurance\n";
    std::cout << "6. Personal & Debt\n";
    std::cout << "7. Other\n";
}

/**
 * @brief Returns the category name corresponding to a given number.
 *
 * @param num The category number (1-7).
 * @return The category name as a string, or an empty string if the number is invalid.
 */
std::string getCategoryByNumber(int num) {
    switch (num) {
        case 1: return "Housing";
        case 2: return "Transportation";
        case 3: return "Food & Dining";
        case 4: return "Utilities & Communication";
        case 5: return "Healthcare & Insurance";
        case 6: return "Personal & Debt";
        case 7: return "Other";
        default: return "";
    }
}

/**
 * @brief Parses a date string in YYYY-MM-DD format to a time_t value.
 *
 * @param dateStr The date string in "YYYY-MM-DD" format.
 * @return The corresponding time_t value, or -1 if parsing fails.
 */
time_t parseDate(const std::string &dateStr) {
    tm t = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&t, "%Y-%m-%d");
    return mktime(&t);
}

/**
 * @brief Displays the main menu for the expense tracker CLI application.
 *
 * This function prints all available user actions to the standard output.
 */
void displayMenu() {
    std::cout << "\n------------- MENU -------------\n";
    std::cout << "1. Add Expense\n";
    std::cout << "2. View All Expenses\n";
    std::cout << "3. Delete Expense\n";
    std::cout << "4. Filter by Category\n";
    std::cout << "5. Filter by Date Range\n";
    std::cout << "6. Generate Summary Report\n";
    std::cout << "7. Save to CSV\n";
    std::cout << "8. Load from CSV\n";
    std::cout << "9. Save to JSON\n";
    std::cout << "10. Load from JSON\n";
    std::cout << "11. Exit\n";
    std::cout << "--------------------------------\n";
}
