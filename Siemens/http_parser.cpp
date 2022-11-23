#pragma warning(disable: 4996)

#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <sstream>
#include <restbed>
#include <cstdio>
#include <stdio.h>

using namespace std;
using namespace restbed;

string run_cmd(string cmd) 
{
    /*
    * Runs command and returns output
    * 
    * @param cmd The command to be run
    * @return the output of the command in stdout
    */
    char buffer[256];
    string result = "";
    FILE* pipe = _popen(cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    
    _pclose(pipe);
    return result;
}
string parse_request(string request) 
{
    /*
    * Parses Http request message and returns response message
    * 
    * @param request The Http request message
    * @return the output response
    */
    auto tokens = String::split(request, ' ');
    string result;
    // Print help
    if (tokens[0] == "-h" || tokens[0] == "--help" || tokens[0] == "help") 
    {
        result = "Possible requests:\n"
                    "\t run <command with arguments>\t runs command and returns output as response.\n"
                    "\t tshark <file_name>\t\t runs tshark for given pcap file nameand returns output as response.\n"
                    "\t hello\t\t\t\t says hello back :)";
        return result;
    }
    // Run command
    if (tokens[0] == "run")
    {
        if(tokens.size() > 1)
        {
            result = run_cmd(request.substr(4));
            if (result.empty()) return "Invalid command";
        }
        else
        {
            result = "Please specify command to run.";
        }
        return result;
    }
    // Run tshark
    if (tokens[0] == "tshark")
    {
        if (tokens.size() == 2)
        {
            result = run_cmd("tshark -cc -r " + tokens[1] + " - V - o ber.show_internals:true");
            if (result.empty()) return "Invalid Arguments";
        }
        else
        {
            result = "Invalid Arguments";
        }
        return result;
    }
    if (tokens[0] == "hello")
    {
        result = "Hello!";
        return result;
    }

    result = "Invalid request. Use -h OR --help OR help for more information.";
    return result;
}

void post_method_handler(const shared_ptr< Session > session)
{
    const auto request = session->get_request();
    size_t content_length = request->get_header("Content-Length", 0);
    
    session->fetch(content_length, [request](const shared_ptr< Session > session, const Bytes& body)
        {      
            std::string data = std::string((char*)body.data(), body.size());
            string result = parse_request(data);
            session->close(OK, result + "\n", {{"Content-Length", to_string(result.size() + 1)}, {"Connection", "close"}});
        });
    return;
}

int main()
{
    auto resource = make_shared< Resource >();
    resource->set_path("/resource");
    resource->set_method_handler("POST", post_method_handler);

    auto settings = make_shared< Settings >();
    settings->set_port(800);

    Service service;
    service.publish(resource);
    service.start(settings);

    return EXIT_SUCCESS;
}

