/**
 * @name    Large Number Arithmetic with Fractions Too!
 * @author  Rafael Betita & Edgar Flores
 * @date    2019-06-12
 *
 * @brief
 */

#include "includes/io.h"
#include "includes/mylib.h"
#include "includes/math.h"
using namespace std;

void introduction();
void loadCommands(map<string, int> &commands);
bool getLine(string &line);
void process(string &line, const map<string, int> &commands,
             map<int, string>& expressions, bool &stored);
void trimLineAndStandardize(string &line);
bool purgeSpacesAndFormat(string &line);
string formatInfix(string input);
void findCommand(string line, string &command, string &suffix);
void executeCommand(const string &command, const string &suffix,
                    const map<string, int> &commands,
                    map<int, string>& expressions, bool &stored);
void let(const string& suffix, map<int, string>& expressions);
void is(const string& suffix, map<int, string> expressions);
void edit(const string& suffix, map<int, string>& expressions);
void show(const string& suffix, map<int, string> expressions);
void list(map<int, string>& expressions);
void save(const string &suffix, map<int, string> expressions, bool &stored);
void exit(const string &suffix, map<int, string> expressions, bool stored);
void wexit(const string &suffix, map<int, string> expressions);
void clear(map<int, string> &expressions);
void load(const string &suffix, map<int, string>& expressions);
bool shuntingYard(string expression, string& postfix, map<int, string>& expressions);
string rpnEval(const string& postfix);
bool loadPrecedence(map<char, int> &operators);
string format_result(string result);
string formatRPN(string rpn);
void help();
bool REPL(map<int, string> &expressions);

int main()
{
    map<int, string> expressions;
    introduction();
    map<string, int> commands;
    string line;
    bool stored = false;
    loadCommands(commands);
    while(getLine(line))
        process(line, commands, expressions, stored);
    exit(line, expressions, stored);
}

void introduction()
{
    string border(70, '*');
    cout << border << endl;
    cout << "The purpose of this program is to evaluate large number expressions\n";
    cout << border << endl;
}

bool getLine(string &line)
{
    cout<<"Command: ";
    getline(cin, line);
    fflush(stdin);
    return !line.empty();
}

void loadCommands(map<string, int> &commands)
{
    commands["LET"]   = 0;
    commands["SHOW"]  = 1;
    commands["LIST"]  = 2;
    commands["HELP"]  = 3;
    commands["SAVE"]  = 4;
    commands["LOAD"]  = 5;
    commands["EDIT"]  = 6;
    commands["EXIT"]  = 7;
    commands["QUIT"]  = 7;
    commands["WEXIT"] = 8;
    commands["WQUIT"] = 8;
    commands["CLEAR"] = 9;
    commands["REPL"]  = 10;
    commands["IS"]  = 11;
}

/**
 * This function takes in any line as a string and trims leading and extraneous spaces.
 *
 * @param line {string] as a reference
 */
void trimLineAndStandardize(string &line)
{
    unsigned int pos = 0;
    //while first element is a space, delete it
    while(line[0] == ' ' || line[0] == '\t')
        line.erase(0,1);
    //while last element is a space, delete it
    while(line[line.size()-1] == ' ' || line[line.size()-1] == '\t')
        line.erase(line.size()-1);
    //deletes extra spaces between the command and the suffix
    while((pos = line.find(' ', ++pos)) < line.size())
        while(line[pos+1] == ' ')
            line.erase(pos+1,1);
    while((pos = line.find('\t', ++pos)) < line.size())
        while(line[pos+1] == '\t')
            line.erase(pos+1,1);
    //modifies input so all chars are uppercase
    for(unsigned int i = 0; i < line.size(); ++i)
        line[i] = toupper(line[i]);
}

/**
 * This function removes all spaces from a given string
 * also formats mixed numbers, gcd, functions
 * @param suffix
 */
