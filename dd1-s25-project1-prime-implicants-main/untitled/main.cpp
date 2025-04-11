#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <bitset>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <cstdlib>

using namespace std;
class arrangementError : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Arrangement error! ";
        }
    };
class spacingError : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Internal space error! Incorrect format due to spacing issues in the file you provided. ";
            }
        };
//
class DontCareErrorInLineTwo : public std::exception {
    public:
        const char* what() const noexcept override {
             return "Don't Care Detected in Line 2! Incorrect format. ";
                }
            };
bool checkString(string &line)
{
    for (char c : line) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

void checkForDuplicates(const vector<vector<string>>& allTokens) {
    unordered_set<string> seenNumbers;
    bool duplicateFound = false;

    for (const auto& tokenVector : allTokens) {
        for (const auto& token : tokenVector) {
            string numberPart = token.substr(1); // Extract numeric part
            if (seenNumbers.find(numberPart) != seenNumbers.end()) {
                cout << "Warning: Duplicate entry detected -> " << token << endl;
                duplicateFound = true;
            } else {
                seenNumbers.insert(numberPart);
            }
        }
    }

    if (!duplicateFound) {
        // cout << "No duplicate entries found." << endl;
    }
}

bool hasInternalSpace(const std::string& token) {
        if (token.find(' ') != std::string::npos) { // Check if token contains a space anywhere
            std::cout << "Issue detected in token: \"" << token << "\"\n";
            return true;;
        }
        return false;
}

bool containDontCare(string & token)
{
    if( token.find('d') != std::string::npos)
    {
        return true;
    }
    return false;
}
void validateAndParseToken(const string &input) {
    if (input.empty()) {
        throw invalid_argument("Error: Empty token found.");
    }

    // The first character must be 'm', 'd', 'M', or 'D'
    if (input[0] != 'm' && input[0] != 'd' && input[0] != 'M') {
        throw invalid_argument("Error: Token '" + input + "' must start with 'm', 'd', or 'M'");
    }

    // Ensure the second character is a digit (not another letter)
    if (!isdigit(input[1])) {
        throw invalid_argument("Error: Token '" + input + " does not have the correct format as specified. ");
    }

    // Extract the number part
    string numberPart = input.substr(1);

    for (char c : numberPart) {
        if (!isdigit(c)) {
            throw invalid_argument("Error: Token '" + input + "' contains non-numeric characters.");
        }
    }

    int number = stoi(numberPart);

}
bool containMinterm(const string &token)
{
    if( token.find('m') != std::string::npos || token.find('M') != std::string::npos)
    {
        return true;
    }
    return false;
}

void checkMintermDontCareConflict(const vector<vector<string>>& tokenVectors) {
    // Create sets to store minterms and dont cares
    unordered_set<string> minterms;
    unordered_set<string> dontCares;

    // Traverse through each vector in the input
    for (const auto& tokenVector : tokenVectors) {
        for (const auto& token : tokenVector) {
            // Check if token starts with 'm' or 'M' for minterm
            if (token[0] == 'm' || token[0] == 'M') {
                // If the same minterm already exists in the dont care set, throw an error
                if (dontCares.find(token) != dontCares.end()) {
                    cout << "Error: Minterm " << token << " and Don't care term conflict!" << endl;
                    exit(0);
                }
                minterms.insert(token);
            }
            // Check if token starts with 'd'
            else if (token[0] == 'd') {
                // If the same don't care already exists in the minterm set, throw an error
                if (minterms.find(token) != minterms.end()) {
                    cout << "Error: Minterm " << token << " and Don't care term conflict!" << endl;
                    exit(0);
                }
                dontCares.insert(token);
            }
            else {
                // If the token is not valid minterm or don't care term, throw an error
                cout << "Error: Invalid token format - " << token << endl;
                exit(0);
            }
        }
    }
    cout << "No conflicts found between minterms and don't cares." << endl;
}
void trackMintermDontCare(vector<vector<string>> &token)
{
    // this function keeps track of whether a number in the minterm line has the same number as the number in the dont care line
    vector < vector<int> > numbers;
    for (auto &tokens : token)
    {   vector <int> mintermOrDontCareNumbers;
        for (auto &mintermOrMaxterm : tokens)
        {
            string numberstr = mintermOrMaxterm.substr(1);
            int number = stoi(numberstr);
            mintermOrDontCareNumbers.push_back(number);
        }
        numbers.push_back(mintermOrDontCareNumbers);
    }



    //start looping over the two minterm and dont care numbers and see whether the second array has numbers that are found in the first one

    auto &mintermNumbers = numbers[0];
    auto &dontCareNumber = numbers[1];

    for (auto &l : mintermNumbers)
    {
        if (find(dontCareNumber.begin(),dontCareNumber.end(),l) != dontCareNumber.end())
        {
            cout<< "Error as dont care numbers and minterm numbers share the same number. Please correct your file.";
            exit(0);
        }
    }
    return;
}
void numberBiggerThanPossible (vector<vector<string>> &allTokens, int numberOfBits)
{
    int biggestNumberPossible = (1<< numberOfBits) -1;
    for (auto &k : allTokens)
    {
        for (auto &l : k)
        {
            int number = stoi(l.substr(1));
            if (number > biggestNumberPossible)
            {
                cout<<"Some number in your minterm or Don't Care lines are bigger than the maximum representable number based on the bits.";
                exit(0);
            }
        }
    }
}
void sameLineMintermMaxterm(vector < vector<string> > &allTokens)
 {
//     for (auto &tokens : allTokens) {
//         bool hasMinterm = false;
//         bool hasMaxterm = false;
//         // Check if any token in the current line is a minterm or maxterm
//         for (auto &token : tokens) {
//             if (token[0] == 'm' || token[0] == 'M') {
//                 hasMinterm = true;
//             }
//             if (token[0] == 'M') {
//                 hasMaxterm = true;
//             }
//         }
//
//         if (hasMinterm && hasMaxterm) {
//             cout << "Error: Minterm and Maxterm detected in the same line." << endl;
//             exit(1);
//         }
//     }
}

bool MintermDontCareSameLine(vector<string> &tokens) {
    bool minterm = false;
    bool dontcare = false;

    for (const string &token : tokens) {
        // Check if the token is a minterm (starts with 'm' or 'M')
        if (token[0] == 'm' || token[0] == 'M') {
            if (dontcare) {  // If don't care has already been found, report error
                cout << "Error: Minterm and Don't Care term detected on the same line." << endl;
                return true;
            }
            minterm = true;  // Found a minterm
        }
        // Check if the token is a don't care (starts with 'd' or 'D')
        else if (token[0] == 'd' || token[0] == 'D') {
            if (minterm) {  // If minterm has already been found, report error
                cout << "Error: Minterm and Don't Care term detected on the same line." << endl;
                return true;
            }
            dontcare = true;
        }
    }

    return false;
}

void viewTokens (vector <vector <string> > AllTokens)
{
    // for (auto &k : AllTokens)
    // {
    //     for (auto &l : k)
    //     {
    //         cout << l << endl;
    //     }
    // }
}
void rtrim(std::string &str) {
    size_t end = str.find_last_not_of(" ");  // Find the last non-space character
    if (end != std::string::npos) {
        str.erase(end + 1);  // Erase everything if the last character is a non-space character.
    } else {
        str.clear();  // If only spaces, clear the string
    }
}
void checkCommaErrors(const std::string &str) {
    // Check if the string ends with a comma
    if (!str.empty() && str.back() == ',') {
        throw std::runtime_error("Error: String ends with a comma!");
    }

    // Check for consecutive commas in the string
    if (str.find(",,") != std::string::npos) {
        throw std::runtime_error("Error: String contains consecutive commas!");
    }
}

void handleErrors(string &filename)
{
    vector <vector<string> > AllTokens;
    ifstream infile_1;
    infile_1.open(filename);
    string numberOfLinesCheck;
    int lineSizee = 0;
    while(getline(infile_1,numberOfLinesCheck))
    {
    lineSizee+=1;
    }
    if (lineSizee < 2 || lineSizee > 3)
    {
        cout<< "Incorrect Format, check your file. ";
        exit(0);
    }

    ifstream infile_2;
    infile_2.open(filename);
    string line;
    int lineNumber = 0;
    int numberOfBits;
    while (getline(infile_2,line))
    {
        rtrim(line);
        checkCommaErrors(line);
        if (lineNumber == 0)
        {
            try {
                if (!checkString(line))
                {
                    throw arrangementError();
                }
                else
                {
                    numberOfBits = stoi(line);
                }

            }
            catch (const arrangementError& e) {
                // Handle the custom arrangement error for the first line
                cout << e.what() << endl;
              //   cout << "Error at line : " << lineNumber+1;
                exit(0); // Exit the program on error
            }
        }
        if (lineNumber == 1 && lineSizee == 3)
        {
            std::vector<std::string> tokens;

            // Create a stringstream from the input string
            std::stringstream ss(line);
            std::string token;

            // Use getline to extract tokens separated by commas
            try{
            while (getline(ss, token, ',')) {
                if (!token.empty()) {
                    tokens.push_back(token);
                }
            }
            for (auto &singleToken : tokens)
            {
                if(hasInternalSpace(singleToken))
                {
                    throw spacingError();
                }
            }
        }
        catch (const spacingError &t)
        {
            cout << t.what() << endl;
          //   cout << "Error at line : " << lineNumber+1;
            exit(0); // Exit the program on error
        }
        try
        {
            for (auto &token: tokens)
            {
                if ( containDontCare(token) )
                {throw DontCareErrorInLineTwo();}
            }
        }
        catch (const DontCareErrorInLineTwo &l)
        {
            cout << l.what() <<endl;
           //  cout << "Error at line : " << lineNumber+1;
            exit(0);
        }
        try
        {
            for (auto &token: tokens)
            {
                validateAndParseToken(token);
            }
        }
        catch (const invalid_argument &e)
        {
            cout << e.what() <<endl;
          //  cout << "Error at line : " << lineNumber+1;
            exit(0);
        }

        AllTokens.push_back(tokens);
    }
    /* *********************************************************************** */
    if (lineNumber == 1 && lineSizee == 2)
    {
        std::vector<std::string> tokens;
        // Create a stringstream from the input string
        std::stringstream ss(line);
        std::string token;

        // Use getline to extract tokens separated by commas
        try{
            while (getline(ss, token, ',')) {
                if (!token.empty()) {
                    tokens.push_back(token);
                }
            }
            for (auto &singleToken : tokens)
            {
                if(hasInternalSpace(singleToken))
                {
                    throw spacingError();
                }
            }
        }
        catch (const spacingError &t)
        {
            cout << t.what() << endl;
          //  cout << "Error at line : " << lineNumber+1;
            exit(0); // Exit the program on error
        }
        try
        {
            for (auto &token: tokens)
            {
                validateAndParseToken(token);
            }
        }
        catch (const invalid_argument &e)
        {
            cout << e.what() <<endl;
         //   cout << "Error at line : " << lineNumber+1;
            exit(0);
        }
        try
        {
                if(MintermDontCareSameLine(tokens))
                {
                throw DontCareErrorInLineTwo();
                }
        }
        catch (const DontCareErrorInLineTwo &l)
        {
            cout << l.what() <<endl;
          //  cout << "Error at line : " << lineNumber+1;
            exit(0);
        }

        // AllTokens.push_back(tokens);
        //     break;
        }

        /* *********************************************************************** */

    if (lineNumber == 2 && lineNumber < lineSizee)
        {
            std::vector<std::string> tokens;

            // Create a stringstream from the input string
            std::stringstream ss(line);
            std::string token;

            // Use getline to extract tokens separated by commas
            try{
            while (getline(ss, token, ',')) {
                if (!token.empty()) {
                    tokens.push_back(token);
                }
            }
            for (auto &singleToken : tokens)
            {
                if(hasInternalSpace(singleToken))
                {
                    throw spacingError();
                }
            }
        }
        catch (const spacingError &t)
        {
            cout << t.what() << endl;
          //  cout << "Error at line : " << lineNumber+1;
            exit(0); // Exit the program on error
        }
        try
        {
            for (auto &token: tokens)
            {
                if ( containMinterm(token) )
                {throw DontCareErrorInLineTwo();}
            }
        }
        catch (const DontCareErrorInLineTwo &l)
        {
            cout << l.what() <<endl;
           // cout << "Error at line : " << lineNumber+1;
            exit(0);
        }
        try
        {
            for (auto &token: tokens)
            {
                validateAndParseToken(token);
            }
        }
        catch (const invalid_argument &e)
        {
            cout << e.what() <<endl;
           //  cout << "Error at line : " << lineNumber+1;
            exit(0);
        }

        AllTokens.push_back(tokens);
    }

    lineNumber +=1;
}
   viewTokens(AllTokens);
    if (lineSizee == 3)
    {
    trackMintermDontCare(AllTokens);
    numberBiggerThanPossible(AllTokens,numberOfBits);
    sameLineMintermMaxterm(AllTokens);
    checkForDuplicates(AllTokens);
    }
   else
   {
        numberBiggerThanPossible(AllTokens,numberOfBits);
       sameLineMintermMaxterm(AllTokens);
       checkForDuplicates(AllTokens);
   }

    // cout << "Test Case files passed! " << endl;
}

// Function to read input from file
void readInput(const string& filename, int& num, string& min, string& dc) {
    ifstream inp(filename);
    if (!inp) {
        cerr << "Error: Unable to open file!\n";
        exit(1);
    }

    inp >> num;
    inp.ignore();

    if (!getline(inp, min) ) {
        cerr << "Error: Invalid file format!\n";
        exit(1);
    }
    getline(inp, dc);

    inp.close();
}

// Function to count ones on each boolean expression
int countOnes(const string& bin) {
    return count(bin.begin(), bin.end(), '1');
}
vector<string> split_terms(const string& expression) {
    vector<string> terms;
    stringstream ss(expression);
    string term;

    // Split based on the '+' symbol (OR operation)
    while (getline(ss, term, '+')) {
        // Trim spaces
        term.erase(remove_if(term.begin(), term.end(), ::isspace), term.end());
        terms.push_back(term);
    }

    return terms;
}

//function to count number of terms in boolean expression
int count_terms(string s) {
    if (s.empty()) return 0;
    int count=1;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '+')
            count++;
    }
    return count;
}

