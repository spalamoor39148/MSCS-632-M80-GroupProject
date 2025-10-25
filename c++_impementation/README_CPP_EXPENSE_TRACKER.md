# Expense Tracker (C++ Implementation)

## Authors
Oishani Ganguly, Sri Sai Palamoor

This is a menu-driven CLI expense tracker for C++. It supports adding, viewing, deleting, filtering, summarizing, saving, and loading expenses, with strong input validation, user-friendly prompts, and persistent storage.

## Directory Structure

- `expense_tracker_src/` — All C++ source files, Makefile, and persistent data files (e.g., `expenses_persistent.csv`).
- `input_data_samples/` — Sample CSV/JSON files for preloading/demo only. Not used for ongoing persistence unless explicitly chosen.
- `tests/` — C++ test file (`test_expense_tracker.cpp`), test binary, and `json.hpp`.

## Features

- **Add Expense:** Enter date (YYYY-MM-DD), amount, select category from a numbered list, and description. All fields are validated.
- **View All Expenses:** See all expenses in a table with IDs, dates, amounts, categories, and descriptions. The current data file (persistent or sample) is always shown.
- **Delete Expense by ID:** Remove an expense by its displayed ID.
- **Filter by Category:** Select a category by number to view only those expenses.
- **Filter by Date Range:** Enter start and end dates (YYYY-MM-DD) to view matching expenses.
- **Summary Report:** See total expenses and subtotals for each category.
- **Persistence:**
  - The app automatically loads from `expenses_persistent.csv` or `expenses_persistent.json` (if present) on startup. If neither exists, you start with an empty list.
  - All changes (add/delete) are auto-saved to the last-used persistent file (CSV or JSON) in `expense_tracker_src/`.
  - You can explicitly save/load to any file (CSV/JSON) via the menu. The current file is always shown when viewing expenses.
  - Preload/sample files (`input_data_samples/expenses.csv`, `input_data_samples/expenses.json`) are for demo/preloading only and are never overwritten unless you explicitly save to them.
- **Sample Preload:**
  - The files in `input_data_samples/` are sample data. You can load them at any time to quickly populate the tracker, but they are not used for ongoing persistence unless you explicitly choose to save to them.
- **Load/Save to CSV/JSON:**
  - You can import/export expenses from/to any CSV or JSON file. The app will prompt for the file type and filename. Invalid records are skipped with clear error messages.
- **Exit:** All data is auto-saved to the current persistent file before quitting.

## Supported Categories

1. Housing
2. Transportation
3. Food & Dining
4. Utilities & Communication
5. Healthcare & Insurance
6. Personal & Debt
7. Other

## Data Format

- **Date:** `YYYY-MM-DD`
- **Amount:** Positive number
- **Category:** One of the above (selected by number)
- **Description:** Non-empty text

## File Import Format

- **CSV/JSON:** For preloading or persistence, the file must have columns/fields: `id`, `date`, `amount`, `category`, `description` (matching the structure of the sample files in `input_data_samples/`).
- **Sample files:** See `input_data_samples/expenses.csv` and `input_data_samples/expenses.json` for valid examples.

## Installation & Setup

1. Clone the repository and navigate to the C++ implementation:

   ```zsh
   git clone https://github.com/spalamoor39148/MSCS-632-M80-GroupProject.git
   cd MSCS-632-M80-GroupProject/c++_impementation/expense_tracker_src
   ```

2. Ensure you have a C++17 compiler (e.g., g++ on macOS) and the header-only [nlohmann/json](https://github.com/nlohmann/json) (included in the `tests/` folder as `json.hpp`).

## Running the App

From the `expense_tracker_src/` directory, run:

```zsh
make
./expense_tracker
```

## What to Expect

- **Menu:** The app displays a menu with all options. Enter the number for your choice.
- **Validation:** All inputs are validated. If you enter an invalid value, you will be prompted again.
- **Category Selection:** Categories are shown as a numbered list. Enter the number, not the name.
- **Date Input:** Enter date as YYYY-MM-DD. (No time support yet.)
- **File Import:** If a record in a CSV/JSON file is invalid, the app will show a clear error message for that record and skip it.
- **Deleting:** When viewing expenses, each has an ID. Use this ID to delete the correct record.
- **Persistence:** The app always shows which file is being used for persistence. All changes are auto-saved to this file.
- **Sample Data:** Loading a sample file does not affect your persistent data unless you explicitly save to that file.

## Testing

To run all tests from the `tests/` directory:

```zsh
cd ../tests
# Build and run all tests
g++ -std=c++17 -I../expense_tracker_src -o test_expense_tracker test_expense_tracker.cpp ../expense_tracker_src/Expense.cpp ../expense_tracker_src/FileManager.cpp ../expense_tracker_src/Utils.cpp && ./test_expense_tracker
```

This will run all core and edge case tests, including input validation, file import, deletion, and persistence. All logic is covered.

## Troubleshooting

- If you see a missing header error for `json.hpp`, ensure it is present in the `tests/` folder.
- If you see a build error, ensure you are using a C++17-compatible compiler.
- If you load a file and see errors, check that the file matches the required format and categories.