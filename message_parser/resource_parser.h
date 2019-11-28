#ifndef _resource_parser
#define _resource_parser

#include <iostream>
#include "../dci/types.h"

using namespace bbque::stat;

class resource_parser
{
private:
    res_bitset_t bbq_id;
    std::string model;
    uint8_t occupancy;
    uint8_t load;
    uint32_t power; 
    uint32_t temp;
    uint32_t fans;

public:
    resource_parser(resource_status_t &message);
    static std::string id_parser(res_bitset_t id);
    void print_string();
    std::string to_json_string();
};



#endif