#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

class Expense {
public:
    double amount;
    std::string category;
    std::string date;

    Expense(double amt, std::string cat, std::string dt)
        : amount(amt), category(cat), date(dt) {}

    void display() const {
        std::cout << "Amount: $" << std::fixed << std::setprecision(2) << amount
                  << ", Category: " << category
                  << ", Date: " << date << std::endl;
    }
};

class ExpenseManager {
private:
    std::vector<Expense> expenses;
    std::string filename;

public:
    ExpenseManager(std::string file) : filename(file) {
        loadFromFile();
    }

    void addExpense(const Expense& exp) {
        expenses.push_back(exp);
        saveToFile();
    }

    void viewExpenses() const {
        for (size_t i = 0; i < expenses.size(); ++i) {
            std::cout << i + 1 << ". ";
            expenses[i].display();
        }
    }

    void editExpense(int index, const Expense& exp) {
        if (index < 1 || index > expenses.size()) {
            std::cout << "Invalid expense index." << std::endl;
            return;
        }
        expenses[index - 1] = exp;
        saveToFile();
    }

    void deleteExpense(int index) {
        if (index < 1 || index > expenses.size()) {
            std::cout << "Invalid expense index." << std::endl;
            return;
        }
        expenses.erase(expenses.begin() + index - 1);
        saveToFile();
    }

    void loadFromFile() {
        std::ifstream file(filename);
        if (!file.is_open()) return;

        std::string line, category, date;
        double amount;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            iss >> amount >> category >> date;
            expenses.push_back(Expense(amount, category, date));
        }

        file.close();
    }

    void saveToFile() const {
        std::ofstream file(filename);
        for (const auto& exp : expenses) {
            file << exp.amount << " " << exp.category << " " << exp.date << std::endl;
        }
        file.close();
    }

    void displayStatistics() const {
        double total = 0;
        for (const auto& exp : expenses) {
            total += exp.amount;
        }

        std::cout << "Total Expenses: $" << std::fixed << std::setprecision(2) << total << std::endl;
        std::cout << "Average Expense: $" << std::fixed << std::setprecision(2) << total / expenses.size() << std::endl;
    }
};

void showMenu() {
    std::cout << "\nExpense Tracker Menu:\n";
    std::cout << "1. Add Expense\n";
    std::cout << "2. View Expenses\n";
    std::cout << "3. Edit Expense\n";
    std::cout << "4. Delete Expense\n";
    std::cout << "5. View Statistics\n";
    std::cout << "6. Exit\n";
}

void handleAddExpense(ExpenseManager& manager) {
    double amount;
    std::string category, date;

    std::cout << "Enter amount: $";
    std::cin >> amount;
    std::cout << "Enter category: ";
    std::cin >> category;
    std::cout << "Enter date (YYYY-MM-DD): ";
    std::cin >> date;

    Expense exp(amount, category, date);
    manager.addExpense(exp);
}

void handleViewExpenses(const ExpenseManager& manager) {
    manager.viewExpenses();
}

void handleEditExpense(ExpenseManager& manager) {
    int index;
    double amount;
    std::string category, date;

    manager.viewExpenses();
    std::cout << "Enter expense number to edit: ";
    std::cin >> index;

    std::cout << "Enter new amount: $";
    std::cin >> amount;
    std::cout << "Enter new category: ";
    std::cin >> category;
    std::cout << "Enter new date (YYYY-MM-DD): ";
    std::cin >> date;

    Expense exp(amount, category, date);
    manager.editExpense(index, exp);
}

void handleDeleteExpense(ExpenseManager& manager) {
    int index;

    manager.viewExpenses();
    std::cout << "Enter expense number to delete: ";
    std::cin >> index;

    manager.deleteExpense(index);
}

void handleStatistics(const ExpenseManager& manager) {
    manager.displayStatistics();
}

int main() {
    ExpenseManager manager("expenses.txt");
    int choice;

    do {
        showMenu();
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                handleAddExpense(manager);
                break;
            case 2:
                handleViewExpenses(manager);
                break;
            case 3:
                handleEditExpense(manager);
                break;
            case 4:
                handleDeleteExpense(manager);
                break;
            case 5:
                handleStatistics(manager);
                break;
            case 6:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    } while (choice != 6);

    return 0;
}
