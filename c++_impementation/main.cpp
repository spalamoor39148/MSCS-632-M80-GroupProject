#include "Expense.h"
#include "FileManager.h"
#include "Utils.h"
#include <vector>
#include <iostream>

/**
 * @brief Entry point for the Expense Tracker CLI application.
 *
 * Initializes the main expense list and handles the main menu loop.
 * Presents the user with all available options and dispatches to the appropriate
 * function for each menu choice. Handles input validation for menu selection.
 *
 * @return int Exit status code (0 for success).
 */
int main() {
    std::vector<Expense> expenses;
    int choice;

    std::cout << "\n=====================================\n";
    std::cout << "         Expense Tracker CLI          \n";
    std::cout << "=====================================\n";

    do {
        displayMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: addExpense(expenses); break;
            case 2: viewExpenses(expenses); break;
            case 3: deleteExpense(expenses); break;
            case 4: filterByCategory(expenses); break;
            case 5: filterByDateRange(expenses); break;
            case 6: summaryReport(expenses); break;
            case 7: saveExpensesCSV(expenses); break;
            case 8: loadExpensesCSV(expenses); break;
            case 9: saveExpensesJSON(expenses); break;
            case 10: loadExpensesJSON(expenses); break;
            case 11: std::cout << "Exiting program. Goodbye!\n"; break;
            default: std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 11);

    return 0;
}
