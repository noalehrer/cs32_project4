//
//  Table.cpp
//  Project_4
//
//  Created by Noa Lehrer on 5/28/21.
//

#include "Table.h"

Table::Table(std::string keyColumn, const std::vector<std::string>& columns){
    //whatever the first parameter is, that should be the key and the value is the record string
    table.resize(997); //big, prime number
    m_keyColumn = keyColumn;
    m_columnsVector.resize(columns.size());
    for(int i = 0; i<m_columnsVector.size(); i++){
        m_columnsVector[i] = columns[i];
        if(m_columnsVector[i]==m_keyColumn){
            key_field_index = i;
        }
    }

}

Table::~Table(){
    
}

bool Table::good() const{
    //ways constructing a table could go wrong
    //columns vector empty
    if(m_columnsVector.empty()){
        return false;
    }
    //columns vector has duplicate strings
    for(int j = 0; j<m_columnsVector.size(); j++){
        for(int k = 0; k<m_columnsVector.size(); k++){
            if(j!=k){
                if(m_columnsVector[j]==m_columnsVector[k]){
                    return false;
                }
            }
        }
    }
    
    //key column not included of list of column names
    int i = 0;
    for(; i<m_columnsVector.size(); i++){
        if(m_columnsVector[i]==m_keyColumn){
            //key column is included in columns vector
            break;
        }
    }
    if(i==m_columnsVector.size()){
        return false;
    }
    return true;
}


int Table::customerHash(string customer) const{
    unsigned int hashValue = 2166136261U;
    for(int i = 0; i<customer.size(); i++){
        hashValue+=customer[i];
        hashValue*=16777619;
    }
    return hashValue%997;
}

bool StringParser::getNextField(std::string& fieldText)
{
    m_start = m_text.find_first_not_of(" \t\r\n", m_start);
    if (m_start == std::string::npos)
    {
        m_start = m_text.size();
        fieldText = "";
        return false;
    }
    if (m_text[m_start] != '\'')
    {
        size_t end = m_text.find_first_of(" \t\r\n", m_start+1);
        fieldText = m_text.substr(m_start, end-m_start);
        m_start = end;
        return true;
    }
    fieldText = "";
    for (;;)
    {
        m_start++;
        size_t end = m_text.find('\'', m_start);
        fieldText += m_text.substr(m_start, end-m_start);
        m_start = (end != std::string::npos ? end+1 : m_text.size());
        if (m_start == m_text.size()  ||  m_text[m_start] != '\'')
            break;
        fieldText += '\'';
    }
    return true;
}

bool Table::insert(const std::string& recordString){
    //if the string is valid (correct number of fields)
    //do i also need to check if the fields are the right data type?
    int field_count = 0;
    StringParser parser(recordString);
    string s;
    string key_value = "";
    while(parser.getNextField(s)==true){
        field_count++;
        if(field_count==key_field_index+1){
            key_value = s;
        }
    }
    
    //if there aren't the right amount of fields
    if(field_count!=m_columnsVector.size()){
        return false;
    }
    
    //we want to insert into table based on the value of the keycolumn of the string
    table[customerHash(key_value)].push_back(recordString);
    
    //maybe instead of inserting the whole record string i could have list store a vector to represent the items in the record string...
    return true;
}

void Table::find(std::string key, std::vector<std::vector<std::string>>& records) const{
    records.clear();
    int index = customerHash(key);
    //go to bucket at index, iterate thru list
    for(list<string>::const_iterator it = table[index].cbegin(); it!=table[index].cend(); it++){
        //if value in keyfield is equal to the key string
        string key_field_value = "";
        StringParser record_string = StringParser(*it);
        for(int i = 0; i<key_field_index+1; i++){
            record_string.getNextField(key_field_value);
        }
        if(key_field_value==key){
            record_string = StringParser(*it); //need to reset record string because we have been modifying it in the for loop
            vector<string> vector_record_string;
            string s = "";
            while(record_string.getNextField(s)==true){
                vector_record_string.push_back(s);
            }
            records.push_back(vector_record_string);
            //now vector_record_string contains each field in the record string
        }
    }
}

int Table::select(std::string query, std::vector<std::vector<std::string>>& records) const{
    //can we assume valid queries?
    StringParser query_parser = StringParser(query);
    string column_name, comparison_operator, comparison_value;
    query_parser.getNextField(column_name);
    query_parser.getNextField(comparison_operator);
    query_parser.getNextField(comparison_value);

    return 0;
}

