#ifndef EXPENSE_H
#define EXPENSE_H

#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Represents a single expense record.
 *
 * Fields:
 *   - id: Unique identifier for the expense.
 *   - date: Date of the expense (YYYY-MM-DD).
 *   - amount: Amount spent.
 *   - category: Expense category.
 *   - description: Description of the expense.
 */
struct Expense {
    int id;
    std::string date;
    double amount;
    std::string category;
    std::string description;
};

/**
 * @brief Global variable to assign unique IDs to expenses.
 */
extern int nextID;

// Expense-related function declarations
/**
 * @brief Prompt the user for all fields and add a new expense to the list.
 * @param expenses Reference to the vector of all expenses.
 */
void addExpense(std::vector<Expense> &expenses);

/**
 * @brief Display all expenses in a formatted table with IDs, dates, amounts, categories, and descriptions.
 * @param expenses Const reference to the vector of all expenses.
 */
void viewExpenses(const std::vector<Expense> &expenses);

/**
 * @brief Delete an expense by its unique ID, with user prompt and validation.
 * @param expenses Reference to the vector of all expenses.
 */
void deleteExpense(std::vector<Expense> &expenses);

/**
 * @brief Prompt the user to select a category and display only matching expenses.
 * @param expenses Const reference to the vector of all expenses.
 */
void filterByCategory(const std::vector<Expense> &expenses);

/**
 * @brief Prompt the user for a start and end date, and display expenses in that range.
 *        Dates are expected in YYYY-MM-DD format.
 * @param expenses Const reference to the vector of all expenses.
 */
void filterByDateRange(const std::vector<Expense> &expenses);

/**
 * @brief Print a summary report of total expenses and subtotals for each category.
 * @param expenses Const reference to the vector of all expenses.
 */
void summaryReport(const std::vector<Expense> &expenses);

#endif
