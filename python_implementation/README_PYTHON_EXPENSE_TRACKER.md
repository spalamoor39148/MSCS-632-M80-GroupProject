# Expense Tracker (Python Implementation)

## Oishani Ganguly, Sri Sai Palamoor

This is a menu-driven CLI expense tracker for Python. It supports adding, viewing, deleting, filtering, summarizing, saving, and loading expenses, with input validation and user-friendly prompts.

## Features

- **Add Expense:** Enter date (YYYY-MM-DD or YYYY-MM-DD HH:MM), amount, select category from a numbered list, and description. All fields are validated.
- **View All Expenses:** See all expenses in a table with IDs, dates, amounts, categories, and descriptions.
- **Delete Expense by ID:** Remove an expense by its displayed ID.
- **Filter by Category:** Select a category by number to view only those expenses.
- **Filter by Date Range:** Enter start and end dates (with or without time) to view matching expenses.
- **Summary Report:** See total expenses and subtotals for each category.
- **Save Expenses:** Write all current expenses to `expenses.json`.
- **Load Expenses from CSV/JSON:** Import expenses from a file. Invalid records are skipped with clear error messages.
- **Exit:** Save and quit.

## Supported Categories

1. Housing
2. Transportation
3. Food & Dining
4. Utilities & Communication
5. Healthcare & Insurance
6. Personal & Debt
7. Other

## Data Format

- **Date:** `YYYY-MM-DD` or `YYYY-MM-DD HH:MM`
- **Amount:** Positive number
- **Category:** One of the above (selected by number)
- **Description:** Non-empty text

## File Import Format

- **CSV/JSON:** Must have columns/fields: `date`, `amount`, `category`, `description`.
- **Sample files:** See `python_implementation/input_data_samples/expenses_sample.csv` and `python_implementation/input_data_samples/expenses_sample.json` for valid examples.

## Installation & Setup

1. Clone the repository and navigate to the Python implementation:

   ```zsh
   git clone https://github.com/spalamoor39148/MSCS-632-M80-GroupProject.git
   cd MSCS-632-M80-GroupProject/python_implementation
   ```

2. Create a virtual environment:

   ```zsh
   python3 -m venv venv
   source venv/bin/activate
   ```

3. Install test dependencies:

   ```zsh
   pip install pytest
   ```

## Running the App

From the `python_implementation` directory, run:

```zsh
python3 -m expense_tracker.main
```

## What to Expect

- **Menu:** The app displays a menu with all options. Enter the number for your choice.
- **Validation:** All inputs are validated. If you enter an invalid value, you will be prompted again.
- **Category Selection:** Categories are shown as a numbered list. Enter the number, not the name.
- **Date Input:** You can enter just a date or a date and time. Both are accepted and validated.
- **File Import:** If a record in a CSV/JSON file is invalid, the app will show a clear error message for that record and skip it.
- **Deleting:** When viewing expenses, each has an ID. Use this ID to delete the correct record.

## Testing

To run all tests from the `python_implementation` directory:

```zsh
python3 -m pytest tests/
```

This will run 27 tests - both the unit tests and end-to-end CLI feature tests. All core and edge cases are covered, including input validation, file import, and deletion.

## Troubleshooting

- If you see an import error, make sure you are running the app with `python3 -m expense_tracker.main` from the `python_implementation` directory.
- The app requires Python 3.7+.
- If you load a file and see errors, check that the file matches the required format and categories.