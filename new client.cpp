#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
using namespace std;

const int PORT = 8080;

int main() 
{
    int sckt = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    
    //socket
    if ((sckt = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cerr << "Couldn't create socket." << endl;
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    //converting IPs from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        cerr << "Invalid address" << endl;
        return -1;
    }
    
    //connection to server
    if (connect(sckt, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cerr << "Failed to connect." << endl;
        return -1;
    }
    cout << "Successfully connected to server!\n";
    
    while(true) 
    {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(sckt, buffer, sizeof(buffer));
        if (valread <= 0)
        {
            cout << "[Disconnected from server]\n";
            break;
        }
        string msg(buffer);
        msg.erase(msg.find_last_not_of("\r\n") + 1);
        
        cout << msg << endl;
        
        if (msg.find("ENTER_ID") != string::npos)
        {
            cout << "Enter your player ID (1-3): ";
            string playerID;
            getline (cin, playerID);
            send (sckt, playerID.c_str(), playerID.length(), 0);
        }
        
        if (msg.find("YOUR_TURN") != string::npos)
        {
            cout << "Draw a card\n2. Pass\n";
            cout << "Your choice: ";
            string choice;
            getline(cin, choice);
            send(sckt, choice.c_str(), choice.length(), 0);
        }
    }
    close(sckt);
    return 0;
}
