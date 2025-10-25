# Expense Tracker

## Authors
Oishani Ganguly, Sri Sai Palamoor

## Overview
This project is a cross-language CLI Expense Tracker implemented in both Python and C++. It features robust input validation, file import/export, automated testing, and clear documentation. The tracker supports persistent storage, sample data preloading, and a user-friendly command-line interface.

---

## Features (Python & C++ Parity)
- **Add, View, Delete Expenses**: Enter expenses with date, amount, category, and description. View all, filter, or delete by ID.
- **Input Validation**: Strict checks for date (YYYY-MM-DD), positive numeric amount, and valid category.
- **Filter & Summary**: Filter by category or date range. View summary by category and total.
- **File Import/Export**:
  - **CSV/JSON Support**: Load and save expenses in either format.
  - **Sample Files**: Preload with `c++_impementation/input_data_samples/expenses.csv`/`.json` (C++) or `python_implementation/input_data_samples/expenses_sample.csv`/`.json` (Python). These are for demo/preloading only and are never overwritten unless you explicitly save to them.
  - **Persistence**: All changes (add/delete) are auto-saved to the last-used persistent file (`c++_impementation/expense_tracker_src/expenses_persistent.csv`/`.json` for C++, `python_implementation/expenses.json` for Python). On startup, the app auto-loads from the persistent file if present.
- **Automated Testing**:
  - Python: `pytest`-based tests for all CLI and logic features.
  - C++: Single assert-based test file covering all core features, edge cases, and persistence.
- **Documentation**: Comprehensive docstrings, comments, and READMEs for both implementations.

---

## Installation

### C++ Dependencies
- **Homebrew** (macOS):
  ```zsh
  /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
  ```
- **g++ (C++17+) and make:**
  ```zsh
  brew install gcc make
  ```
  (Or use your system's package manager if not on macOS.)
- **nlohmann/json:** Already included as `json.hpp` in the source directory.

### Python Dependencies
- **Python 3.7+** (install via Homebrew if needed):
  ```zsh
  brew install python
  ```
- **pip** (Python package manager):
  ```zsh
  python3 -m ensurepip --upgrade
  ```
- **pytest** (for testing):
  ```zsh
  python3 -m pip install --upgrade pip pytest
  ```

---

## Directory Structure
- `c++_impementation/expense_tracker_src/` — C++ source, Makefile, persistent data, and binary.
- `c++_impementation/input_data_samples/` — C++ sample data for preloading/demo only.
- `c++_impementation/tests/` — C++ test file, binary.
- `python_implementation/expense_tracker/` — Python CLI source.
- `python_implementation/input_data_samples/` — Python sample data for preloading/demo only.
- `python_implementation/tests/` — Python test files.
- `deliverable_reports/` — Project deliverables.

---

## How to Run (macOS/zsh)
### C++
1. `cd MSCS-632-M80-GroupProject/c++_impementation/expense_tracker_src`
2. `make`
3. `./expense_tracker`
   - On first run, loads from `expenses_persistent.csv`/`.json` if present.
   - Use menu to add, view, filter, delete, save, or load expenses.
   - All changes are auto-saved to the last-used persistent file.
4. Run tests: `cd ../tests && g++ -std=c++17 -I../expense_tracker_src -o test_expense_tracker test_expense_tracker.cpp ../expense_tracker_src/Expense.cpp ../expense_tracker_src/FileManager.cpp ../expense_tracker_src/Utils.cpp && ./test_expense_tracker`

### Python
1. `cd MSCS-632-M80-GroupProject/python_implementation/expense_tracker`
2. `python3 main.py`
   - On first run, loads from `../expenses.json` if present.
   - Use menu to add, view, filter, delete, save, or load expenses.
   - All changes are auto-saved to the last-used persistent file.
3. Run tests:
   1. `cd ../tests`
   2. `python3 -m pytest`
      - All tests should pass if your environment is set up correctly.

## Testing
- **C++**: `tests/test_expense_tracker.cpp` covers all features, edge cases, and persistence.
- **Python**: `tests/test_cli_features.py` and `test_expense_manager.py` cover all CLI and logic features.

---

For full details, see the language-specific READMEs in each implementation folder.