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
#include <boost/program_options.hpp>
#include <string>
#include <iostream>

using namespace conclave;
using namespace boost::asio;
using namespace boost::program_options;
const static std::string DEFAULT_CONFIG_FILE = "/usr/local/etc/conclaved-config.json";
// Program option keys
const static std::string HELP_KEY = "help";
const static std::string CONFIG_KEY = "config-file";

static void sigHandler(const boost::system::error_code error, int signal)
{
    if (!error) {
        std::cout << "Caught signal " << signal << "...Exiting" << std::endl;
    }
}

int main(int argc, char** argv)
{
    // Construct a signal set registered for process termination.
    io_context ioContext;
    signal_set signals(ioContext, SIGINT, SIGTERM);
    
    // Start an asynchronous wait for one of the signals to occur.
    signals.async_wait(sigHandler);
    
    // Read program options
    variables_map vm;
    options_description desc{"Options"};
    desc.add_options()(HELP_KEY.c_str(), "Help Screen")
            (CONFIG_KEY.c_str(), value<std::string>()->default_value(DEFAULT_CONFIG_FILE), "Config file");
    
    // read variables map
    store(parse_command_line(argc, argv, desc), vm);
    
    // Display welcome message
    std::cout << "CONCLAVE - Making Bitcoin Scale And Be Useful" << std::endl;
    std::cout << "Copyright (C) 2019-2020 Noel P. O'Donnell <noel.odonnell.2020@mumail.ie>" << std::endl;
    
    // check if user needs help
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }
    
    // Load config
    const std::string configFilePath = vm[CONFIG_KEY].as<std::string>();
    std::cout << "Attempting to read config from " << configFilePath << std::endl;
    const Config config(configFilePath);
    
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
