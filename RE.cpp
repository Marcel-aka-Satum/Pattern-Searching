//
// Created by aaditya on 11.05.22.
//

#include "RE.h"
#include <fstream>
#include <set>

RE::RE(string r, char e) {
    regex = r;
    string epsilon(1, e);
    eps = epsilon;
}
void RE::addState(int name, bool accepting, bool starting) {
    renfa["states"].push_back({{"name",      name},
                               {"starting",  starting},
                               {"accepting", accepting}});
}
void RE::addTransition(int from, int to, string input) {
    renfa["transitions"].push_back({{"from",  from},
                                    {"to",    to},
                                    {"input", input}});
}
void RE::sum(block *R, block *S) {
    addTransition(R->start - 1, R->start, eps);
    addTransition(R->start - 1, S->start, eps);
    addTransition(S->end, S->end + 1, eps);
    addTransition(R->end, S->end + 1, eps);
}
void RE::concat(block *R, block *S) {
    addTransition(R->end, S->start, eps);
}
void RE::kleene(block *R) {
    addTransition(R->start - 1, R->start, eps);
    addTransition(R->start - 1, R->end + 1, eps);
    addTransition(R->end, R->end + 1, eps);
    addTransition(R->end, R->start, eps);
}
string RE::removeBracket(string expression) {
    int open = 0;
    int close = 0;
    bool check = true;
    for (int i = 0; i < expression.size(); i++) {
        if (expression[i] == '(')
            open++;
        else if (expression[i] == ')')
            close++;

        if (close == open && i < expression.size() - 1)
            check = false;
    }

    if (expression[0] == '(' && expression[expression.size() - 1] == ')' && check)
        return expression.substr(1, expression.size() - 2);
    return expression;
}
bool RE::noBrackets(string const &expression) {
    return all_of(expression.begin(), expression.end(), [](char i) { return (i != '(' && i != ')'); });
}
int RE::calculateOperations(string const &expression) {
    int op = 0;
    for (auto elem : expression) {
        if (elem == '+' || elem == '*' || (elem != ')' && elem != '('))
            op += 2;
    }
    return op;
}
void RE::doOperation(block *R, block *S, string const &operation) {
    if (operation == "*")
        kleene(R);
    else if (operation == "+")
        sum(R, S);
    else {
        concat(R, S);
    }
}
bool RE::noOperators(string const &expression) {
    for (auto const &c : expression) {
        if (c == '(' || c == ')' || c == '+' || c == '*')
            return false;
    }
    return true;
}
void RE::findStartEnd(block *a, int *startPoint, int *endPoint) {
    int open = 0;
    int close = 0;

    if (noOperators(a->expression)) {
        *startPoint = 0;
        *endPoint = 1;
    } else if (noBrackets(a->expression)) {
        *startPoint = 0;
        for (int i = 0; i < a->expression.size(); i++) {
            if (a->expression[i] == '+' || a->expression[i] == '*')
                *endPoint = i;
        }
    } else {
        for (int i = 0; i < a->expression.size(); i++) {
            if (a->expression[i] == '(' && open == 0) {
                open++;
                *startPoint = i;
            } else if (a->expression[i] == '(')
                open++;
            else if (a->expression[i] == ')')
                close++;

            if (open == close && open > 0) {
                if (a->expression[i + 1] == '*' && a->expression.size() > i + 2)
                    *endPoint = i + 2;
                else
                    *endPoint = i + 1;
                break;
            }
        }
    }
}

void RE::findExpressions(int startPoint, int endPoint, string *expression1, string *expression2, string *operation,
                         block a) {
    if (startPoint == 0) {
        *expression1 = removeBracket(a.expression.substr(startPoint, endPoint));
        *expression2 = removeBracket(a.expression.substr(endPoint + 1, a.expression.size() - expression1->size() - 1));
        *operation = a.expression[endPoint];

        if (*operation != "+" && *operation != "*")
            *expression2 = removeBracket(*operation + *expression2);
    } else {
        *expression1 = removeBracket(a.expression.substr(0, startPoint - 1));
        *expression2 = removeBracket(a.expression.substr(startPoint, a.expression.size() - expression1->size()));
        *operation = a.expression[startPoint - 1];
        if (*operation != "+" && *operation != "*")
            *expression1 = removeBracket(*expression1 + *operation);
    }
}

void RE::buildBlock(block *x, block *y, block const &a, string const &expression1, string const &expression2,
                    string const &operation) {
    if (operation == "*") {
        *x = {a.start + 1, a.end - 1, expression1};
    } else if (operation == "+") {
        *x = {a.start + 1, a.start + calculateOperations(expression1), expression1};
        *y = {x->end + 1, a.end - 1, expression2};
    } else {
        *x = {a.start, a.start + calculateOperations(expression1) - 1, expression1};
        *y = {x->end + 1, a.end, expression2};
    }
}
pair<RE::block, RE::block> RE::breakUp(block a) {
    pair<RE::block, RE::block> p;
    int startPoint;
    int endPoint;
    string operation;
    string expression1;
    string expression2;

    findStartEnd(&a, &startPoint, &endPoint);
    findExpressions(startPoint, endPoint, &expression1, &expression2, &operation, a);
    block x;
    block y;

    buildBlock(&x, &y, a, expression1, expression2, operation);
    doOperation(&x, &y, operation);

    p.first = x;
    p.second = y;
    return p;
}
void RE::findTransitions(block &a) {
    pair<RE::block, RE::block> p;

    if (a.expression.size() <= 1) {
        if (!a.expression.empty())
            transitions.push_back(a);
        return;
    }

    p = breakUp(a);
    findTransitions(p.first);
    findTransitions(p.second);
}

void RE::buildTransitions() {
    for (auto const &elem : transitions) {
        addTransition(elem.start, elem.end, elem.expression);
        alph.insert(elem.expression);
    }
}

void RE::buildStates(int end) {
    for (int i = 1; i < end; i++) {
        addState(i, false, false);
    }
}

void RE::buildAlph() {
    for (auto a : alph) {
        if (!a.empty() && a != eps)
            renfa["alphabet"].push_back(a);
    }
}
ENFA RE::toENFA() {
    int end = calculateOperations(regex) - 1;
    renfa = {{"type",        "ENFA"},
             {"eps",         eps},
             {"transitions", {"", ""}},
             {"alphabet",    {"", ""}},
             {"states",      {{{"name", 0}, {"starting", true}, {"accepting", false}},
                                  {{"name", end}, {"starting", false}, {"accepting", true}}}}};

    block start = {0, end, regex};

    buildStates(end);
    findTransitions(start);
    buildTransitions();
    buildAlph();

    renfa["alphabet"].erase(renfa["alphabet"].begin());
    renfa["alphabet"].erase(renfa["alphabet"].begin());
    renfa["transitions"].erase(renfa["transitions"].begin());
    renfa["transitions"].erase(renfa["transitions"].begin());
    ofstream file("enfa.json");
    file << renfa;
    file.close();
    ENFA enfa("enfa.json");
    return enfa;
}