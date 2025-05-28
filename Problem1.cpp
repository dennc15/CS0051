#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
using namespace std;

#define maxEvents 200
#define maxUsers 200
#define maxTickets 200

//struct
struct Event
{
    string eventID;   //unique
    string eventName;
    string eventDate;
    bool isActive = false;
};

struct User 
{
    string userID;  //unique
    string userName;
    string userPass;
    bool isLoggedin = false;
};

struct Ticket 
{
    string ticketID;    //unique
    string eventID, eventName, eventDate;
    string userID, userName;
    bool isCanceled = false;
};

//classes
class eventManagement
{
    private:
        Event events[maxEvents];
        int eventCount = 0;
        mutable std::shared_mutex eventMtx;
    public:
        bool addEvent (const Event& newEvent)   //function to add an event
        {
            std::unique_lock<std::shared_mutex> lock(eventMtx);
            //prevention of duplicate event IDs
            for (int i = 0; i < eventCount; ++i)
            {
                if (events[i].eventID == newEvent.eventID)
                {
                    return false;
                }
            }
            if (eventCount >= maxEvents)
            {
                return false;
            }
            events[eventCount++] = newEvent;
            cout << "Event has been added!\n";
            return true;
        }
        bool updateEvent (const Event& update)  //function to update event details
        {
            std::unique_lock<std::shared_mutex> lock(eventMtx);
            for (int i = 0; i < eventCount; ++i)
            {
                if (events[i].eventID == update.eventID)
                {
                    events[i] = update;
                    cout << "Event has been updated.\n";
                    return true;
                }
            }
            cout << "Update failed. The event does not exist.\n";
            return false;
        }
        
        void viewEvents() const       //function to display all events and their details
        {
            std::shared_lock<std::shared_mutex> lock(eventMtx);
            if (eventCount == 0)
            {
                cout << "No listed events." << endl;
                return;
            }
            cout << "\n--------Event Details--------\n";
            for (int i = 0; i < eventCount; ++i)
            {
                cout << "Event Name: " << events[i].eventName << "\n";
                cout << "Event ID: " << events[i].eventID << "\n";
                cout << "Date of Event: " << events[i].eventDate << "\n";
                cout << "Status: " << (events[i].isActive ? "Active" : "Inactive") << endl;
                cout << "\n";
            }
        }
        
        bool removeEvent (const string& eventId)    //function to remove an event from the list
        {
            std::unique_lock<std::shared_mutex> lock(eventMtx);
            for (int i = 0; i < eventCount; ++i)
            {
                if (events[i].eventID == eventId)
                {
                    for (int j = i; j < eventCount - 1; ++j)
                    {
                        events[j] = events [j + 1];
                    }
                    eventCount--;
                    cout << "Event has been removed.\n";
                    return true;
                }
            }
            cout << "Couldn't find event.\n";
            return false;
        }
};

//global instances
eventManagement event;


//prototype
 void displayMenu();
 void manageEvents();
 void manageUsers();
 void manageTickets();
 void concurrencyControl();
 void liveness();

int main(){
	displayMenu();
	
}

//function
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
			case 1:             //event management
				manageEvents();
				break;
			case 2:            //user management
				manageUsers();
				break;
			case 3:            //ticket management
				manageTickets();
				break;
			case 4:
				//concurrencyControl();
				break;
			case 5:
				//liveness();
				break;
			case 6:            //exits the program
				cout << "Exiting Program..." << endl;
				return;
			default:           //will be displayed when the user enters a number greater than 6.
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
			case 1:                     //add new event
			{	
				Event newEvent;
				cout << "Event ID: ";
				cin >> newEvent.eventID;
				cout << "Event Name: ";
				cin.ignore();
				getline(cin, newEvent.eventName);
				cout << "Date of Event: ";
				getline(cin, newEvent.eventDate);
				cout << "Event status (1 = Yes, 0 = No): ";
				cin >> newEvent.isActive;
				
				event.addEvent(newEvent);
				break;
			}
			case 2:                     //update event details
			{	
				Event update;
				cout << "Update to Event ID: ";
				cin >> update.eventID;
				cout << "Update to Event Name: ";
				cin.ignore();
				getline(cin, update.eventName);
				cout << "Update to the Date of Event: ";
				getline(cin, update.eventDate);
				cout << "Is the event still active? (1 = Yes, 0 = No): ";
				cin >> update.isActive;
				
				event.updateEvent(update);
				break;
			}
			case 3:                    //remove an event from the existing events
			{
			    event.viewEvents();
			    
			    string eventToremove;
			    cout << "What event would you like to remove? Enter the Event ID: ";
			    cin >> eventToremove;
			    
			    event.removeEvent(eventToremove);
				break;
			}
			case 4:                    //displays all existing events and their corresponding details
				event.viewEvents();
				break;
			case 5:                    //returns to main menu (Event Ticketing System)
				return;
			default:                   //will be displayed when the user enters a number greater than 5.
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
