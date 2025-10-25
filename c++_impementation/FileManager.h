#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "Expense.h"
#include <vector>

void saveExpensesCSV(const std::vector<Expense> &expenses);
void loadExpensesCSV(std::vector<Expense> &expenses);
void saveExpensesJSON(const std::vector<Expense> &expenses);
void loadExpensesJSON(std::vector<Expense> &expenses);

#endif
