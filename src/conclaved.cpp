/*
 * CONCLAVE - Scaling Bitcoin Simply.
 * Copyright (C) 2019-2021 Conclave development team
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

#include "util/filesystem.h"
#include "conclave_node.h"
#include "config/config.h"
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <string>
#include <iostream>

using namespace conclave;
using namespace fs;
using namespace boost::asio;
using namespace boost::program_options;

static void sigHandler(const boost::system::error_code error, int signal)
{
    if (!error) {
        std::cout << "Caught signal " << signal << "...Exiting" << std::endl;
    }
}

int main(int argc, char** argv)
{
    try {
        options_description desc{"Options"};
        variables_map vm;
        std::cout << "CONCLAVE - Scaling Bitcoin Simply." << std::endl;
        std::cout << "Copyright (C) 2019-2021 Conclave development team" << std::endl;
        std::cout << "This software is protected by the GNU General Public License v3" << std::endl;
        std::cout << "https://www.gnu.org/licenses/gpl-3.0.en.html" << std::endl;
        std::cout << "https://conclave.network" << std::endl;
        std::cout << "Current path is: " << fs::current_path() << std::endl;
        desc.add_options()
                ("help,h", "Help Screen")
                ("config-file,c", value<std::string>(), "Config file");
        
        // read variables map
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);
        
        // check if user needs help
        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return 0;
        }
        
        // Ensure a config file
        if (!vm.count("config-file")) {
            std::cout << "Please specify a config file." << std::endl;
            return 1;
        }
        
        // Load config
        const std::string configFilePath = vm["config-file"].as<std::string>();
        const Config config(configFilePath);
        std::cout << "Config loaded from " << configFilePath << std::endl;
        std::cout << "Testnet mode is " << (config.getTestnet() ? "on" : "off") << std::endl;
        
        // Create the node
        ConclaveNode conclaveNode(config);
        
        // Construct a signal set registered for process termination.
        io_context ioContext;
        signal_set signals(ioContext, SIGINT, SIGTERM);
        
        // Start an asynchronous wait for one of the signals to occur.
        signals.async_wait(sigHandler);
        
        // Start the node
        conclaveNode.start();
        
        // Await signal
        ioContext.run();
        
        // Stop the node
        conclaveNode.stop();
        std::cout << "Clean exit with code 0" << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