bool purgeSpacesAndFormat(string &line)
{
    std::string temp = "";
    string formatted = "";
    bool validMixed = false;
    bool validSymb = false;
    int fbad = 0;
    int nbad = 0;
    int dbad = 0;
    int fcommas = 0;
    int ncommas = 0;
    int dcommas = 0;
    int fdigits = 0;
    int ndigits = 0;
    int ddigits = 0;
    bool fcomma = false;
    bool ncomma = false;
    bool dcomma = false;
    int j = 0;
    for (unsigned int i = 0; i < line.length(); ++i)
    {
        fbad = 0;
        nbad = 0;
        dbad = 0;
        fcommas = 0;
        ncommas = 0;
        dcommas = 0;
        fdigits = 0;
        ndigits = 0;
        ddigits = 0;
        fcomma = false;
        ncomma = false;
        dcomma = false;
        //skip tabs
        if(line[i] == '\t')
        {
            continue;
        }
        //handle mixed here
        //if digit
        if ((isdigit(line[i])))
        {
            formatted += line[i];
            fdigits++;
            //continues accepting numbers
            while (i + 1 < line.length() && (isdigit(line[i+1])||line[i+1] == ','))
            {
                if(line[i+1] == ',')
                {
                    fcomma = true;
                    i++;
                    fcommas++;
                }
                else {
                    if(fcomma)
                    {
                        fbad++;
                        if(fbad == 3)
                        {
                            fbad = 0;
                        }
                    }
                    fdigits++;
                    i++;
                    formatted += line[i];
                }
            }
            if(fcomma && (fdigits - fcommas < (2*fcommas + 1) || fdigits - fcommas > (2*fcommas + 3) || fbad != 0))
            {
                cout << "Invalid commas: check position or amount of commas to digits" << endl;
                return false;
            }
            //possible mixed number
            if(i+1 < line.length() && (line[i+1] == ' ' || line[i+1] == '\t'))
            {
                //skip that white space
                i++;
                //skip remaining white spaces
                while(i+1 < line.length() && (line[i+1] == ' ' || line[i+1] == '\t'))
                    i++;
                j = i;
                //if we find anything but a digit, get out
                if(!isdigit(line[i+1]))
                {
                    continue;
                }
                //if we find an op get out
                //check if mixed number
                if(j+1 < line.length() && isdigit(line[j+1]))
                {
                    temp += '_';
                    //keep taking digits
                    while (j + 1 < line.length() && (isdigit(line[j+1]) || line[j+1] == ','))
                    {
                        if(line[j+1] == ',')
                        {
                            ncomma = true;
                            j++;
                            ncommas++;
                        }
                        else {
                            if(ncomma)
                            {
                                nbad++;
                                if(nbad == 3)
                                {
                                    nbad = 0;
                                }
                            }
                            ndigits++;
                            j++;
                            temp += line[j];
                        }
                    }
                    //take white spaaces
                    while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                        j++;

                    //fraction part
                    if(j+1 < line.length() && line[j+1] == '/')
                    {
                        temp += '|';
                        j++;
                        while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                            j++;
                        if(!isdigit(line[j+1]))
                        {
                            cout << "Improper mixed number" << endl;
                            return false;
                        }
                        while (j + 1 < line.length() && (isdigit(line[j+1]) || line[j+1] == ','))
                        {
                            if(line[j+1] == ',')
                            {
                                dcomma = true;
                                validMixed = false;
                                j++;
                                dcommas++;
                            }
                            else {
                                if(dcomma)
                                {
                                    dbad++;
                                    if(dbad == 3)
                                    {
                                        dbad = 0;
                                    }
                                }
                                validMixed = true;
                                ddigits++;
                                j++;
                                temp += line[j];
                            }
                        }
                        if(ncomma && (ndigits - ncommas < (2*ncommas + 1) || ndigits - ncommas > (2*ncommas + 3) || nbad != 0))
                        {
                            cout << "Invalid amount of decimals to digits" << endl;
                            return false;
                        }
                        if(dcomma && (ddigits - dcommas < (2*dcommas + 1) || ddigits - dcommas > (2*dcommas + 3) || dbad != 0))
                        {
                            cout << "Invalid amount of decimals to digits" << endl;
                            return false;
                        }
                        else if(validMixed)
                        {
                            i = j;
                            formatted += temp;
                        }
                        else {
                            cout << "Improper mixed number" << endl;
                            return false;
                        }
                    }
                    else {
                        cout << "Invalid Input" << endl;
                        return false;
                    }
                    temp = "";
                }
                else
                    continue;

            }
        }
        //FOR COMBINATION
        //handle weird inputs
        else if(line[i] == 'C')
        {
            //check if combination
            j = i;
            //skip remaining white spaces
            while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                j++;
            //combination indicator
            if(j+1 < line.length() && line[j+1] == '(')
            {
                j++;
                //clean more spaces
                while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                    j++;
                if(!isdigit(line[j+1]))
                {
                    cout << "Improper format for combination" << endl;
                    return false;
                }
                //get first number
                while (j + 1 < line.length() && isdigit(line[j+1]))
                {
                    j++;
                    temp += line[j];
                }
                //add symbol for C
                temp += '@';
                //clean more spaces
                while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                    j++;
                if(j+1 < line.length() && line[j+1] == ',')
                {
                    j++;
                    //clean more spaces
                    while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                        j++;

                    if(!isdigit(line[j+1]))
                    {
                        cout << "Improper format for combination" << endl;
                        return false;
                    }
                    //get second number
                    while (j + 1 < line.length() && isdigit(line[j+1]))
                    {
                        j++;
                        temp += line[j];
                    }

                    //clean more spaces
                    while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                        j++;
                    if(j+1 < line.length() && line[j+1] == ')')
                    {
                        j++;
                        validSymb = true;
                    }
                    if(validSymb = true)
                    {
                        i = j;
                        formatted += temp;
                    }
                    else {
                        cout << "Improper format for combination" << endl;
                        return false;
                    }
                    temp = "";
                }
                else {
                    cout << "Improper format for combination" << endl;
                    return false;
                }
            }
            else {
                formatted += line[i];
            }
        }
        //FOR PERMUTATION
        else if(line[i] == 'P')
        {
            //check if permutation
            j = i;
            //skip remaining white spaces
            while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                j++;
            //combination indicator
            if(j+1 < line.length() && line[j+1] == '(')
            {
                j++;
                //clean more spaces
                while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                    j++;
                if(!isdigit(line[j+1]))
                {
                    cout << "Improper format for permuation" << endl;
                    return false;
                }
                //get first number
                while (j + 1 < line.length() && isdigit(line[j+1]))
                {
                    j++;
                    temp += line[j];
                }
                //add symbol for C
                temp += '#';
                //clean more spaces
                while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                    j++;
                if(j+1 < line.length() && line[j+1] == ',')
                {
                    j++;
                    //clean more spaces
                    while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                        j++;

                    if(!isdigit(line[j+1]))
                    {
                        cout << "Improper format for permutation" << endl;
                        return false;
                    }
                    //get second number
                    while (j + 1 < line.length() && isdigit(line[j+1]))
                    {
                        j++;
                        temp += line[j];
                    }

                    //clean more spaces
                    while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                        j++;
                    if(j+1 < line.length() && line[j+1] == ')')
                    {
                        j++;
                        validSymb = true;
                    }
                    if(validSymb = true)
                    {
                        i = j;
                        formatted += temp;
                    }
                    else {
                        cout << "Improper format for permutation" << endl;
                        return false;
                    }
                    temp = "";
                }
                else {
                    cout << "Improper format for permutation" << endl;
                    return false;
                }
            }
            else {
                formatted += line[i];
            }
        }
        //for GCD
        else if(line[i] == 'G')
        {
            //clean spaces and tabs around here
            //check if combination
            j = i;
            while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                j++;
            if(j+1 < line.length() && line[j+1] == 'C')
            {
                j++;
                while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                    j++;
                if(j+1 < line.length() && line[j+1] == 'D')
                {
                    j++;
                    while(j+1 < line.length() && line[j+1] == ' ')
                        j++;
                    j++;
                    //clean more spaces
                    while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                        j++;
                    if(!isdigit(line[j+1]))
                    {
                        cout << "Improper format for GCD" << endl;
                        return false;
                    }
                    //get first number
                    while (j + 1 < line.length() && isdigit(line[j+1]))
                    {
                        j++;
                        temp += line[j];
                    }
                    //add symbol for C
                    temp += '$';
                    //clean more spaces
                    while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                        j++;
                    if(j+1 < line.length() && line[j+1] == ',')
                    {
                        j++;
                        //clean more spaces
                        while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                            j++;

                        if(!isdigit(line[j+1]))
                        {
                            cout << "Improper format for GCD" << endl;
                            return false;
                        }
                        //get second number
                        while (j + 1 < line.length() && isdigit(line[j+1]))
                        {
                            j++;
                            temp += line[j];
                        }

                        //clean more spaces
                        while(j+1 < line.length() && (line[j+1] == ' ' || line[j+1] == '\t'))
                            j++;
                        if(j+1 < line.length() && line[j+1] == ')')
                        {
                            j++;
                            validSymb = true;
                        }
                        if(validSymb = true)
                        {
                            i = j;
                            formatted += temp;
                        }
                        else {
                            cout << "Improper format for GCD" << endl;
                            return false;
                        }
                        temp = "";
                    }
                    else {
                        cout << "Improper format for GCD" << endl;
                        return false;
                    }
                }
                else {
                    cout << "Improper format for GCD" << endl;
                    return false;
                }
            }
            else {
                formatted += line[i];
            }
        }
        //takes in letters
        else if(line[i] == ',')
        {
            cout << "Improper position for comma" << endl;
            return false;
        }
        else if(line[i] != ' ' && line[i] != ',')
        {
            formatted += line[i];
        }
    }
    line = formatted;
    return true;
}

