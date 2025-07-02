#include<iostream>
#include<thread>
#include<chrono>
#include<barrier> //for waiting for all players to draw their cards before showing 
#include<latch> //for waiting for all players to be ready before the game starts
#include<future> //aynchonous evaluation
#include<mutex>  //for synchronized printing
#include<cstdlib> //for random card generation
#include<ctime>
#include<vector>
using namespace std;

//constants
const int playerNum = 4;
const int roundTotal = 3;

//card rank and suits
vector<string> ranks = //from lowest value to highest value (2-13; val=2(rank 3), val=12(rank "Ace"))
{
    "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace", "2"
};

vector<string> suits = //from lowest to highest
{
  "Diamonds", "Clubs", "Hearts", "Spades"  
};

mutex printMtx;
latch playerReady(playerNum);
latch startGame(1); 
int roundNum = 1;
vector<int> roundWins(playerNum, 0);
vector<pair<pair<int, int>, string>> cardDrawn(playerNum);
vector<vector<int>> playerAns(roundTotal, vector<int>(playerNum, 0));

struct Sync {
    promise<void> p;
    future<void> f;
    
    Sync() :f(p.get_future()) {}
};

vector<Sync> roundSync(playerNum);

barrier roundDone(playerNum, [] {
    lock_guard<mutex> lock(printMtx);
    
    cout << "\nEvaluating cards..." << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    for(int i = 0; i < playerNum; ++i)
    {
        if(cardDrawn[i].first.first > 0){
            cout << "[Player " << i + 1 << "] has drawn " <<cardDrawn[i].second << ".\n";
        } else {
            cout << "[Player " << i + 1 << "] has passed the turn.\n";
        }
    }
    
    //check the winner of the round
    int winner = -1;
    pair<int, int> best = {0, 0};
    for(int i = 0; i < playerNum; ++i)
    {
        if(cardDrawn[i].first.first > 0 && cardDrawn[i].first > best){
            best = cardDrawn[i].first;
            winner = i;
        }
    }
    
    if (winner != -1)
    {
        cout << "\n[Player " << winner + 1 << "] has the highest card!" << endl;
        cout << "The winner for Round " << roundNum << " is: Player " << winner + 1 << "!" <<endl;
        roundWins[winner]++;
    } else {
        cout << "\nNo winner. All players skipped their turn." << endl;
    }
    roundNum++;
});

int randRank();
int randSuit();
string displayCard(int rankNum, int suitVal);
void evaluateWinner();
void playerThread(int id);
void gameStart(int playerNum, bool isAuto);

int main() 
{
    int choice;
    
    while(true)
    {
        cout << "WELCOME TO PUSOY CLASH!\n\n";
        cout << "----------Menu----------" << endl;
        cout << "1. Manual Play" << endl;
        cout << "2. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice)
        {
            case 1: //manual Play
                gameStart(playerNum, false);
                return 0;
            case 2:
                cout << "Exiting..." << endl;
                return 0;
            default: //error handling
                if (cin.fail()){
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Please enter a number only." << endl;
                } else {
                    cout << "Error. Please choose between 1 and 2 only." << endl;
                }
        }
    }
    
}

//functions
int randRank()
{
    return rand() % 13 + 1; 
}

int randSuit()
{
    return rand() % 4 + 1; 
}

string displayCard(int rankNum, int suitVal)
{
    return ranks[rankNum - 1] + " of " + suits[suitVal - 1];
}

void evaluateWinner() //player with most rounds won wins the whole game
{
    lock_guard<mutex> lock(printMtx);
    cout << "\n-----Game is Finished-----\n";
    
    for(int i = 0; i < playerNum; ++i)
    {
        cout << "Player " << i + 1 << " won " <<roundWins[i] << " round(s).\n";
    }
    
    int mostRoundsWon = 0;
    vector<int> gameWinner;
    
    for(int i = 0; i < playerNum; ++i)
    {
        if(roundWins[i] > mostRoundsWon){
            mostRoundsWon = roundWins[i];
            gameWinner = {i};
        } else if (roundWins[i] == mostRoundsWon){
            gameWinner.push_back(i);
        }
    }
    
    if(mostRoundsWon == 0){
        cout << "No one won. It's a draw!"  << endl;
    } else if (gameWinner.size() == 1) {
        cout << "\nThe winner of Pusoy Clash is Player " << (gameWinner[0] + 1) << "!" << endl;
    } else {
        cout << "\nIt's a tie between players: ";
        for(int id: gameWinner)
            cout << id + 1 << " ";
        cout << "\n";
    }
}
void playerThread(int id)
{
    {
        lock_guard<mutex> lock(printMtx);
        cout << "[Player " << id << "] is ready to play..." << endl;
    }
    
    playerReady.count_down();
    startGame.wait();
    
    for(int Round = 1; Round <= roundTotal; ++Round) {
        roundSync[id - 1].f.wait();
        int choice = playerAns[Round - 1][id - 1];
        
        if (choice == 1){
            int rankNum = randRank();
            int suitVal = randSuit();
            cardDrawn[id - 1] = {{rankNum, suitVal}, displayCard(rankNum, suitVal)};
        } else {
            cardDrawn[id - 1] = {{0, 0}, "Pass"};
        }
        roundDone.arrive_and_wait(); //wait for players to finish their turn before evaluating
    }
}
void gameStart(int playerNum, bool isAuto)
{
    srand(time(0));
    vector<thread> players;
    
    for(int i = 1; i <= playerNum; ++i){
        players.emplace_back(playerThread, i);
    }
    
    playerReady.wait();
    {
        lock_guard<mutex> lock(printMtx);
        cout << "\nAll players are ready to play! Game start!\n" << endl;
        cout << "\nThe cards are being shuffled..." << endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cout << "\nCards are shuffled!\n" << endl;
    }
    startGame.count_down();
    
    for(int Round = 1; Round <= roundTotal; ++Round){
        
        for(int i = 0; i < playerNum; ++i)
            roundSync[i] = Sync();
        
        for(int i = 0; i < playerNum; ++i){
            int choice;
            lock_guard<mutex> lock(printMtx);
            cout << "\n[Player " <<i + 1 << "'s Turn]" << endl;
            cout << "-----Round " << Round << "-----" << endl;
            cout << "1. Draw a card" << endl;
            cout << "2. Pass the turn" << endl;
            cout << "---------------" << endl;
            cout << "Your choice: ";
            while (!(cin >> choice) || (choice != 1 && choice != 2)){
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Error. Please choose between 1 and 2 only: ";
            }
            playerAns[Round - 1][i] = choice;
            roundSync[i].p.set_value();
        }
    }
    
    for(auto& t : players) t.join();
    auto evaluate = std::async(std::launch::async, evaluateWinner);
    evaluate.get(); //wait for evaluation to be done
}