//function to generate PI of grouped terms
string generateBinary(set<int> group, int bits) {
    if (group.empty()) return "";

    vector<string> binStrings;
    for (int num : group) {
        binStrings.push_back(bitset<32>(num).to_string().substr(32 - bits));
    }

    string result = binStrings[0];

    for (size_t i = 1; i < binStrings.size(); i++) {
        for (size_t j = 0; j < result.size(); j++) {
            if (result[j] != binStrings[i][j]) {
                result[j] = '-';  // Replace differing bits with '-'
            }
        }
    }

    return result;
}

// Check essential PI
 map<int, set<int>> findEssentialPIs(vector<int>& minterms, vector<set<int>>& finalGroup, bool flagEPI) {
     map<int, set<int>> corrGroup;
     map<int, int> countMinterms; // Map to store minterm occurrences
     map<int, int> mintermToIndex;  //Store the index for the essential PIs
     int count = 0;
     // count occurrences of each minterm in the final groups
     for (size_t i = 0; i < finalGroup.size(); i++) {
         for (const int& mint : minterms) {
             if (finalGroup[i].find(mint) != finalGroup[i].end()) {
                 countMinterms[mint]++;
                 mintermToIndex[mint] = i; // store the implicant index (Will use it to get it back from the binary form)
             }
         }
     }

     // essential prime implicants (minterms that appear in only one group)
     for (const auto& it : countMinterms) {
         if (it.second == 1) { // Appears in only one group : Essential
             int index = mintermToIndex[it.first];
             corrGroup[index] = finalGroup[index];
         }
     }
     return corrGroup;
 }

