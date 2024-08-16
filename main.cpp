#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <limits>
#include <ctime>

// User class
class User {
private:
    std::string username;
    std::string password;

public:
    User(const std::string& user, const std::string& pass)
        : username(user), password(pass) {}

    bool authenticate(const std::string& pass) const {
        return password == pass;
    }

    void registerUser(const std::string& user, const std::string& pass) {
        username = user;
        password = pass;
    }
};

// CSV Exporter class
class CSVExporter {
public:
    void exportExpenses(const ExpenseManager& expManager, const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Could not open file for writing.\n";
            return;
        }

        file << "Amount,Category,Date\n";
        for (const auto& exp : expManager.getExpenses()) {
            file << exp.amount << "," << exp.category << "," << exp.date << "\n";
        }

        file.close();
        std::cout << "Expenses exported to " << filename << std::endl;
    }
};

// Custom Report Generator class
class CustomReportGenerator {
public:
    void generateReport(const ExpenseManager& expManager, double minAmount, double maxAmount) const {
        std::cout << "Custom Report:\n";
        for (const auto& exp : expManager.getExpenses()) {
            if (exp.amount >= minAmount && exp.amount <= maxAmount) {
                exp.display();
            }
        }
    }
};

// Data Backup Manager class
class DataBackupManager {
public:
    void backupData(const ExpenseManager& expManager, const CategoryManager& catManager, const std::string& backupFilename) const {
        std::ofstream file(backupFilename);
        if (!file.is_open()) {
            std::cout << "Could not open file for backup.\n";
            return;
        }

        // Backup expenses
        file << "Expenses:\n";
        for (const auto& exp : expManager.getExpenses()) {
            file << exp.amount << "," << exp.category << "," << exp.date << "\n";
        }

        // Backup categories
        file << "\nCategories:\n";
        for (const auto& cat : catManager.getCategories()) {
            file << cat << "\n";
        }

        file.close();
        std::cout << "Data backed up to " << backupFilename << std::endl;
    }

    void restoreData(ExpenseManager& expManager, CategoryManager& catManager, const std::string& backupFilename) const {
        std::ifstream file(backupFilename);
        if (!file.is_open()) {
            std::cout << "Could not open file for restore.\n";
            return;
        }

        std::string line;
        std::getline(file, line); // Skip "Expenses:" header
        while (std::getline(file, line) && !line.empty()) {
            std::istringstream iss(line);
            double amount;
            std::string category, date;
            std::getline(iss, date, ',');
            iss >> amount;
            std::getline(iss, category, ',');
            expManager.addExpense(Expense(amount, category, date));
        }

        std::getline(file, line); // Skip "Categories:" header
        while (std::getline(file, line)) {
            catManager.addCategory(line);
        }

        file.close();
        std::cout << "Data restored from " << backupFilename << std::endl;
    }
};

// Expense class
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

// Category Manager class
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

    const std::vector<std::string>& getCategories() const {
        return categories;
    }
};

// Expense Manager class
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

    const std::vector<Expense>& getExpenses() const {
        return expenses;
    }
};

// Function declarations for managing expenses
void handleAddExpense(ExpenseManager& expManager, CategoryManager& catManager) {
    double amount;
    std::string category, date;

    std::cout << "Enter amount: ";
    std::cin >> amount;
    std::cin.ignore();

    std::cout << "Enter category (select a number from the list):\n";
    catManager.viewCategories();
    int catIndex;
    std::cin >> catIndex;
    std::cin.ignore();
    category = catManager.getCategories()[catIndex - 1];

    std::cout << "Enter date (YYYY-MM-DD): ";
    std::getline(std::cin, date);

    expManager.addExpense(Expense(amount, category, date));
    std::cout << "Expense added.\n";
}

void handleViewExpenses(const ExpenseManager& expManager) {
    std::cout << "Viewing all expenses:\n";
    expManager.viewExpenses();
}

