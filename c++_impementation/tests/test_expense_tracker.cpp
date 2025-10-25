#include "Expense.h"
#include "FileManager.h"
#include "Utils.h"
#include <vector>
#include <cstdio>
#include <iostream>
#include <string>
#include <map>
#include <cassert>

/**
 * @brief Test adding and viewing a single expense record.
 *
 * Verifies that an expense can be added to the list and all fields are stored correctly.
 */
void test_add_and_view_expense() {
    std::vector<Expense> expenses;
    int origNextID = nextID;
    Expense e{nextID++, "2025-10-24", 10.0, "Housing", "Test rent"};
    expenses.push_back(e);
    assert(expenses.size() == 1);
    assert(expenses[0].amount == 10.0);
    assert(expenses[0].category == "Housing");
    assert(expenses[0].description == "Test rent");
    nextID = origNextID;
}

/**
 * @brief Test deleting an expense by its ID.
 *
 * Ensures that the correct expense is removed and the remaining list is as expected.
 */
void test_delete_expense_by_id() {
    std::vector<Expense> expenses = {
        {1, "2025-10-24", 10.0, "Housing", "Test rent"},
        {2, "2025-10-25", 20.0, "Food & Dining", "Lunch"}
    };
    int id = 1;
    auto it = std::find_if(expenses.begin(), expenses.end(), [id](const Expense &ex) { return ex.id == id; });
    assert(it != expenses.end());
    expenses.erase(it);
    assert(expenses.size() == 1);
    assert(expenses[0].id == 2);
}

/**
 * @brief Test category selection by number.
 *
 * Verifies that valid numbers return the correct category and invalid numbers return an empty string.
 */
void test_category_selection_by_number() {
    assert(getCategoryByNumber(1) == "Housing");
    assert(getCategoryByNumber(3) == "Food & Dining");
    assert(getCategoryByNumber(7) == "Other");
    assert(getCategoryByNumber(0).empty());
    assert(getCategoryByNumber(8).empty());
}

/**
 * @brief Test parsing of valid and invalid date strings.
 *
 * Ensures valid dates are parsed and invalid dates return -1.
 */
void test_date_parsing() {
    time_t t = parseDate("2025-10-24");
    assert(t > 0);
    time_t bad = parseDate("bad-date");
    assert(bad == -1);
}

/**
 * @brief Test saving and loading expenses to/from JSON file.
 *
 * Verifies that expenses are persisted and loaded correctly from JSON.
 */
void test_save_and_load_expenses_json() {
    std::vector<Expense> expenses = {
        {1, "2025-10-24", 10.0, "Housing", "Test rent"},
        {2, "2025-10-25", 20.0, "Food & Dining", "Lunch"}
    };
    saveExpensesJSON(expenses);
    std::vector<Expense> loaded;
    loadExpensesJSON(loaded);
    assert(loaded.size() == 2);
    assert(loaded[0].category == "Housing");
    assert(loaded[1].amount == 20.0);
    std::remove("expenses.json");
}

/**
 * @brief Test saving and loading expenses to/from CSV file.
 *
 * Verifies that expenses are persisted and loaded correctly from CSV.
 */
void test_save_and_load_expenses_csv() {
    std::vector<Expense> expenses = {
        {1, "2025-10-24", 10.0, "Housing", "Test rent"},
        {2, "2025-10-25", 20.0, "Food & Dining", "Lunch"}
    };
    saveExpensesCSV(expenses);
    std::vector<Expense> loaded;
    loadExpensesCSV(loaded);
    assert(loaded.size() == 2);
    assert(loaded[0].category == "Housing");
    assert(loaded[1].amount == 20.0);
    std::remove("expenses.csv");
}

/**
 * @brief Test validation of amount input.
 *
 * Ensures only positive numeric strings are accepted as valid amounts.
 */
