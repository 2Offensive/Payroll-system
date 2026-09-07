
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

const string EMP_FILE = "employees.txt";

int getIntInput(const string &prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return val; }
        cout << "Invalid input.\n"; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
double getDoubleInput(const string &prompt) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return val; }
        cout << "Invalid input.\n"; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

class Employee {
public:
    int id;
    string name, department, designation;
    double basicSalary;
    double bonus = 0, deductions = 0;
    int presentDays = 0, absentDays = 0, overtimeHours = 0;

    double perDaySalary(int workingDaysInMonth = 30) const {
        return basicSalary / workingDaysInMonth;
    }

    double calculateNetSalary(int workingDaysInMonth = 30) const {
        double earned = perDaySalary(workingDaysInMonth) * presentDays;
        double overtimePay = overtimeHours * (perDaySalary(workingDaysInMonth) / 8.0); // approx hourly
        double net = earned + overtimePay + bonus - deductions;
        return net > 0 ? net : 0;
    }

    string toFileLine() const {
        stringstream ss;
        ss << id << "|" << name << "|" << department << "|" << designation << "|"
           << basicSalary << "|" << bonus << "|" << deductions << "|"
           << presentDays << "|" << absentDays << "|" << overtimeHours;
        return ss.str();
    }
    static Employee fromFileLine(const string &line) {
        Employee e;
        stringstream ss(line);
        string tok;
        getline(ss, tok, '|'); e.id = stoi(tok);
        getline(ss, e.name, '|');
        getline(ss, e.department, '|');
        getline(ss, e.designation, '|');
        getline(ss, tok, '|'); e.basicSalary = stod(tok);
        getline(ss, tok, '|'); e.bonus = stod(tok);
        getline(ss, tok, '|'); e.deductions = stod(tok);
        getline(ss, tok, '|'); e.presentDays = stoi(tok);
        getline(ss, tok, '|'); e.absentDays = stoi(tok);
        getline(ss, tok, '|'); e.overtimeHours = stoi(tok);
        return e;
    }
};

class PayrollSystem {
    vector<Employee> employees;

public:
    PayrollSystem() { load(); }

    void load() {
        employees.clear();
        ifstream fin(EMP_FILE);
        string line;
        while (getline(fin, line)) if (!line.empty()) employees.push_back(Employee::fromFileLine(line));
    }
    void save() {
        ofstream fout(EMP_FILE, ios::trunc);
        for (auto &e : employees) fout << e.toFileLine() << "\n";
    }

    int generateId() {
        int maxId = 1000;
        for (auto &e : employees) maxId = max(maxId, e.id);
        return maxId + 1;
    }

    Employee* findById(int id) {
        for (auto &e : employees) if (e.id == id) return &e;
        return nullptr;
    }

    void addEmployee() {
        Employee e;
        e.id = generateId();
        cout << "\n--- Add Employee ---\n";
        cout << "Name: "; getline(cin, e.name);
        cout << "Department: "; getline(cin, e.department);
        cout << "Designation: "; getline(cin, e.designation);
        e.basicSalary = getDoubleInput("Basic Salary: Rs ");
        e.bonus = getDoubleInput("Bonus (0 if none): Rs ");
        e.deductions = getDoubleInput("Deductions (0 if none): Rs ");
        e.presentDays = getIntInput("Present days this month: ");
        e.absentDays = getIntInput("Absent days this month: ");
        e.overtimeHours = getIntInput("Overtime hours this month: ");
        employees.push_back(e);
        save();
        cout << "Employee added successfully. Employee ID: " << e.id << "\n";
    }

    void updateEmployee() {
        int id = getIntInput("Enter Employee ID to update: ");
        Employee *e = findById(id);
        if (!e) { cout << "Employee not found.\n"; return; }
        cout << "Leave field blank to keep the current value.\n";
        string input;

        cout << "Name [" << e->name << "]: "; getline(cin, input); if (!input.empty()) e->name = input;
        cout << "Department [" << e->department << "]: "; getline(cin, input); if (!input.empty()) e->department = input;
        cout << "Designation [" << e->designation << "]: "; getline(cin, input); if (!input.empty()) e->designation = input;

        cout << "Basic Salary [" << e->basicSalary << "] (enter -1 to skip): ";
        double d = getDoubleInput("");
        if (d >= 0) e->basicSalary = d;

        cout << "Present Days [" << e->presentDays << "] (enter -1 to skip): ";
        int i = getIntInput("");
        if (i >= 0) e->presentDays = i;

        cout << "Absent Days [" << e->absentDays << "] (enter -1 to skip): ";
        i = getIntInput("");
        if (i >= 0) e->absentDays = i;

        cout << "Overtime Hours [" << e->overtimeHours << "] (enter -1 to skip): ";
        i = getIntInput("");
        if (i >= 0) e->overtimeHours = i;

        save();
        cout << "Employee record updated.\n";
    }

