#include <iostream>
#include <set>

using namespace std;

void T(istream& in);
void F(istream& in);
void A1(istream& in);
void A2(istream& in);
void E(istream& in);

char ch;
set<char> validSymbols = { '*', '+', '-', '9', 'a', '(', ')' };

void A2(istream& in) {
    if (ch == '*') {
        in >> ch;
        F(in);
        A2(in);
        return;
    }
}

void A1(istream& in) {
    if (ch == '+') {
        in >> ch;
        T(in);
        A1(in);
        return;
    }
}

void S(istream& in) {
    E(in);
    if (ch != '#')
        throw exception();
}

void E(istream& in) {
    in >> ch;
    T(in);
    A1(in);
}

void T(istream& in) {
    F(in);
    A2(in);
}

void F(istream& in) {
    if (ch == '-') {
        in >> ch;
        F(in);
        return;
    }
    else if (ch == '9') {
        in >> ch;
        return;
    }
    else if (ch == 'a') {
        in >> ch;
        return;
    }
    else if (ch == '(') {
        E(in);
        if (ch == ')') {
            in >> ch;
            return;
        }
    }
    if (validSymbols.find(ch) != validSymbols.end())
        throw exception("Ошибка");
}

int main()
{
    try
    {
        S(cin);
        cout << "norm" << endl;
    }
    catch (...)
    {
        cout << "not norm" << endl;
    }
}