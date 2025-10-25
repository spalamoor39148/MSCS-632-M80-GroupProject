import os
import sys
import tempfile
import json
import csv
import pytest
from datetime import datetime, timedelta

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../expense_tracker')))
import main as cli

CATEGORIES = cli.CATEGORIES

@pytest.fixture(autouse=True)
def reset_expenses():
    """
    Automatically clear the global expenses list before and after each test.
    Ensures test isolation for all CLI feature tests.
    """
    cli.expenses.clear()
    yield
    cli.expenses.clear()

def run_add_expense(date, amount, category_idx, description):
    """
    Helper to simulate user input for adding an expense via the CLI.
    Returns the last expense added.
    """
    input_values = [date, str(amount), str(category_idx), description]
    def mock_input(prompt=None):
        return input_values.pop(0)
    orig_get_input = cli.get_input
    cli.get_input = mock_input
    try:
        cli.add_expense()
    finally:
        cli.get_input = orig_get_input
    return cli.expenses[-1]

def test_add_expense_valid():
    """
    Test adding a valid expense through the CLI.
    """
    exp = run_add_expense('2025-10-24 12:00', 100, 1, 'Test Housing')
    assert exp['date'] == '2025-10-24 12:00'
    assert exp['amount'] == 100
    assert exp['category'] == CATEGORIES[0]
    assert exp['description'] == 'Test Housing'

def test_add_expense_invalid_amount(monkeypatch):
    """
    Test that entering an invalid amount prompts again and accepts a valid value.
    """
    input_values = ['2025-10-24', '-5', '1', '1', 'Test']
    def mock_input(prompt=None):
        return input_values.pop(0)
    monkeypatch.setattr(cli, 'get_input', mock_input)
    cli.add_expense()
    assert len(cli.expenses) == 1
    assert cli.expenses[0]['amount'] == 1.0

def test_add_expense_invalid_date(monkeypatch):
    """
    Test that entering an invalid date prompts again and accepts a valid value.
    """
    input_values = ['bad-date', '2025-10-24', '1', '1', 'Test']
    def mock_input(prompt=None):
        return input_values.pop(0)
    monkeypatch.setattr(cli, 'get_input', mock_input)
    cli.add_expense()
    assert len(cli.expenses) == 1

def test_add_expense_empty_description(monkeypatch):
    """
    Test that an empty description prompts again and accepts a valid value.
    """
    input_values = ['2025-10-24', '10', '1', '', 'Test']
    def mock_input(prompt=None):
        return input_values.pop(0)
    monkeypatch.setattr(cli, 'get_input', mock_input)
    cli.add_expense()
    assert cli.expenses[0]['description'] == 'Test'

def test_add_expense_invalid_category(monkeypatch):
    """
    Test that an invalid category number prompts again and accepts a valid value.
    """
    input_values = ['2025-10-24', '10', '99', '1', 'Test']
    def mock_input(prompt=None):
        return input_values.pop(0)
    monkeypatch.setattr(cli, 'get_input', mock_input)
    cli.add_expense()
    assert cli.expenses[0]['category'] == CATEGORIES[0]

def test_view_expenses_empty(capsys):
    """
    Test that viewing expenses when none exist prints the correct message.
    """
    cli.expenses.clear()
    cli.view_expenses()
    out = capsys.readouterr().out
    assert 'No expenses found.' in out

def test_view_expenses_with_data(capsys):
    """
    Test that viewing expenses with data prints the correct output.
    """
    cli.expenses.append({'date': '2025-10-24', 'amount': 10, 'category': CATEGORIES[0], 'description': 'A'})
    cli.view_expenses()
    out = capsys.readouterr().out
    assert 'A' in out and 'Housing' in out

def test_delete_expense(monkeypatch):
    """
    Test deleting an expense by ID through the CLI.
    """
    cli.expenses.append({'date': '2025-10-24', 'amount': 10, 'category': CATEGORIES[0], 'description': 'A'})
    cli.expenses.append({'date': '2025-10-24', 'amount': 20, 'category': CATEGORIES[1], 'description': 'B'})
    input_values = ['2']
    monkeypatch.setattr(cli, 'get_input', lambda prompt=None: input_values.pop(0))
    cli.delete_expense()
    assert len(cli.expenses) == 1
    assert cli.expenses[0]['description'] == 'A'

def test_delete_expense_invalid(monkeypatch, capsys):
    """
    Test that deleting with an invalid ID prompts again and handles errors.
    """
    cli.expenses.append({'date': '2025-10-24', 'amount': 10, 'category': CATEGORIES[0], 'description': 'A'})
    input_values = ['99', '1']
    monkeypatch.setattr(cli, 'get_input', lambda prompt=None: input_values.pop(0))
    cli.delete_expense()
    out = capsys.readouterr().out
    assert 'ID must be between' in out or 'Deleted expense' in out

