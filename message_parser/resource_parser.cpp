#include "resource_parser.h"
#include "../dci/types.h"
#include "../dci/data_client.h"

using namespace bbque::stat;
using namespace bbque;

resource_parser::resource_parser(resource_status_t &message)
{
    bbq_id = message.id;
    std::string path = DataClient::GetResourcePathString(bbq_id);

    this->model = path;
    this->occupancy = 0;
    this->load = 99;
    this->power = 77;
    this->temp = 12;
    this->fans = 44;

    //this->model = message.model;
    this->occupancy = message.occupancy;
    this->load = message.load;
    this->power = message.power;
    //this->temp = message.temp;
    this->fans = message.fans;
}

std::string resource_parser::id_parser(res_bitset_t message_id)
{
    unsigned int pe_id = message_id & 0x7FFFFFFF;
    message_id >>= 31;
    unsigned int pe_type = message_id & 0x1;
    message_id >>= 1;
    unsigned int unit_id = message_id & 0xFFFF;
    message_id >>= 16;
    unsigned int unit_type = message_id & 0xF;
    message_id >>= 4;
    unsigned int group = message_id & 0x7;
    message_id >>= 3;
    unsigned int sys = message_id;

    return "{\"sys\":" + std::to_string(sys) +
           ",\"bbq_group\":" + std::to_string(group) +
           ",\"unit_type\":" + std::to_string(unit_type) +
           ",\"unit_id\":" + std::to_string(unit_id) +
           ",\"pe_type\":" + std::to_string(pe_type) +
           ",\"pe_id\":" + std::to_string(pe_id) +
           "}";
}

void resource_parser::print_string()
{
    printf("model: %s occ: %hhu load: %hhu power: %u temp: %u fans: %u\n", model.c_str(), occupancy, load, power, temp, fans);
}

std::string resource_parser::to_json_string()
{
    return "{\"bbq_id\": " + id_parser(bbq_id) +
           " ,\"model\":\"" + model +
           "\",\"occupancy\":" + std::to_string(occupancy) +
           ",\"fans\":" + std::to_string(fans) +
           ",\"power\":" + std::to_string(power) +
           ",\"temp\":" + std::to_string(temp) +
           ",\"bbq_load\":" + std::to_string(load) + "}";
}