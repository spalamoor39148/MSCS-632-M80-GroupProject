#ifndef EXPENSE_H
#define EXPENSE_H

#include <iostream>
#include <string>
#include <vector>

struct Expense {
    int id;
    std::string date;
    double amount;
    std::string category;
    std::string description;
};

extern int nextID;

// Expense-related function declarations
void addExpense(std::vector<Expense> &expenses);
void viewExpenses(const std::vector<Expense> &expenses);
void deleteExpense(std::vector<Expense> &expenses);
void filterByCategory(const std::vector<Expense> &expenses);
void filterByDateRange(const std::vector<Expense> &expenses);
void summaryReport(const std::vector<Expense> &expenses);

#endif
