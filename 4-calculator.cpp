#include <iostream>
#include <cassert>
#include <istream>
#include <regex>

using namespace std;

typedef float Item;


// Default size of stack
#define MAX_SIZE 1024
class StackImpIUsingVector
{
private:
    int sp;
    Item *_stack; // TOS+Low2High
    int size;

public:
    StackImpIUsingVector(int size=MAX_SIZE) {
        sp = -1;
        _stack = new Item[size];
        this->size = size;
    }
    ~StackImpIUsingVector() {
        delete []_stack;
    }
    void push(Item x) {
        assert(sp<this->size-1);
        _stack[++sp] = x;
    }
    Item pop() {
        assert(sp>=0);
        return _stack[sp--];
    }
    int isEmpty() {
        return (sp==-1);
    }
};


regex rgx_float_op("\\s*(-?[0-9]+(?:\\.[0-9]+)?)|(\\+|-|\\*|/|=|d)"); // Combined regex to match floats and operators into their own capturing groups. Also added whitespace ignoring.

int calculate(string s, StackImpIUsingVector &fStack, StackImpIUsingVector &opStack) {
    Item f;
    char op;
    smatch match; // Used for storing operand and operator groups.

    Item f1;
    Item f2;

    // Using regex_search to find operands and operators from user input and pushing them into their corresponding stacks.
    while (regex_search(s, match, rgx_float_op)) {
        for (size_t i = 1; i < match.size(); ++i) { //For loop iterates between the operand capturing group and operator capturing group.
            if (match[i].matched) { // If true, it means that the current capturing group matched something. "1 == operands" and "2 == operators".
                if (i == 1) {
                    f = stod(match.str(i));
                    fStack.push(f);

                } else if (i == 2) {
                    op = match.str(i)[0];
                    opStack.push(op);

                }
            }
        }
        s = match.suffix(); // Update the remaining string to search
    }

        // Executing the calculations as long as operator stack has items.
        while(!opStack.isEmpty()) {
            op = opStack.pop();
            if(op == '='&&opStack.isEmpty()) { // Prints the result if True.
                f = fStack.pop();
                cout << "= " << f << endl;
                return 0;
            }

            f2 = fStack.pop();
            f1 = fStack.pop();
            switch(op) {
                case '+':
                    f = f1 + f2;
                    fStack.push(f);
                    break;
                case '-':
                    f = f1 - f2;
                    fStack.push(f);
                    break;
                case '/':
                    f = f1 / f2;
                    fStack.push(f);
                    break;
                case '*':
                    f = f1 * f2;
                    fStack.push(f);
                    break;
                default:
                    break;
            }
        }
    return 0;
}

int main() {
    StackImpIUsingVector fStack; // Stack for operands
    StackImpIUsingVector opStack; //Stack for operators
    int done = 0;
    string s;

    // Get input from user
    while(!done) {
        cout << "Give calculation, separate numbers with a space: ";
        getline(cin, s);
        if(s.length() == 0) return 0;

        calculate(s, fStack, opStack);
    }
    return 0;
}
