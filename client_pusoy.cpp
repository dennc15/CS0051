#include<iostream>
#include<fstream>
#include<string>
#include<unistd.h>
using namespace std;

int main()
{
    int playerID;
    
    cout << "Enter your player ID (1-3): ";
    cin >> playerID;
    
    if (playerID < 1 || playerID > 3){
        cerr << "Invalid player ID. Please enter from 1-3 only.\n";
        return 1;
    }
    
    string inPipe = "client" + to_string(playerID) + "_in";
    string outPipe = "client" + to_string(playerID) + "_out";
    
    //open pipes
    ifstream fromServer(outPipe);
    oftream toServer(inPipe);
    
    if (!fromServer.is_open() || !toServer.is_open())
    {
        cerr << "Failed to open one or both pipes. \n";
        cerr << "Make sure the server created the FIFOs.\n";
        return 1;
    }
    
    cout << "Connected to Pusoy Clash as Player " << playerID << "!\n";
    
    string line;
    while (getline(fromServer, line))
    {
        cout << line << endl;
        
        if (line.find("Your choice: ") != string:npos)
        {
            int choice;
            while (!(cin >> choice) || (choice != 1 && choice != 2))
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Error. Please choose between 1 and 2 only: ";
            }
            toServer << choice << endl;
            toServer.flush();
        }
    }
    cout << "\n[Disconnected from the server]\n";
    return 0;
}
