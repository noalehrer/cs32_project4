//
//  Table.cpp
//  Project_4
//
//  Created by Noa Lehrer on 5/28/21.
//

#include "Table.h"

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

//I CAN USE THE BUILT IN HASH FUNCTION BY INCLUDING FUNCTIONAL...
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
    vector<string> record_string_vector;
    
    s = "";
    parser = recordString;
    while(parser.getNextField(s)==true){
        record_string_vector.push_back(s);
    }
    table[customerHash(key_value)].push_back(record_string_vector);
    
    //maybe instead of inserting the whole record string i could have list store a vector to represent the items in the record string...
    return true;
}

void Table::find(std::string key, std::vector<std::vector<std::string>>& records) const{
    records.clear();
    int index = customerHash(key);
    //go to bucket at index, iterate thru list
    for(list<vector<string>>::const_iterator it = table[index].cbegin(); it!=table[index].cend(); it++){
        //if value in keyfield is equal to the key string
        string key_field_value = (*it)[key_field_index];
        if(key_field_value==key){
            vector<string> vector_record_string;
            for(vector<string>::const_iterator i = (*it).begin(); i!=(*it).end(); i++){
                vector_record_string.push_back(*i);
            }
            records.push_back(vector_record_string);
            //now vector_record_string contains each field in the record string
        }
    }
}

bool Table::isValidQuery(string column_name, string comparison_operator, string comparison_value, int &column_index) const{
    bool valid_column_name = false;
    for(int i = 0; i<m_columnsVector.size(); i++){
        if(m_columnsVector[i]==column_name){
            valid_column_name = true;
            column_index = i;
        }
    }
    //column not found
    if(valid_column_name==false){
        return false;
    }
    //comparison value valid
    return true;
}

void Table::searchTableNumber(int column_name_index, string comparison_operator, int numerical_comparison_value, std::vector<std::vector<std::string>>& records, int &improper_record_count)const{
    records.clear();
    switch(comparison_operator[0]){
        case '<':
            //less than or equal
            if(comparison_operator.size()==2){
                //this probs can be optimized later... considering that hash table is organized by key field
                //iterate through the whole hash table
                for(int i = 0; i<table.size(); i++){
                    //look at each item in each list in each bucket
                    for(list<vector<string>>::const_iterator it = table[i].cbegin(); it!=table[i].cend(); it++){
                        //if comparison is true
                        double d;
                        if(stringToDouble((*it)[column_name_index], d) && d<=numerical_comparison_value){
                            //push back the whole vector into records
                            records.push_back(*it);
                        }
                        else if(stringToDouble((*it)[column_name_index], d)==false){
                            improper_record_count+=1;
                        }
                    }
                }
            }
            //less than
            else{
                for(int i = 0; i<table.size(); i++){
                    for(list<vector<string>>::const_iterator it = table[i].cbegin(); it!=table[i].cend(); it++){
                        double d;
                        if(stringToDouble((*it)[column_name_index], d) && d<numerical_comparison_value){
                            //push back the whole vector into records
                            records.push_back(*it);
                        }
                        else if(stringToDouble((*it)[column_name_index], d)==false){
                            improper_record_count+=1;
                        }
                    }
                }
            }
            break;
        case '>':
            //greater than or equal
            if(comparison_operator.size()==2){
                for(int i = 0; i<table.size(); i++){
                    for(list<vector<string>>::const_iterator it = table[i].cbegin(); it!=table[i].cend(); it++){
                        double d;
                        if(stringToDouble((*it)[column_name_index], d) && d>=numerical_comparison_value){
                            //push back the whole vector into records
                            records.push_back(*it);
                        }
                        else if(stringToDouble((*it)[column_name_index], d)==false){
                            improper_record_count+=1;
                        }
                    }
                }
            }
            //greater than
            else{
                for(int i = 0; i<table.size(); i++){
                    for(list<vector<string>>::const_iterator it = table[i].cbegin(); it!=table[i].cend(); it++){
                        double d;
                        if(stringToDouble((*it)[column_name_index], d) && d>numerical_comparison_value){
                            //push back the whole vector into records
                            records.push_back(*it);
                        }
                        else if(stringToDouble((*it)[column_name_index], d)==false){
                            improper_record_count+=1;
                        }
                    }
                }
            }
            break;
        //equal to
        case '=':
            for(int i = 0; i<table.size(); i++){
                for(list<vector<string>>::const_iterator it = table[i].cbegin(); it!=table[i].cend(); it++){
                    double d;
                    if(stringToDouble((*it)[column_name_index], d) && d==numerical_comparison_value){
                        //push back the whole vector into records
                        records.push_back(*it);
                    }
                    else if(stringToDouble((*it)[column_name_index], d)==false){
                        improper_record_count+=1;
                    }
                }
            }
            break;
        //not equal to
        case '!':
            for(int i = 0; i<table.size(); i++){
                for(list<vector<string>>::const_iterator it = table[i].cbegin(); it!=table[i].cend(); it++){
                    double d;
                    if(stringToDouble((*it)[column_name_index], d) && d==numerical_comparison_value){
                        //push back the whole vector into records
                        records.push_back(*it);
                    }
                    else if(stringToDouble((*it)[column_name_index], d)==false){
                        improper_record_count+=1;
                    }
                }
            }
            break;
        default:
            break;
    }
}

