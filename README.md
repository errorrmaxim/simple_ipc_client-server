# simple_ipc_client-server
Sending structure with abstract data by using boost::asio.

##That application show IPC between client side and server side##

#Client process is sending structure: 
    BYTE cPriority;
    DWORD dwTicks;
    std::string dwClientId;
    std::string Data;
    
without any response from server. 
Server geting requests from multiples client and push structures into priority queue. 
Structure with recv server has overload operator<(), for sorting data inside queue. Also server write recv data to 
.log file be using Consumer/Producer.
