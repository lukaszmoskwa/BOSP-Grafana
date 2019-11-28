#ifndef _app_parser
#define _app_parser

#include <iostream>
#include "../dci/types.h"
#include "resource_parser.h"

using namespace bbque::stat;

class app_parser
{
private:
    uint64_t id;
    std::string name;
    std::list<task_status_t> tasks;
    std::list<res_bitset_t> mapping;
    std::string tasks_to_json();
    std::string mapping_to_json();

public:
    app_parser(app_status_t &as) : name(as.name), id(as.id), tasks(as.tasks), mapping(as.mapping){};
    void print_string();
    std::string to_json_string();
};

#endif