void Table::searchTableString(int column_name_index, string comparison_operator, string comparison_value, std::vector<std::vector<std::string>>& records)const{
    //we already know that the comparison operator is valid
    //Note: do not pass invalid comparison operator to this function
    
    //record being passed should already be clear, this is just to confirm
    records.clear();
    
    switch(comparison_operator[0]){
        case '<':
            //less than or equal
            if(comparison_operator.size()==2){
                //this probs can be optimized later... considering that hash table is organized by key field
                //iterate through the whole hash table
                for(int i = 0; i<table.size(); i++){
                    //look at each item in each list in each bucket
                    for(list<vector<string>>::const_iterator it = table[i].cbegin(); it!=table[i].cend(); it++){
                        //if comparison is true
                        if((*it)[column_name_index]<=comparison_value){
                            //push back the whole vector into records
                            records.push_back(*it);
                        }
                    }
                    
                }
            }
            //less than
            else{
                for(int i = 0; i<table.size(); i++){
                    for(list<vector<string>>::const_iterator it = table[i].cbegin(); it!=table[i].cend(); it++){
                        if((*it)[column_name_index]<comparison_value){
                            records.push_back(*it);
                        }
                    }
                }
            }
            break;
        case '>':
            //greater than or equal
            if(comparison_operator.size()==2){
                for(int i = 0; i<table.size(); i++){
                    for(list<vector<string>>::const_iterator it = table[i].cbegin(); it!=table[i].cend(); it++){
                        if((*it)[column_name_index]>=comparison_value){
                            records.push_back(*it);
                        }
                    }
                }
            }
            //greater than
            else{
                for(int i = 0; i<table.size(); i++){
                    for(list<vector<string>>::const_iterator it = table[i].cbegin(); it!=table[i].cend(); it++){
                        if((*it)[column_name_index]>comparison_value){
                            records.push_back(*it);
                        }
                    }
                }
            }
            break;
        //equal to
        case '=':
            for(int i = 0; i<table.size(); i++){
                for(list<vector<string>>::const_iterator it = table[i].cbegin(); it!=table[i].cend(); it++){
                    if((*it)[column_name_index]==comparison_value){
                        records.push_back(*it);
                    }
                }
            }
            break;
        //not equal to
        case '!':
            for(int i = 0; i<table.size(); i++){
                for(list<vector<string>>::const_iterator it = table[i].cbegin(); it!=table[i].cend(); it++){
                    if((*it)[column_name_index]!=comparison_value){
                        records.push_back(*it);
                    }
                }
            }
            break;
        default:
            break;
    }
}


