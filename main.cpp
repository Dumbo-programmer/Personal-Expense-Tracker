#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <limits>
#include <ctime>

// Forward declaration of classes
class ExpenseManager;
class CategoryManager;

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

// Expense class
class Expense {
public:
    double amount;
    std::string category;
    std::string date;

    Expense(double amt, const std::string& cat, const std::string& dt)
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
    CategoryManager(const std::string& file) : filename(file) {
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
    ExpenseManager(const std::string& file) : filename(file) {
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

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string amount_str, category, date;
            std::getline(iss, amount_str, ',');
            std::getline(iss, category, ',');
            std::getline(iss, date, ',');

            double amount = std::stod(amount_str);
            expenses.emplace_back(amount, category, date);
        }

        file.close();
    }

    void saveToFile() const {
        std::ofstream file(filename);
        for (const auto& exp : expenses) {
            file << exp.amount << "," << exp.category << "," << exp.date << std::endl;
        }
        file.close();
    }

    const std::vector<Expense>& getExpenses() const {
        return expenses;
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
            std::getline(iss, line, ',');
            amount = std::stod(line);
            std::getline(iss, category, ',');
            std::getline(iss, date, ',');
            expManager.addExpense(Expense(amount, category, date));
        }

        std::getline(file, line); // Skip "Categories:" header
        while (std::getline(file, line)) {
            if (!line.empty()) {
                catManager.addCategory(line);
            }
        }

        file.close();
        std::cout << "Data restored from " << backupFilename << std::endl;
    }
};

// Function declarations for managing expenses
void handleAddExpense(ExpenseManager& expManager, CategoryManager& catManager) {
    double amount;
    std::string category, date;

    std::cout << "Enter expense amount: $";
    std::cin >> amount;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    catManager.viewCategories();
    std::cout << "Enter category number: ";
    int catIndex;
    std::cin >> catIndex;
    if (catIndex < 1 || catIndex > catManager.getCategories().size()) {
        std::cout << "Invalid category selected.\n";
        return;
    }
    category = catManager.getCategories()[catIndex - 1];

    std::cout << "Enter date (YYYY-MM-DD): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, date);

    expManager.addExpense(Expense(amount, category, date));
    std::cout << "Expense added successfully!\n";
}

// Main function
int main() {
    ExpenseManager expManager("expenses.txt");
    CategoryManager catManager("categories.txt");
    CSVExporter csvExporter;
    CustomReportGenerator reportGenerator;
    DataBackupManager backupManager;

    User user("admin", "password");

    std::cout << "Welcome to Personal Finance Manager!\n";

    while (true) {
        std::cout << "\nMenu:\n"
                  << "1. Add Expense\n"
                  << "2. View Expenses\n"
                  << "3. Edit Expense\n"
                  << "4. Delete Expense\n"
                  << "5. Manage Categories\n"
                  << "6. Export Expenses to CSV\n"
                  << "7. Generate Custom Report\n"
                  << "8. Backup Data\n"
                  << "9. Restore Data\n"
                  << "10. Exit\n"
                  << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                handleAddExpense(expManager, catManager);
                break;
            case 2:
                expManager.viewExpenses();
                break;
            case 3: {
                std::cout << "Enter the index of the expense to edit: ";
                int index;
                std::cin >> index;
                handleAddExpense(expManager, catManager);
                expManager.editExpense(index, expManager.getExpenses().back());
                break;
            }
            case 4: {
                std::cout << "Enter the index of the expense to delete: ";
                int index;
                std::cin >> index;
                expManager.deleteExpense(index);
                break;
            }
            case 5: {
                std::cout << "\nCategory Management:\n"
                          << "1. Add Category\n"
                          << "2. View Categories\n"
                          << "3. Edit Category\n"
                          << "4. Delete Category\n"
                          << "Enter your choice: ";
                int catChoice;
                std::cin >> catChoice;

                if (catChoice == 1) {
                    std::cout << "Enter new category name: ";
                    std::string newCategory;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, newCategory);
                    catManager.addCategory(newCategory);
                } else if (catChoice == 2) {
                    catManager.viewCategories();
                } else if (catChoice == 3) {
                    std::cout << "Enter the index of the category to edit: ";
                    int index;
                    std::cin >> index;
                    std::cout << "Enter the new category name: ";
                    std::string newCategory;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, newCategory);
                    catManager.editCategory(index, newCategory);
                } else if (catChoice == 4) {
                    std::cout << "Enter the index of the category to delete: ";
                    int index;
                    std::cin >> index;
                    catManager.deleteCategory(index);
                }
                break;
            }
            case 6: {
                std::cout << "Enter filename for CSV export: ";
                std::string filename;
                std::cin >> filename;
                csvExporter.exportExpenses(expManager, filename);
                break;
            }
            case 7: {
                std::cout << "Enter minimum amount: $";
                double minAmount, maxAmount;
                std::cin >> minAmount;
                std::cout << "Enter maximum amount: $";
                std::cin >> maxAmount;
                reportGenerator.generateReport(expManager, minAmount, maxAmount);
                break;
            }
            case 8: {
                std::cout << "Enter backup filename: ";
                std::string filename;
                std::cin >> filename;
                backupManager.backupData(expManager, catManager, filename);
                break;
            }
            case 9: {
                std::cout << "Enter backup filename to restore: ";
                std::string filename;
                std::cin >> filename;
                backupManager.restoreData(expManager, catManager, filename);
                break;
            }
            case 10:
                std::cout << "Exiting...\n";
                return 0;
            default:
                std::cout << "Invalid choice.\n";
                break;
        }
    }
    return 0;
}

