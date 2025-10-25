import os
import sys
import tempfile
import pytest
from datetime import datetime
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../expense_tracker')))
from expense_tracker.expense_manager import ExpenseManager
from expense_tracker.storage import ExpenseStorage

CATEGORIES = [
    'Housing',
    'Transportation',
    'Food & Dining',
    'Utilities & Communication',
    'Healthcare & Insurance',
    'Personal & Debt',
    'Other'
]

def make_manager(tmpfile=None):
    """
    Create an ExpenseManager and ExpenseStorage using a temporary file.
    Returns a tuple (manager, tmpfile).
    """
    if tmpfile is None:
        tf = tempfile.NamedTemporaryFile(delete=False)
        tmpfile = tf.name
        tf.close()
    storage = ExpenseStorage(tmpfile)
    manager = ExpenseManager(storage)
    return manager, tmpfile

def test_add_and_list_expense():
    """
    Test adding an expense and listing it from the manager.
    """
    manager, tmpfile = make_manager()
    manager.add_expense(10.0, CATEGORIES[0], 'Test housing', datetime(2025, 10, 24, 12, 0))
    expenses = manager.list_expenses()
    assert len(expenses) == 1
    assert expenses[0].amount == 10.0
    assert expenses[0].category == CATEGORIES[0]
    assert expenses[0].description == 'Test housing'
    os.remove(tmpfile)

def test_delete_expense():
    """
    Test deleting an expense by ID and verifying removal.
    """
    manager, tmpfile = make_manager()
    exp = manager.add_expense(5.0, CATEGORIES[1], 'Bus', datetime(2025, 10, 24, 9, 0))
    assert manager.delete_expense(exp.id) is True
    assert len(manager.list_expenses()) == 0
    os.remove(tmpfile)

def test_delete_expense_invalid():
    """
    Test attempting to delete a non-existent expense ID.
    """
    manager, tmpfile = make_manager()
    manager.add_expense(5.0, CATEGORIES[1], 'Bus', datetime(2025, 10, 24, 9, 0))
    assert manager.delete_expense(999) is False
    os.remove(tmpfile)

def test_report():
    """
    Test generating a summary report for totals by category.
    """
    manager, tmpfile = make_manager()
    manager.add_expense(10.0, CATEGORIES[2], 'Lunch')
    manager.add_expense(5.0, CATEGORIES[2], 'Coffee')
    manager.add_expense(7.0, CATEGORIES[1], 'Bus')
    report = manager.get_report()
    assert report[CATEGORIES[2]] == 15.0
    assert report[CATEGORIES[1]] == 7.0
    os.remove(tmpfile)

def test_empty_report():
    """
    Test that an empty manager returns an empty report.
    """
    manager, tmpfile = make_manager()
    report = manager.get_report()
    assert report == {}
    os.remove(tmpfile)

def test_persistence():
    """
    Test saving and loading expenses to ensure persistence works.
    """
    manager, tmpfile = make_manager()
    manager.add_expense(10.0, CATEGORIES[0], 'Persist')
    # Reload manager
    manager2, _ = make_manager(tmpfile)
    expenses = manager2.list_expenses()
    assert len(expenses) == 1
    assert expenses[0].description == 'Persist'
    os.remove(tmpfile)

def test_add_expense_invalid_category():
    """
    Test that adding an expense with an invalid category raises an exception.
    """
    manager, tmpfile = make_manager()
    with pytest.raises(Exception):
        manager.add_expense(10.0, 'InvalidCat', 'desc')
    os.remove(tmpfile)

def test_add_expense_invalid_amount():
    """
    Test that adding an expense with a negative amount raises an exception.
    """
    manager, tmpfile = make_manager()
    with pytest.raises(Exception):
        manager.add_expense(-5.0, CATEGORIES[0], 'desc')
    os.remove(tmpfile)

def test_add_expense_invalid_date():
    """
    Test that adding an expense with an invalid date raises an exception.
    """
    manager, tmpfile = make_manager()
    with pytest.raises(Exception):
        manager.add_expense(10.0, CATEGORIES[0], 'desc', 'bad-date')
    os.remove(tmpfile)

def test_list_expenses_order():
    """
    Test that expenses are returned in chronological order.
    """
    manager, tmpfile = make_manager()
    manager.add_expense(10.0, CATEGORIES[0], 'A', datetime(2025, 10, 25, 12, 0))
    manager.add_expense(20.0, CATEGORIES[0], 'B', datetime(2025, 10, 24, 12, 0))
    expenses = manager.list_expenses()
    assert expenses[0].date < expenses[1].date
    os.remove(tmpfile)
