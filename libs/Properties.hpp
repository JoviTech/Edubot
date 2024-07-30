#include <map>
#include <string>
#include <fstream>
#include <iostream>

class Properties {

private:

    std::string propertiesFile;
    std::map<std::string, std::string> propertiesMap;
    

public:

    Properties (std::string propertiesFile)  {
        this->propertiesFile = propertiesFile;
    }

    bool readProperties () {

        std::ifstream is(this->propertiesFile.c_str());

        if (!is.is_open()) return false;

        while (!is.eof()) {

            std::string line;
            std::string equalChar("=");

            getline(is, line);

            //std::cout << "Line: " << line << std::endl ;

            size_t npos = line.find(equalChar);

            //std::cout << "npos: " << npos << std::endl ;

            if (std::string::npos == npos) continue; // no '=', invalid line;

            std::string key = line.substr(0, npos);

            std::string value = line.substr(npos + 1, line.length() - npos + 1);

            //std::cout << key << ':' << value << std::endl ;

            this->propertiesMap.insert(std::map<std::string, std::string>::value_type(key,value));
        }

        return true;
    }

    void printPropertiesMap(){
        std::map<std::string, std::string>::iterator it;

        for ( it = this->propertiesMap.begin(); it != this->propertiesMap.end(); it++ ){
            std::cout << it->first  // string (key)
                      << ':'
                      << it->second   // string's value 
                      << std::endl ;
        }
    }

    std::string getPropertieValue(std::string key){

        std::map<std::string, std::string>::iterator i;

        i = this->propertiesMap.find(key);

        if (i != this->propertiesMap.end()) {
            return i->second;
        }

        return "";
    }

};
