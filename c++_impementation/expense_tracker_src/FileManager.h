#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "Expense.h"
#include <vector>
#include <string>

/**
 * @brief Saves the list of expenses to a CSV file.
 *
 * @param expenses The vector of Expense objects to be saved.
 */
void saveExpensesCSV(const std::vector<Expense> &expenses);

/**
 * @brief Loads expenses from a CSV file into the provided vector.
 *
 * @param expenses The vector to populate with loaded Expense objects.
 */
void loadExpensesCSV(std::vector<Expense> &expenses);

/**
 * @brief Saves the list of expenses to a JSON file.
 *
 * @param expenses The vector of Expense objects to be saved.
 */
void saveExpensesJSON(const std::vector<Expense> &expenses);

/**
 * @brief Loads expenses from a JSON file into the provided vector.
 *
 * @param expenses The vector to populate with loaded Expense objects.
 */
void loadExpensesJSON(std::vector<Expense> &expenses);

/**
 * @brief Saves the list of expenses to a CSV file with a custom filename.
 *
 * @param expenses The vector of Expense objects to be saved.
 * @param filename The name of the file to save the expenses to.
 */
void saveExpensesCSV(const std::vector<Expense> &expenses, const std::string &filename);

/**
 * @brief Loads expenses from a CSV file with a custom filename into the provided vector.
 *
 * @param expenses The vector to populate with loaded Expense objects.
 * @param filename The name of the file to load the expenses from.
 */
void loadExpensesCSV(std::vector<Expense> &expenses, const std::string &filename);

/**
 * @brief Saves the list of expenses to a JSON file with a custom filename.
 *
 * @param expenses The vector of Expense objects to be saved.
 * @param filename The name of the file to save the expenses to.
 */
void saveExpensesJSON(const std::vector<Expense> &expenses, const std::string &filename);

/**
 * @brief Loads expenses from a JSON file with a custom filename into the provided vector.
 *
 * @param expenses The vector to populate with loaded Expense objects.
 * @param filename The name of the file to load the expenses from.
 */
void loadExpensesJSON(std::vector<Expense> &expenses, const std::string &filename);

#endif
