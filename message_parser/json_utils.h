#ifndef _json_utils
#define _json_utils

#include <iostream>
#include <list>
#include <map>

class json_utils
{

public:
    static std::string array_to_string(std::list<std::string> &arr)
    {
        std::list<std::string>::iterator it;
        std::string finalString = "[";
        for (it = arr.begin(); it != arr.end(); it++)
        {
            finalString += *it + ",";
        }
        finalString += "]";
        return finalString;
    }
    static std::string obj_to_string(std::map<std::string, std::string> map) {

    } 
};

#endif