# Data models for Expense Tracker
from dataclasses import dataclass, field
from datetime import datetime
from typing import Optional

@dataclass
class Expense:
    """
    Represents a single expense record.
    Attributes:
        id (int): Unique identifier for the expense.
        amount (float): Amount spent.
        category (str): Expense category.
        description (str): Description of the expense.
        date (datetime): Date and time of the expense.
    """
    id: int
    amount: float
    category: str
    description: str
    date: datetime = field(default_factory=datetime.now)

    @staticmethod
    def from_dict(data: dict) -> 'Expense':
        """
        Create an Expense object from a dictionary.
        Expects keys: id, amount, category, description, date (ISO format).
        """
        return Expense(
            id=int(data['id']),
            amount=float(data['amount']),
            category=data['category'],
            description=data['description'],
            date=datetime.fromisoformat(data['date']) if 'date' in data else datetime.now()
        )

    def to_dict(self) -> dict:
        """
        Convert the Expense object to a dictionary suitable for JSON serialization.
        """
        return {
            'id': self.id,
            'amount': self.amount,
            'category': self.category,
            'description': self.description,
            'date': self.date.isoformat()
        }
