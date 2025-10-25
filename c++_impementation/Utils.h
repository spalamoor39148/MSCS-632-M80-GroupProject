#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <ctime>

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

#endif