// Function to remove EPIs and their minterms
void removeEPIs(vector<int>& minterms, vector<set<int>>& finalGroup, map<int, set<int>> &essentialPIs, vector<string>& pi) {
    set<int> mintermsToRemove;

    // Collect all minterms covered by Essential Prime Implicants
    for (const auto& [index, piSet] : essentialPIs) {
        mintermsToRemove.insert(piSet.begin(), piSet.end());
    }

    // Remove covered minterms from minterms list
    vector<int> reducedMinterms;
    for (int mint : minterms) {
        if (mintermsToRemove.find(mint) == mintermsToRemove.end()) {
            reducedMinterms.push_back(mint);
        }
    }
    minterms = reducedMinterms;

    // Remove covered minterms from finalGroup but keep the non-covered ones
    vector<set<int>> reducedFinalGroup;
    vector<string> reducedPi;  // New vector to store the reduced pi values

    for (size_t i = 0; i < finalGroup.size(); i++) {
        // If this group corresponds to an essential PI, skip it
        if (essentialPIs.find(i) == essentialPIs.end()) {  // Ignore EPIs
            set<int> newGroup;
            for (int mint : finalGroup[i]) {
                if (mintermsToRemove.find(mint) == mintermsToRemove.end()) {
                    newGroup.insert(mint);  // Keep only the uncovered minterms
                }
            }

            // Check if the group is not empty, and if not, add to reducedFinalGroup
            if (!newGroup.empty()) {
                reducedFinalGroup.push_back(newGroup);
                reducedPi.push_back(pi[i]);  // Add the corresponding pi to the reduced list
            }
        }
    }

    // Remove any empty pi from the reduced list
    vector<string> finalPiWithoutEmpty;
    for (size_t i = 0; i < reducedPi.size(); ++i) {
        if (!reducedPi[i].empty()) {
            finalPiWithoutEmpty.push_back(reducedPi[i]);
        }
    }

    finalGroup = reducedFinalGroup;
    pi = finalPiWithoutEmpty;  // Update pi to remove empty pi values
}

