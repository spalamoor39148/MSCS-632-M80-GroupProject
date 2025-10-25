#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include "json.hpp" // Use local header for nlohmann/json

using json = nlohmann::json;

// Global variable for unique expense IDs (defined elsewhere)
extern int nextID;

/**
 * @brief Save all expenses to a CSV file (expenses.csv).
 *
 * @param expenses Const reference to the vector of all expenses.
 * Writes each expense as a row in the CSV file with headers.
 */
void saveExpensesCSV(const std::vector<Expense> &expenses) {
    saveExpensesCSV(expenses, "expenses.csv");
}

/**
 * @brief Save all expenses to a CSV file with a specified filename.
 *
 * @param expenses Const reference to the vector of all expenses.
 * @param filename The name of the file to save the expenses to.
 * Writes each expense as a row in the CSV file with headers.
 */
void saveExpensesCSV(const std::vector<Expense> &expenses, const std::string &filename) {
    std::ofstream out(filename);
    out << "ID,Date,Amount,Category,Description\n";
    for (const auto &e : expenses) {
        out << e.id << "," << e.date << "," << e.amount << ","
            << e.category << "," << e.description << "\n";
    }
    out.close();
    std::cout << "✅ Expenses saved to " << filename << "\n";
}

/**
 * @brief Load expenses from a CSV file (expenses.csv) into the expenses vector.
 *        Updates nextID to ensure unique IDs for new expenses.
 * @param expenses Reference to the vector of all expenses.
 * Reads each row and parses fields into Expense objects.
 */
void loadExpensesCSV(std::vector<Expense> &expenses) {
    loadExpensesCSV(expenses, "expenses.csv");
}

/**
 * @brief Load expenses from a CSV file with a specified filename into the expenses vector.
 *        Updates nextID to ensure unique IDs for new expenses.
 * @param expenses Reference to the vector of all expenses.
 * @param filename The name of the file to load the expenses from.
 * Reads each row and parses fields into Expense objects.
 */
void loadExpensesCSV(std::vector<Expense> &expenses, const std::string &filename) {
    expenses.clear();
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cout << "❌ No CSV file found: " << filename << "\n";
        return;
    }
    std::string line;
    getline(in, line); // skip header
    while (getline(in, line)) {
        std::stringstream ss(line);
        Expense e;
        std::string idStr, amountStr;
        getline(ss, idStr, ',');
        getline(ss, e.date, ',');
        getline(ss, amountStr, ',');
        getline(ss, e.category, ',');
        getline(ss, e.description);
        e.id = stoi(idStr);
        e.amount = stod(amountStr);
        expenses.push_back(e);
        if (e.id >= nextID)
            nextID = e.id + 1;
    }
    in.close();
    std::cout << "✅ Expenses loaded from " << filename << "\n";
}

/**
 * @brief Save all expenses to a JSON file (expenses.json) using nlohmann::json.
 *
 * @param expenses Const reference to the vector of all expenses.
 * Serializes all expenses as an array of objects in JSON format.
 */
void saveExpensesJSON(const std::vector<Expense> &expenses) {
    saveExpensesJSON(expenses, "expenses.json");
}

/**
 * @brief Save all expenses to a JSON file with a specified filename using nlohmann::json.
 *
 * @param expenses Const reference to the vector of all expenses.
 * @param filename The name of the file to save the expenses to.
 * Serializes all expenses as an array of objects in JSON format.
 */
void saveExpensesJSON(const std::vector<Expense> &expenses, const std::string &filename) {
    json jExpenses = json::array();
    for (const auto &e : expenses) {
        jExpenses.push_back({
            {"id", e.id},
            {"date", e.date},
            {"amount", e.amount},
            {"category", e.category},
            {"description", e.description}
        });
    }
    std::ofstream file(filename);
    file << std::setw(4) << jExpenses;
    std::cout << "✅ Expenses saved to " << filename << "\n";
}

/**
 * @brief Load expenses from a JSON file (expenses.json) into the expenses vector.
 *        Updates nextID to ensure unique IDs for new expenses.
 * @param expenses Reference to the vector of all expenses.
 * Reads and parses the JSON array, handling errors gracefully.
 */
void loadExpensesJSON(std::vector<Expense> &expenses) {
    loadExpensesJSON(expenses, "expenses.json");
}

/**
 * @brief Load expenses from a JSON file with a specified filename into the expenses vector.
 *        Updates nextID to ensure unique IDs for new expenses.
 * @param expenses Reference to the vector of all expenses.
 * @param filename The name of the file to load the expenses from.
 * Reads and parses the JSON array, handling errors gracefully.
 */
void loadExpensesJSON(std::vector<Expense> &expenses, const std::string &filename) {
    expenses.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "❌ No JSON file found: " << filename << "\n";
        return;
    }
    try {
        json jExpenses;
        file >> jExpenses;
        for (const auto &item : jExpenses) {
            Expense e;
            item.at("id").get_to(e.id);
            item.at("date").get_to(e.date);
            item.at("amount").get_to(e.amount);
            item.at("category").get_to(e.category);
            item.at("description").get_to(e.description);
            expenses.push_back(e);
            if (e.id >= nextID)
                nextID = e.id + 1;
        }
    } catch (const json::exception &) {
        expenses.clear();
        std::cout << "❌ Error parsing JSON file: " << filename << "\n";
    }
    std::cout << "✅ Expenses loaded from " << filename << "\n";
}
