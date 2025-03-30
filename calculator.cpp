#include <iostream>
#include <stack>
#include <cstring> // For strcpy

using namespace std;

class Calculator {
private:
    bool isOperator(const char* s) {
        return (s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/');
    }

    bool isNumber(const char* s) {
        bool hasDecimal = false;
        if (s[0] == '\0') return false;
        for (int i = 0; s[i] != '\0'; ++i) {
            char c = s[i];
            if (c == '.') {
                if (hasDecimal) return false;
                hasDecimal = true;
            } else if (!isdigit(c)) return false;
        }
        return true;
    }

    float calculate(float a, float b, const char* op) {
        if (op[0] == '+') return a + b;
        if (op[0] == '-') return a - b;
        if (op[0] == '*') return a * b;
        if (op[0] == '/' && b != 0) return a / b;
        return 0;
    }

    int getPrecedence(const char* op) {
        if (op[0] == '+' || op[0] == '-') return 1;
        if (op[0] == '*' || op[0] == '/') return 2;
        return 0;
    }

public:
    void processCalculation() {
        char expression[1000];
        cout << "Enter expression (use spaces between numbers and operators):" << endl;
        cout << "Example: 3 + 4 * 2" << endl;
        cout << ">> ";
        cin.getline(expression, 1000);

        char tokens[200][50];
        int tokenCount = 0;
        int tokenIndex = 0;

        for (int i = 0; expression[i] != '\0'; ++i) {
            if (expression[i] == ' ') {
                if (tokenIndex > 0) {
                    tokens[tokenCount][tokenIndex] = '\0';
                    tokenCount++;
                    tokenIndex = 0;
                }
            } else {
                tokens[tokenCount][tokenIndex++] = expression[i];
            }
        }
        if (tokenIndex > 0) {
            tokens[tokenCount][tokenIndex] = '\0';
            tokenCount++;
        }

        char postfix[200][50];
        int postfixCount = 0;
        stack<char*> ops;

        for (int i = 0; i < tokenCount; ++i) {
            if (isNumber(tokens[i])) {
                strcpy(postfix[postfixCount], tokens[i]); // Copy the string
                postfixCount++;
            } else if (isOperator(tokens[i])) {
                while (!ops.empty() && isOperator(ops.top()) && getPrecedence(ops.top()) >= getPrecedence(tokens[i])) {
                    strcpy(postfix[postfixCount], ops.top()); // Copy the string
                    postfixCount++;
                    ops.pop();
                }
                ops.push(tokens[i]);
            }
        }

        while (!ops.empty()) {
            strcpy(postfix[postfixCount], ops.top()); // Copy the string
            postfixCount++;
            ops.pop();
        }

        stack<float> stack;

        for (int i = 0; i < postfixCount; ++i) {
            if (isNumber(postfix[i])) {
                stack.push(atof(postfix[i]));
            } else if (isOperator(postfix[i])) {
                if (stack.size() < 2) {
                    cout << "Error: Insufficient operands" << endl;
                    return;
                }

                float b = stack.top();
                stack.pop();
                float a = stack.top();
                stack.pop();

                if (postfix[i][0] == '/' && b == 0) {
                    cout << "Error: Division by zero" << endl;
                    return;
                }

                float result = calculate(a, b, postfix[i]);
                stack.push(result);
            } else {
                cout << "Error: Invalid token '" << postfix[i] << "'" << endl;
                return;
            }
        }

        if (stack.size() != 1) {
            cout << "Error: Invalid expression" << endl;
            return;
        }

        cout << "Result: " << stack.top() << endl;
    }
};

int main() {
    Calculator calc;
    char continueCalc;

    do {
        calc.processCalculation();

        cout << "\nWould you like to calculate again? (y/n): ";
        cin >> continueCalc;
        cin.ignore();
    } while (tolower(continueCalc) == 'y');

    cout << "Calculator terminated." << endl;
    return 0;
}