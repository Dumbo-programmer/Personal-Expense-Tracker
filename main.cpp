#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <limits>

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

// Expense and Category Manager classes (same as before)
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

    const std::vector<std::string>& getCategories() const {
        return categories;
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

    const std::vector<Expense>& getExpenses() const {
        return expenses;
    }
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
    std::cout << "9. Export Expenses to CSV\n";
    std::cout << "10. Generate Custom Report\n";
    std::cout << "11. Backup Data\n";
    std::cout << "12. Restore Data\n";
    std::cout << "13. Exit\n";
}

void handleExportToCSV(const ExpenseManager& expManager) {
    std::string filename;
    std::cout << "Enter filename to export to (e.g., expenses.csv): ";
    std::getline(std::cin, filename);
    CSVExporter exporter;
    exporter.exportExpenses(expManager, filename);
}

void handleCustomReport(const ExpenseManager& expManager) {
    double minAmount, maxAmount;
    std::cout << "Enter minimum amount: ";
    std::cin >> minAmount;
    std::cout << "Enter maximum amount: ";
    std::cin >> maxAmount;
    std::cin.ignore();  // Clear newline character from buffer

    CustomReportGenerator reportGenerator;
    reportGenerator.generateReport(expManager, minAmount, maxAmount);
}

void handleBackupData(const ExpenseManager& expManager, const CategoryManager& catManager) {
    std::string filename;
    std::cout << "Enter filename for backup (e.g., backup.txt): ";
    std::getline(std::cin, filename);
    DataBackupManager backupManager;
    backupManager.backupData(expManager, catManager, filename);
}

void handleRestoreData(ExpenseManager& expManager, CategoryManager& catManager) {
    std::string filename;
    std::cout << "Enter filename to restore from (e.g., backup.txt): ";
    std::getline(std::cin, filename);
    DataBackupManager backupManager;
    backupManager.restoreData(expManager, catManager, filename);
}

int main() {
    ExpenseManager expManager("expenses.txt");
    CategoryManager catManager("categories.txt");
    User user("admin", "password");  // Simple example; in practice, handle securely

    // Simulate user authentication
    std::string username, password;
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    if (!user.authenticate(password)) {
        std::cout << "Invalid username or password.\n";
        return 1;
    }

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
                handleExportToCSV(expManager);
                break;
            case 10:
                handleCustomReport(expManager);
                break;
            case 11:
                handleBackupData(expManager, catManager);
                break;
            case 12:
                handleRestoreData(expManager, catManager);
                break;
            case 13:
                std::cout << "Exiting program.\n";
                return 0;
            default:
                std::cout << "Invalid choice.\n";
        }
    }
}
