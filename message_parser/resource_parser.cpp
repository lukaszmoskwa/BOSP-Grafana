#include "resource_parser.h"
#include "json_utils.h"
#include "../dci/types.h"
#include "../dci/data_client.h"

using namespace bbque::stat;
using namespace bbque;

resource_parser::resource_parser(resource_status_t &message)
{
    bbq_id = message.id;
    std::string path = DataClient::GetResourcePathString(bbq_id);

    this->model = path;
    this->occupancy = (message.occupancy > 100) ? 0 : message.occupancy;
    this->load = (message.load > 100) ? 0 : message.load;
    this->power = (message.power > 100) ? 0 : message.power;
    this->temp = (message.temp > 100) ? 0 : message.temp;
    this->fans = (message.fans > 100) ? 0 : message.fans;
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

    std::map<std::string, std::string> map = {
        {"sys", std::to_string(sys)},
        {"bbq_group", std::to_string(group)},
        {"unit_type", std::to_string(unit_type)},
        {"unit_id", std::to_string(unit_id)},
        {"pe_type", std::to_string(pe_type)},
        {"pe_id", std::to_string(pe_id)}};
    return json_utils::obj_to_string(map);
}

void resource_parser::print_string()
{
    printf("model: %s occ: %hhu load: %hhu power: %u temp: %u fans: %u\n", model.c_str(), occupancy, load, power, temp, fans);
}

std::string resource_parser::to_json_string()
{
    std::map<std::string, std::string> map = {
        {"bbq_id", id_parser(bbq_id)},
        {"model", json_utils::quote(model)},
        {"occupancy", std::to_string(occupancy)},
        {"fans", std::to_string(fans)},
        {"power", std::to_string(power)},
        {"temp", std::to_string(temp)},
        {"bbq_load", std::to_string(load)}};
    return json_utils::obj_to_string(map);
}