#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <ctime>
#include <algorithm>
using namespace std;

struct Expense {
    int id;
    string date;          
    double amount;
    string category;
    string description;
};

// Global ID counter
int nextID = 1;

// Function declarations
void addExpense(vector<Expense> &expenses);
void viewExpenses(const vector<Expense> &expenses);
void deleteExpense(vector<Expense> &expenses);
void filterByCategory(const vector<Expense> &expenses);
void filterByDateRange(const vector<Expense> &expenses);
void summaryReport(const vector<Expense> &expenses);
void saveExpenses(const vector<Expense> &expenses);
void loadExpenses(vector<Expense> &expenses);
time_t parseDate(const string &dateStr);
string getCategoryByNumber(int num);
void displayCategories();
void displayMenu();

int main() {
    vector<Expense> expenses;
    int choice;

    cout << "\n=====================================\n";
    cout << "         Expense Tracker CLI          \n";
    cout << "=====================================\n";

    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addExpense(expenses); break;
            case 2: viewExpenses(expenses); break;
            case 3: deleteExpense(expenses); break;
            case 4: filterByCategory(expenses); break;
            case 5: filterByDateRange(expenses); break;
            case 6: summaryReport(expenses); break;
            case 7: saveExpenses(expenses); break;
            case 8: loadExpenses(expenses); break;
            case 9: cout << "Exiting program. Goodbye!\n"; break;
            default: cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 9);

    return 0;
}

// --------------------------- MENU ----------------------------
void displayMenu() {
    cout << "\n------------- MENU -------------\n";
    cout << "1. Add Expense\n";
    cout << "2. View All Expenses\n";
    cout << "3. Delete Expense\n";
    cout << "4. Filter by Category\n";
    cout << "5. Filter by Date Range\n";
    cout << "6. Generate Summary Report\n";
    cout << "7. Save Expenses\n";
    cout << "8. Load Expenses from CSV\n";
    cout << "9. Exit\n";
    cout << "--------------------------------\n";
}

// --------------------------- CATEGORY UTILS ----------------------------
void displayCategories() {
    cout << "\nAvailable Categories:\n";
    cout << "1. Housing\n";
    cout << "2. Transportation\n";
    cout << "3. Food & Dining\n";
    cout << "4. Utilities & Communication\n";
    cout << "5. Healthcare & Insurance\n";
    cout << "6. Personal & Debt\n";
    cout << "7. Other\n";
}

string getCategoryByNumber(int num) {
    switch (num) {
        case 1: return "Housing";
        case 2: return "Transportation";
        case 3: return "Food & Dining";
        case 4: return "Utilities & Communication";
        case 5: return "Healthcare & Insurance";
        case 6: return "Personal & Debt";
        case 7: return "Other";
        default: return "";
    }
}

// --------------------------- ADD EXPENSE ----------------------------
void addExpense(vector<Expense> &expenses) {
    Expense e;
    e.id = nextID++;
    cout << "Enter date (YYYY-MM-DD): ";
    getline(cin, e.date);

    cout << "Enter amount: ";
    cin >> e.amount;
    cin.ignore();

    int catChoice;
    displayCategories();
    cout << "Select category by number (1–7): ";
    cin >> catChoice;
    cin.ignore();

    e.category = getCategoryByNumber(catChoice);
    while (e.category.empty()) {
        cout << "Invalid number. Please select 1–7: ";
        cin >> catChoice;
        cin.ignore();
        e.category = getCategoryByNumber(catChoice);
    }

    cout << "Enter description: ";
    getline(cin, e.description);

    expenses.push_back(e);
    cout << "\n✅ Expense added successfully! (ID: " << e.id << ")\n";
}

// --------------------------- VIEW EXPENSES ----------------------------
void viewExpenses(const vector<Expense> &expenses) {
    if (expenses.empty()) {
        cout << "No expenses found.\n";
        return;
    }

    cout << "\n---------------- ALL EXPENSES ----------------\n";
    cout << left << setw(5) << "ID" << setw(12) << "Date" << setw(12) << "Amount" 
         << setw(25) << "Category" << "Description\n";
    cout << "---------------------------------------------------------------\n";

    for (const auto &e : expenses) {
        cout << left << setw(5) << e.id 
             << setw(12) << e.date 
             << setw(12) << fixed << setprecision(2) << e.amount
             << setw(25) << e.category 
             << e.description << "\n";
    }
}