/**
 * This function separates a command from a given line by its command and suffix
 *
 * @param line
 * @param command
 * @param suffix
 */
void findCommand(string line, string &command, string &suffix)
{
    unsigned int pos = line.find(' ');
    if(pos < line.size()) {
        command = line.substr(0,pos);
        suffix = line.substr(pos+1);
    } else {
        command = line;
        suffix = "";
    }
}

void process(string &line, const map<string, int> &commands,
             map<int, string>& expressions, bool &stored)
{
    bool debug = false;

    string command, suffix;
    if(debug){cout<<"Processing...."<<endl;}
    trimLineAndStandardize(line);
    if(debug){cout<<line<<endl;}
    findCommand(line, command, suffix);
    if(debug){cout<<"Command: "<<command<<" Suffix: "<<suffix<<endl;}
    if(command != "EXIT") {
        stored = false;
    }
    if(purgeSpacesAndFormat(suffix))
        executeCommand(command, suffix, commands, expressions, stored);
    else
    {

        return;
    }
}

void executeCommand(const string &command, const string &suffix, const map<string, int> &commands,
                    map<int, string>& expressions, bool &stored)
{
    int whatToDo;
    if(commands.count(command))
        whatToDo = commands.at(command);
    else
        whatToDo = 99;
    switch(whatToDo)
    {
    case 0:
        let(suffix, expressions);
        break;
    case 1:
        show(suffix, expressions);
        break;
    case 2:
        list(expressions);
        break;
    case 3:
        help();
        break;
    case 4:
        save(suffix, expressions, stored);
        break;
    case 5:
        load(suffix, expressions);
        break;
    case 6:
        edit(suffix, expressions);
        break;
    case 7:
        exit(suffix, expressions, stored);
        break;
    case 8:
        wexit(suffix, expressions);
        break;
    case 9:
        clear(expressions);
        break;
    case 10:
        while (REPL(expressions));
        break;
    case 11:
        is(suffix, expressions);
        break;

    default: cout << "Invalid command!" << endl;
    }
}

