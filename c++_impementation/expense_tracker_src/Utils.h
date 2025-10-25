#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <ctime>
#include <vector>
#include <map>
#include "Expense.h"

/**
 * @brief Displays the list of available expense categories with their corresponding numbers.
 *
 * Prints all supported categories to the standard output, allowing users to select a category by number.
 */
void displayCategories();

/**
 * @brief Returns the category name corresponding to a given number.
 *
 * @param num The category number (1-7).
 * @return The category name as a string, or an empty string if the number is invalid.
 */
std::string getCategoryByNumber(int num);

/**
 * @brief Parses a date string in YYYY-MM-DD format to a time_t value.
 *
 * @param dateStr The date string in "YYYY-MM-DD" format.
 * @return The corresponding time_t value, or -1 if parsing fails.
 */
time_t parseDate(const std::string &dateStr);

/**
 * @brief Displays the main menu for the expense tracker CLI application.
 *
 * Prints all available user actions to the standard output.
 */
void displayMenu();

/**
 * @brief Validates that the amount string is a positive, finite number.
 *
 * @param amountStr The amount as a string.
 * @return True if valid, false otherwise.
 */
bool isValidAmount(const std::string &amountStr);

/**
 * @brief Validates that the date string is in YYYY-MM-DD format and is a real date.
 *
 * @param dateStr The date as a string.
 * @return True if valid, false otherwise.
 */
bool isValidDate(const std::string &dateStr);

/**
 * @brief Validates that the category is one of the supported categories.
 *
 * @param category The category as a string.
 * @return True if valid, false otherwise.
 */
bool isValidCategory(const std::string &category);

/**
 * @brief Validates that the description is non-empty and <= 200 characters.
 *
 * @param desc The description string.
 * @return True if valid, false otherwise.
 */
bool isValidDescription(const std::string &desc);

/**
 * @brief Filters expenses by category.
 *
 * @param expenses The list of expenses.
 * @param category The category to filter by.
 * @return A vector of expenses matching the category.
 */
std::vector<Expense> filterByCategory(const std::vector<Expense> &expenses, const std::string &category);

/**
 * @brief Filters expenses by date.
 *
 * @param expenses The list of expenses.
 * @param dateStr The date to filter by (YYYY-MM-DD).
 * @return A vector of expenses matching the date.
 */
std::vector<Expense> filterByDate(const std::vector<Expense> &expenses, const std::string &dateStr);

/**
 * @brief Calculates the total amount of all expenses.
 *
 * @param expenses The list of expenses.
 * @return The sum of all expense amounts.
 */
double getTotalExpenses(const std::vector<Expense> &expenses);

/**
 * @brief Calculates the total amount spent per category.
 *
 * @param expenses The list of expenses.
 * @return A map from category name to total amount spent in that category.
 */
std::map<std::string, double> getCategoryTotals(const std::vector<Expense> &expenses);

/**
 * @brief Saves the list of expenses to a file.
 *
 * @param expenses The list of expenses to save.
 * @param currentDataFile The current data file being used.
 * @param currentFileType The current file type being used.
 */
void saveExpensesMenu(const std::vector<Expense> &expenses, std::string &currentDataFile, std::string &currentFileType);

/**
 * @brief Loads the list of expenses from a file.
 *
 * @param expenses The list of expenses to load into.
 * @param currentDataFile The current data file being used.
 * @param currentFileType The current file type being used.
 */
void loadExpensesMenu(std::vector<Expense> &expenses, std::string &currentDataFile, std::string &currentFileType);

/**
 * @brief Displays the list of expenses.
 *
 * @param expenses The list of expenses to display.
 * @param currentDataFile The current data file being used.
 */
void viewExpenses(const std::vector<Expense> &expenses, const std::string &currentDataFile);

#endif
