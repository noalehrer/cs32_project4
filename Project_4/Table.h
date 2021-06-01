//
//  Table.hpp
//  Project_4
//
//  Created by Noa Lehrer on 5/28/21.
//

#ifndef Table_hpp
#define Table_hpp

#include <string>
#include <vector>
#include <list>
using namespace std;

class Table
{
  public:
    Table(std::string keyColumn, const std::vector<std::string>& columns); //construct an empty table with vector items as column names
    ~Table();
    bool good() const; //return true if table was successfully constructed
    bool insert(const std::string& recordString);
    void find(std::string key, std::vector<std::vector<std::string>>& records) const;
    int select(std::string query, std::vector<std::vector<std::string>>& records) const;

      // We prevent a Table object from being copied or assigned by
      // making the copy constructor and assignment operator unavailable.
    Table(const Table&) = delete;
    Table& operator=(const Table&) = delete;
  private:
    //these are so we can assess the validity of the table structure, we don't keep the records in here tho
    string m_keyColumn; //this is just so we know the key column name, this isn't an actual record string
    vector<string> m_columnsVector; //this is just so we know all the column names, thi doesn't contain actual record strings
    
    int key_field_index; //this tells us the index of the item in the vector that is the key field
    
    //this is where we store the actual record strings
    //let's make it 997 buckets
    vector<list<vector<string>>> table;
    
    int customerHash(string customer) const;
};

class StringParser
{
  public:
    StringParser(std::string text = "")
    {
        setString(text);
    }

    void setString(std::string text)
    {
        m_text = text;
        m_start = 0;
    }

    bool getNextField(std::string& field);

  private:
    std::string m_text;
    size_t m_start;
};

#endif /* Table_hpp */
