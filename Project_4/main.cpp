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
    
    vector<vector<string>> v1;
        t.find("Patel", v1);
        assert(v1.size() == 2);
        vector<vector<string>> expected1 = {
            { "Patel", "12345", "42.54", "Westwood" },
            { "Patel", "67890", "142.75", "Hollywood" }
        };
        assert((v1[0] == expected1[0]  &&  v1[1] == expected1[1])  ||
               (v1[0] == expected1[1]  &&  v1[1] == expected1[0]) );


    
    vector<vector<string>> v;
    assert(t.select("location < Westwood", v) == 0);
    assert(v.size() == 2);
    vector<vector<string>> expected = {
        { "Hoang", "12345", "30.46", "Santa Monica" },
        { "Patel", "67890", "142.75", "Hollywood" }
    };
    assert((v[0] == expected[0]  &&  v[1] == expected[1])||(v[0] == expected[1]  &&  v[1] == expected[0]) );
    
    
    vector<string> cols1 = { "item name", "price" };
        Table t1("item name", cols1);
        assert(t1.good());
        assert(t1.insert("'chocolate bar' 1.69"));
        assert(t1.insert("coffee 7.99"));
        assert(t1.insert("hummus 3.49"));
        vector<vector<string>> v2;
        assert(t1.select("price LT 5", v2) == 0);
        assert(v2.size() == 2);  // chocolate bar and hummus
        assert(t1.insert("pretzels 1.W9"));
        assert(t1.select("price LT H", v2) == -1);
        assert(v2.size() == 0);
        assert(t1.select("price LT 5", v2) == 1);  // 1 because pretzels 1.W9
        assert(v2.size() == 2);  // chocolate bar and hummus
    cerr<<"Passed all tests"<<endl;
    return 0;
}
