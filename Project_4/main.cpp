//
//  main.cpp
//  Project_4
//
//  Created by Noa Lehrer on 5/28/21.
//

#include <iostream>
#include "Table.h"

using namespace std;

int main() {
    vector<string> cols = {
        "customer", "product", "price", "location"
    };
    Table t("customer", cols);
    assert(t.good());
    assert(t.insert("Patel 12345 42.54 Westwood"));
    assert(t.insert("O'Reilly 34567     4.99 Westwood   "));
    assert(t.insert("   Hoang  12345 30.46 'Santa Monica' "));
    assert(t.insert("Patel\t67890\t142.75  \t \t\t  \tHollywood"));
    assert( ! t.insert("Figueroa 54321 59.95"));
    
    vector<vector<string>> v;
        t.find("Patel", v);
        assert(v.size() == 2);
        vector<vector<string>> expected = {
            { "Patel", "12345", "42.54", "Westwood" },
            { "Patel", "67890", "142.75", "Hollywood" }
        };
        assert((v[0] == expected[0]  &&  v[1] == expected[1])  ||
               (v[0] == expected[1]  &&  v[1] == expected[0]) );
    
    cerr<<"Passed all tests"<<endl;
    return 0;
}