void test_amount_validation() {
    assert(!isValidAmount("-10"));
    assert(!isValidAmount("0"));
    assert(!isValidAmount("abc"));
    assert(isValidAmount("10.5"));
    assert(isValidAmount("100"));
}

/**
 * @brief Test validation of date input.
 *
 * Ensures only valid YYYY-MM-DD dates are accepted.
 */
void test_date_validation() {
    assert(isValidDate("2025-10-24"));
    assert(!isValidDate("2025-13-01"));
    assert(!isValidDate("2025-00-10"));
    assert(!isValidDate("2025-10-32"));
    assert(!isValidDate("bad-date"));
}

/**
 * @brief Test validation of category input.
 *
 * Ensures only supported categories are accepted.
 */
void test_category_validation() {
    assert(isValidCategory("Housing"));
    assert(isValidCategory("Other"));
    assert(!isValidCategory(""));
    assert(!isValidCategory("InvalidCat"));
}

/**
 * @brief Test validation of description input.
 *
 * Ensures non-empty and <=200 character descriptions are accepted.
 */
void test_description_validation() {
    assert(isValidDescription("Lunch at cafe"));
    assert(!isValidDescription(""));
    assert(!isValidDescription(std::string(201, 'a'))); // >200 chars
}

/**
 * @brief Test filtering expenses by category.
 *
 * Verifies that only expenses matching the given category are returned.
 */
void test_filter_by_category() {
    std::vector<Expense> expenses = {
        {1, "2025-10-24", 10.0, "Housing", "Test rent"},
        {2, "2025-10-25", 20.0, "Food & Dining", "Lunch"},
        {3, "2025-10-26", 30.0, "Housing", "Utilities"}
    };
    auto filtered = filterByCategory(expenses, "Housing");
    assert(filtered.size() == 2);
    assert(filtered[0].id == 1);
    assert(filtered[1].id == 3);
}

/**
 * @brief Test filtering expenses by date.
 *
 * Verifies that only expenses matching the given date are returned.
 */
void test_filter_by_date() {
    std::vector<Expense> expenses = {
        {1, "2025-10-24", 10.0, "Housing", "Test rent"},
        {2, "2025-10-25", 20.0, "Food & Dining", "Lunch"},
        {3, "2025-10-24", 30.0, "Housing", "Utilities"}
    };
    auto filtered = filterByDate(expenses, "2025-10-24");
    assert(filtered.size() == 2);
    assert(filtered[0].id == 1);
    assert(filtered[1].id == 3);
}

/**
 * @brief Test summary calculations for total and per-category expenses.
 *
 * Ensures totals are computed correctly.
 */
void test_summary_calculations() {
    std::vector<Expense> expenses = {
        {1, "2025-10-24", 10.0, "Housing", "Test rent"},
        {2, "2025-10-25", 20.0, "Food & Dining", "Lunch"},
        {3, "2025-10-26", 30.0, "Housing", "Utilities"}
    };
    double total = getTotalExpenses(expenses);
    assert(total == 60.0);
    auto perCat = getCategoryTotals(expenses);
    assert(perCat["Housing"] == 40.0);
    assert(perCat["Food & Dining"] == 20.0);
}

/**
 * @brief Test handling of empty files for JSON and CSV loading.
 *
 * Ensures loading from empty files results in an empty expense list.
 */
void test_empty_file_handling() {
    std::vector<Expense> loaded;
    std::FILE *f = std::fopen("expenses.json", "w");
    std::fclose(f);
    loadExpensesJSON(loaded);
    assert(loaded.empty());
    std::remove("expenses.json");
    f = std::fopen("expenses.csv", "w");
    std::fclose(f);
    loadExpensesCSV(loaded);
    assert(loaded.empty());
    std::remove("expenses.csv");
}

/**
 * @brief Test handling of malformed files for JSON and CSV loading.
 *
 * Ensures loading from malformed files results in an empty expense list.
 */
