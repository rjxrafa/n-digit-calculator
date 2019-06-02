/**
 * @name    Large Number Arithmetic with Fractions Too!
 * @author  Rafael Betita & Edgar Flores
 * @date    2019-06-12
 *
 * @brief
 */

#include "includes/io.h"
#include "includes/mylib.h"
#include "includes/arithmetic.h"
#include <thread>
#include <afxres.h>

using namespace std;

void introduction();
void loadCommands(map<string, int> &commands);
bool getLine(string &line);
void process(string &line, const map<string, int> &commands,
             map<int, string>& expressions, bool &stored);
void trimLineAndStandardize(string &line);
void purgeSpaces(string &line);
void findCommand(string line, string &command, string &suffix);
void executeCommand(const string &command, const string &suffix,
                    const map<string, int> &commands,
                    map<int, string>& expressions, bool &stored);


void let(const string& suffix, map<int, string>& expressions);
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
void help();


int main()
{
    introduction();
    map<int, string> expressions;
    map<string, int> commands;
    string line;
    bool stored = false;
    loadCommands(commands);
    while(getLine(line))
        process(line, commands, expressions, stored);
    exit(line, expressions, stored);
    return 0;

//  std::string op1, op2, prod;

//  while (1) {
//    std::thread a;
////    CreateThread a;
////    a
//    while (!GetInput(op1, "Enter operand 1: "));
//    while (!GetInput(op2, "Enter operand 2: "));
//    prod = Add(op1, op2);
//    std::cout << prod << '\n';

////    std::cout << Factorial(op1) << '\n';
////    std::cout << CommaSeparator(op1) << std::endl;
//  }

  return 0;
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
    commands["LET"]  = 0;
    commands["SHOW"] = 1;
    commands["LIST"] = 2;
    commands["HELP"] = 3;
    commands["SAVE"] = 4;
    commands["LOAD"] = 5;
    commands["EDIT"]   = 6;
    commands["EXIT"] = 7;
    commands["QUIT"] = 7;
    commands["WEXIT"] = 8;
    commands["WQUIT"] = 8;
    commands["CLEAR"] = 9;
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
    while((pos = line.find('\t ', ++pos)) < line.size())
        while(line[pos+1] == '\t')
            line.erase(pos+1,1);
    //modifies input so all chars are uppercase
    for(unsigned int i = 0; i < line.size(); ++i)
        line[i] = toupper(line[i]);
}

/**
 * This function removes all spaces from a given string
 * @param suffix
 */
void purgeSpaces(string &line)
{
    std::string temp = "";
    for (unsigned int i = 0; i < line.length(); ++i)
    {
        //skip tabs
        if(line[i] == '\t')
        {
            continue;
        }
        if ((line[i] != ' ')) {
            temp += toupper(line[i]);
        }
    }
    line = temp;
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
    purgeSpaces(suffix);
    executeCommand(command, suffix, commands, expressions, stored);
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
    else if(suffix.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*=+-/") < suffix.size())
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
    else if(ss>>index>>temp>>infix)
    {
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

            if(debug){cout << "POSTFIX: " << postfix << endl;}
//            if(invalidInput(postfix))
//            {
//                return;
//            }
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
            cout << suffix[0] << " = " << expressions[int(suffix[0]) -65] << endl;
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

    //if no expression have been entered throw error
    if (expressions.empty()) {
        cout << "Error, no expressions have been entered" << endl;
        return;
    }

    for(int i = 0; i < expressions.size(); ++i) {
        cout << char(i+65) << " = ";
        cout << expressions[i] << endl;
    }
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
        purgeSpaces(line);

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
        out << char(i + 65) << " = " << expressions[i] << endl;

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
        while (getline(in, line)) {
            trimLineAndStandardize(line);
            purgeSpaces(line);
            let(line, expressions);
        }
        std::cout << "The file \"" << filename << "\" was loaded! \n";
    }
    out.close();
    in.close();
}

/**
 * This function implements the Shunting Yard algorithm.
 * @param expression - Infix expression
 * @return {string} - RPN expression
 */
bool shuntingYard(string expression, string& postfix, map<int, string>& expressions)
{
    bool debug = false;
    string outputQueue;
    string operatorStack;
    map <char, int> op;
    loadPrecedence(op);
    bool unary = false;
    int posA, posB;

    for (size_t i = 0; i < expression.size(); ++i)
    {
        //As long as the input is a digit,
        //create and append to new string
        //then add to output queue
        if(isdigit(expression[i]))
        {
            string numbers = "", set;
            numbers += expression[i];
            while(i < expression.size() && isdigit(expression[i+1]))
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
            operand += expressions[int(expression[i]) - 65];
            operand += " ";

            //puts space to indicate digit
            outputQueue += operand;
        }

        else if (op.count(expression[i]))
        {
            // stack can't be empty AND top of stack cant be left parent AND token has to be greater
            // in precedence to top of stack
            while (!operatorStack.empty() && (operatorStack.back() != '(') &&
                   op.at(operatorStack.back()) <= op.at(expression[i]))
            {
                outputQueue.push_back(operatorStack.back());
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
                outputQueue.push_back(operatorStack.back());
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
        else if(expression[i] == '}')
        {
            cout << "Mismatched Bracket" << endl;
            return false;
        }
    }

    for (size_t i = operatorStack.length() - 1; i != string::npos; --i)
    {
        if(operatorStack[i] == '(')
            return false;
        outputQueue += operatorStack[i];
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
            while(i < postfix.size() && isdigit(postfix[i+1]))
            {
                numbers+= postfix[i+1];
                i++;
            }
            outputStack.push_back(numbers);
        }
        else
        {
            string first, second, result;
            switch(postfix[i])
            {
                case '+':
                    first = outputStack.back();
                    outputStack.pop_back();
                    second = outputStack.back();
                    outputStack.pop_back();
                    result = Add(second, first);
                    outputStack.push_back(result);
                    break;
                case '-':
                    first = outputStack.back();
                    outputStack.pop_back();
                    second = outputStack.back();
                    outputStack.pop_back();
                    result = Subtract(second, first);
                    outputStack.push_back(result);
                    break;
//                case '*':
//                    first = outputStack.back();
//                    outputStack.pop_back();
//                    second = outputStack.back();
//                    outputStack.pop_back();
//                    result = setIntersection(second, first);
//                    outputStack.push_back(result);
//                    break;
//                case '/':
//                    first = outputStack.back();
//                    outputStack.pop_back();
//                    second = outputStack.back();
//                    outputStack.pop_back();
//                    result = setDifference(second, first);
//                    outputStack.push_back(result);
//                    break;
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
    try {
        operators['+'] = 2u;
        operators['-'] = 2u;
        operators['*'] = 1u;
        operators['/'] = 1u;
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
    cout << "[SHOW <exp>]                               Show the content of a particular memory location!" << endl << endl;
    cout << "[LIST]                                     Shows all the big num expression in memory!\n\n";
    cout << "[HELP]                                     Describes the commands of this program!" << endl << endl;
    cout << "[SAVE <FILENAME>]                          Saves big num expression entered into a file!\n\n";
    cout << "[EXIT | QUIT]                              Exits the program!\n\n";
    cout << "[WEXIT <FILENAME> | WQUIT <FILENAME>]      Saves big num expression to a file and exits the program!\n\n";
    cout << "[LOAD <FILENAME>]                          Loads big num expressions from a given file!" << endl << endl;

    cout << border << endl << endl;
}
