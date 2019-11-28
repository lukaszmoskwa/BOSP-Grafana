/*
 * Copyright (C) 2018  Politecnico di Milano
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BBQUE_DATA_CLIENT_H_
#define BBQUE_DATA_CLIENT_H_

#include "dci/types.h"

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <boost/asio.hpp>

namespace bbque {

using namespace bbque::stat;


/**
 * @class DataClient
 * @brief This class is the base class for third-party client to interface
 * with the BarbequeRTRM DataManager module.
 * It hides from the developers the connection and network management.
 * It provides a method to subscribe to specific event and information filter.
 * It allows the client to set a callback function to update the client
 * when new status informationare published by BarbequeRTRM instance.
 * This class should be instantiated by the third-party client.
 */
class DataClient {
public:

	/**
	 * @enum ExitCode_t
	 * @brief Class specific return codes
	 */
	enum ExitCode_t {
		OK = 0,           /// Successful call
		ERR_SERVER_UNREACHABLE,  /// Server unreachable
		ERR_MISSING_CONNECTION,
		ERR_UNKNOWN       /// Unspecified error code
	};

	/**
	 * @enum SubscriptionMode_t
	 */
	enum subscription_mode_t {
		SUBSCRIBE   = 0,
		UNSUBSCRIBE = 1
	};

	DataClient() = delete;

	/**
	 * @brief Constructor
	 * This construct the client object and connect to the server
	 * @param ip_addr BarbequeRTRM server address
	 * @param server_port Server port
	 * @param client_port Client port
	 * @param callback_fn Callback function for incoming data
	 */
	DataClient(
		std::string ip_addr,
		uint32_t server_port,
		uint32_t client_port,
		std::function<void(status_message_t)> callback_fn =
			[](status_message_t s){(void)s;});

	/**
	 * @brief Destructor
	 * This perform also disconnect the client from the BarbequeRTRM server
	 */
	virtual ~DataClient();

	/**
	 * @brief Check if the client has successfully connected to BarbequeRTRM server
	 * @return true for success, false otherwise
	 */
	bool IsConnected();

	/**
	 * @brief It performs the subscription for the Client to the BarbequeRTRM DataManager
	 * server instance basing on the given preferences.
	 * @param filter The subscription target type filter
	 * @param event The event filter
	 * @param period The rate for the update
	 * @param mode The subscription mode according to SubMode_t
	 * @return A DataClient exit code. 0 if OK, 1 if ERR_SERVER_COMM.
	 */
	ExitCode_t Subscribe(
			status_filter_t filter,
			status_event_t event,
			uint16_t period,
			subscription_mode_t mode);

	/**
	 * @brief Set the callback function.
	 * The callback function is then invoked when new information message
	 * are published by the BarbequeRTRM server instance.
	 * The callback function has to be implemented by the third-party client
	 * and has to have a status message parameter.
	 * @param client_callback The client's callback function.
	 */
	inline void SetCallback(
			std::function<void(status_message_t)> callback_fn) noexcept {
		client_callback = callback_fn;
	}

	/**
	 * @brief Utility function to convert a resource bitset into a resource path string
	 * @param bitset The bitset to convert
	 * @return A pointer to the char string
	 */
	static std::string GetResourcePathString(res_bitset_t bitset);

private:

	/**
	 * @brief This method performs the connection with the BarbequeRTRM DataManager
	 * server instance starting the receiver thread for incoming published updates.
	 * @return A DataClient exit code. 0 if OK, 1 if ERR_SERVER_COMM.
	 */
	ExitCode_t Connect();

	/**
	 * @brief This method stops the receiver thread and closes connection to the
	 * BarbequeRTRM DataManager server instance.
	 * @return A DataClient exit code. 0 if OK, 1 if ERR_SERVER_COMM.
	 */
	ExitCode_t Disconnect();

	/**
	 * @brief Utility template function to drop bits outside the range
	 * [R, L) == [R, L - 1]
	 * R The rightmost bits of range
	 * L The leftmost bits of range
	 * N The size of the bitset
	 * @param b The original bitset
	 * @return A new bitset between the requested range
	 */
	template<std::size_t R, std::size_t L, std::size_t N>
	static std::bitset<N> RangeBitset(std::bitset<N> b) {
	    static_assert(R <= L && L <= N, "invalid range");
	    b >>= R;            // drop R rightmost bits
	    b <<= (N - L + R);  // drop L-1 leftmost bits
	    b >>= (N - L);      // shift back into place
	    return b;
	}

	/**
	 * @brief This is the receiver loop-function to manage incoming information
	 * published by the DataManager server instance.
	 */
	void ClientReceiver();


	/// IP address of the server
	std::string server_ip;

	/// TCP port of the server
	uint32_t server_port;

	/// The TCP port of the client
	uint32_t client_port;

	/**
	 * @brief This is the callback function of the third-party client
	 * It is called when new information are published by the
	 * DataManager server instance.
	 */
	std::function<void(status_message_t)> client_callback;

	/// Connection status flag
	bool is_connected = false;

	/// Mutex protecting connection status flag
	std::mutex mtx_connection;

	/// Condition variable to notify connections status changes
	std::condition_variable cv_connection;

	/// The client receiver thread
	std::thread client_thread;

	/// The client receiver thread tid
	pid_t client_thread_tid;

	/// The acceptor of the client receiver thread
	std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_ptr;

};

} // namespace bbque

#endif // BBQUE_DATA_CLIENT_H_
