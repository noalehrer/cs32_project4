//
//  Table.hpp
//  Project_4
//
//  Created by Noa Lehrer on 5/28/21.
//

#ifndef Table_hpp
#define Table_hpp

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cstdlib>
using namespace std;

class Table
{
  public:
    Table(std::string keyColumn, const std::vector<std::string>& columns);
    ~Table();
    bool good() const;
    bool insert(const std::string& recordString);
    void find(std::string key, std::vector<std::vector<std::string>>& records) const;
    int select(std::string query, std::vector<std::vector<std::string>>& records) const;
    Table(const Table&) = delete;
    Table& operator=(const Table&) = delete;
    
  private:
    //MARK: - database table
    string m_keyColumn; //this is just so we know the key column name, this isn't an actual record string
    vector<string> m_columnsVector; //this is just so we know all the column names, this doesn't contain actual record strings
    int key_field_index = -1; //this tells us the index of the key field item in m_columnsVector
    vector<list<vector<string>>> table; //hash table, each item has a linked list, each item in the linked list contains a vector of strings which was created from the record string
    
    //MARK: - helper functions
    int hashFunction(string key) const;
    
    bool isValidQuery(string column_name, string comparison_operator, string comparison_value, int &column_index) const;
    
    void searchTableString(int column_name_index, string comparison_operator, string comparison_value, std::vector<std::vector<std::string>>& records)const;
    void searchTableNumber(int column_name_index, string comparison_operator, int numerical_comparison_value, std::vector<std::vector<std::string>>& records, int &improper_record_count)const;
    bool stringToDouble(string s, double &d) const;
    bool inValidColumnName(string column_name)const;
};
#endif /* Table_hpp */
