#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>

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

class CategoryManager {
private:
    std::vector<std::string> categories;
    std::string filename;

public:
    CategoryManager(std::string file) : filename(file) {
        loadFromFile();
    }

    void addCategory(const std::string& category) {
        categories.push_back(category);
        saveToFile();
    }

    void viewCategories() const {
        for (size_t i = 0; i < categories.size(); ++i) {
            std::cout << i + 1 << ". " << categories[i] << std::endl;
        }
    }

    void editCategory(int index, const std::string& newCategory) {
        if (index < 1 || index > categories.size()) {
            std::cout << "Invalid category index." << std::endl;
            return;
        }
        categories[index - 1] = newCategory;
        saveToFile();
    }

    void deleteCategory(int index) {
        if (index < 1 || index > categories.size()) {
            std::cout << "Invalid category index." << std::endl;
            return;
        }
        categories.erase(categories.begin() + index - 1);
        saveToFile();
    }

    void loadFromFile() {
        std::ifstream file(filename);
        if (!file.is_open()) return;

        std::string category;
        while (std::getline(file, category)) {
            categories.push_back(category);
        }

        file.close();
    }

    void saveToFile() const {
        std::ofstream file(filename);
        for (const auto& cat : categories) {
            file << cat << std::endl;
        }
        file.close();
    }
        std::vector<std::string> categories;  // Change this to public

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

    void searchExpensesByCategory(const std::string& category) const {
        for (const auto& exp : expenses) {
            if (exp.category == category) {
                exp.display();
            }
        }
    }

    void searchExpensesByDate(const std::string& date) const {
        for (const auto& exp : expenses) {
            if (exp.date == date) {
                exp.display();
            }
        }
    }

    void searchExpensesByAmount(double amount) const {
        for (const auto& exp : expenses) {
            if (exp.amount == amount) {
                exp.display();
            }
        }
    }

    void generateMonthlyReport(const std::string& month) const {
        double total = 0;
        std::map<std::string, double> categoryTotals;
        double highest = 0, lowest = std::numeric_limits<double>::max();

        for (const auto& exp : expenses) {
            if (exp.date.substr(0, 7) == month) {
                total += exp.amount;
                categoryTotals[exp.category] += exp.amount;
                if (exp.amount > highest) highest = exp.amount;
                if (exp.amount < lowest) lowest = exp.amount;
            }
        }

        std::cout << "Monthly Report for " << month << ":\n";
        std::cout << "Total Expenses: $" << std::fixed << std::setprecision(2) << total << std::endl;
        std::cout << "Category-wise Breakdown:\n";
        for (const auto& [category, total] : categoryTotals) {
            std::cout << "- " << category << ": $" << total << std::endl;
        }
        std::cout << "Highest Expense: $" << highest << std::endl;
        std::cout << "Lowest Expense: $" << lowest << std::endl;
    }

    void displayStatistics() const {
        double total = 0;
        std::map<std::string, double> categoryTotals;

        for (const auto& exp : expenses) {
            total += exp.amount;
            categoryTotals[exp.category] += exp.amount;
        }

        std::cout << "Total Expenses: $" << std::fixed << std::setprecision(2) << total << std::endl;
        std::cout << "Average Expense: $" << std::fixed << std::setprecision(2) << total / expenses.size() << std::endl;
        std::cout << "Number of Expenses: " << expenses.size() << std::endl;

        std::string mostCommonCategory;
        double maxCategoryTotal = 0;
        for (const auto& [category, total] : categoryTotals) {
            if (total > maxCategoryTotal) {
                maxCategoryTotal = total;
                mostCommonCategory = category;
            }
        }

        std::cout << "Most Common Category: " << mostCommonCategory << std::endl;
        std::cout << "Expense Distribution:\n";
        for (const auto& [category, total] : categoryTotals) {
            std::cout << "- " << category << ": $" << total << std::endl;
        }
    }
        std::vector<Expense> expenses;  // Change this to public

};

// Functions for handling user input and managing the program
void showMenu() {
    std::cout << "\nExpense Tracker Menu:\n";
    std::cout << "1. Add Expense\n";
    std::cout << "2. View Expenses\n";
    std::cout << "3. Edit Expense\n";
    std::cout << "4. Delete Expense\n";
    std::cout << "5. Search Expenses\n";
    std::cout << "6. Monthly Report\n";
    std::cout << "7. View Statistics\n";
    std::cout << "8. Manage Categories\n";
    std::cout << "9. Exit\n";
}

