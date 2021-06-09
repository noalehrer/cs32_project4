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

int test1(){
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


int test2() {
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

int test3(){
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
    //MARK: - sometimes this is weird
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

void ctest1(){
    vector<string> goodtest1_v = {"column1", "column2"};
    Table goodtest1("notcolumnname", goodtest1_v);
    assert(!goodtest1.good());
    assert(!goodtest1.insert("one two"));
    vector<vector<string>> goodtest1_w;
    goodtest1.find("column1", goodtest1_w);
    assert(goodtest1_w.empty());
    assert(goodtest1.select("idk", goodtest1_w) == -1);
        
    vector<string> goodtest2_v;
    Table goodtest2("any", goodtest2_v);
    assert(!goodtest2.good());
    assert(!goodtest2.insert("one two"));
        
    vector<string> goodtest3_v = {"duplicate", "duplicate"};
    Table goodtest3("duplicate", goodtest3_v);
    assert(!goodtest3.good());
    assert(!goodtest3.insert("one two"));
}

void ctest2(){
    vector<string> friends = { "Name", "Birthday", "Phone", "FavColor" };
        Table myFriends("Name", friends);
        assert(myFriends.good());
        assert(myFriends.insert("Patty 0423 1234567 pink"));
        assert(myFriends.insert("Monica            0223 12435235 blue"));
        assert(myFriends.insert("   Natalie  1225 12481241       'light purple'"));
        assert(myFriends.insert("Matthew 0312        14685343 \t\t\t black"));
        vector<vector<string>> records;
        myFriends.find("Natalie", records);
        assert(records.size() == 1);
        myFriends.find("125451", records);
        assert(records.size() == 0);
        vector<vector<string>> anotherVector;
        assert(myFriends.select("name = Natale", anotherVector) == -1);
        assert(myFriends.select("Birthday on Christmas", anotherVector) == -1);
        assert(myFriends.select("Phone GT ''", anotherVector) == -1);
        assert(myFriends.select("Birthday eq 0423", anotherVector) == 0);
        assert(anotherVector.size() == 1);
        assert(myFriends.select("FavColor > apricot", anotherVector) == 0);
        assert(anotherVector.size() == 4);
        assert(myFriends.insert("Patty\t\t 0734 1455345 orange"));
        myFriends.find("Patty", records);
        assert(records.size() == 2);
        assert(!myFriends.insert("Patrick 0222 124125125"));
        assert(myFriends.insert("Brad 0812 1423521 brown"));
        assert(myFriends.insert("Chad 0118 1253463 red"));
        assert(myFriends.insert("'Maddy Tom' 1124 12354632 indigo"));
        myFriends.find("Maddy", records);
        assert(records.size() == 0);
        myFriends.find("Maddy Tom", records);
        assert(records.size() == 1);
        assert(myFriends.select("Phone gT 1", anotherVector) == 0);
        assert(anotherVector.size() == 8);
        assert(myFriends.insert("Frank December07 12435233 charcoal"));
        assert(myFriends.insert("Phil 12December 12343523 yellow"));
        assert(myFriends.select("Birthday LT 12512356", anotherVector) == 2);
        assert(anotherVector.size() == 8);
}

void select_bonus(){
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
        
        vector<vector<string>> vtest;
        assert(t.select("customer = Patel", vtest) == 0);
        assert(vtest.size() == 2);
        assert(t.select("( customer = Patel ')'", vtest) == 0);
        assert(vtest.size() == 2);
        assert(t.select("( location == Westwood | location == Hollywood ) '&' price LT 100", vtest) == 0);
        assert(vtest.size() == 2);
        assert(t.select("price GE 20  &  location != Westwood", vtest) == 0);
        assert(vtest.size() == 2);
        assert(t.select("customer > (  &  location != |", vtest) == 0);
        
        assert(t.select("(customer = Patel)", vtest) == -1);
        assert(vtest.size() == 0);
        assert(t.select("product = 12345 location != Westwood", vtest) == -1);
        assert(vtest.size() == 0);
        assert(t.select("location = ( Westwood | Hollywood )", vtest) == -1);
        assert(vtest.size() == 0);
        
        assert(t.select("", vtest) == 0);
        assert(vtest.size() == 0);
        
        assert(t.select("location GE 100", vtest) == 4);
        assert(vtest.size() == 0);

}

void ivan(){
    vector<string> usccols = { "last name", "first name", "frat", "UID", "GPA" };
        Table usc("frat", usccols);
        vector<vector<string>> findchecker;
        assert(usc.good());
        assert(usc.insert("spoiled kid akpsi 6055050 2.1"));
        usc.find("akpsi", findchecker);
        assert(findchecker.size() == 1);
        usc.find("dsp", findchecker);
        assert(findchecker.size() == 0);
        usc.find("6055050", findchecker);
        assert(findchecker.size() == 0);
        usc.find("akpsii", findchecker);
        assert(findchecker.size() == 0);
        usc.find("", findchecker);
        assert(findchecker.size() == 0);
        assert(usc.insert("spoiled kid akpsi 6055050 2.1"));
        usc.find("akpsi", findchecker);
        assert(findchecker.size() == 2);
        assert(usc.insert("spoiled jock akpsi 6055050 2.1"));
        usc.find("akpsi", findchecker);
        assert(findchecker.size() == 3);
        assert(usc.insert("refined man dsp 6055050 4.0"));
        usc.find("dsp", findchecker);
        assert(findchecker.size() == 1);
        assert(usc.select("frat GE 4.0", findchecker) == 4);
        assert(findchecker.size() == 0);
        assert(usc.select("frat is 4.0", findchecker) == -1);
        assert(findchecker.size() == 0);
        assert(usc.select("frat GED 4.0", findchecker) == -1);
        assert(findchecker.size() == 0);
        assert(usc.select("frat 4.0", findchecker) == -1);
        assert(findchecker.size() == 0);
        assert(usc.select("frat GE 4.0 rawr", findchecker) == -1);
        assert(findchecker.size() == 0);
        assert(usc.select("GPA GE 4.0rawr", findchecker) == -1);
        assert(findchecker.size() == 0);
        assert(usc.select("", findchecker) == -1);
        assert(findchecker.size() == 0);
        assert(usc.select("yooooooooooooo", findchecker) == -1);
        assert(findchecker.size() == 0);
        assert(usc.select("parties GE 40", findchecker) == -1);
        assert(findchecker.size() == 0);
        assert(usc.select("frat > b", findchecker) == 0);
        assert(findchecker.size() == 1);
        assert(usc.select("frat > a", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("frat > akpsi", findchecker) == 0);
        assert(findchecker.size() == 1);
        assert(usc.select("frat != akps", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("frat != akpsi", findchecker) == 0);
        assert(findchecker.size() == 1);
        assert(usc.select("frat == akpsi", findchecker) == 0);
        assert(findchecker.size() == 3);
        assert(usc.select("frat = akpsi", findchecker) == 0);
        assert(findchecker.size() == 3);
        assert(usc.select("frat >= akpsi", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("frat <= akpsi", findchecker) == 0);
        assert(findchecker.size() == 3);
        assert(usc.select("frat < akpsi", findchecker) == 0);
        assert(findchecker.size() == 0);
        assert(usc.select("frat <= dsp", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("frat < dsp", findchecker) == 0);
        assert(findchecker.size() == 3);
        assert(usc.select("frat < stephencurry", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("frat <= stephencurry", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("GPA lT 4.0", findchecker) == 0);
        assert(findchecker.size() == 3);
        assert(usc.select("GPA LT 2.1", findchecker) == 0);
        assert(findchecker.size() == 0);
        assert(usc.select("GPA Lt 2.2", findchecker) == 0);
        assert(findchecker.size() == 3);
        assert(usc.select("GPA lt 2.5", findchecker) == 0);
        assert(findchecker.size() == 3);
        assert(usc.select("GPA Le 4.0", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("GPA le 4.0", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("GPA LE 4.0", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("GPA lE 4.0", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("GPA GT 0", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("GPA gt 2.0", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("GPA gT 2.1", findchecker) == 0);
        assert(findchecker.size() == 1);
        assert(usc.select("GPA Gt 4.0", findchecker) == 0);
        assert(findchecker.size() == 0);
        assert(usc.select("GPA GE 2.1", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("GPA ge 4.0", findchecker) == 0);
        assert(findchecker.size() == 1);
        assert(usc.select("GPA Ge 4.0", findchecker) == 0);
        assert(findchecker.size() == 1);
        assert(usc.select("GPA gE 2.1", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("GPA NE 2.1", findchecker) == 0);
        assert(findchecker.size() == 1);
        assert(usc.select("GPA ne 4.0", findchecker) == 0);
        assert(findchecker.size() == 3);
        assert(usc.select("GPA Ne 2.0", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("GPA nE 4.7", findchecker) == 0);
        assert(findchecker.size() == 4);
        assert(usc.select("GPA EQ 4.7", findchecker) == 0);
        assert(findchecker.size() == 0);
        assert(usc.select("GPA eq 4.0", findchecker) == 0);
        assert(findchecker.size() == 1);
        assert(usc.select("GPA Eq 2.1", findchecker) == 0);
        assert(findchecker.size() == 3);
        assert(usc.select("GPA eQ 2.1", findchecker) == 0);
        assert(findchecker.size() == 3);
        assert(usc.insert("Ivan Yau dsp 6055050 ASS"));
        assert(usc.select("GPA eQ 2.1", findchecker) == 1);
        assert(findchecker.size() == 3);
        assert(usc.insert("Ivan Yau dsp 6055050 4.YEET"));
        assert(usc.select("GPA eQ 2.1", findchecker) == 2);
        assert(findchecker.size() == 3);
        assert(usc.insert("Ivan Yau dsp 6055050 4.08347YEET"));
        assert(usc.select("GPA eQ 2.1", findchecker) == 3);
        assert(findchecker.size() == 3);
     
        vector<string> emptycols = {""};
        Table empty("", emptycols);
        assert(!empty.good());
     
        vector<string> emptyitem = { "last name", "first name", "", "UID", "GPA" };
        Table emptycol("first name", emptyitem);
        assert(!emptycol.good());
     
        vector<string> dupe = { "last name", "last name", "frat", "UID", "GPA" };
        Table dupl("last name", dupe);
        assert(!dupl.good());
     
        vector<string> nokey = { "frat", "UID", "GPA" };
        Table keygone("last name", nokey);
        assert(!keygone.good());

}

void another(){
    vector<string> cols = {"customer", "product", "price", "location"};
     
    Table emptyRecord("customer", cols);
    assert(emptyRecord.good());
    vector<vector<string>> eR;
    assert(emptyRecord.insert("' '  ' '  ' '  ' '"));  //has four fields, so valid
    emptyRecord.find(" ", eR);
    assert(eR.size() == 1);
    vector<string> theEmptyFields = { " ", " ", " ", " " };
    assert(eR[0] == theEmptyFields);
    emptyRecord.find("", eR);  //whether there's a space or not in between the quotes matters
    assert(eR.size() == 0);
    vector<vector<string>> testSelect;
    assert(emptyRecord.select("price < 5.00", testSelect) == 0);
     
    Table t("customer", cols);
    assert(t.good());
    Table t1("name", cols); //key isn't in cols
    assert(!t1.good());
    assert(t.insert("Patel 12345 42.54 Westwood"));
    assert(t.insert("O'Reilly 34567     4.99 Westwood   "));
    assert(t.insert("   Hoang  12345 30.46 'Santa Monica' "));
    assert(t.insert("Patel\t67890\t142.75  \t \t\t  \tHollywood"));
    assert(!t.insert("Figueroa 54321 59.95")); //not the right amount of fields
    assert(!t.insert("")); //not the right amount of fields
    assert(t.insert("'Baba O''Riley' 58201\n12.34 'Cool Hip City'")); //playing around with single quote
    assert(t.insert("       Alexa       '03510'77.77       'Outer Space City'       "));
    assert(t.insert("'Maca-roni'\r72945\t1.25\n'San Jose'"));
    assert(!t.insert("'Maca-roni'\r72945\t1.25\nSan Jose"));
    std::vector<std::vector<std::string>> v;
    t.find("Alexa", v);
    assert(v.size() == 1);
    std::vector<std::string> hasAlexa = { "Alexa", "03510", "77.77", "Outer Space City" };
    assert(v[0] == hasAlexa);
    assert(t.insert("Alexa '03510'77.77 'Outer Space City'"));
    t.find("Alexa", v);
    assert(v.size() == 2);
    assert(v[0] == hasAlexa && v[1] == hasAlexa);
     
     
//    std::vector<std::vector<std::string>> testSelect;
    assert(t.select("price < 5.00", testSelect) == 0);  // '<' will do string comparisons not numeric comparisons
    assert(testSelect.size() == 6);
    vector<vector<string>> lessThanFive = {
        {"O'Reilly", "34567", "4.99", "Westwood"},
        {"Maca-roni", "72945", "1.25", "San Jose"},
        {"Patel", "12345", "42.54", "Westwood"},
        {"Hoang", "12345", "30.46", "Santa Monica"},
        {"Patel", "67890", "142.75", "Hollywood"},
        {"Baba O'Riley", "58201", "12.34", "Cool Hip City"}
    };
    assert(testSelect[0] == lessThanFive[0] ||testSelect[0] == lessThanFive[1]  || testSelect[0] == lessThanFive[2] ||        testSelect[0] == lessThanFive[3] || testSelect[0] == lessThanFive[4]  || testSelect[0] == lessThanFive[5]);
    assert(t.select("price LT 5.00", testSelect) == 0 && t.select("price lt 5.00", testSelect) == 0);
    assert(testSelect.size() == 2);
    lessThanFive.clear();
    lessThanFive = {
        {"O'Reilly", "34567", "4.99", "Westwood"},
        {"Maca-roni", "72945", "1.25", "San Jose"}
    };
    assert((testSelect[0] == lessThanFive[0] && testSelect[1] == lessThanFive[1]) || testSelect[0] == lessThanFive[1] &&  testSelect[1] == lessThanFive[0]);
     
    assert(t.select("Hollywood = location", testSelect) == -1);
    assert(t.select("location = 'Outer Space City'", testSelect) == 0);
    assert(testSelect.size() == 2);
    vector<vector<string>> selectLocation
    {
        {"Alexa", "03510", "77.77", "Outer Space City"}
    };
    assert(testSelect[0] == selectLocation[0]);
    assert(t.select("location == Cool Hip City", testSelect) == -1);
    assert(t.select("location == 'Cool Hip City'", testSelect) == 0);
    assert(testSelect.size() == 1);
    vector<vector<string>> hippityHop{
        {"Baba O'Riley", "58201", "12.34", "Cool Hip City"}
    };
    assert(testSelect[0] == hippityHop[0]);

}
int main(){
    test1();
    test2();
    test3();
    ctest1();
    ctest2();
    ivan();
    another();
//    select_bonus();
    cerr<<"ALL DONE"<<endl;
    return 0;
}
