# Employee Payroll System (C++)

A console-based payroll management tool I built for Project Phase 1 of my internship at Sqrock IT Solutions. It's meant to simulate how an HR/admin team would manage employee records and generate salaries at the end of the month, based on attendance.

## What it does

- Add, update, delete, and view employee records (name, department, designation, basic salary).
- Track attendance for each employee — present days, absent days, and overtime hours — and the salary calculation actually factors this in rather than just paying a flat basic salary every month.
- Generate a proper salary slip for any employee, showing the full breakdown: basic pay, bonus, deductions, and the final net salary.
- Generate a full monthly payroll report across every employee, along with the total amount the company would need to pay out that month.
- Search or filter employees by name or department, which is handy once you've got more than a handful of records.

## How salary is calculated

Net salary isn't just `basicSalary` — it's:

```
(basic salary / working days) × present days
+ overtime pay (approximated from hourly rate)
+ bonus
- deductions
```

So if someone was absent for a chunk of the month, that reflects properly in their final payout, and overtime actually adds to their pay instead of being ignored.

## How it's built

Everything revolves around a single `Employee` class that knows how to calculate its own net salary and serialize itself to a line of text in `employees.txt`. Kept it fairly simple on purpose — one file, one class, one clean data source — since the goal was to nail the payroll logic rather than over-engineer the structure.

## Technologies used

- Core C++
- OOP (single responsibility `Employee` class)
- STL (`vector`, `algorithm` for search/filter and delete)
- File Handling for persistent employee records

## How to run it

```bash
g++ -std=c++17 -o payroll_system payroll_system.cpp
./payroll_system
```

On Windows (VS Code terminal):

```powershell
g++ -std=c++17 -o payroll_system payroll_system.cpp
.\payroll_system.exe
```

## Things I'd add if I had more time

- Tax deduction logic based on salary brackets
- Login authentication so only an "admin" role can edit records
- Exporting salary slips as actual text/PDF files instead of just printing to console
- Department-wise payroll summaries

## What I learned

The interesting part of this project wasn't the CRUD operations (those are pretty standard) — it was figuring out a salary formula that actually made sense and correctly reflected attendance and overtime instead of just being a flat number. It made me think more carefully about how real-world payroll logic works, and how a small bug in a formula like this could actually cause someone to be underpaid or overpaid.

---
Built as part of Project Phase 1, Sqrock IT Solutions Internship.