// Function to convert a number to a binary string with fixed length
string toBinary(int num, int length) {
    string binStr = "";
    for (int i = length - 1; i >= 0; i--) {
        binStr += ((num >> i) & 1) ? '1' : '0';
    }
    return binStr;
}

// Function to generate prime implicant string from a set of minterms. Converting minterms to Prime Implicant
void ConvertBackToVectorSet(map<int, vector<string>>& ColumnsToRows, vector<set<int>>& finalGroup) {
    map<string, set<int>> PrimeImplicantToMinterms;

    // Step 1: Reverse the mapping
    for (const auto& entry : ColumnsToRows) {
        int minterm = entry.first;  // The minterm
        for (const string& primeImplicant : entry.second) {
            PrimeImplicantToMinterms[primeImplicant].insert(minterm);
        }
    }

    // Step 2: Convert to vector<set<int>>
    finalGroup.clear();
    for (const auto& entry : PrimeImplicantToMinterms) {
        finalGroup.push_back(entry.second);
    }
}

//function to detect dominating columns and remove them
void dominatingColumns(vector<set<int>> &finalGroup, vector<int> &minterms, vector<string> &pi) {
    map<int, vector<set<int>>> mintermsToPrime;

    // Map each minterm to the list of prime implicants covering it
    for (int minterm : minterms) {
        vector<set<int>> PIAssociatedWithMinterm;
        for (auto &PI : finalGroup) {
            if (PI.find(minterm) != PI.end()) {
                PIAssociatedWithMinterm.push_back(PI);
            }
        }
        mintermsToPrime[minterm] = PIAssociatedWithMinterm;
    }

    for (auto it1 = minterms.begin(); it1 != minterms.end(); ) {
        int column_1 = *it1;
        vector<set<int>> vPI_1 = mintermsToPrime[column_1]; //prime implicant of the first column
        bool dominated = false; //dominated false or not.

        for (auto it2 = minterms.begin(); it2 != minterms.end(); ++it2) {
            if (it1 == it2) continue; // Skip comparing the same column with itself

            int column_2 = *it2;
            vector<set<int>> vPI_2 = mintermsToPrime[column_2];

            bool isDominated = true;
            for (auto &PI : vPI_2) {
                if (find(vPI_1.begin(), vPI_1.end(), PI) == vPI_1.end()) {
                    isDominated = false;
                    break;
                }
            }

            if (isDominated) {
                dominated = true;
                break; // Stop checking further if column_1 dominates column_2
            }
        }

        if (dominated) {
            //cout << "Removing column " << column_1 << " as it is dominating." << endl;  //for check
            mintermsToPrime.erase(column_1);
            for (int k=0;k<finalGroup.size();k++) {
                finalGroup[k].erase(column_1);
            }
            it1 = minterms.erase(it1); // Remove column_1 from minterms
        } else {
            ++it1; // Move forward if not removed
        }
    }

    // Remove empty rows from finalGroup and their corresponding pi values
    vector<set<int>> newFinalGroup;
    vector<string> newPi;

    for (size_t i = 0; i < finalGroup.size(); ++i) {
        if (!finalGroup[i].empty()) {
            newFinalGroup.push_back(finalGroup[i]);
            newPi.push_back(pi[i]);
        }
    }

    finalGroup = newFinalGroup;
    pi = newPi;
}