    void deleteEmployee() {
        int id = getIntInput("Enter Employee ID to delete: ");
        auto it = remove_if(employees.begin(), employees.end(),
                             [&](const Employee &e) { return e.id == id; });
        if (it == employees.end()) { cout << "Employee not found.\n"; return; }
        employees.erase(it, employees.end());
        save();
        cout << "Employee deleted successfully.\n";
    }

    void viewAllEmployees() {
        cout << fixed << setprecision(2);
        cout << "\n--- All Employees ---\n";
        if (employees.empty()) { cout << "No employees found.\n"; return; }
        for (auto &e : employees) {
            cout << "ID: " << e.id << " | " << e.name << " | " << e.department << " | "
                 << e.designation << " | Basic: Rs " << e.basicSalary
                 << " | Net (est.): Rs " << e.calculateNetSalary() << "\n";
        }
    }

    void searchFilterEmployees() {
        cout << "\n--- Search / Filter ---\n1. By Name\n2. By Department\n";
        int choice = getIntInput("Choose: ");
        string query;
        cout << "Enter search term: "; getline(cin, query);
        cout << fixed << setprecision(2);
        bool found = false;
        for (auto &e : employees) {
            bool match = (choice == 1) ? (e.name.find(query) != string::npos)
                                        : (e.department.find(query) != string::npos);
            if (match) {
                found = true;
                cout << "ID: " << e.id << " | " << e.name << " | " << e.department
                     << " | " << e.designation << " | Rs " << e.basicSalary << "\n";
            }
        }
        if (!found) cout << "No matching employees found.\n";
    }

    void generateSalarySlip() {
        int id = getIntInput("Enter Employee ID for salary slip: ");
        Employee *e = findById(id);
        if (!e) { cout << "Employee not found.\n"; return; }
        double net = e->calculateNetSalary();
        cout << fixed << setprecision(2);
        cout << "\n==================================\n";
        cout << "         SALARY SLIP\n";
        cout << "==================================\n";
        cout << "Employee ID   : " << e->id << "\n";
        cout << "Name          : " << e->name << "\n";
        cout << "Department    : " << e->department << "\n";
        cout << "Designation   : " << e->designation << "\n";
        cout << "----------------------------------\n";
        cout << "Basic Salary  : Rs " << e->basicSalary << "\n";
        cout << "Present Days  : " << e->presentDays << "\n";
        cout << "Absent Days   : " << e->absentDays << "\n";
        cout << "Overtime Hrs  : " << e->overtimeHours << "\n";
        cout << "Bonus         : Rs " << e->bonus << "\n";
        cout << "Deductions    : Rs " << e->deductions << "\n";
        cout << "----------------------------------\n";
        cout << "NET SALARY    : Rs " << net << "\n";
        cout << "==================================\n";
    }

    void monthlyPayrollReport() {
        cout << fixed << setprecision(2);
        cout << "\n===== MONTHLY PAYROLL REPORT =====\n";
        double totalPayout = 0;
        for (auto &e : employees) {
            double net = e.calculateNetSalary();
            totalPayout += net;
            cout << e.id << " | " << e.name << " (" << e.department << ") -> Rs " << net << "\n";
        }
        cout << "-----------------------------------\n";
        cout << "Total Payroll Payout: Rs " << totalPayout << "\n";
    }

    void mainMenu() {
        int choice;
        do {
            cout << "\n===== EMPLOYEE PAYROLL SYSTEM =====\n";
            cout << "1. Add Employee\n2. Update Employee\n3. Delete Employee\n"
                 << "4. View All Employees\n5. Search/Filter Employees\n"
                 << "6. Generate Salary Slip\n7. Monthly Payroll Report\n0. Exit\n";
            choice = getIntInput("Enter choice: ");
            switch (choice) {
                case 1: addEmployee(); break;
                case 2: updateEmployee(); break;
                case 3: deleteEmployee(); break;
                case 4: viewAllEmployees(); break;
                case 5: searchFilterEmployees(); break;
                case 6: generateSalarySlip(); break;
                case 7: monthlyPayrollReport(); break;
                case 0: cout << "Exiting Payroll System.\n"; break;
                default: cout << "Invalid choice.\n";
            }
        } while (choice != 0);
    }
};

int main() {
    PayrollSystem system;
    system.mainMenu();
    return 0;
}
