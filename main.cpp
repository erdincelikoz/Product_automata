#include "DFA.h"

using namespace std;

int main() {
    DFA dfa1("C:/Users/erdin/CLionProjects/Productautomaton/input-product-and1.json");
    DFA dfa2("C:/Users/erdin/CLionProjects/Productautomaton/input-product-and2.json");
    DFA product(dfa1,dfa2,false); // true betekent doorsnede, false betekent unie
    product.print();
    return 0;
}