//function to detect dominated rows and remove them
void dominatingRows(vector<set<int>> &finalGroup, vector<int> &minterms, vector<string> &pi) {
    // Step 1: Create a map from prime implicants to minterms they cover
    map<set<int>, vector<int>> primeToMinterms;
    for (size_t i = 0; i < finalGroup.size(); ++i) {
        vector<int> mintermsCoveredByThisPI;
        for (int minterm : minterms) {
            if (finalGroup[i].find(minterm) != finalGroup[i].end()) {
                mintermsCoveredByThisPI.push_back(minterm);
            }
        }
        primeToMinterms[finalGroup[i]] = mintermsCoveredByThisPI;
    }

    vector<bool> toRemove(finalGroup.size(), false); // To track rows to remove

    // Step 2: Compare prime implicants to identify dominated ones
    for (size_t i = 0; i < finalGroup.size(); ++i) {
        if (toRemove[i]) continue; // Skip already marked for removal

        vector<int> mintermsOfFirstPI = primeToMinterms[finalGroup[i]];

        for (size_t j = 0; j < finalGroup.size(); ++j) {
            if (i == j || toRemove[j]) continue; // Skip self-comparison and already removed

            vector<int> mintermsOfSecondPI = primeToMinterms[finalGroup[j]];
            bool isDominated = true;

            // Check if minterms of both PIs are identical
            if (mintermsOfFirstPI == mintermsOfSecondPI) {
                continue; // Skip if both PIs cover the same minterms
            }

            // Compare minterms: if all minterms of secondPI are covered by firstPI, secondPI is dominated
            for (int minterm : mintermsOfSecondPI) {
                if (find(mintermsOfFirstPI.begin(), mintermsOfFirstPI.end(), minterm) == mintermsOfFirstPI.end()) {
                    isDominated = false;
                    break;
                }
            }

            if (isDominated) {
                // Print the dominated PI before removing it   //for check
                // cout << "Dominated PI: " << "{";
                // for (auto &minterm : finalGroup[j]) {
                //     cout << minterm << " ";
                // }
                // cout << "}\n";

                // Mark the dominated PI for removal
                toRemove[j] = true;
            }
        }
    }

    // Step 3: Remove dominated rows from finalGroup and corresponding pi values
    vector<set<int>> newFinalGroup;
    vector<string> newPi;

    for (size_t i = 0; i < finalGroup.size(); ++i) {
        if (!toRemove[i]) {  // Only keep non-dominated rows
            newFinalGroup.push_back(finalGroup[i]);
            newPi.push_back(pi[i]); // Remove the corresponding pi as well
        }
    }

    finalGroup = newFinalGroup; // Update the finalGroup to include only non-dominated rows
    pi = newPi; // Update the pi vector to include only non-dominated prime implicants
}

// function to multiply two expressions in sum-of-products form
set<set<string>> multiply(set<set<string>> A, set<set<string>> B) {
    set<set<string>> result;
    for (auto &a : A) {
        for (auto &b : B) {
            set<string> temp = a;
            temp.insert(b.begin(), b.end());
            result.insert(temp);
        }
    }
    return result;
}

// Function to apply Petrick's method and find all possible minimized solutions
vector<set<string>> petrickMethod(const map<string, set<int>> &primeImplicants, const vector<int> &minterms) {
    set<set<string>> P;
    for (int minterm : minterms) {
        set<set<string>> term;
        for (const auto &[implicant, coveredMinterms] : primeImplicants) {
            if (coveredMinterms.count(minterm)) {
                term.insert({implicant});
            }
        }
        if (P.empty()) {
            P = term;
        } else {
            P = multiply(P, term);
        }
    }

    // Find minimal solutions by removing
    vector<set<string>> solutions(P.begin(), P.end());
    sort(solutions.begin(), solutions.end(), [](const set<string> &a, const set<string> &b) {
        return a.size() < b.size();
    });

    vector<set<string>> minimized;
   int minsize = solutions.empty() ? 0 : solutions.begin()->size(); //Handle if it is empty
    for (auto &s : solutions) {
        if (s.size() == minsize) {
            minimized.push_back(s);
        }
    }

    return minimized;
}

//function to convert from binary to letters
string convertToLetter(string x) {
    int i=-1;
    string neww;
    for (auto &c : x) {
        if (c=='-')
            i++;
        else {
            if (c=='+') {
                i=-1;
                neww = neww + " + ";
            }
            else {
                if (c=='1'||c=='0')
                    neww += static_cast<char>('A' + i);
                if(c=='0') {
                    neww+= "'";
                }
                i++;
            }
        }
    }
    return neww;
}
//function to store don't care terms as in integer vector and their boolean format in another vector
vector<int> parseTermsdoncare(const string& line, vector<string>& binaryTerms, int inputsNumber, char type) {
    vector<int> terms;
    stringstream ss(line);
    string token;

    while (getline(ss, token, ',')) {
        if (token[0] == type) {
            int term = stoi(token.substr(1));
            terms.push_back(term);
            string binary = bitset<32>(term).to_string().substr(32 - inputsNumber);
            binaryTerms.push_back(binary);
        }
    }
    return terms;
}

