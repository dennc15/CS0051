//counting semaphore
//count -> max (4)
#include<iostream>
#include<thread>
#include<barrier>
#include<semaphore>
#include<mutex>
#include<chrono>
using namespace std;

const int MAX_TABLES = 4; //4 tables are available
const int GROUP_NUM = 7;  //there are 7 groups to be seated
const int GROUP_MEM = 5; //there are 5 members per group


counting_semaphore<MAX_TABLES> freeTables(MAX_TABLES); //4 tables to occupy
mutex mtx;

void member(int groupID, int memberID, barrier<>& groupSyncPoint)
{
    {
        lock_guard<mutex> lock(mtx);
        cout << "Group " << groupID << ", member " << memberID << " has arrived and has seated.\n";
    }
    groupSyncPoint.arrive_and_wait(); //waits for all members to be seated before ordering
    
    {
        lock_guard<mutex> lock(mtx);
        cout << "All members of Group " << groupID << " are ready. Ordering...\n\n";
    }
    this_thread::sleep_for(chrono::seconds(1));
    
    {
        lock_guard<mutex> lock(mtx);
        cout << "Group " << groupID << ", member " << memberID << " is eating...\n";
    }
    this_thread::sleep_for(chrono::milliseconds(100));
    
    {
        lock_guard<mutex> lock(mtx);
        cout << "Group " << groupID << ", member " << memberID << " has finished eating.\n";
    }
    groupSyncPoint.arrive_and_wait(); //wait for all member to finish eating before paying the bill
    
    //a group member pays the bill
    if (memberID == 1)
    {
        lock_guard<mutex> lock(mtx);
        cout << "\nGroup " << groupID << ", member " << memberID << " paid the bill.\n\n";
    }
    this_thread::sleep_for(chrono::milliseconds(100));
    
    {
        lock_guard<mutex> lock(mtx);
        cout << "Group " << groupID << ", member " << memberID << " is leaving.\n";
    }
}

void group(int groupID)
{
    freeTables.acquire();
    {
        lock_guard<mutex> lock(mtx);
        cout << "Group " << groupID << " got a table.\n";
    }
    
    //barrier for the groups
    barrier groupBarrier(GROUP_MEM);
    
    thread groupMembers[GROUP_MEM];
    for(int i = 0; i < GROUP_MEM; ++i)
    {
        groupMembers[i] = thread(member, groupID, i, ref(groupBarrier));
    }
    
    //join threads
    for(int i = 0; i < GROUP_MEM; ++i)
    {
        groupMembers[i].join();
    }
    
    {
        lock_guard<mutex> lock(mtx);
        cout << "\nAll members of Group " << groupID << " left. Table is now available.\n\n";
    }
    
    freeTables.release(); //free up the table
}

void manualRun()
{
    thread groups[GROUP_NUM];
    for(int i = 0; i < GROUP_NUM; ++i)
    {
        string enter;
        cout << "\nPress the Enter key to seat groups...\n";
        getline(cin, enter);
        
        groups[i] = thread(group, i + 1);
    }
    
    for(int i = 0; i < GROUP_NUM; ++i)
    {
        groups[i].join();
    }
}

void automatedRun()
{
    thread groups[GROUP_NUM];
    
    for(int i = 0; i < GROUP_NUM; ++i)
    {
        groups[i] = thread(group, i + 1);
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    
    for(int i = 0; i < GROUP_NUM; ++i)
    {
        groups[i].join();
    }
}

int main(){
    int choice;
    do
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "  Restaurant Simulation Menu  \n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "1. Manual Run" << endl;
        cout << "2. Automated Run" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        
        switch(choice)
        {
            case 1:
                manualRun();
                break;
            case 2:
                automatedRun();
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid option. Please choose from 1-3 only. Please try again." << endl;
        }
        
    } while (choice != 3);

    return 0;
}
