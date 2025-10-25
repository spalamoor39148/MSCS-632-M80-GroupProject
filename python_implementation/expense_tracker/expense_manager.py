from typing import List, Optional
from .models import Expense
from .storage import ExpenseStorage
from datetime import datetime

class ExpenseManager:
    """
    Manages expense operations: add, view, delete, and generate reports.
    Handles in-memory expense list and persistence via ExpenseStorage.
    """
    def __init__(self, storage: ExpenseStorage):
        """
        Initialize the ExpenseManager with a storage backend.
        Loads existing expenses and sets up the next available ID.
        """
        self.storage = storage
        self.expenses = self.storage.load_expenses()
        self.next_id = self._get_next_id()

    def _get_next_id(self) -> int:
        """
        Compute the next available unique ID for a new expense.
        Returns 1 if no expenses exist.
        """
        if not self.expenses:
            return 1
        return max(e.id for e in self.expenses) + 1

    def add_expense(self, amount: float, category: str, description: str, date=None) -> Expense:
        """
        Add a new expense to the tracker after validating all fields.
        Accepts amount, category, description, and optional date (str or datetime).
        Returns the created Expense object.
        Raises ValueError for invalid input.
        """
        from .main import CATEGORIES
        if category not in CATEGORIES:
            raise ValueError(f"Invalid category: {category}")
        if not isinstance(amount, (int, float)) or amount <= 0:
            raise ValueError("Amount must be positive.")
        if not description:
            raise ValueError("Description cannot be empty.")
        if date is None:
            date = datetime.now()
        elif isinstance(date, str):
            try:
                if len(date) == 10:
                    date = datetime.strptime(date, "%Y-%m-%d")
                else:
                    date = datetime.strptime(date, "%Y-%m-%d %H:%M")
            except Exception:
                raise ValueError("Invalid date format.")
        expense = Expense(
            id=self.next_id,
            amount=amount,
            category=category,
            description=description,
            date=date
        )
        self.expenses.append(expense)
        self.next_id += 1
        self.storage.save_expenses(self.expenses)
        return expense

    def list_expenses(self) -> List[Expense]:
        """
        Return all expenses sorted chronologically by date.
        """
        return sorted(self.expenses, key=lambda e: e.date)

    def delete_expense(self, expense_id: int) -> bool:
        """
        Delete an expense by its unique ID.
        Returns True if deleted, False if not found.
        """
        for i, e in enumerate(self.expenses):
            if e.id == expense_id:
                del self.expenses[i]
                self.storage.save_expenses(self.expenses)
                return True
        return False

    def get_report(self) -> dict:
        """
        Generate a summary report of total expenses per category.
        Returns a dictionary mapping category to total amount spent.
        """
        report = {}
        for e in self.expenses:
            report.setdefault(e.category, 0)
            report[e.category] += e.amount
        return report