//function to store minterms as in integer vector and their boolean format in another vector
vector<int> parseTerms(const string& line, vector<string>& binaryTerms, int inputsNumber, char type, const vector<int>& dontCares) {
    vector<int> terms;
    vector<int> maxterms;
    stringstream ss(line);
    string token;

    while (getline(ss, token, ',')) {
        if (token[0] == 'm') {
            int term = stoi(token.substr(1));
            terms.push_back(term);
        } else if (token[0] == 'M') {
            int term = stoi(token.substr(1));
            maxterms.push_back(term);
        }
    }

    if (type == 'M') {
        int totalTerms = 1 << inputsNumber;
        terms.clear();

        for (int i = 0; i < totalTerms; i++) {
            if (find(maxterms.begin(), maxterms.end(), i) == maxterms.end() &&
            find(dontCares.begin(), dontCares.end(), i) == dontCares.end()) {
                 terms.push_back(i);
                 }
        }
    }
    for (int term : terms) {
        string binary = bitset<32>(term).to_string().substr(32 - inputsNumber);
        binaryTerms.push_back(binary);
    }

    return terms;
}

//function to print all the terms
void printTerms(const string& title, const vector<int>& terms) {
    cout << title << ": ";
    for (int term : terms) cout << term << " ";
    cout << endl;
}
// function to print terms in their binary representation
void printBinaryTerms(const string& title, const vector<string>& binaryTerms) {
    cout << title << ":\n";
    for (const string& bin : binaryTerms) cout << bin << endl;
}
// function to classify terms according to their number of ones
void classifyTerms(const vector<string>& terms, map<int, vector<string>>& groupedTerms) {
    for (const string& term : terms) {
        int onesCount = count(term.begin(), term.end(), '1');
        groupedTerms[onesCount].push_back(term);
    }
}

//Function to identify logical adjacent
bool isAdjacent(const string& term1, const string& term2, string& combined) {
    int differences = 0, pos = -1;
    for (size_t i = 0; i < term1.size(); ++i) {
        if (term1[i] != term2[i]) {
            differences++;
            pos = i;
        }
    }
    if (differences == 1) {
        combined = term1;
        combined[pos] = '-';
        return true;
    }
    return false;
}

