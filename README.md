# Siemens-Task
application for working student position - Working Student (f/m/d) Coding Project (C/C++).
## Problem statement
"Create a windows-based application which will be hosted as a service that starts Web Interface or REST API to access the internal application features.

The windows-based application will receive, process and send HTTP requests and response.

When a HTTP request is received on the Web Interface, the application will parse the HTTP request and take actions accordingly.

For example, when a HTTP request to run a tshark application is received, the application will parse the request,  run a tshark application with the parameters in the request and send back the dissection information as HTTP response.

The parameters for the request will be a pcap file name for which tshark need to be run,

tshark -cc -r <file_name> -V -o ber.show_internals:true”

## How to use
send HTTP requests to 
`localhost:800\resource`

this can be done using any method. For Example:

`curl -d "help" -X POST localhost:800/resource`

use `-d` to specify message and send "help" to see a list of recognized requests

## Technologies used
### REST API
The c++ restbed package was used to receive handle and send HTTP requests and responses
### POSIX
The POSIX interface was used to run commands externally and receive their output as a String