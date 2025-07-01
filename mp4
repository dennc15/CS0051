#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <barrier>
#include <future>
#include <mutex>
#include <latch>
#include <vector>
#include <random>

using namespace std;

recursive_mutex printMtx;
int roundNo = 1;
latch login(4);
int points[4] = {0,0,0,0};
bool correct[4] = {0,0,0,0};

barrier finishRound(4, [](){
    lock_guard<recursive_mutex> lock(printMtx);
    cout<<"\n[ROUND "<<roundNo<<" FINISHED]"<<endl;
    cout<<"Checking scores..."<<endl;
    for(int i=0; i<4; i++){
        if(correct[i]){
            cout<<"Player "<<i+1<<" has answered correctly!"<<endl;
        }else{
            cout<<"Player "<<i+1<<" has answered incorrectly!"<<endl;
        }
    }
    cout<<"\nCurrent Scores:"<<endl;
    for(int i=0; i<4; i++)
        cout<<"Player "<<i+1<<" -- "<<points[i]<<endl;
    
    roundNo++;
});

//Questions
struct Question{
    string q;
    char ans;
    vector<string> ch;
};
vector<Question> questions = { 
    {"Which of the following would a magnet most likely attract?", 'a', {"a) Metal", "b) Plastic", "c) Wood", "d) The wrong person"}},
    {"What is the most populated country in the world?", 'b', {"a) China", "b) India", "c) United States", "d) Russia"}},
    {"Which insect shorted out an early supercomputer and inspired the term 'computer bug'?", 'd', {"a) Beetle", "b) Cockroach", "c) Fly", "d) Moth"}},
    {"Who sang the song 'Billy Jean'?", 'b', {"a) Michael B. Jordan", "b) Michael Jackson", "c) Michael Jordan", "d) Michael Wazowski"}},
    {"Who lives in a pineapple under the sea?", 'c', {"a) Patrick Star", "b) Phineas and Ferb", "c) Spongebob Squarepants", "d) Ariel"}},
    {"Who painted the Mona Lisa?", 'd', {"a) Vincent van Gogh", "b) Pablo Picasso", "c) Claude Monet", "d) Leanardo da Vinci"}},
    {"What is the deadliest animal in the world?", 'b', {"a) Scorpion", "b) Mosquito", "c) Crocodile", "d) Shark"}},
    {"The Earth is approximately how many miles away from the Sun?", 'c', {"a) 9.3 million", "b) 39 million", "c) 93 million", "d) 193 million"}},
    {"Who was the first female scientist to win a Nobel Prize?", 'b', {"a) Emily Blackwell", "b) Marie Curie", "c) Florence Nightingale", "d) Grace Hopper"}}
};

//PROTOTYPES
char askQ(int id);
void gameplay(int id);
void start();
bool checkAns(int id, char ans); 
string checkHighest();


int main()
{
    start();
}