/**
 * @brief adds a user defined expression to expressions
 * @param suffix
 * @param mySets
 */
void let(const string& suffix, map<int, string>& expressions)
{
    bool debug = false;

    stringstream ss(suffix);
    string infix;
    string postfix;
    vector<string> expression;
    char index;
    char temp;
    int num(0);


    if (suffix.empty())
    {
        cout << "Error, no expression was given" << endl;
        return;
    }
    //if there are any chars other than valid input or if there are no variables in the suffix
    else if(suffix.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$*=+!()-/|_") < suffix.size())
    {
        cout << "Error, Invalid character input" << endl;
        return;
    }
    else if (!(suffix.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") < suffix.size())) {
        cout << "Error, Invalid input" << endl;
        return;
    }
    //we cannot have two operators or operands in a row
    //or mismatched parenthesis
    else if(ss>>index>>temp)
    {
        getline(ss, infix);
        //grab letter
        char index = suffix.substr(0, suffix.find('='))[0];

        if(temp!= '=')
        {
            cout << "INVALID EXPRESSION" << endl;
            return;
        }

        //if letters are in infix
        else if(shuntingYard(infix, postfix, expressions))
        {

            if(debug){cout << "Postfix ->: " << formatRPN(postfix) << endl;}
//            if(invalidInput(postfix))
//            {
//                return;
//            }
            if(!rpnEval(postfix).empty())
                expressions[int(index) - 65] = rpnEval(postfix);
        }
        return;
    }
    else
    {
        cout << "Invalid Expression" << endl;
        return;
    }
}

void is(const string& suffix, map<int, string> expressions)
{
    stringstream ss(suffix);
    char op1, op2, op;
    if(suffix.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ<>=") < suffix.size() || suffix.size() != 3) {
        cout << "Error, Invalid input" << endl;
        return;
    }
    if(ss>>op1>>op>>op2)
    {
        if (!isalpha(op1) || !isalpha(op2)) {
            cout << "Error, Invalid input" << endl;
            return;
        }
        else if(expressions[int(op1) - 65].empty() || expressions[int(op2) - 65].empty())
        {
            cout << "Error, set is empty" << endl;
            return;
        }
        switch(op)
        {
            case '<':
                if(LessThan(expressions[int(op1) - 65], expressions[int(op2) - 65]))
                    cout << "TRUE: " << op1 << " is less than " << op2 << endl;
                else
                    cout << "FALSE: " << op1 << " is not less than " << op2 << endl;
                break;
            case '>':
                if(GreaterThan(expressions[int(op1) - 65], expressions[int(op2) - 65]))
                    cout << "TRUE: " <<  op1 << " is greater than " << op2 << endl;
                else
                    cout << "FALSE: " << op1 << " is not greater than " << op2 << endl;
                break;
            case '=':
                if(EqualTo(expressions[int(op1) - 65], expressions[int(op2) - 65]))
                    cout << "TRUE: " <<  op1 << " is equal to " << op2 << endl;
                else
                    cout << "FALSE: " << op1 << " is not equal to " << op2 << endl;
                break;
            default:
                cout << "Error, invalid operator found" << endl;
                return;
        }
    }
}

/**
 * This function displays a given expression based on user input
 *
 * @param suffix
 * @param expressions
 */
void show(const string& suffix, map<int, string> expressions)
{
    if(suffix.empty())
    {
        cout << "No expression given" << endl;
        return;
    }
    else if (suffix.size() > 1
             || suffix.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") < suffix.size())
    {
        cout << "Error, Invalid input" << endl;
        return;
    }
    else
    {
        if(int(suffix[0]-65) < expressions.size())
        {
            if(expressions[int(suffix[0]) -65].empty())
                cout << "This expression is empty" << endl;
            else {
                cout << suffix[0] << " = " << format_result(CommaSeparator(expressions[int(suffix[0]) -65])) << endl;
            }
        }
        else
        {
            cout << "Invalid Index" << endl;
        }
    }
}

/**
 * This function displays all expression given by the user
 *
 * @param expressions
 */
void list(map<int, string>& expressions)
{
    bool debug = true;

    bool notempty = false;

    //if no expression have been entered throw error
    if (expressions.empty()) {
        cout << "Error, no expressions have been entered" << endl;
        return;
    }

    for(int i = 0; i < expressions.size(); ++i)
    {
        if(!expressions[i].empty())
        {
            notempty = true;
            cout << char(i+65) << " = ";
            cout << format_result(CommaSeparator(expressions[i])) << endl;
        }
    }

    if(!notempty)
        cout << "Error, no expressions have been entered" << endl;

}

//cannot have two ops or operands in a row
void edit(const string& suffix, map<int, string>& expressions)
{
    if (expressions.empty()) {
        cout << "Error, no expressions have been entered" << endl;
        return;
    } else if (suffix.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") < suffix.size() || suffix.size() > 1) {
        cout << "Error, Invalid input" << endl;
        return;
    } else {
        int index = int(suffix[0]) - 65;

        //input validation for index size
        if (index > expressions.size()) {
            cout << "Invalid Index" << endl;
            return;
        }

        string line;
        string postfix;

        cout << "Enter new expression: ";
        getline(cin, line);
        trimLineAndStandardize(line);
        purgeSpacesAndFormat(line);

        if (line.empty())
        {
            cout << "Error, no expression was given" << endl;
            return;
        }
        //if there are any chars other than valid input or if there are no variables in the suffix
        else if(line.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*+-/") < suffix.size())
        {
            cout << "Error, Invalid character input" << endl;
            return;
        }

        shuntingYard(line, postfix, expressions);
        expressions[index] = rpnEval(postfix);

        cout << "Expression " << char(index+65) << " has been edited" << endl;
    }
}

/**
 * @brief empties maps
 * @param mySets
 */
void clear(map<int, string> &expressions)
{
    expressions.clear();
    cout << "EXPRESSIONS CLEARED!" << endl << endl;
}

void exit(const string &suffix, map<int, string> expressions, bool stored)
{
    //this part asks if the user wants to save their current expressions
    //will only ask if they did not save right before asking the program to end
    if(stored) {
        std::cout << "Thank You!" << std::endl;
        exit(1);
    } else {
        char ans;
        std::string filename;

        std::cout << "Do you wish to save the data to a file? "<<std::endl;
        std::cin >> ans;

        char symbol;
        do {
            std::cin.get(symbol);
        } while (symbol != '\n');

        if(ans == 'Y' || ans == 'y') {
            std::cout << "What is the name of the file? "<<std::endl;
            getline(std::cin, filename);
            save(filename, expressions, stored);
            std::cout <<"Thank You!" << std::endl;
            exit(1);
        }

        std::cout << "Thank You!" << std::endl;
        exit(1);
    }
}

void wexit(const string &suffix, map<int, string> expressions)
{
    bool stored = false;
    if (expressions.empty()) {
        cout << "Error, no expressions have been entered" << endl;
        return;
    }
    save(suffix, expressions, stored);
    std::cout <<"Thank You!" << std::endl;
    exit(1);
}

/**
 * @brief saves all user entered expresison to file and sets stored to true
 * @param suffix, expressions, stored
 */
void save(const string &suffix, map<int, string> expressions, bool &stored)
{
    std::ofstream out;
    std::ifstream in;
    std::string filename = suffix;

    if (expressions.empty()) {
        cout << "Error, no expressions have been entered" << endl;
        return;
    }
    //check for empty argument
    char ans;
    //all filenames for this program must be of  the format filename.truth
    //if the user does not enter .truth after the filename, the program
    //must automatically do it for the user
    if(filename.find('.') > filename.size())
        filename += ".exp";
    in.open(filename);
    in.close();
    if(in.fail())
        out.open(filename);
    //if file already exists, the program must notify the user
    //that it does and ask if they want to erase the current file of give
    //another file name.
    else {
        in.clear();
        std::cout << "That file exists!!" << std::endl;
        std::cout << "Do you wish to overwrite it?" << std::endl;
        std::cin >> ans;

        char symbol;
        do {
            std::cin.get(symbol);
        } while (symbol != '\n');

        if(ans == 'Y' || ans == 'y')
            out.open(filename);
        else {
            std::cout << "You chose not to overwrite." << std::endl;
            return;
        }
    }

    for(int i = 0; i < expressions.size(); ++i)
    {
        //output if expression aren't empty
        if(!expressions[i].empty())
            out << char(i + 65) << " = " << format_result(CommaSeparator(expressions[i])) << endl;
    }

    std::cout << "Save successful." << std::endl;

    stored = true;
}

void load(const string &suffix, map<int, string>& expressions)
{
    ofstream out;
    ifstream in;
    string filename = suffix;
    string line;
    // Open with arg name, check to overwrite
    if(filename.find('.') > filename.size())
        filename+= ".exp";
    in.open(filename);
    if((in.fail())) {
        std::cout << "The input file does not exist!" << std::endl;
    } else {
        cout << endl;
        while (getline(in, line)) {
            cout << "let " << line << endl;
            trimLineAndStandardize(line);
            if(purgeSpacesAndFormat(line))
                let(line, expressions);
        }
        cout << endl;
        std::cout << "The file \"" << filename << "\" was loaded! \n";
        cout << endl;

    }
    out.close();
    in.close();
}

string formatInfix(string input)
{
    //removes all whitespaces
    string formatted = "";
    bool negative_start = false;

    for (int i = 0; i < input.length(); ++i)
    {
        //negative check
        if(input[0] == '-' && !negative_start)
        {
            negative_start = true;
            formatted += '~';
            formatted += " ";
            while(i + 1 < input.length() && input[i+1] == '-')
            {
                i++;
                formatted += '~';
                formatted += " ";
            }
            while (i + 1 < input.length() && isdigit(input[i+1]) || input[i+1] == '.')
            {
                i++;
                formatted += input[i];
            }
            if(i+1 < input.length() && input[i+1] == '_')
            {
                i++;
                formatted += input[i];
                while(i+1 < input.size() && input[i+1] != '+' && input[i+1] != '/'
                      && input[i+1] != '-' && input[i+1] != '*' && input[i+1] != ' '
                      && input[i+1] != '@' && input[i+1] != '#' && input[i+1] != '$'
                      && input[i+1] != '^')
                {
                    i++;
                    formatted += input[i];
                }
            }
            formatted += " ";
        }

        //EX. formatting 123
        else if (isdigit(input[i]))
        {
            formatted += input[i];
            while (i + 1 < input.length() && (isdigit(input[i+1])))
            {
                i++;
                formatted += input[i];
            }
            //mixed number
            if(i+1 < input.length() && input[i+1] == '_')
            {
                i++;
                formatted += input[i];
                while(i+1 < input.size() && input[i+1] != '+' && input[i+1] != '/'
                      && input[i+1] != '-' && input[i+1] != '*' && input[i+1] != ' '
                      && input[i+1] != '@' && input[i+1] != '#' && input[i+1] != '$'
                      && input[i+1] != '^')
                {
                    i++;
                    formatted += input[i];
                }
            }
            formatted += " ";
        }

        //-123
        else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/'
                 || input[i] == '@' || input[i] == '#' || input[i] == '$' || input[i] == '^')
        {
            formatted += input[i];
            formatted += " ";
            //negative check
            if(i + 1 < input.length() && ((input[i+1] == '-') || (input[i+1] == '+')))
            {
                //check if negative
                if(input[i+1] == '-')
                {
                    i++;
                    formatted += '~';
                    formatted += " ";
                    while(i + 1 < input.length() && input[i+1] == '-')
                    {
                        i++;
                        formatted += '~';
                        formatted += " ";
                    }
                }
                else {
                    i++;
                }

                while (i + 1 < input.length() && (isdigit(input[i+1])))
                {
                    i++;
                    formatted += input[i];
                }
                if(i+1 < input.length() && input[i+1] == '_')
                {
                    i++;
                    formatted += input[i];
                    while(i+1 < input.size() && input[i+1] != '+' && input[i+1] != '/'
                          && input[i+1] != '-' && input[i+1] != '*' && input[i+1] != ' '
                          && input[i+1] != '@' && input[i+1] != '#' && input[i+1] != '$'
                          && input[i+1] != '^')
                    {
                        i++;
                        formatted += input[i];
                    }
                }
                formatted += " ";
            }
        }

        //if left parenthesis, check for negatives after
        else if( input[i] == '(')
        {
            formatted += input[i];
            formatted += " ";
            //(-123
            if(i + 1 < input.length() && ((input[i+1] == '-') || (input[i+1] == '+')))
            {
                if (i + 2 < input.length() && isdigit(input[i+2]))
                {
                    if(input[i+1] == '-')
                    {
                        formatted += '~';
                        formatted += " ";
                    }
                    formatted += input[i+2];
                    i++;
                    i++;
                    while (i + 1 < input.length() && (isdigit(input[i+1])))
                    {
                        i++;
                        formatted += input[i];
                    }
                    if(i+1 < input.length() && input[i+1] == '_')
                    {
                        i++;
                        formatted += input[i];
                        while(i+1 < input.size() && input[i+1] != '+' && input[i+1] != '/'
                              && input[i+1] != '-' && input[i+1] != '*' && input[i+1] != ' '
                              && input[i+1] != '@' && input[i+1] != '#' && input[i+1] != '$'
                              && input[i+1] != '^')
                        {
                            i++;
                            formatted += input[i];
                        }
                    }
                    formatted+= " ";
                }
                else if( i+2 < input.length() && input[i+2] >= 'A' && input[i+2] <= 'Z')
                {
                    if(input[i+1] == '-')
                    {
                        formatted += '~';
                        formatted += " ";
                    }
                    formatted += input[i+2];
                    formatted += " ";
                    i++;
                    i++;
                }
                else if(i+2 < input.length() && input[i+2] == '(')
                {
                    formatted += '~';
                    formatted += " ";
                    i++;
                }
                else if(i+2 < input.length() && input[i+2] == '-')
                {
                    i++;
                    i++;
                    formatted += '~';
                    formatted += " ";
                    formatted += '~';
                    formatted += " ";
                    while(i + 1 < input.length() && input[i+1] == '-')
                    {
                        i++;
                        formatted += '~';
                        formatted += " ";
                    }
                }
            }
        }
        else if(input[i] >= 'A' && input[i] <= 'Z')
        {
            formatted += input[i];
            formatted += " ";
        }
        //if right parenthesis add it to formated
        else if(input[i] == ')')
        {
            formatted += input[i];
            formatted += " ";
        }
        else if(input[i] == '!')
        {
            formatted += input[i];
            formatted += " ";
        }

        //throw error
    }
    return formatted;
}

/**
 * This function implements the Shunting Yard algorithm.
 * @param expression - Infix expression
 * @return {string} - RPN expression
 */
bool shuntingYard(string expression, string& postfix, map<int, string>& expressions)
{
    expression = formatInfix(expression);
//    cout << expression << endl;
    bool debug = false;
    string outputQueue = "";
    postfix = "";
    string operatorStack = "";
    map <char, int> op;
    loadPrecedence(op);
    bool unary = false;
    int posA, posB, posF;

    for (size_t i = 0; i < expression.size(); ++i)
    {
        //As long as the input is a digit,
        //create and append to new string
        //then add to output queue
        if(isdigit(expression[i]))
        {
            string numbers = "", set;
            numbers += expression[i];
            //check for mixed numbers or mixed numbers
            while(i < expression.size() && (isdigit(expression[i+1]) || expression[i+1] == '_'
                                            || expression[i+1] == '|'))
            {
                numbers+= expression[i+1];
                i++;
            }
            //puts space to seperate digits
            numbers += " ";
            outputQueue += numbers;
        }

        //add stored variable expression to string.
        else if(expression[i] >= 'A' && expression[i] <= 'Z')
        {
            string operand = "";
            if(expressions[int(expression[i]) - 65].empty())
            {
                cout << "Error. An empty set was entered" << endl;
                return false;
            }
            else {
                operand += expressions[int(expression[i]) - 65];
                operand += " ";

                //puts space to indicate digit
                outputQueue += operand;
            }
        }

        else if (op.count(expression[i]))
        {
            if(expression[i] == '~')
            {
                operatorStack.push_back(expression[i]);
                continue;
            }

            // stack can't be empty AND top of stack cant be left parent AND token has to be greater
            // in precedence to top of stack
            while (!operatorStack.empty() && (operatorStack.back() != '(') &&
                   op.at(operatorStack.back()) >= op.at(expression[i]))
            {
                outputQueue += operatorStack.back();
                outputQueue += " ";
                operatorStack.pop_back();
            }
            operatorStack.push_back(expression[i]);
        }

        //Dealing with parenthesis
        else if (expression[i] == '(') {
            operatorStack.push_back(expression[i]);
        }

        else if (expression[i] == ')') {
            while (!operatorStack.empty() && operatorStack.back() != '(') {
                outputQueue += operatorStack.back();
                outputQueue += " ";
                operatorStack.pop_back();
            }
            if(operatorStack.size() == 0)
            {
                cout << "Mismatched Parenthesis" << endl;
                return false;
            }
            else {
                operatorStack.pop_back();
            }
            //mismatched parenthesis
        }
    }

    for (size_t i = operatorStack.length() - 1; i != string::npos; --i)
    {
        if(operatorStack[i] == '(')
        {
            cout << "Mismatched Parenthesis" << endl;
            return false;
        }
        outputQueue += operatorStack[i];
        outputQueue += " ";
        operatorStack.pop_back();
    }

    postfix = outputQueue;
    if (debug){cout << "postfix: " << postfix << endl;}
    return true;
}


/**
 * @brief This function evaluates a postfix expression
 * @param postfix expression
 * @return a string of rpn eval
 */
string rpnEval(const string& postfix)
{
    int posA, posB;
    string operand;
    string result;
    vector<string> outputStack;

    for (size_t i = 0; i < postfix.size(); ++i)
    {
        //if operand push into operand stack
        if(isdigit(postfix[i]))
        {
            string numbers = "", set;
            numbers += postfix[i];
            //check for mixed or normal number
            while(i < postfix.size() && (isdigit(postfix[i+1]) || postfix[i+1] == '_'
                                         || postfix[i+1] == '|'))
            {
                numbers+= postfix[i+1];
                i++;
            }
            numbers = SimplifyFraction(numbers);
            outputStack.push_back(numbers);
        }
        else
        {
            string first, second, result;
            switch(postfix[i])
            {
            case '^':
                first = outputStack.back();
                outputStack.pop_back();
                second = outputStack.back();
                outputStack.pop_back();
                result = Power(second, first);
                if(result == "{}")
                    return "";
                outputStack.push_back(result);
                break;
            //combination
            case '@':
                first = outputStack.back();
                outputStack.pop_back();
                second = outputStack.back();
                outputStack.pop_back();
                result = Combination(second, first);
                if(result == "{}")
                    return "";
                outputStack.push_back(result);
                break;
            //permutation
            case '#':
                first = outputStack.back();
                outputStack.pop_back();
                second = outputStack.back();
                outputStack.pop_back();
                result = Permutation(second, first);
                if(result == "{}")
                    return "";
                outputStack.push_back(result);
                break;
            //GCD
            case '$':
                first = outputStack.back();
                outputStack.pop_back();
                second = outputStack.back();
                outputStack.pop_back();
                result = GCD(second, first);
                if(result == "{}")
                    return "";
                outputStack.push_back(result);
                break;
            case '!':
                first = outputStack.back();
                outputStack.pop_back();
                result = Factorial(first);
                if(result == "{}")
                    return "";
                outputStack.push_back(result);
                break;
            case '~':
                first = outputStack.back();
                outputStack.pop_back();
                result = Negate(first);
                if(result == "{}")
                    return "";
                outputStack.push_back(result);
                break;
            case '+':
                first = outputStack.back();
                outputStack.pop_back();
                second = outputStack.back();
                outputStack.pop_back();
                result = Add(second, first);
                if(result == "{}")
                    return "";
                outputStack.push_back(result);
                break;
            case '-':
                first = outputStack.back();
                outputStack.pop_back();
                second = outputStack.back();
                outputStack.pop_back();
                result = Subtract(second, first);
                if(result == "{}")
                    return "";
                outputStack.push_back(result);
                break;
            case '*':
                first = outputStack.back();
                outputStack.pop_back();
                second = outputStack.back();
                outputStack.pop_back();
                result = Multiply(second, first);
                if(result == "{}")
                    return "";
                outputStack.push_back(result);
                break;
            case '/':
                first = outputStack.back();
                outputStack.pop_back();
                second = outputStack.back();
                outputStack.pop_back();
                result = Divide(second, first);
                if(result == "{}")
                    return "";
                outputStack.push_back(result);
                break;
            case '(':
            case ')':
                cout << "Mismatched Parenthesis" << endl;
                return "";
                break;
            default:
                break;
            }
        }
    }
    return outputStack.back();
}

/**
 * Function takes in an empty map and loads it with operator precedence. Returns true in success.
 * @param operators
 * @return true/false depending on success
 */
bool loadPrecedence(map<char, int> &operators)
{
    try
    {
        //need to implement negatives
        operators['@'] = 6; // Combination
        operators['#'] = 6; // Permutation
        operators['$'] = 6; // GCD
        operators['^'] = 5; // Power
        operators['~'] = 4; // Negate
        operators['!'] = 3; // Factorial
        operators['*'] = 2; // Multiply
        operators['/'] = 2; // Divide
        operators['+'] = 1; // Add
        operators['-'] = 1; // Subtract
        return true;
    } catch (...) {
        return false;
    }
}

/**
 * This function displays a help menu to the console output stream.
 */
void help()
{
    string border(70, '*');
    cout << border << endl << endl;

    cout << "[LET <exp>]                                Assigns a bignum expression to a memory location!" << endl << endl;
    cout << "[EDIT <exp>]                               Edits the bignum expression stored at a particular memory location!\n\n";
    cout << "[IS <exp> <relationalOp> <exp>]            Checks = < or > between two saved evaluations!\n\n";
    cout << "[SHOW <exp>]                               Show the content of a particular memory location!" << endl << endl;
    cout << "[LIST]                                     Shows all the big num expression in memory!\n\n";
    cout << "[HELP]                                     Describes the commands of this program!" << endl << endl;
    cout << "[SAVE <FILENAME>]                          Saves big num expression entered into a file!\n\n";
    cout << "[EXIT | QUIT]                              Exits the program!\n\n";
    cout << "[WEXIT <FILENAME> | WQUIT <FILENAME>]      Saves big num expression to a file and exits the program!\n\n";
    cout << "[LOAD <FILENAME>]                          Loads big num expressions from a given file!" << endl << endl;
    cout << border << endl << endl;
}

//functions for nice output
string formatRPN(string rpn)
{
    string temp = "";
    for(int i = 0; i < rpn.size(); ++i)
    {
        if(rpn[i] == '|')
            temp+= '/';
        else if(rpn[i] == '_')
            temp+= ' ';
        else {
            temp+= rpn[i];
        }
    }
    return temp;
}

string format_result(string result)
{
    string temp = "";
    if(result.empty())
        temp = "{}";
    else {
        for(int i = 0; i < result.size(); ++i)
        {
            if(result[i] == '|')
                temp+= '/';
            else if(result[i] == '_')
                temp+= ' ';
            else {
                temp+= result[i];
            }
        }
    }
    return temp;
}

/**
 * This function allows for immediate evaluation of expressions.
 * @param expressions
 * @return
 */
bool REPL(map<int, string> &expressions) {

    std::string input, postfix;
    try {
        if (!GetInput(input, ">> "))
            return true;

        if (input.empty()) {
            printf("Exiting REPL mode.\n");
            return false;
        }

        purgeSpacesAndFormat(input);


        if (shuntingYard(input, postfix, expressions))
        {
            cout << postfix << endl;
            printf("=> %s\n", format_result(rpnEval(postfix)).c_str());
        }
        else {
            printf("\nError : Syntax Error\n");
            return true;
        }
    }
    catch (...) { // todo :: this doesnt seem to catch errors
        printf("Error! Exiting REPL mode");
        return false;
    }
    return true;
}
