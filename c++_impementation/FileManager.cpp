#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Global variable for unique expense IDs (defined elsewhere)
extern int nextID;

/**
 * @brief Save all expenses to a CSV file (expenses.csv).
 * @param expenses Const reference to the vector of all expenses.
 */
void saveExpensesCSV(const std::vector<Expense> &expenses) {
    std::ofstream out("expenses.csv");
    out << "ID,Date,Amount,Category,Description\n";
    for (const auto &e : expenses) {
        out << e.id << "," << e.date << "," << e.amount << ","
            << e.category << "," << e.description << "\n";
    }
    out.close();
    std::cout << "✅ Expenses saved to expenses.csv\n";
}

/**
 * @brief Load expenses from a CSV file (expenses.csv) into the expenses vector.
 *        Updates nextID to ensure unique IDs for new expenses.
 * @param expenses Reference to the vector of all expenses.
 */
void loadExpensesCSV(std::vector<Expense> &expenses) {
    std::ifstream in("expenses.csv");
    if (!in.is_open()) {
        std::cout << "❌ No CSV file found.\n";
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
    std::cout << "✅ Expenses loaded from expenses.csv\n";
}

/**
 * @brief Save all expenses to a JSON file (expenses.json) using nlohmann::json.
 * @param expenses Const reference to the vector of all expenses.
 */
void saveExpensesJSON(const std::vector<Expense> &expenses) {
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

    std::ofstream file("expenses.json");
    file << std::setw(4) << jExpenses;
    std::cout << "✅ Expenses saved to expenses.json\n";
}

/**
 * @brief Load expenses from a JSON file (expenses.json) into the expenses vector.
 *        Updates nextID to ensure unique IDs for new expenses.
 * @param expenses Reference to the vector of all expenses.
 */
void loadExpensesJSON(std::vector<Expense> &expenses) {
    std::ifstream file("expenses.json");
    if (!file.is_open()) {
        std::cout << "❌ No JSON file found.\n";
        return;
    }

    json jExpenses;
    file >> jExpenses;

    for (const auto &item : jExpenses) {
        Expense e;
        e.id = item.at("id").get<int>();
        e.date = item.at("date").get<std::string>();
        e.amount = item.at("amount").get<double>();
        e.category = item.at("category").get<std::string>();
        e.description = item.at("description").get<std::string>();

        expenses.push_back(e);
        if (e.id >= nextID)
            nextID = e.id + 1;
    }

    std::cout << "✅ Expenses loaded from expenses.json\n";
}
