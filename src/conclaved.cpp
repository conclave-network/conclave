/*
 * CONCLAVE - Making Bitcoin Scale And Be Useful
 * Copyright (C) 2019-2020 Noel P. O'Donnell <noel.odonnell.2020@mumail.ie>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "conclave_node.h"
#include "config/config.h"
#include <boost/asio.hpp>
#include <iostream>

using namespace conclave;
const static std::string CONFIG_FILE = "etc/conclaved-config.json";

void sigHandler(const boost::system::error_code error, int signal)
{
    if (!error) {
        std::cout << "Caught signal " << signal << "...Exiting" << std::endl;
    }
}

int main()
{
    // Construct a signal set registered for process termination.
    boost::asio::io_context ioContext;
    boost::asio::signal_set signals(ioContext, SIGINT, SIGTERM);
    
    // Start an asynchronous wait for one of the signals to occur.
    signals.async_wait(sigHandler);
    
    // Load config
    Config config(CONFIG_FILE);
    std::cout << "CONCLAVE - Making Bitcoin Scale And Be Useful" << std::endl;
    std::cout << "Copyright (C) 2019-2020 Noel P. O'Donnell <noel.odonnell.2020@mumail.ie>" << std::endl;
    
    // Create the node
    ConclaveNode conclaveNode(config);
    
    // Start the node
    conclaveNode.start();
    
    // Await signal
    ioContext.run();
    
    // Stop the node
    conclaveNode.stop();
    return 0;
}