//function to extract PIs
set<string> extractPrimeImplicants(map<int, vector<string>>& groupedTerms) {
    set<string> primeImplicants, checkedTerms;
    bool hasNewCombinations = true;
    map<int, vector<string>> currentGroups = groupedTerms;

    while (hasNewCombinations) {
        hasNewCombinations = false;
        map<int, vector<string>> newGroups;
        set<string> usedTerms;

        for (auto it = currentGroups.begin(); it != currentGroups.end(); it++) {
            auto nextIt = next(it);
            if (nextIt == currentGroups.end()) break;

            for (string& term1 : it->second) {
                for (string& term2 : nextIt->second) {
                    string combined;
                    if (isAdjacent(term1, term2, combined)) {
                        newGroups[it->first].push_back(combined);
                        usedTerms.insert(term1);
                        usedTerms.insert(term2);
                        hasNewCombinations = true;
                    }
                }
            }
        }

        for (auto& group : currentGroups) {
            for (string& term : group.second) {
                if (usedTerms.find(term) == usedTerms.end() && checkedTerms.find(term) == checkedTerms.end()) {
                    primeImplicants.insert(term);
                    checkedTerms.insert(term);
                }
            }
        }
        currentGroups = newGroups;
    }

    for (const auto& group : currentGroups) {
        for (const string& term : group.second) {
            primeImplicants.insert(term);
        }
    }

    return primeImplicants;
}
//function to identify convered minterms
void printCoveredMinterms(vector<set<int>> &fin,const set<string>& primeImplicants) {
    vector<set<int>> finalGroup; // Store grouped minterms in decimal form

    for (const string& term : primeImplicants) {
        set<int> mintermSet;
        int numVars = term.length();
        int dashCount = count(term.begin(), term.end(), '-');  // Count the number of '-'

        // Generate all possible replacements for '-'
        for (int i = 0; i < (1 << dashCount); i++) {
            string newTerm = term;
            int replaceIndex = 0;

            for (char& c : newTerm) {
                if (c == '-') {
                    c = ((i & (1 << replaceIndex)) != 0) ? '1' : '0';  // Replace '-' with '0' or '1'
                    replaceIndex++;
                }
            }

            // Convert binary string to decimal and store
            int decimalValue = stoi(newTerm, nullptr, 2);
            mintermSet.insert(decimalValue);
        }

        // Store the set in finalGroup
        finalGroup.push_back(mintermSet);
        fin=finalGroup;

    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input.txt>" << endl;
        return 1;
    }
    string filename = argv[1];
    int inputsNumber;
    string mintermsLine, dontCaresLine;

    readInput(filename, inputsNumber, mintermsLine, dontCaresLine);
     handleErrors(filename);
    char minmax = mintermsLine.empty() ? ' ' : mintermsLine[0];
    int swcase;
    cout << "Enter 1 for detailed output, 2 for final expressions, and 3 for Verilog module: \n";
    cin >> swcase;
    cout << "You entered: " << swcase << endl;
    vector<string> binaryMinterms, binarydcterms;
    vector<int> mintermNum;
    vector<int> dcterms = parseTermsdoncare(dontCaresLine, binarydcterms, inputsNumber, 'd');

    if(minmax=='m')
    {
        mintermNum = parseTerms(mintermsLine, binaryMinterms, inputsNumber, 'm', dcterms);
    }
    else if(minmax=='M')
    {
        mintermNum = parseTerms(mintermsLine, binaryMinterms, inputsNumber, 'M',dcterms);

    }
    vector<string> allterms(binaryMinterms.begin(), binaryMinterms.end());
    allterms.insert(allterms.end(), binarydcterms.begin(), binarydcterms.end());

    if (mintermNum.empty()) {
        cout<<"No minterms found!"<<endl;
        cout<<"f = 0";
        exit(1);
    }
    map<int, vector<string>> groupedTerms;
    classifyTerms(allterms, groupedTerms);
    set<string> primeImplicants = extractPrimeImplicants(groupedTerms);

    //Essential PI
    vector<set<int>> finalGroup; // Store final grouped minterms in decimal form

    printCoveredMinterms(finalGroup,primeImplicants);
    vector<set<int>> finalcovered = finalGroup;

    // Get Essential Prime Implicants
    map<int, set<int>> essentialPIs = findEssentialPIs(mintermNum, finalGroup, true);
    // Remove elements from each set in finalGroup
    for (set<int>& group : finalGroup) {
        auto it = group.begin();
        while (it != group.end()) {
            // If current element is in dcterms, erase it
            if (find(dcterms.begin(), dcterms.end(), *it) != dcterms.end()) {
                it = group.erase(it); // erase() returns iterator to the next element
            } else {
                ++it; // Move forward if no deletion
            }
        }
    }

    vector<set<int>> finalGroupMinOnly=finalGroup;
    vector<int> notCovered;  // Stores uncovered minterms


    // Step 1: Collect all covered minterms
    set<int> coveredMinterms;
    for (auto& pair : essentialPIs) {
        coveredMinterms.insert(pair.second.begin(), pair.second.end());  // Store all covered minterms
    }

    // Step 2: Identify minterms that are NOT covered
    for (int minterm : mintermNum) {
        if (coveredMinterms.find(minterm) == coveredMinterms.end()) {
            notCovered.push_back(minterm);  // If minterm is not found in coveredMinterms, it's uncovered
        }
    }
    vector<string> primeImplicantsvec;
    for (const string& pi : primeImplicants) {
        primeImplicantsvec.push_back(pi);
    }

    bool flagEPI = false;
    bool flagDomColumn = false;
    bool flagDomRow = false;

    //remove EPI
    removeEPIs(mintermNum, finalGroup, essentialPIs,primeImplicantsvec);

    // **Apply Dominating Column Elimination**
    dominatingColumns(finalGroup, mintermNum,primeImplicantsvec);

    // **Apply Dominated Row Elimination**
    dominatingRows(finalGroup, mintermNum,primeImplicantsvec);

    map<string,set<int>> finalGroupWithPIs;
    for (int i=0;i< primeImplicantsvec.size();i++) {
        finalGroupWithPIs.insert(make_pair(primeImplicantsvec[i],finalGroup[i]));
    }

    //apply petrick method to select the possible combinations of the remaining PIs
    vector<set<string>> minimizedSolutions = petrickMethod(finalGroupWithPIs,mintermNum );
    vector<string> Final_expression;
    vector<string> allEPIs;
    string EPIs= " ";

    for (const auto& group : essentialPIs) {
        allEPIs.push_back(generateBinary(group.second, inputsNumber));
    }
    for (auto it = allEPIs.begin(); it != allEPIs.end(); ++it) {
        EPIs += *it;
        EPIs += " + ";
        }

    //forming final expressions
    if (minimizedSolutions.empty()) {
        Final_expression.push_back(EPIs);
        if (!Final_expression[0].empty() && Final_expression[0].size() > 3 &&
            Final_expression[0].substr(Final_expression[0].size() - 3) == " + ") {
            Final_expression[0].erase(Final_expression[0].size() - 3);
            }
    } else {
        int i = 0;
        for (const auto& solutionSet : minimizedSolutions) {
            if (!EPIs.empty()) {
                Final_expression.push_back(EPIs);
            } else {
                Final_expression.push_back("");
            }

            for (auto solution = solutionSet.begin(); solution != solutionSet.end(); ++solution) {
                Final_expression[i] += *solution;
                if (std::next(solution) != solutionSet.end()) {
                    Final_expression[i] += " + ";
                }
            }

            if (!Final_expression[i].empty() &&
                Final_expression[i].substr(Final_expression[i].size() - 3) == " + ") {
                Final_expression[i].erase(Final_expression[i].size() - 3);
                }

            i++;
        }
    }


 //convert final expression to letters
    vector<string> Final_expressionLetter;
    for (const auto& group : Final_expression) {
        Final_expressionLetter.push_back(convertToLetter(group));
    }

    // a switch case to decide whether to show detailed output or final expressions only
    switch (swcase) {
    case 1:
        {
            //printTerms("Minterms", mintermNum);
            printBinaryTerms("Binary Minterms", binaryMinterms);
            printBinaryTerms("Binary DCterms", binarydcterms);
            cout << "\nProcessing Combinations:\n";
            cout << "\nFinal Prime Implicants:\n";
            for (const string& pi : primeImplicants) cout << pi << endl;
            cout << "\nFinal grouped terms:" << endl;


            for (const auto& group : finalcovered) {
                cout << "{ ";
                for (int minterm : group) {
                    cout << minterm << " ";
                }
                cout << "} \n";
            }
        cout << "\nFinal grouped minterms only:" << endl;


            set<std::set<int>> printedGroups;  // To track printed groups

            for (const auto& group : finalGroupMinOnly)
            {
                if (group.empty()) continue;
                if (printedGroups.find(group) != printedGroups.end()) {
                    continue;
                }
                std::cout << "{ ";
                for (int minterm : group) {
                    std::cout << minterm << " ";
                }
                std::cout << "} \n";
            }
            cout << "\nEssential PI will be:" << endl;
            if(essentialPIs.empty())
                cout<<"\nNo EPIs existed"<<endl;
            else
            {
                int i = 1;
                for (auto pair : essentialPIs) {
                    auto index = pair.first;
                    auto group = pair.second;
                    cout << "Grouped correponded to Essential PI { ";
                    for (int num : group) {
                        cout << num << " ";
                    }
                    cout << "}";

                    cout << "\n" << i << ") Essential PI is ";
                    i++;
                    // find the corresponding binary implicant (by index)
                    auto it = primeImplicants.begin();
                    advance(it, index); //
                    cout << *it << endl;  // It will print the essential PIs
                }

                //printing epi as boolean expression
                cout<<"\nEssential PIs as Boolean Expression will be:"<<endl;
                string epilet=convertToLetter(EPIs);
                if (epilet[epilet.length()-2] =='+') {
                epilet.erase(epilet.length()-2);
                }
                cout << epilet << endl;
                //printing not covered minterms
                cout << "\nMinterms not covered by Essential Prime Implicants: ";
                int count = mintermNum.size();

                for (int mint : notCovered) {
                    cout << mint << " ";
                }
                cout << endl;
                if (count == 0) {
                    cout << "\nAll minterms have been covered.";
                }
                cout << "\nRemoving Essential Prime Implicants and Their Minterms";
            }
            cout << "\nApplying Dominating Column Elimination...\n";
            cout << "\nApplying Dominating Row Elimination...\n";

            cout << "\nFinal Reduced Prime Implicants after domination:\n";
            for (const auto &group : finalGroupWithPIs) {
                cout << group.first << " -> ";
                cout << "{ ";
                for (int minterm : group.second) {
                    cout << minterm << " ";
                }
                cout << "} \n";
            }
            // Print all possible minimized solutions
            cout << "\nPossible minimized solutions using Petrick's method:\n";
            for (auto &solution : minimizedSolutions) {
                cout << "{";
                for (auto it = solution.begin(); it != solution.end(); ++it) {
                    cout << *it;
                    if (next(it) != solution.end()) cout << ", ";
                }
                cout << "}\n";
            }
            //printing all possible minimzed final expressions
            cout<< "\nFinal expressions are: \n";
            for (const auto& group : Final_expression) {
                cout << group << endl;
            }
            break;


        }
    case 2:
        {
            //printing all possible minimzed final expressions
            cout<< "\nFinal expressions are: \n";
            for (const auto& group : Final_expression) {
                cout << group << endl;
            }
            break;
        }
    case 3:
        {
              //handling bonus part
        //generate verilog module to include all the possible minimized expressions
   for (int i = 0; i < Final_expressionLetter.size(); i++) {
    string final = Final_expressionLetter[i];

       int terms = count_terms(final);
       vector<char> letters(inputsNumber);
       for (int j = 0; j < inputsNumber; j++) {
           letters[j] = 'A' + j;
       }

       string verilog = "\n\nVerilog code of function " + to_string(i + 1) + "\n"
                        "module function" + to_string(i + 1) + "(\n"
                        "    input ";

       for (int j = 0; j < inputsNumber; j++) {
           verilog += letters[j];
           if (j < inputsNumber - 1) verilog += ", ";
       }

       verilog += ",\n    output f);\n\n";

       for (int j = 0; j < terms; j++) {
           verilog += "    wire w" + to_string(j) + ";\n";
       }

       map<char, string> notWires;
       for (char var : letters) {
           if (final.find(var + string("'")) != string::npos) {
               string wireName = "n" + string(1, var);
               verilog+="    wire " + wireName + ";\n";
               verilog += "    not(" + wireName + ", " + var + ");\n";
               notWires[var] = wireName;
           }
       }
       vector<string> termsList = split_terms(final);
       for (int j = 0; j < termsList.size(); j++) {
           string term = termsList[j];
           vector<string> andInputs;

           for (char var : letters) {
               if (term.find(var) != string::npos) {
                   if (term.find(var + string("'")) != string::npos) {
                       andInputs.push_back(notWires[var]);
                   } else {
                       andInputs.push_back(string(1, var));
                   }
               }
           }

           verilog += "    and(w" + to_string(j);
           for (const string& inp : andInputs) {
               verilog += ", " + inp;
           }
           verilog += ");\n";
       }

       verilog += "\n    or(f";
       for (int j = 0; j < terms; j++) {
           verilog += ", w" + to_string(j);
       }
       verilog += ");\n";
       verilog += "endmodule\n";
       cout << verilog;
   }
break;
        }

    }
    cout<< "\nFinal expressions with letters are: \n";
    for (const auto& group : Final_expressionLetter) {
        cout << group << endl;
    }

    return 0;
}
