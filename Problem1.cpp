#include <iostream>
#include <thread>
#include <shared_mutex>
using namespace std;

#define maxEvents 100
#define maxUsers 100
#define maxTickets 100

//struct
struct Event
{
    string eventID;
    string eventName;
    string eventDate;
    int availableTickets;
    bool isActive = false;
};

struct User 
{
    string userID;
    string userName;
    string userPass;
    bool isLoggedin = false;
};



//prototypes
 void displayMenu();
 void manageEvents();
 void manageUsers();
 void manageTickets();
 void concurrencyControl();
 void liveness();

int main(){
	displayMenu();
	
}

//functions
void displayMenu()
{
	int choice;
	
	while (true)
	{
		cout << "==============================\n";
		cout << "    EVENT TICKETING SYSTEM    \n";
		cout << "==============================\n";
		cout << "1. Event Management" << endl;
		cout << "2. User Management" << endl;
		cout << "3. Ticket Management" << endl;
		cout << "4. Concurrency Control" << endl;
		cout << "5. Liveness" << endl; 
		cout << "6. Exit" <<endl;
		cout << "==============================\n";
		cout << "Enter your choice: ";
		cin >> choice;
		
		switch (choice)
		{
			case 1:
				manageEvents();
				break;
			case 2:
				manageUsers();
				break;
			case 3:
				manageTickets();
				break;
			case 4:
				//concurrencyControl();
				break;
			case 5:
				//liveness();
				break;
			case 6:
				cout << "Exiting Program..." << endl;
				return;
			default:
				cout << "Invalid input. Please choose from 1-6 only.\n";
				continue;
		}
	}
}

void manageEvents()
{
	int choice;
	
	while (true)
	{
		cout << "==============================\n";
		cout << "        Event Management      \n";
		cout << "==============================\n";
		cout << "1. Add new event" << endl;
		cout << "2. Update event details" << endl;
		cout << "3. Remove an event" << endl;
		cout << "4. View all events" << endl;
		cout << "5. Return to Main Menu" << endl;
		cout << "==============================\n";
		cout << "Enter your choice: ";
		cin >> choice;
		
		switch (choice)
		{
			case 1:
				//function call here
				cout << "Event has been added!" << endl;
				break;
			case 2:
				//function call here
				cout << "Event has been successfully updated!" << endl;
				break;
			case 3:
				//function call here
				cout << "Event has been removed." << endl;
				break;
			case 4:
				cout << "--------Events List--------" << endl;
				//function call here
				break;
			case 5:
				return;
			default:
				cout << "Invalid input. Please choose fom 1-5 only. \n";
				continue;
		}
	}
}

void manageUsers()
{
	int choice;
	
	while (true)
	{
		cout << "==============================\n";
		cout << "        User Management       \n";
		cout << "==============================\n";
		cout << "1. Register new user" << endl;
		cout << "2. User Login" << endl;
		cout << "3. User Logout" << endl;
		cout << "4. Return to Main Menu" << endl;
		cout << "==============================\n";
		cout << "Enter your choice: ";
		cin >> choice;
		
		switch (choice)
		{
			case 1:
				//function call here
				cout << "A new user has been registered!" << endl;
				break;
			case 2:
				//function call here
				cout << "Login successful!" << endl;
				break;
			case 3:
				//function call here
				cout << "User has been logged out." << endl;
				break;
			case 4:
				return;
			default:
				cout << "Invalid input. Please choose from 1-4 only.\n";
				continue;
		}
	}
}

void manageTickets()
{
	int choice;
	
	while (true)
	{
		cout << "=============================================\n";
		cout << "              Ticket Management              \n";
		cout << "=============================================\n";
		cout << "1. Purchase a ticket" << endl;
		cout << "2. View available tickets for an event" << endl;
		cout << "3. Cancel purchased tickets" << endl;
		cout << "4. Return to Main Menu " << endl;
		cout << "=============================================\n";
		cout << "Enter your choice: ";
		cin >> choice;
		
		switch (choice)
		{
			case 1:
				//function call here
				cout << "Thank you for your purchase!" << endl;
				break;
			case 2:
				cout << "--------Available Tickets--------" << endl;
				//function call here
				break;
			case 3:
				//function call here
				cout << "Ticket has been successfully canceled." << endl;
				break;
			case 4:
				return;
			default:
				cout << "Invalid input. Please choose from 1-4 only.\n";
				continue;	
		}
	}
}
