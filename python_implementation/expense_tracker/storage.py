# Storage logic for Expense Tracker
import json
from typing import List
from .models import Expense

class ExpenseStorage:
    """
    Handles loading and saving expenses to a JSON file for persistence.
    """
    def __init__(self, filename: str = 'expenses.json'):
        """
        Initialize the storage with a target filename.
        """
        self.filename = filename

    def load_expenses(self) -> List[Expense]:
        """
        Load expenses from the JSON file and return a list of Expense objects.
        Returns an empty list if the file does not exist or is invalid.
        """
        try:
            with open(self.filename, 'r') as f:
                data = json.load(f)
                return [Expense.from_dict(item) for item in data]
        except (FileNotFoundError, json.JSONDecodeError):
            return []

    def save_expenses(self, expenses: List[Expense]):
        """
        Save a list of Expense objects to the JSON file.
        Overwrites the file with the new data.
        """
        with open(self.filename, 'w') as f:
            json.dump([e.to_dict() for e in expenses], f, indent=2)