char askQ(int id){ 
    Question q = questions[roundNo-1];
    char ch = ' ';
    string answer;
    
    bool lifeline = false;
    
    while (true) {
        {
            lock_guard<recursive_mutex> lock(printMtx);
            cout <<"\n"<< q.q << endl;
            for (auto& choice : q.ch) cout << choice << endl;
            cout << "[You have 30 seconds to answer]\n";
            cout << "Player " << id << "'s choice (Press 0 to use lifeline): ";
        }
        
        auto start = chrono::steady_clock::now(); //start timer
        
        cin >> answer; //answer will be marked wrong if entered after the time limit
        
        auto end = chrono::steady_clock::now();
        auto timeElapsed = chrono::duration_cast<chrono::seconds>(end - start).count();
        
        if (timeElapsed > 30){
            lock_guard<recursive_mutex> lock(printMtx);
            cout << "\nInvalid answer. Time limit exceeded!\n";
            return 'x'; //player's answer is automatically marked
        }
        
        ch = answer[0];
        if (ch == '0') {
            if (!lifeline) {
                int choice;
                {
                    lock_guard<recursive_mutex> lock(printMtx);
                    cout << "\n-----Lifeline-----\n";
                    cout << "1. Phone a friend\n";
                    cout << "2. 50/50\n";
                    cout << "3. Ask the Audience\n";
                    cout << "What lifeline would you like to use? ";
                    cin >> choice;
                }
        
                // Use lifeline...
                random_device randomA;
                mt19937 rng(randomA());
                uniform_int_distribution<int> dist(0, 3);
                int lifelineAns;
        
                switch (choice) {
                    case 1:
                    {
                        lifelineAns = dist(rng);
                        lock_guard<recursive_mutex> lock(printMtx);
                        cout << "\nI think the answer is " << q.ch[lifelineAns] << endl;
                        break;
                    }
                    case 2:
                    {
                        vector<int> wrongAns;
                        for (int i = 0; i < 4; ++i) {
                            if ('a' + i != q.ans)
                                wrongAns.push_back(i);
                        }
                        shuffle(wrongAns.begin(), wrongAns.end(), rng);
                        lock_guard<recursive_mutex> lock(printMtx);
                        cout << "\nRemaining choices:\n";
                        cout << q.ch[q.ans - 'a'] << endl;
                        cout << q.ch[wrongAns[0]] << endl;
                        break;
                    }
                    case 3:
                    {
                        lifelineAns = dist(rng);
                        lock_guard<recursive_mutex> lock(printMtx);
                        cout << "\nThe audience think the answer is: " << q.ch[lifelineAns] << endl;
                        break;
                    }
                    default:
                    {
                        lock_guard<recursive_mutex> lock(printMtx);
                        cout << "Invalid choice. Please choose from 1–3 only\n";
                    }
                }
        
                lifeline = true;
                continue;
            } else {
                cout << "You already used a lifeline." << endl;
                continue;
            }
        } else {
            break; // answer is valid, exit loop
        }
    }
    return ch;
}


bool checkAns(int id, char ch) {
    char cor = questions[roundNo - 1].ans;
    if (ch == cor) {
        points[id - 1] += 10;
        correct[id - 1] = 1;
        return true;
    } else {
        correct[id - 1] = 0;
        return false;
    }
}

void gameplay(int id){
    char ch;
    int plPoints = points[id-1];
    {
        lock_guard<recursive_mutex> lock(printMtx);
        cout<<"Player "<< id<<" Logging in.."<<endl;
    }
    this_thread::sleep_for(chrono::seconds(1)); // login delay

    login.count_down(); // signal that this player is done logging in
    login.wait();       // wait for all players to log in

    for (int i = 1; i <= 3; ++i){
        {
            lock_guard<recursive_mutex> lock(printMtx);
            cout<<"\n[ROUND "<<roundNo<<" -- Player "<<id<<"'s turn]"<<endl;
            ch = askQ(id);
        }
        //async
        future<bool> res = async(launch::async, checkAns, id, ch);
        res.get(); 
        finishRound.arrive_and_wait();
        
    }

}

string checkHighest(){
    int max = points[0];
    vector<int> top = {0};

    for (int i = 1; i < 4; ++i) {
        if (points[i] > max) {
            max = points[i];
            top.clear();
            top.push_back(i);
        } else if (points[i] == max) {
            top.push_back(i);
        }
    }

    string winners;
    for (size_t i = 0; i < top.size(); ++i) {
        winners += "Player " + to_string(top[i] + 1);
        if (i < top.size() - 1)
            winners += " and ";
    }

    return winners;
}
    
void start(){
    //randomize questions
    random_device randomQ;
    mt19937 rng(randomQ());
    shuffle(questions.begin(), questions.end(), rng);
    
    vector<thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(gameplay, i+1);
    }
    
    for (auto& t : threads) t.join();
    
    cout<<"The Player(s) with the highest score is.. "<<checkHighest()<<"!"<<endl;
    
    cout<<"\nExiting..."<<endl;
    cout<<"Group 4"<<endl;

}
