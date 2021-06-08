//
//  main.cpp
//  Project_4
//
//  Created by Noa Lehrer on 5/28/21.
//

#include <iostream>
#include "Table.h"
#include <cassert>

using namespace std;

int testone(){
    vector<string> cols = { "N", "Z" };
            Table t("Z", cols);
            assert(t.good());
            assert(t.insert("UCLA 90095"));
            assert(t.insert("Caltech 91125"));
            vector<vector<string>> v;
            t.find("90095", v);
            assert(v.size() == 1);
            assert(v[0][0] == "UCLA"  &&  v[0][1] == "90095");
            assert(t.select("Z > 90210", v) == 0);
            assert(v.size() == 1);
            assert(v[0][0] == "Caltech"  &&  v[0][1] == "91125");

            vector<string> uclacols = { "last name", "first name", "major", "UID", "GPA" };
            Table ucla("UID", uclacols);
            assert(ucla.good());
            assert(ucla.insert("Bruin Jose 'COG SCI' 304567890 3.4"));
            assert(ucla.insert("Bruin Josephine 'COM SCI' 605432109 3.8"));
            assert(ucla.insert("Trojan Tommy LOSING 000000000 1.7"));
              // Troy brought a wooden horse full of Greek soldiers inside the
              // city walls and lost the Trojan War.  How strange to look up to
              // gullible losers as role models.
            assert(ucla.select("GPA GE 3.2", v) == 0);
            assert(v.size() == 2);
            assert(v[0][0] == v[1][0]  &&  v[0][1] != v[1][1]);

            cout << "DONE" << endl;
    return 0;
}


int testtwo() {
    vector<string> cols = {
        "customer", "product", "price", "location"
    };
    Table t("customer", cols);
    Table r("invalid", cols);
    Table s("", cols);
    assert(!r.good());
    assert(!s.good());
    assert(t.good());
    assert(t.insert("Patel 12345 42.54 Westwood"));
    assert(t.insert("O'Reilly 34567     4.99 Westwood   "));
    assert(t.insert("   Hoang  12345 30.46 'Santa Monica' "));
    assert(t.insert("Patel\t67890\t142.75  \t \t\t  \tHollywood"));
    assert( ! t.insert("Figueroa 54321 59.95"));
    assert( ! t.insert("Figueroa 54321 59.95 xyz pqr"));

    
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

int testthree(){
    vector<string> cols = {"customer", "product", "price", "location"};
    Table t("customer", cols);
    assert(t.good());
    assert(t.insert("Patel 12345 42.54 Westwood"));
    assert(t.insert("O'Reilly 34567     4.99 Westwood   "));
    assert(t.insert("   Hoang  12345 30.46 'Santa Monica' "));
    assert(t.insert("Patel\t67890\t142.75  \t \t\t  \tHollywood"));
    assert( ! t.insert("Figueroa 54321 59.95"));
    
    vector<vector<string>> v;
    //well formed
    assert(t.select("location = 'Santa Monica'", v)==0);
    assert(v.size()>=1);
    v.clear();
    assert(t.select("product == 12345", v)==0);
    assert(v.size()>=1);
    v.clear();
    assert(t.select("product != 00012345", v)==0);
    assert(v.size()>=1);
    v.clear();
    assert(t.select("product EQ 12345", v)==0);
    assert(v.size()>=1);
    v.clear();
    assert(t.select("product EQ 00012345", v)==0);
    assert(v.size()>=1);
    v.clear();
    assert(t.select("product < 900", v)==0);
    assert(v.size()>=1);
    v.clear();
    assert(t.select("product GT 900", v)==0);
    assert(v.size()>=1);
    v.clear();
    
    //not well formed
    assert(t.select("Hollywood == location", v)==-1);
    assert(v.size()==0);
    v.clear();
    assert(t.select("price is expensive", v)==-1);
    assert(v.size()==0);
    v.clear();
    assert(t.select("location = Santa Monica", v)==-1);
    assert(v.size()==0);
    v.clear();
    assert(t.select("price GT ''", v)==-1);
    assert(v.size()==0);
    v.clear();
    assert(t.select("price GT 100K", v)==-1);
    assert(v.size()==0);
    v.clear();
    return 0;
}

int main(){
    testone();
    testtwo();
    testthree();
    cerr<<"ALL DONE"<<endl;
    return 0;
}