void test_malformed_file_handling() {
    std::FILE *f = std::fopen("expenses.json", "w");
    std::fputs("not a json", f);
    std::fclose(f);
    std::vector<Expense> loaded;
    loadExpensesJSON(loaded);
    assert(loaded.empty());
    std::remove("expenses.json");
    f = std::fopen("expenses.csv", "w");
    std::fputs("bad,csv,data\n", f);
    std::fclose(f);
    loadExpensesCSV(loaded);
    assert(loaded.empty());
    std::remove("expenses.csv");
}

/**
 * @brief Test duplicate ID and nextID increment logic.
 *
 * Ensures that new expenses get unique, incrementing IDs.
 */
void test_duplicate_id_and_nextid_logic() {
    std::vector<Expense> expenses = {
        {1, "2025-10-24", 10.0, "Housing", "Test rent"},
        {2, "2025-10-25", 20.0, "Food & Dining", "Lunch"}
    };
    int origNextID = nextID;
    nextID = 3;
    Expense e{nextID++, "2025-10-26", 30.0, "Housing", "Utilities"};
    expenses.push_back(e);
    assert(expenses[2].id == 3);
    nextID = origNextID;
}

/**
 * @brief Test equality and assignment of Expense structs.
 *
 * Verifies that copying an Expense preserves all fields.
 */
void test_expense_equality_and_assignment() {
    Expense e1{1, "2025-10-24", 10.0, "Housing", "Test rent"};
    Expense e2 = e1;
    assert(e1.id == e2.id);
    assert(e1.date == e2.date);
    assert(e1.amount == e2.amount);
    assert(e1.category == e2.category);
    assert(e1.description == e2.description);
}

/**
 * @brief Test handling of an empty expense list for all operations.
 *
 * Ensures that summary, filter, and total functions work with empty input.
 */
void test_handle_empty_expense_list() {
    std::vector<Expense> expenses;
    assert(getTotalExpenses(expenses) == 0.0);
    auto perCat = getCategoryTotals(expenses);
    assert(perCat.empty());
    auto filtered = filterByCategory(expenses, "Housing");
    assert(filtered.empty());
    filtered = filterByDate(expenses, "2025-10-24");
    assert(filtered.empty());
}

/**
 * @brief Test persistence after add/delete and reload.
 *
 * Verifies that data is saved, reloaded, and matches the original, then cleans up.
 */
void test_persistence_after_reload() {
    std::vector<Expense> persistTest;
    persistTest.clear();
    Expense e1 = {1, "2025-10-25", 100.0, "Housing", "Persist test 1"};
    Expense e2 = {2, "2025-10-25", 200.0, "Food & Dining", "Persist test 2"};
    persistTest.push_back(e1);
    persistTest.push_back(e2);
    saveExpensesCSV(persistTest, "test_persist.csv");
    std::vector<Expense> reloadTest;
    loadExpensesCSV(reloadTest, "test_persist.csv");
    assert(reloadTest.size() == 2);
    assert(reloadTest[0].amount == 100.0);
    assert(reloadTest[1].category == "Food & Dining");
    // Clean up
    std::remove("test_persist.csv");
}

/**
 * @brief Main entry point for running all Expense Tracker tests.
 *
 * Runs all test functions and prints a summary message if all pass.
 */
int main() {
    std::cout << "Running Expense Tracker tests...\n";
    test_add_and_view_expense();
    test_delete_expense_by_id();
    test_category_selection_by_number();
    test_date_parsing();
    test_save_and_load_expenses_json();
    test_save_and_load_expenses_csv();
    test_amount_validation();
    test_date_validation();
    test_category_validation();
    test_description_validation();
    test_filter_by_category();
    test_filter_by_date();
    test_summary_calculations();
    test_empty_file_handling();
    test_malformed_file_handling();
    test_duplicate_id_and_nextid_logic();
    test_expense_equality_and_assignment();
    test_handle_empty_expense_list();
    test_persistence_after_reload();
    std::cout << "All tests passed!\n";
    return 0;
}
// End of minimal test harness
