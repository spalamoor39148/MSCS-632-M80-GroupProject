// Expense.cpp - Core expense operations for the CLI Expense Tracker
#include "Expense.h"
#include "Utils.h"
#include <iomanip>
#include <algorithm>
#include <unordered_map>
#include <sstream>

// Global variable to assign unique IDs to expenses
int nextID = 1;

/**
 * @brief Prompt the user for all fields and add a new expense to the list.
 *        Handles input validation for category selection.
 * @param expenses Reference to the vector of all expenses.
 */
void addExpense(std::vector<Expense> &expenses) {
    Expense e;
    e.id = nextID++;
    // Date validation
    do {
        std::cout << "Enter date (YYYY-MM-DD): ";
        std::getline(std::cin, e.date);
        if (!isValidDate(e.date)) {
            std::cout << "❌ Invalid date format or value. Please enter a valid date in YYYY-MM-DD format.\n";
        }
    } while (!isValidDate(e.date));

    // Amount validation
    std::string amountStr;
    do {
        std::cout << "Enter amount: ";
        std::getline(std::cin, amountStr);
        if (!isValidAmount(amountStr)) {
            std::cout << "❌ Invalid amount. Please enter a positive number.\n";
        }
    } while (!isValidAmount(amountStr));
    e.amount = std::stod(amountStr);

    // Category validation (robust, string input)
    int catChoice = 0;
    std::string catInput;
    displayCategories();
    while (true) {
        std::cout << "Select category by number (1–7): ";
        std::getline(std::cin, catInput);
        std::stringstream ss(catInput);
        if (ss >> catChoice && catChoice >= 1 && catChoice <= 7) break;
        std::cout << "Invalid number. Please select 1–7.\n";
    }
    e.category = getCategoryByNumber(catChoice);

    std::cout << "Enter description: ";
    std::getline(std::cin, e.description);

    expenses.push_back(e);
    std::cout << "\n✅ Expense added successfully! (ID: " << e.id << ")\n";
}

/**
 * @brief Display all expenses in a formatted table with IDs, dates, amounts, categories, and descriptions.
 * @param expenses Const reference to the vector of all expenses.
 */
void viewExpenses(const std::vector<Expense> &expenses, const std::string &currentDataFile) {
    if (expenses.empty()) {
        std::cout << "No expenses found.\n";
        return;
    }
    std::cout << "\n---------------- ALL EXPENSES ----------------\n";
    std::cout << "(Current data file: " << currentDataFile << ")\n";
    std::cout << std::left << std::setw(5) << "ID" << std::setw(12) << "Date"
              << std::setw(12) << "Amount" << std::setw(25) << "Category"
              << "Description\n";
    std::cout << "---------------------------------------------------------------\n";
    for (const auto &e : expenses) {
        std::cout << std::left << std::setw(5) << e.id
                  << std::setw(12) << e.date
                  << std::setw(12) << std::fixed << std::setprecision(2) << e.amount
                  << std::setw(25) << e.category
                  << e.description << "\n";
    }
}

/**
 * @brief Delete an expense by its unique ID, with user prompt and validation.
 * @param expenses Reference to the vector of all expenses.
 */
void deleteExpense(std::vector<Expense> &expenses) {
    if (expenses.empty()) {
        std::cout << "No expenses to delete.\n";
        return;
    }
    std::string idInput;
    int id = 0;
    while (true) {
        std::cout << "Enter Expense ID to delete: ";
        std::getline(std::cin, idInput);
        std::stringstream ss(idInput);
        if (ss >> id && id > 0) break;
        std::cout << "Invalid ID. Please enter a positive number.\n";
    }
    auto it = std::find_if(expenses.begin(), expenses.end(),
                           [id](const Expense &e) { return e.id == id; });
    if (it != expenses.end()) {
        expenses.erase(it);
        std::cout << "✅ Expense with ID " << id << " deleted successfully.\n";
    } else {
        std::cout << "❌ Expense with ID " << id << " not found.\n";
    }
}

/**
 * @brief Prompt the user to select a category and display only matching expenses.
 * @param expenses Const reference to the vector of all expenses.
 */
void filterByCategory(const std::vector<Expense> &expenses) {
    if (expenses.empty()) {
        std::cout << "No expenses available.\n";
        return;
    }
    int catChoice = 0;
    std::string catInput;
    displayCategories();
    while (true) {
        std::cout << "Select category number to filter: ";
        std::getline(std::cin, catInput);
        std::stringstream ss(catInput);
        if (ss >> catChoice && catChoice >= 1 && catChoice <= 7) break;
        std::cout << "Invalid number. Please select 1–7.\n";
    }
    std::string cat = getCategoryByNumber(catChoice);
    if (cat.empty()) {
        std::cout << "Invalid selection.\n";
        return;
    }
    std::cout << "\nExpenses in category: " << cat << "\n";
    bool found = false;
    for (const auto &e : expenses) {
        if (e.category == cat) {
            std::cout << std::left << std::setw(5) << e.id
                      << std::setw(12) << e.date
                      << std::setw(12) << e.amount << e.description << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "No expenses found in this category.\n";
}

/**
 * @brief Prompt the user for a start and end date, and display expenses in that range.
 *        Dates are expected in YYYY-MM-DD format.
 * @param expenses Const reference to the vector of all expenses.
 */
void filterByDateRange(const std::vector<Expense> &expenses) {
    if (expenses.empty()) {
        std::cout << "No expenses available.\n";
        return;
    }
    std::string start, end;
    do {
        std::cout << "Enter start date (YYYY-MM-DD): ";
        std::getline(std::cin, start);
        if (!isValidDate(start)) {
            std::cout << "❌ Invalid date format or value. Please enter a valid date in YYYY-MM-DD format.\n";
        }
    } while (!isValidDate(start));
    do {
        std::cout << "Enter end date (YYYY-MM-DD): ";
        std::getline(std::cin, end);
        if (!isValidDate(end)) {
            std::cout << "❌ Invalid date format or value. Please enter a valid date in YYYY-MM-DD format.\n";
        }
    } while (!isValidDate(end));
    time_t startTime = parseDate(start);
    time_t endTime = parseDate(end);
    std::cout << "\nExpenses between " << start << " and " << end << ":\n";
    bool found = false;
    for (const auto &e : expenses) {
        time_t d = parseDate(e.date);
        if (d >= startTime && d <= endTime) {
            std::cout << std::left << std::setw(5) << e.id
                      << std::setw(12) << e.date
                      << std::setw(12) << e.amount
                      << std::setw(25) << e.category
                      << e.description << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "No expenses found in this range.\n";
}

/**
 * @brief Print a summary report of total expenses and subtotals for each category.
 * @param expenses Const reference to the vector of all expenses.
 */
void summaryReport(const std::vector<Expense> &expenses) {
    if (expenses.empty()) {
        std::cout << "No expenses recorded.\n";
        return;
    }

    std::unordered_map<std::string, double> totals;
    double grandTotal = 0;

    for (const auto &e : expenses) {
        totals[e.category] += e.amount;
        grandTotal += e.amount;
    }

    std::cout << "\n------- Summary by Category -------\n";
    for (const auto &pair : totals) {
        std::cout << std::left << std::setw(25) << pair.first
                  << "$" << std::fixed << std::setprecision(2)
                  << pair.second << "\n";
    }
    std::cout << "-----------------------------------\n";
    std::cout << std::left << std::setw(25) << "Total"
              << "$" << grandTotal << "\n";
}