void handleEditExpense(ExpenseManager& expManager, CategoryManager& catManager) {
    int index;
    std::cout << "Enter the index of the expense to edit: ";
    std::cin >> index;
    std::cin.ignore();

    double amount;
    std::string category, date;

    std::cout << "Enter new amount: ";
    std::cin >> amount;
    std::cin.ignore();

    std::cout << "Enter new category (select a number from the list):\n";
    catManager.viewCategories();
    int catIndex;
    std::cin >> catIndex;
    std::cin.ignore();
    category = catManager.getCategories()[catIndex - 1];

    std::cout << "Enter new date (YYYY-MM-DD): ";
    std::getline(std::cin, date);

    expManager.editExpense(index, Expense(amount, category, date));
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

// Function declarations for managing categories
void handleAddCategory(CategoryManager& catManager) {
    std::string category;
    std::cout << "Enter new category: ";
    std::getline(std::cin, category);

    catManager.addCategory(category);
    std::cout << "Category added.\n";
}

void handleViewCategories(const CategoryManager& catManager) {
    std::cout << "Viewing all categories:\n";
    catManager.viewCategories();
}

void handleEditCategory(CategoryManager& catManager) {
    int index;
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
    std::cout << "Enter the index of the category to delete: ";
    std::cin >> index;
    std::cin.ignore();

    catManager.deleteCategory(index);
    std::cout << "Category deleted.\n";
}

// Main function
int main() {
    std::string expensesFile = "expenses.txt";
    std::string categoriesFile = "categories.txt";

    ExpenseManager expManager(expensesFile);
    CategoryManager catManager(categoriesFile);

    User user("admin", "password"); // Replace with actual user registration and login system
    std::string password;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (!user.authenticate(password)) {
        std::cout << "Authentication failed. Exiting program.\n";
        return 0;
    }

    int choice;

    do {
        std::cout << "\nPersonal Finance Tracker Menu:\n";
        std::cout << "1. Add Expense\n";
        std::cout << "2. View Expenses\n";
        std::cout << "3. Edit Expense\n";
        std::cout << "4. Delete Expense\n";
        std::cout << "5. Add Category\n";
        std::cout << "6. View Categories\n";
        std::cout << "7. Edit Category\n";
        std::cout << "8. Delete Category\n";
        std::cout << "9. Export Expenses to CSV\n";
        std::cout << "10. Generate Custom Report\n";
        std::cout << "11. Backup Data\n";
        std::cout << "12. Restore Data\n";
        std::cout << "13. Exit\n";
        std::cout << "Enter your choice: ";
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
                handleAddCategory(catManager);
                break;
            case 6:
                handleViewCategories(catManager);
                break;
            case 7:
                handleEditCategory(catManager);
                break;
            case 8:
                handleDeleteCategory(catManager);
                break;
            case 9: {
                CSVExporter exporter;
                std::string filename;
                std::cout << "Enter filename for CSV export: ";
                std::getline(std::cin, filename);
                exporter.exportExpenses(expManager, filename);
                break;
            }
            case 10: {
                CustomReportGenerator reportGen;
                double minAmount, maxAmount;
                std::cout << "Enter minimum amount for the report: ";
                std::cin >> minAmount;
                std::cout << "Enter maximum amount for the report: ";
                std::cin >> maxAmount;
                reportGen.generateReport(expManager, minAmount, maxAmount);
                break;
            }
            case 11: {
                DataBackupManager backupManager;
                std::string backupFilename;
                std::cout << "Enter filename for data backup: ";
                std::getline(std::cin, backupFilename);
                backupManager.backupData(expManager, catManager, backupFilename);
                break;
            }
            case 12: {
                DataBackupManager backupManager;
                std::string restoreFilename;
                std::cout << "Enter filename for data restore: ";
                std::getline(std::cin, restoreFilename);
                backupManager.restoreData(expManager, catManager, restoreFilename);
                break;
            }
            case 13:
                std::cout << "Exiting program.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 13);

    return 0;
}
