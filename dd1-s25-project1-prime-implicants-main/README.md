# Prime Implicants - Quine-McCluskey Logic Minimization

##  Objective
The objective of this project is to gain familiarity with the Quine-McCluskey Logic Minimization algorithm by implementing it in C/C++ to efficiently simplify Boolean functions.

## Project Description
The goal is to develop a C/C++ program that processes Boolean functions and minimizes them using the Quine-McCluskey method. The program will accomplish the following tasks:

### **1. Input Processing**
The program reads a Boolean function from a **structured text file**, which includes:
- The first line indicates the number of variables.
- The second line lists minterms (marked with 'm') or maxterms (marked with 'M'), separated by commas.
- The third line specifies don’t-care terms (marked with 'd'), also separated by commas.

### 2. Generating Prime Implicants (PIs):
The program identifies and lists all prime implicants, showing their corresponding minterms, don’t-care terms, and binary representations.

### 3. Extracting Essential Prime Implicants (EPIs):
- Essential prime implicants are determined and displayed as Boolean expressions.
- The program will also identify any minterms that remain uncovered by EPIs.

### 4.  Minimizing the Boolean Function:
- A prime implicant table is generated and solved to derive the minimal Boolean expression.
- If multiple minimized expressions exist, all possible solutions are presented.

### 5. Bonus feature (15 points):
The program will automatically generate a Verilog module corresponding to the minimized Boolean function using Verilog primitives.

## Requirements
- **Programming Language:** C/C++
- **Input Format:** Text file containing Boolean function details.
- **Output:** Prime implicants, essential prime implicants, minimized Boolean expressions, and an optional **Verilog module**.
- **Error Handling:** Includes **input validation** and error detection for incorrect input formats.
- **Test Cases:** 10 well-defined test cases showcasing program capabilities.
- **Version Control:** Development must be tracked using GitHub with a well-structured repository.


#### **Project Report (5-6 Pages + Cover Page)**
1. Explanation of program design
2. Description of encountered issues (if any)
3. Overview of the testing approach
4. Step-by-step guide for building and executing the program
5. Contributions of each team member

##  Getting Started
###  **Installation & Compilation**
1. **Clone the repository:**
   ```sh
   git clone https://github.com/CSCE2301/dd1-s25-project1-prime-implicants.git
   cd dd1-s25-project1-prime-implicants
2. **Compile the program:**
    ```sh
    g++ -o quine_mccluskey main.cpp

3. **Run the program:**
    ```sh
    ./quine_mccluskey input.txt