int Table::select(std::string query, std::vector<std::vector<std::string>>& records) const{
    StringParser query_parser = StringParser(query);
    string column_name, comparison_operator, comparison_value;
    query_parser.getNextField(column_name);
    query_parser.getNextField(comparison_operator);
    query_parser.getNextField(comparison_value);
    //too many arguments in the query, if i do the bonus, i'll have to change this
    string s;
    if(query_parser.getNextField(s)==true){
        return -1;
    }
    
    int column_index = -1;
    
    int improper_record_count = 0;
    
    double numerical_comparison_value = -1;
    
    records.clear();
    //invalid query
    
    if(isValidQuery(column_name, comparison_operator, comparison_value, column_index)==false){
        return -1;
    }

    //else valid query
    switch(char ch = comparison_operator[0]){
        //numerical comparisons
        case 'l':
        case 'L':
            if(comparison_operator.size()==2){
                if(stringToDouble(comparison_value, numerical_comparison_value)==false){
                    return -1;
                }
                //less than
                if(comparison_operator[1]=='T'||comparison_operator[1]=='t'){
                    searchTableNumber(column_index,"<", numerical_comparison_value, records, improper_record_count);
                    break;
                }
                //less than or equal to
                else if(comparison_operator[1]=='E'||comparison_operator[1]=='e'){
                    searchTableNumber(column_index,"<=", numerical_comparison_value, records,improper_record_count);
                    break;
                }
                else{
                    return -1;
                }
            }
            else{
                return -1;
            }
        case 'g':
        case 'G':
            if(comparison_operator.size()==2){
                if(stringToDouble(comparison_value, numerical_comparison_value)==false){
                    return -1;
                }
                //greater than
                if(comparison_operator[1]=='T'||comparison_operator[1]=='t'){
                    searchTableNumber(column_index,">", numerical_comparison_value, records,improper_record_count);
                    break;
                }
                //greater than or equal to
                else if(comparison_operator[1]=='E'||comparison_operator[1]=='e'){
                    searchTableNumber(column_index,">=", numerical_comparison_value, records,improper_record_count);
                    break;
                }
                else{
                    return -1;
                }
            }
            else{
                return -1;
            }
        //not equal to
        case 'n':
        case 'N':
            if(comparison_operator.size()==2){
                if(stringToDouble(comparison_value, numerical_comparison_value)==false){
                    return -1;
                }
                if(comparison_operator[1]=='E'||comparison_operator[1]=='e'){
                    searchTableNumber(column_index,"!=", numerical_comparison_value, records,improper_record_count);
                    break;
                }
                else{
                    return -1;
                }
            }
            else{
                return -1;
            }
        //equal to
        case 'e':
        case 'E':
            if(comparison_operator.size()==2){
                if(stringToDouble(comparison_value, numerical_comparison_value)==false){
                    return -1;
                }
                if(comparison_operator[1]=='Q'||comparison_operator[1]=='q'){
                    searchTableNumber(column_index,"==", numerical_comparison_value, records,improper_record_count);
                    break;
                }
                else{
                    return -1;
                }
            }
            else{
                return -1;
            }
            
        //string comparisons
        case '<':
            //less than or equal
            if(comparison_operator.size()==2 && comparison_operator[1]=='='){
                searchTableString(column_index,"<=", comparison_value, records);
                break;
            }
            //less than
            else if(comparison_operator.size()==1){
                searchTableString(column_index,"<", comparison_value, records);
                break;
            }
            else{
                return -1;
            }
        case '>':
            //greater than or equal
            if(comparison_operator.size()==2 && comparison_operator[1]=='='){
                searchTableString(column_index,">=", comparison_value, records);
                break;
            }
            //greater than
            else if(comparison_operator.size()==1){
                searchTableString(column_index,">", comparison_value, records);
                break;
            }
            else{
                return -1;
            }
        case '!':
            if(comparison_operator.size()==2){
                //not equal to
                if(comparison_operator[1]=='='){
                    searchTableString(column_index,"!=", comparison_value, records);
                    break;
                }
                else{
                    return -1;
                }
            }
            else{
                return -1;
            }
        case '=':
            //equal to
            if((comparison_operator.size()==2 && comparison_operator[1]=='=')||comparison_operator.size()==1){
                searchTableString(column_index,"==", comparison_value, records);
                break;
            }
            else{
                return -1;
            }
        default:
            return -1;
    }

    return improper_record_count;
}

bool Table::stringToDouble(string s, double& d) const{
    char* end;
    d = std::strtof(s.c_str(), &end);
    return end == s.c_str() + s.size()  &&  !s.empty();
}
