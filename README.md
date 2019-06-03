# Large Number Arithmetic with Fractions Too!


On the machines that we've used in class, an integer in C++ is 32 bits, of which 31 can be used for positive integer arithmetic. This is good for representing numbers up to about two billion (2 times 109).

Some compilers, such as GCC, offer a "long long" type, giving 64 bits capable of representing about 9 quintillion (9 times 1018)

This is good for most purposes, but some applications require many more digits than this. For example, public-key encryption with the RSA algorithm typically requires 300 digit numbers. Computing the probabilities of certain real events often involves very large numbers; although the result might fit in a typical C type, the intermediate computations require very large numbers.

For example, what is the probability of winning the Texas Lottery jackpot prize with one ticket? The number of combinations of 50 numbers taken 6 at a time, "50 choose 6", is 50!/((50-6)!6!). That number is 15,890,700, so the odds of winning are 1/15,890,700. The number 15,890,700 can be represented easily by a C integer, but the (naive) computation of that number involves computing 50!, which is:

30,414,093,201,713,378,043,612,608,166,064,768,844,377,641,568,960,512,000,000,000,000
This number will not fit into a C++ integer, not even a 64 bit one.

So we must move to a different representation of non-negative integers. We can represent a number as a sequence of digits stored in an array of integers. We can write functions to add, subtract, multiply, divide, factorial, etc. on those arrays, and then make them as large as we want.

As such, the task is to create a program (in C++, Python, BASH, AWK, Perl, but NOT Java! -- no classes are allowed, hence no Java!), without the aid of programmer-developed classes (this means that you cannot create a class and that the program has to conform to a CS 002-style program, BUT, any header file on cplusplus.com can be used!) The program shall  meet these specifications:

The commands that the user will be able to enter are:

Addition will be represented by the plus sign (+) 

Multiplication will be represented by the asterisk (*)

Subtraction will be represented by the minus sign (-)

Division will be represented by the slash (/)

Factorial will be represented by (!)

Combinations will be represented by C(n,m), where n >= m

Permutations will be represented by P(n,m), where n >= m

The GCD of two numbers will be represented by GCD(n,m)

The equality operations will be represented as normal.

There will be 26 "memory" locations, A to Z.

LET, which enables an individual to create a bignum expression and assign it to a memory location

SHOW, which enables an individual to see the contents of a particular memory location

LIST, which enables an individual to see the contents of all big nums in memory

HELP - Give help

Expressions cannot consist of more than one operation. BUT, expressions can consist of a combination of other expressions. Meaning you could enter LET C = B / A
Remainders must be stored as a fraction in lowest terms! (Structs are not allowed!!! Hint: Multiple arrays)
Numbers such as 2 2/3, 3/4, or 0 have to be able to be entered!
If a stored number has a remainder, you have to perform the operation!
All output must be visually appealing (commas for thousands separators, etc), 
To load expressions from a file, the user will type LOAD and then a file name. 
To edit an expression (meaning to change it without entering a new expression), the user will enter EDIT x, where x is the expression number. The program will then query the user for a new expression.
The program ends whenever the user enters a blank line,  EXIT or QUIT. When exiting, the program must ask the user if they want to save all of the current expressions if and only if the user did not save the expressions right before asking the program to terminate.
If the user types WEXIT filename or WQUIT filename, this will write the file and exit the program.
If the user enters HELP, then the program will write out the precedence relations as well as instructions on each command, and so forth.

---

Extra Credit (200 points): 

Write a fully-functional calculator for bigNum numbers. This calculator must accept the expression in in-fix format, then display and calculate the RPN translation of the expression that the user gave as input.

This program must allow users to enter bignums with or without comma separators (which means that you must correctly identify if the number has been entered correctly). This  program must also handle "mixed bignums" -- meaning positive and negative fractions, and mixed numbers.

 

Extra, Extra Credit (100 points):

Write a program that will compute Pi to n digits, where n is entered by the user. If n = 100, then 100 digits of Pi will be written.

Extra, Extra, Extra Credit (100 points)

Write a program to print out the first n prime numbers, up to the maximum allowed for bigNum which is string::npos digits.

Extra^4 Credit (100 points)

Include in the primary program for the Final Project the ability compute the following:

1. All of the possible poker hands that uses Big Number Arithmetic (Meaning, how many different Full Houses are there? How many Straights are there? and so forth). This would use the "combinations" from statistics/probability.

2. What are the odds of drawing a _____ on the first deal (where _____ is Four of a Kind, Three of a Kind, Full House, etc...)