void handleAddExpense(ExpenseManager& expManager, CategoryManager& catManager) {
    double amount;
    std::string category, date;

    std::cout << "Enter amount: ";
    std::cin >> amount;
    std::cin.ignore();

    catManager.viewCategories();
    std::cout << "Select category: ";
    int catIndex;
    std::cin >> catIndex;
    std::cin.ignore();
    category = catManager.categories[catIndex - 1];

    std::cout << "Enter date (YYYY-MM-DD): ";
    std::getline(std::cin, date);

    expManager.addExpense(Expense(amount, category, date));
    std::cout << "Expense added.\n";
}

void handleViewExpenses(const ExpenseManager& expManager) {
    expManager.viewExpenses();
}

void handleEditExpense(ExpenseManager& expManager, CategoryManager& catManager) {
    int index;
    std::cout << "Enter the index of the expense to edit: ";
    std::cin >> index;
    std::cin.ignore();

    handleAddExpense(expManager, catManager); // reuse addExpense to get new details
    expManager.editExpense(index, expManager.expenses.back());
    std::cout << "Expense edited.\n";
}

void handleDeleteExpense(ExpenseManager& expManager) {
    int index;
    std::cout << "Enter the index of the expense to delete: ";
    std::cin >> index;
    std::cin.ignore();

    expManager.deleteExpense(index);
    std::cout << "Expense deleted.\n";
}

void handleSearchExpenses(const ExpenseManager& expManager) {
    std::cout << "Search by: 1. Category 2. Date 3. Amount\n";
    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 1) {
        std::string category;
        std::cout << "Enter category: ";
        std::getline(std::cin, category);
        expManager.searchExpensesByCategory(category);
    } else if (choice == 2) {
        std::string date;
        std::cout << "Enter date (YYYY-MM-DD): ";
        std::getline(std::cin, date);
        expManager.searchExpensesByDate(date);
    } else if (choice == 3) {
        double amount;
        std::cout << "Enter amount: ";
        std::cin >> amount;
        std::cin.ignore();
        expManager.searchExpensesByAmount(amount);
    } else {
        std::cout << "Invalid choice.\n";
    }
}

void handleMonthlyReport(const ExpenseManager& expManager) {
    std::string month;
    std::cout << "Enter month (YYYY-MM): ";
    std::getline(std::cin, month);
    expManager.generateMonthlyReport(month);
}

void handleViewStatistics(const ExpenseManager& expManager) {
    expManager.displayStatistics();
}

void handleManageCategories(CategoryManager& catManager) {
    std::cout << "Category Management:\n";
    std::cout << "1. Add Category\n";
    std::cout << "2. View Categories\n";
    std::cout << "3. Edit Category\n";
    std::cout << "4. Delete Category\n";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 1) {
        handleAddCategory(catManager);
    } else if (choice == 2) {
        handleViewCategories(catManager);
    } else if (choice == 3) {
        handleEditCategory(catManager);
    } else if (choice == 4) {
        handleDeleteCategory(catManager);
    } else {
        std::cout << "Invalid choice.\n";
    }
}

void handleAddCategory(CategoryManager& catManager) {
    std::string category;
    std::cout << "Enter new category: ";
    std::getline(std::cin, category);
    catManager.addCategory(category);
    std::cout << "Category added.\n";
}

void handleViewCategories(const CategoryManager& catManager) {
    catManager.viewCategories();
}

void handleEditCategory(CategoryManager& catManager) {
    int index;
    catManager.viewCategories();
    std::cout << "Enter the index of the category to edit: ";
    std::cin >> index;
    std::cin.ignore();

    std::string newCategory;
    std::cout << "Enter new category name: ";
    std::getline(std::cin, newCategory);

    catManager.editCategory(index, newCategory);
    std::cout << "Category edited.\n";
}

void handleDeleteCategory(CategoryManager& catManager) {
    int index;
    catManager.viewCategories();
    std::cout << "Enter the index of the category to delete: ";
    std::cin >> index;
    std::cin.ignore();

    catManager.deleteCategory(index);
    std::cout << "Category deleted.\n";
}

int main() {
    ExpenseManager expManager("expenses.txt");
    CategoryManager catManager("categories.txt");

    while (true) {
        showMenu();

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                handleAddExpense(expManager, catManager);
                break;
            case 2:
                handleViewExpenses(expManager);
                break;
            case 3:
                handleEditExpense(expManager, catManager);
                break;
            case 4:
                handleDeleteExpense(expManager);
                break;
            case 5:
                handleSearchExpenses(expManager);
                break;
            case 6:
                handleMonthlyReport(expManager);
                break;
            case 7:
                handleViewStatistics(expManager);
                break;
            case 8:
                handleManageCategories(catManager);
                break;
            case 9:
                std::cout << "Exiting program.\n";
                return 0;
            default:
                std::cout << "Invalid choice.\n";
        }
    }
}
