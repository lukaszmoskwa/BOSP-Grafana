#include "dci/types.h"
#include "dci/data_client.h"
#include <functional>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include "message_parser/resource_parser.h"
#include "message_parser/app_parser.h"
#include "http/httpclient.h"

#define MESS_NUM_MAX 10;

using namespace bbque;

uint16_t num_messages;
uint8_t raw_print;
std::mutex mtx;
std::condition_variable cv;
std::list<status_message_t> messages_list;

void sendHTTPRequest(std::string endpoint, std::string content)
{
	try
	{
		std::string request_url = "http://localhost:3131/" + endpoint;
		http::Request request(request_url);
		//std::cout << content << std::endl;
		const http::Response post = request.send("POST", content, {"Content-Type: application/json"});
	}
	catch (const std::exception &e)
	{
		std::cerr << "Request failed, error: " << e.what() << '\n';
	}
}

void UpdateCallback(status_message_t message)
{
	//std::string model;
	std::uint32_t timestamp = message.ts;
	//std::uint32_t res_status_number = message.n_app_status_msgs;
	std::cout << timestamp << std::endl;
	//std::cout << res_status_number << std::endl;

	std::list<app_status_t>::iterator it;
	for (it = message.app_status_msgs.begin(); it != message.app_status_msgs.end(); ++it)
	{
		app_parser m_app(*it);
		std::cout << m_app.to_json_string() << std::endl;
	}

	std::cout << " model : " << message.res_status_msgs.front().model << " fans: " << message.res_status_msgs.front().fans << std::endl;
	std::list<resource_status_t>::iterator bf;
	for (bf = message.res_status_msgs.begin(); bf != message.res_status_msgs.end(); ++bf)
	{
		resource_parser rs_parser(*bf);
		sendHTTPRequest("resources", rs_parser.to_json_string());
		// Send http client message
	}

	// DataClient::GetResourcePathString(res_bitset_t res)
}

int main(int argc, char *argv[])
{
	char *server_IP;	  // IP address of server
	uint32_t server_port; // Server port
	uint32_t client_port; // Client port
	int struct_len;		  // Length of string to echo
	int resp_string_len;  // Length of received response

	// Arguments parsing

	// Data client instance creation
	server_IP = argv[1];		 // Server IP address
	server_port = atoi(argv[2]); // Server port
	client_port = atoi(argv[3]); // Client port
	status_filter_t filter =
		static_cast<status_filter_t>(atoi(argv[4])); // Filter bitset
	status_event_t event =
		static_cast<status_event_t>(atoi(argv[5])); // Event bitset
	uint16_t period = atoi(argv[6]);				// Period
	if (argv[7])
		num_messages = atoi(argv[7]);
	else
		num_messages = MESS_NUM_MAX;

	// Data client instance creation
	DataClient data_client(server_IP, server_port, client_port, UpdateCallback);
	if (!data_client.IsConnected())
		return -1;

	// Subscription
	int result = data_client.Subscribe(filter, event, period,
									   DataClient::subscription_mode_t::SUBSCRIBE);
	if (result != DataClient::ExitCode_t::OK)
		return -1;
	//else
	//PrintSubscription(client_port, filter, event, period,
	//                         DataClient::subscription_mode_t::SUBSCRIBE);

	// Waiting until the message limit is reached
	std::unique_lock<std::mutex> lk(mtx);
	cv.wait(lk);

	// Unsubscription
	result = data_client.Subscribe(filter, event, period,
								   DataClient::subscription_mode_t::UNSUBSCRIBE);
	if (result != DataClient::ExitCode_t::OK)
	{
		return -1;
	}

	return 0;
}