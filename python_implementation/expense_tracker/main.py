import sys
import json
from datetime import datetime
from collections import defaultdict
import csv

EXPENSES_FILE = 'expenses.json'
CATEGORIES = [
    'Housing',
    'Transportation',
    'Food & Dining',
    'Utilities & Communication',
    'Healthcare & Insurance',
    'Personal & Debt',
    'Other'
]

# List of dictionaries for in-memory storage
expenses = []

def load_expenses():
    """
    Load expenses from the JSON file into the in-memory list.
    If the file does not exist or is invalid, start with an empty list.
    """
    global expenses
    try:
        with open(EXPENSES_FILE, 'r') as f:
            expenses.clear()
            for item in json.load(f):
                # Ensure date is always string in YYYY-MM-DD
                item['date'] = item['date'][:10]
                expenses.append(item)
    except (FileNotFoundError, json.JSONDecodeError):
        expenses.clear()

def save_expenses():
    """
    Save the current in-memory expenses list to the JSON file.
    Prints an error message if saving fails.
    """
    try:
        with open(EXPENSES_FILE, 'w') as f:
            json.dump(expenses, f, indent=2)
    except Exception as e:
        print(f"Error saving expenses: {e}")

def print_menu():
    """
    Print the main menu options for the CLI application.
    """
    print("\nExpense Tracker CLI")
    print("1. Add Expense")
    print("2. View All Expenses")
    print("3. Delete Expense")
    print("4. Filter by Category")
    print("5. Filter by Date Range")
    print("6. Generate Summary Report")
    print("7. Save Expenses")
    print("8. Load Expenses from CSV/JSON")
    print("9. Exit")

def get_input(prompt=None):
    """
    Wrapper for input() to allow easier testing and patching.
    """
    return input(prompt) if prompt else input()

def get_valid_date(prompt, allow_blank=False):
    """
    Prompt the user for a date (YYYY-MM-DD or YYYY-MM-DD HH:MM).
    Repeats until a valid date is entered. Returns the date as a string.
    """
    while True:
        date_str = get_input(prompt).strip()
        if allow_blank and not date_str:
            return None
        try:
            # Accept both YYYY-MM-DD and YYYY-MM-DD HH:MM
            if len(date_str) == 10:
                dt = datetime.strptime(date_str, "%Y-%m-%d")
            else:
                dt = datetime.strptime(date_str, "%Y-%m-%d %H:%M")
            return dt.strftime("%Y-%m-%d %H:%M")
        except ValueError:
            print("Invalid date format. Please use YYYY-MM-DD or YYYY-MM-DD HH:MM (e.g., 2025-10-24 or 2025-10-24 14:30)")

def get_valid_amount():
    """
    Prompt the user for a positive numeric amount.
    Repeats until a valid amount is entered. Returns the amount as float.
    """
    while True:
        amount_str = get_input("Amount: ").strip()
        try:
            amount = float(amount_str)
            if amount <= 0:
                raise ValueError
            return amount
        except ValueError:
            print("Invalid amount. Please enter a positive number.")

def get_valid_category():
    """
    Display a numbered list of categories and prompt the user to select one.
    Repeats until a valid number is entered. Returns the category string.
    """
    print("Select a category:")
    for idx, cat in enumerate(CATEGORIES, 1):
        print(f"  {idx}. {cat}")
    while True:
        choice = get_input("Enter the number of the category: ").strip()
        try:
            idx = int(choice)
            if 1 <= idx <= len(CATEGORIES):
                return CATEGORIES[idx - 1]
            else:
                print(f"Please enter a number between 1 and {len(CATEGORIES)}.")
        except ValueError:
            print("Invalid input. Please enter a number.")

def get_nonempty_input(prompt):
    """
    Prompt the user for a non-empty string. Repeats until non-empty.
    """
    while True:
        val = get_input(prompt).strip()
        if val:
            return val
        print("This field cannot be empty.")

def add_expense():
    """
    Collect all fields from the user and add a new expense to the in-memory list.
    """
    date = get_valid_date("Date (YYYY-MM-DD or YYYY-MM-DD HH:MM): ")
    amount = get_valid_amount()
    category = get_valid_category()
    description = get_nonempty_input("Description: ")
    expense = {'date': date, 'amount': amount, 'category': category, 'description': description}
    expenses.append(expense)
    print("Expense added.")

def view_expenses(exp_list=None):
    """
    Display all expenses (or a provided list) in a formatted table with IDs.
    """
    exp_list = exp_list if exp_list is not None else expenses
    if not exp_list:
        print("No expenses found.")
        return
    print("\nID | Date                | Amount   | Category                | Description")
    print("-"*80)
    for idx, e in enumerate(exp_list, 1):
        e['id'] = idx  # Assign a temporary ID for display and deletion
        print(f"{idx:<2} | {e['date']:<20} | ${e['amount']:<8.2f} | {e['category']:<22} | {e['description']}")

def delete_expense():
    """
    Prompt the user for an expense ID and delete the corresponding expense.
    Handles invalid IDs and empty list gracefully.
    """
    view_expenses()
    if not expenses:
        return
    while True:
        id_str = get_input("Enter the ID of the expense to delete: ").strip()
        try:
            idx = int(id_str)
            if 1 <= idx <= len(expenses):
                removed = expenses.pop(idx - 1)
                print(f"Deleted expense: {removed['description']} on {removed['date']}.")
                return
            else:
                print(f"ID must be between 1 and {len(expenses)}.")
        except ValueError:
            print("Invalid ID. Please enter a valid integer.")