def test_filter_by_category(monkeypatch, capsys):
    """
    Test filtering expenses by category and output correctness.
    """
    cli.expenses.append({'date': '2025-10-24', 'amount': 10, 'category': CATEGORIES[0], 'description': 'A'})
    cli.expenses.append({'date': '2025-10-24', 'amount': 20, 'category': CATEGORIES[1], 'description': 'B'})
    input_values = ['2']
    monkeypatch.setattr(cli, 'get_input', lambda prompt=None: input_values.pop(0))
    cli.filter_by_category()
    out = capsys.readouterr().out
    assert 'B' in out
    for line in out.splitlines():
        if '|' in line and 'Description' not in line:
            assert ' A' not in line

def test_filter_by_date_range(monkeypatch, capsys):
    """
    Test filtering expenses by a date range and output correctness.
    """
    cli.expenses.append({'date': '2025-10-24 10:00', 'amount': 10, 'category': CATEGORIES[0], 'description': 'A'})
    cli.expenses.append({'date': '2025-10-25 10:00', 'amount': 20, 'category': CATEGORIES[1], 'description': 'B'})
    input_values = ['2025-10-24', '2025-10-24 23:59']
    monkeypatch.setattr(cli, 'get_input', lambda prompt=None: input_values.pop(0))
    cli.filter_by_date_range()
    out = capsys.readouterr().out
    assert 'A' in out and 'B' not in out

def test_summary_report_empty(capsys):
    """
    Test that the summary report is correct when there are no expenses.
    """
    cli.expenses.clear()
    cli.summary_report()
    out = capsys.readouterr().out
    assert 'Total expenses: $0.00' in out

def test_summary_report(capsys):
    """
    Test that the summary report is correct with multiple expenses.
    """
    cli.expenses.append({'date': '2025-10-24', 'amount': 10, 'category': CATEGORIES[0], 'description': 'A'})
    cli.expenses.append({'date': '2025-10-24', 'amount': 20, 'category': CATEGORIES[1], 'description': 'B'})
    cli.summary_report()
    out = capsys.readouterr().out
    assert 'Total expenses: $30.00' in out
    assert CATEGORIES[0] in out and CATEGORIES[1] in out

def test_load_from_json_file(tmp_path, capsys):
    """
    Test loading expenses from a JSON file, including invalid records.
    """
    data = [
        {"date": "2025-10-24 08:15", "amount": 12.5, "category": CATEGORIES[2], "description": "Lunch"},
        {"date": "bad-date", "amount": -5, "category": CATEGORIES[2], "description": "Invalid"},
        {"date": "2025-10-24", "amount": 10, "category": "InvalidCat", "description": "Invalid"}
    ]
    file = tmp_path / "test.json"
    file.write_text(json.dumps(data))
    loaded = cli.load_from_json_file(str(file))
    out = capsys.readouterr().out
    assert len(loaded) == 1
    assert loaded[0]['description'] == 'Lunch'
    assert 'Invalid record skipped' in out

def test_load_from_csv_file(tmp_path, capsys):
    """
    Test loading expenses from a CSV file, including invalid records.
    """
    file = tmp_path / "test.csv"
    with open(file, 'w', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=['date','amount','category','description'])
        writer.writeheader()
        writer.writerow({'date': '2025-10-24', 'amount': 10, 'category': CATEGORIES[0], 'description': 'A'})
        writer.writerow({'date': 'bad-date', 'amount': -5, 'category': CATEGORIES[0], 'description': 'Invalid'})
        writer.writerow({'date': '2025-10-24', 'amount': 10, 'category': 'InvalidCat', 'description': 'Invalid'})
    loaded = cli.load_from_csv_file(str(file))
    out = capsys.readouterr().out
    assert len(loaded) == 1
    assert loaded[0]['description'] == 'A'
    assert 'Invalid record skipped' in out

def test_save_and_load_expenses(tmp_path):
    """
    Test saving expenses to a file and loading them back in.
    """
    cli.expenses.clear()
    cli.expenses.append({'date': '2025-10-24', 'amount': 10, 'category': CATEGORIES[0], 'description': 'A'})
    file = tmp_path / 'expenses.json'
    orig_file = cli.EXPENSES_FILE
    cli.EXPENSES_FILE = str(file)
    cli.save_expenses()
    cli.expenses.clear()
    cli.load_expenses()
    assert len(cli.expenses) == 1
    assert cli.expenses[0]['description'] == 'A'
    cli.EXPENSES_FILE = orig_file

def test_load_expenses_file_not_found(tmp_path):
    """
    Test that loading from a non-existent file results in an empty list.
    """
    cli.EXPENSES_FILE = str(tmp_path / 'doesnotexist.json')
    cli.expenses.clear()
    cli.load_expenses()
    assert cli.expenses == []
