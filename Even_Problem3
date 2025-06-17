#include<iostream>
#include<thread>
#include<barrier>
#include<chrono>
#include<mutex>
#include<atomic>
using namespace std;

const int SWIMMER_NUM = 6;

mutex mtx;
atomic<int> step = 0; //tracks the current move the swimmers are on
barrier sync_point(SWIMMER_NUM, [](){
    lock_guard<mutex> lock(mtx);
    cout << "\nAll swimmers are synced! Moving...\n\n";
});

void swimmer(int id){
    {
        lock_guard<mutex> lock(mtx);
        cout << "Swimmer " << id << " is ready.\n";
    }
    while(step < 1) this_thread::yield();
    this_thread::sleep_for(chrono::milliseconds(100));
    sync_point.arrive_and_wait();   //waits until all swimmers are ready
    
    {
        lock_guard<mutex> lock(mtx);
        cout << "Swimmer " << id << " is doing The Tub...\n";
    }
    while(step < 2) this_thread::yield();
    this_thread::sleep_for(chrono::milliseconds(100));
    sync_point.arrive_and_wait(); //waits for all swimmers to finish 'The Tub' move
    
    {
        lock_guard<mutex> lock(mtx);
        cout << "Swimmer " << id << " is doing The Back Tuck...\n";
    }
    while(step < 3) this_thread::yield();
    this_thread::sleep_for(chrono::milliseconds(100));
    sync_point.arrive_and_wait(); //waits for all swimmers to finish 'The Back Tuck' move

    {
        lock_guard<mutex> lock(mtx);
        cout << "Swimmer " << id << " is doing The Water Wheel...\n";
    }
    while(step < 4) this_thread::yield();
    this_thread::sleep_for(chrono::milliseconds(100));
    sync_point.arrive_and_wait(); //waits for all swimmers to finish 'The Water Wheel' move
    
    {
        lock_guard<mutex> lock(mtx);
        cout << "Swimmer " << id << " is doing the final pose...\n";
    }
    this_thread::sleep_for(chrono::milliseconds(50));
}

//function for manual run
void manualRun()
{
    thread swimmers[SWIMMER_NUM];
    for(int i = 0; i < SWIMMER_NUM; ++i)
    {
        swimmers[i] = thread(swimmer, i + 1);
    }
    
    for(int i = 1; i <= 4; ++i)
    {
        string enter;
        cout << "\nPress the Enter key to perform synchronized move " << i << "...\n";
        getline(cin, enter);
        ++step;
    }
    
    for(int i =0; i < SWIMMER_NUM; ++i)
    {
        swimmers[i].join();
    }
}

//function for automated run
void automatedRun()
{
    thread swimmers[SWIMMER_NUM];
    for(int i = 0; i < SWIMMER_NUM; ++i)
    {
        swimmers[i] = thread(swimmer, i + 1);
    }
    
    for(int i = 1; i <= 4; ++i)
    {
        this_thread::sleep_for(chrono::seconds(1));
        ++step;
    }
    
    for(int i = 0; i < SWIMMER_NUM; ++i)
    {
        swimmers[i].join();
    }

}

int main(){
    
    int choice;
do{
    //menu for manual and automated operations
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    cout << "    Synchronized Swimmers Routine Menu     \n";
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    cout << "1. Manual " << endl;
    cout << "2. Automated " << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore();
    
    switch (choice)
    {
        case 1:
            cout << "Manual Run\n";
            cout << "---------------\n";
            manualRun();
            cout << "\nPerformance done! A standing ovation from the crowd!\n";
            break;
        case 2:
            cout << "Automated Run\n";
            cout << "---------------\n";
            automatedRun();
            cout << "\nPerformance done! A standing ovation from the crowd!\n";
            break;
        case 3:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Error. Please choose from 1-3 only.\n";
    }
} while(choice != 3);
    
    return 0;
}