// --------------------------- DELETE EXPENSE ----------------------------
void deleteExpense(vector<Expense> &expenses) {
    if (expenses.empty()) {
        cout << "No expenses to delete.\n";
        return;
    }

    int id;
    cout << "Enter Expense ID to delete: ";
    cin >> id;

    auto it = find_if(expenses.begin(), expenses.end(),
                      [id](const Expense &e) { return e.id == id; });

    if (it != expenses.end()) {
        expenses.erase(it);
        cout << "✅ Expense with ID " << id << " deleted successfully.\n";
    } else {
        cout << "❌ Expense with ID " << id << " not found.\n";
    }
}

// --------------------------- FILTER BY CATEGORY ----------------------------
void filterByCategory(const vector<Expense> &expenses) {
    if (expenses.empty()) {
        cout << "No expenses available.\n";
        return;
    }

    int catChoice;
    displayCategories();
    cout << "Select category number to filter: ";
    cin >> catChoice;
    cin.ignore();

    string cat = getCategoryByNumber(catChoice);
    if (cat.empty()) {
        cout << "Invalid selection.\n";
        return;
    }

    cout << "\nExpenses in category: " << cat << "\n";
    bool found = false;
    for (const auto &e : expenses) {
        if (e.category == cat) {
            cout << left << setw(5) << e.id << setw(12) << e.date 
                 << setw(12) << e.amount << e.description << "\n";
            found = true;
        }
    }
    if (!found) cout << "No expenses found in this category.\n";
}

// --------------------------- DATE RANGE FILTER ----------------------------
time_t parseDate(const string &dateStr) {
    tm t = {};
    istringstream ss(dateStr);
    ss >> get_time(&t, "%Y-%m-%d");
    return mktime(&t);
}

void filterByDateRange(const vector<Expense> &expenses) {
    if (expenses.empty()) {
        cout << "No expenses available.\n";
        return;
    }

    string start, end;
    cout << "Enter start date (YYYY-MM-DD): ";
    getline(cin, start);
    cout << "Enter end date (YYYY-MM-DD): ";
    getline(cin, end);

    time_t startTime = parseDate(start);
    time_t endTime = parseDate(end);

    cout << "\nExpenses between " << start << " and " << end << ":\n";
    bool found = false;
    for (const auto &e : expenses) {
        time_t d = parseDate(e.date);
        if (d >= startTime && d <= endTime) {
            cout << left << setw(5) << e.id << setw(12) << e.date 
                 << setw(12) << e.amount << setw(25) << e.category 
                 << e.description << "\n";
            found = true;
        }
    }
    if (!found) cout << "No expenses found in this range.\n";
}

// --------------------------- SUMMARY REPORT ----------------------------
void summaryReport(const vector<Expense> &expenses) {
    if (expenses.empty()) {
        cout << "No expenses recorded.\n";
        return;
    }

    unordered_map<string, double> totals;
    double grandTotal = 0;

    for (const auto &e : expenses) {
        totals[e.category] += e.amount;
        grandTotal += e.amount;
    }

    cout << "\n------- Summary by Category -------\n";
    for (const auto &pair : totals) {
        cout << left << setw(25) << pair.first 
             << "$" << fixed << setprecision(2) << pair.second << "\n";
    }
    cout << "-----------------------------------\n";
    cout << left << setw(25) << "Total" << "$" << grandTotal << "\n";
}

// --------------------------- SAVE ----------------------------
void saveExpenses(const vector<Expense> &expenses) {
    if (expenses.empty()) {
        cout << "No expenses to save.\n";
        return;
    }

    ofstream out("expenses.csv");
    out << "ID,Date,Amount,Category,Description\n";
    for (const auto &e : expenses) {
        out << e.id << "," << e.date << "," << e.amount << "," 
            << e.category << "," << e.description << "\n";
    }
    out.close();
    cout << "✅ Expenses saved to expenses.csv\n";
}

// --------------------------- LOAD ----------------------------
void loadExpenses(vector<Expense> &expenses) {
    ifstream in("expenses.csv");
    if (!in.is_open()) {
        cout << "No CSV file found.\n";
        return;
    }

    string line;
    getline(in, line); // skip header
    while (getline(in, line)) {
        stringstream ss(line);
        Expense e;
        string idStr, amountStr;
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
    cout << "✅ Expenses loaded from expenses.csv\n";
}