def filter_by_category():
    """
    Prompt the user to select a category and display only matching expenses.
    """
    category = get_valid_category()
    filtered = [e for e in expenses if e['category'] == category]
    view_expenses(filtered)

def filter_by_date_range():
    """
    Prompt the user for a start and end date, and display expenses in that range.
    Accepts both date and datetime formats.
    """
    start = get_valid_date("Start date (YYYY-MM-DD or YYYY-MM-DD HH:MM): ")
    end = get_valid_date("End date (YYYY-MM-DD or YYYY-MM-DD HH:MM): ")
    def parse_date(d):
        try:
            if len(d) == 10:
                return datetime.strptime(d, "%Y-%m-%d")
            return datetime.strptime(d, "%Y-%m-%d %H:%M")
        except Exception:
            return datetime.min
    start_dt = parse_date(start)
    end_dt = parse_date(end)
    filtered = [e for e in expenses if start_dt <= parse_date(e['date']) <= end_dt]
    view_expenses(filtered)

def summary_report():
    """
    Print the total expenses and subtotals for each category.
    """
    total = sum(e['amount'] for e in expenses)
    cat_totals = defaultdict(float)
    for e in expenses:
        cat_totals[e['category']] += e['amount']
    print(f"\nTotal expenses: ${total:.2f}")
    print("By category:")
    for cat in CATEGORIES:
        print(f"  {cat}: ${cat_totals[cat]:.2f}")

def validate_expense_dict(item):
    """
    Validate a dictionary representing an expense record.
    Returns True if valid, otherwise adds an error message to the dict and returns False.
    """
    try:
        # Accept both YYYY-MM-DD and YYYY-MM-DD HH:MM
        if len(item['date']) == 10:
            datetime.strptime(item['date'], "%Y-%m-%d")
        else:
            datetime.strptime(item['date'], "%Y-%m-%d %H:%M")
        amount = float(item['amount'])
        if amount <= 0:
            raise ValueError("Amount must be positive.")
        if item['category'] not in CATEGORIES:
            raise ValueError(f"Category '{item['category']}' is not supported.")
        if not item['description']:
            raise ValueError("Description cannot be empty.")
        return True
    except Exception as e:
        item['__error__'] = str(e)
        return False

def load_from_json_file(filepath):
    """
    Load expenses from a JSON file, validating each record.
    Returns a list of valid expenses. Prints errors for invalid records.
    """
    try:
        with open(filepath, 'r') as f:
            data = json.load(f)
            if not isinstance(data, list):
                print("JSON file must contain a list of expense records.")
                return []
            valid = []
            for item in data:
                if validate_expense_dict(item):
                    valid.append({
                        'date': item['date'],
                        'amount': float(item['amount']),
                        'category': item['category'],
                        'description': item['description']
                    })
                else:
                    print(f"Invalid record skipped: {item} Reason: {item.get('__error__','Unknown error')}")
            return valid
    except Exception as e:
        print(f"Error loading JSON: {e}")
        return []

def load_from_csv_file(filepath):
    """
    Load expenses from a CSV file, validating each record.
    Returns a list of valid expenses. Prints errors for invalid records.
    """
    try:
        with open(filepath, 'r', newline='') as f:
            reader = csv.DictReader(f)
            valid = []
            for item in reader:
                if validate_expense_dict(item):
                    valid.append({
                        'date': item['date'],
                        'amount': float(item['amount']),
                        'category': item['category'],
                        'description': item['description']
                    })
                else:
                    print(f"Invalid record skipped: {item} Reason: {item.get('__error__','Unknown error')}")
            return valid
    except Exception as e:
        print(f"Error loading CSV: {e}")
        return []

def load_expenses_from_file():
    """
    Prompt the user for a CSV or JSON file path and load valid expenses from it.
    Prints a summary of how many records were loaded.
    """
    print("Enter file path to load (CSV or JSON): ", end='')
    path = get_input().strip()
    if path.lower().endswith('.json'):
        new_expenses = load_from_json_file(path)
    elif path.lower().endswith('.csv'):
        new_expenses = load_from_csv_file(path)
    else:
        print("Unsupported file type. Please provide a .csv or .json file.")
        return
    if new_expenses:
        expenses.extend(new_expenses)
        print(f"Loaded {len(new_expenses)} valid expenses from {path}.")
    else:
        print("No valid expenses loaded.")

def main():
    """
    Main loop for the CLI application. Handles menu navigation and user actions.
    """
    load_expenses()
    while True:
        print_menu()
        choice = get_input("Select an option: ").strip()
        if choice == '1':
            add_expense()
        elif choice == '2':
            view_expenses()
        elif choice == '3':
            delete_expense()
        elif choice == '4':
            filter_by_category()
        elif choice == '5':
            filter_by_date_range()
        elif choice == '6':
            summary_report()
        elif choice == '7':
            save_expenses()
            print("Expenses saved.")
        elif choice == '8':
            load_expenses_from_file()
        elif choice == '9':
            save_expenses()
            print("Goodbye!")
            sys.exit(0)
        else:
            print("Invalid option. Try again.")

if __name__ == "__main__":
    main()
