#include "Expense.h"
#include "FileManager.h"
#include "Utils.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <filesystem>

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
    std::string currentDataFile = "";
    std::string currentFileType = ""; // "csv" or "json"
    int choice;

    // Auto-load persistent file if it exists
    if (std::filesystem::exists("expenses_persistent.csv")) {
        loadExpensesCSV(expenses, "expenses_persistent.csv");
        currentDataFile = "expenses_persistent.csv";
        currentFileType = "csv";
    } else if (std::filesystem::exists("expenses_persistent.json")) {
        loadExpensesJSON(expenses, "expenses_persistent.json");
        currentDataFile = "expenses_persistent.json";
        currentFileType = "json";
    }

    std::cout << "\n=====================================\n";
    std::cout << "         Expense Tracker CLI          \n";
    std::cout << "=====================================\n";

    do {
        displayMenu();
        std::cout << "Enter your choice: ";
        std::string choiceInput;
        std::getline(std::cin, choiceInput);
        std::stringstream ss(choiceInput);
        ss >> choice;

        switch (choice) {
            case 1:
                addExpense(expenses);
                if (!currentDataFile.empty() && !currentFileType.empty()) {
                    if (currentFileType == "csv") saveExpensesCSV(expenses, currentDataFile);
                    else if (currentFileType == "json") saveExpensesJSON(expenses, currentDataFile);
                }
                break;
            case 2: viewExpenses(expenses, currentDataFile.empty() ? "(unsaved/manual entry)" : currentDataFile); break;
            case 3:
                deleteExpense(expenses);
                if (!currentDataFile.empty() && !currentFileType.empty()) {
                    if (currentFileType == "csv") saveExpensesCSV(expenses, currentDataFile);
                    else if (currentFileType == "json") saveExpensesJSON(expenses, currentDataFile);
                }
                break;
            case 4: filterByCategory(expenses); break;
            case 5: filterByDateRange(expenses); break;
            case 6: summaryReport(expenses); break;
            case 7: saveExpensesMenu(expenses, currentDataFile, currentFileType); break;
            case 8: loadExpensesMenu(expenses, currentDataFile, currentFileType); break;
            case 9:
                if (!currentDataFile.empty() && !currentFileType.empty()) {
                    if (currentFileType == "csv") saveExpensesCSV(expenses, currentDataFile);
                    else if (currentFileType == "json") saveExpensesJSON(expenses, currentDataFile);
                }
                std::cout << "Exiting program. Goodbye!\n";
                break;
            default: std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 9);

    return 0;
}
