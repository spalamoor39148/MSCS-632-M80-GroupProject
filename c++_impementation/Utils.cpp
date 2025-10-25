#include "Utils.h"
#include "FileManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <limits>
#include <cmath>
#include "Expense.h"

/**
 * @brief Displays the list of available expense categories with their corresponding numbers.
 *
 * Prints all supported categories to the standard output, allowing users to select a category by number.
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
 * Prints all available user actions to the standard output.
 */
void displayMenu() {
    std::cout << "\n========= MENU =========\n";
    std::cout << "1. Add Expense\n";
    std::cout << "2. View Expenses\n";
    std::cout << "3. Delete Expense\n";
    std::cout << "4. Filter by Category\n";
    std::cout << "5. Filter by Date Range\n";
    std::cout << "6. Summary Report\n";
    std::cout << "7. Save Expenses (CSV/JSON)\n";
    std::cout << "8. Load Expenses (CSV/JSON)\n";
    std::cout << "9. Exit\n";
}

/**
 * @brief Validates that the amount string is a positive, finite number.
 *
 * @param amountStr The amount as a string.
 * @return True if valid, false otherwise.
 */
bool isValidAmount(const std::string &amountStr) {
    // Only allow numbers with optional decimal, no trailing/leading junk
    if (amountStr.empty()) return false;
    char* endptr = nullptr;
    double val = strtod(amountStr.c_str(), &endptr);
    // Check: valid double, no trailing chars, positive, finite
    return endptr == amountStr.c_str() + amountStr.size() && val > 0 && std::isfinite(val);
}

/**
 * @brief Validates that the date string is in YYYY-MM-DD format and is a real date.
 *
 * @param dateStr The date as a string.
 * @return True if valid, false otherwise.
 */
bool isValidDate(const std::string &dateStr) {
    if (dateStr.size() != 10) return false;
    if (dateStr[4] != '-' || dateStr[7] != '-') return false;
    int y, m, d;
    try {
        y = std::stoi(dateStr.substr(0,4));
        m = std::stoi(dateStr.substr(5,2));
        d = std::stoi(dateStr.substr(8,2));
    } catch (...) { return false; }
    if (y < 1900 || m < 1 || m > 12 || d < 1 || d > 31) return false;
    // Simple check for days in month
    static const int days_in_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int maxd = days_in_month[m-1];
    if (m == 2 && ((y%4==0 && y%100!=0) || (y%400==0))) maxd = 29;
    return d <= maxd;
}

/**
 * @brief Validates that the category is one of the supported categories.
 *
 * @param category The category as a string.
 * @return True if valid, false otherwise.
 */
bool isValidCategory(const std::string &category) {
    static const std::string cats[] = {
        "Housing", "Transportation", "Food & Dining", "Utilities & Communication",
        "Healthcare & Insurance", "Personal & Debt", "Other"
    };
    for (const auto &c : cats) if (c == category) return true;
    return false;
}

/**
 * @brief Validates that the description is non-empty and <= 200 characters.
 *
 * @param desc The description string.
 * @return True if valid, false otherwise.
 */
bool isValidDescription(const std::string &desc) {
    return !desc.empty() && desc.size() <= 200;
}

/**
 * @brief Filters expenses by category.
 *
 * @param expenses The list of expenses.
 * @param category The category to filter by.
 * @return A vector of expenses matching the category.
 */
std::vector<Expense> filterByCategory(const std::vector<Expense> &expenses, const std::string &category) {
    std::vector<Expense> out;
    for (const auto &e : expenses) if (e.category == category) out.push_back(e);
    return out;
}

/**
 * @brief Filters expenses by date.
 *
 * @param expenses The list of expenses.
 * @param dateStr The date to filter by (YYYY-MM-DD).
 * @return A vector of expenses matching the date.
 */
std::vector<Expense> filterByDate(const std::vector<Expense> &expenses, const std::string &dateStr) {
    std::vector<Expense> out;
    for (const auto &e : expenses) if (e.date == dateStr) out.push_back(e);
    return out;
}

/**
 * @brief Calculates the total amount of all expenses.
 *
 * @param expenses The list of expenses.
 * @return The sum of all expense amounts.
 */
double getTotalExpenses(const std::vector<Expense> &expenses) {
    double sum = 0.0;
    for (const auto &e : expenses) sum += e.amount;
    return sum;
}

/**
 * @brief Calculates the total amount spent per category.
 *
 * @param expenses The list of expenses.
 * @return A map from category name to total amount spent in that category.
 */
std::map<std::string, double> getCategoryTotals(const std::vector<Expense> &expenses) {
    std::map<std::string, double> totals;
    for (const auto &e : expenses) totals[e.category] += e.amount;
    return totals;
}

/**
 * @brief Prompt user to choose file type and filename for saving expenses.
 *        Supports CSV and JSON. Persistent file is separate from preload files.
 */
void saveExpensesMenu(const std::vector<Expense> &expenses, std::string &currentDataFile, std::string &currentFileType) {
    std::cout << "Save as: 1) CSV  2) JSON\nChoice: ";
    int type; std::string typeInput;
    std::getline(std::cin, typeInput);
    std::stringstream ss(typeInput);
    ss >> type;
    std::string filename;
    if (type == 1) {
        std::cout << "Enter filename (default: expenses_persistent.csv): ";
        std::getline(std::cin, filename);
        if (filename.empty()) filename = "expenses_persistent.csv";
        saveExpensesCSV(expenses, filename);
        currentDataFile = filename;
        currentFileType = "csv";
    } else if (type == 2) {
        std::cout << "Enter filename (default: expenses_persistent.json): ";
        std::getline(std::cin, filename);
        if (filename.empty()) filename = "expenses_persistent.json";
        saveExpensesJSON(expenses, filename);
        currentDataFile = filename;
        currentFileType = "json";
    } else {
        std::cout << "Invalid choice.\n";
    }
}

/**
 * @brief Prompt user to choose file type and filename for loading expenses.
 *        Supports CSV and JSON. Persistent file is separate from preload files.
 */
void loadExpensesMenu(std::vector<Expense> &expenses, std::string &currentDataFile, std::string &currentFileType) {
    std::cout << "Load from: 1) CSV  2) JSON\nChoice: ";
    int type; std::string typeInput;
    std::getline(std::cin, typeInput);
    std::stringstream ss(typeInput);
    ss >> type;
    std::string filename;
    if (type == 1) {
        std::cout << "Enter filename (default: expenses_persistent.csv): ";
        std::getline(std::cin, filename);
        if (filename.empty()) filename = "expenses_persistent.csv";
        loadExpensesCSV(expenses, filename);
        currentDataFile = filename;
        currentFileType = "csv";
    } else if (type == 2) {
        std::cout << "Enter filename (default: expenses_persistent.json): ";
        std::getline(std::cin, filename);
        if (filename.empty()) filename = "expenses_persistent.json";
        loadExpensesJSON(expenses, filename);
        currentDataFile = filename;
        currentFileType = "json";
    } else {
        std::cout << "Invalid choice.\n";
    }
}
