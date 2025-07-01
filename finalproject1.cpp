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
const int roundNum = 3;

//card rank and suits
vector<string> ranks = //from lowest value to highest value (2-13; val=2(rank 3), val=12(rank "Ace"))
{
    "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace", "2"
};

vector<string> suits = //from lowest to highest
{
  "Diamonds", "Clubs", "Hearts", "Spades"  
};

int roundNum = 1;
mutex printMtx;
latch playerReady(playerNum);
vector<int> roundWins(playerNum, 0);
vector<pair<pair<int, int>, string>> cardDrawn(playerNum);

barrier roundDone(playerNum, [] {
    lock_guard<mutex> lock(printMtx);
    
    cout << "Evaluating cards..." << endl;
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
        
    }
    
}
void playerThread(int id);
void gameStart(int playerNum, bool isAuto);























/*
Pusoy Clash (Pusoy Dos Game)
Simulate a digital card draw battle based on the Filipino 
game Pusoy Dos, where 4 players play through 3 rounds. In 
each round, players “draw” a random card (value 2–13). The 
round winner is the player with the highest card.

output goal
[Player 1] is ready to play...
[Player 2] is ready to play...
[Player 3] is ready to play...
[Player 4] is ready to play...

All players are ready! Game start! 

[Player 1's Turn]
------Round 1-----
1. Draw a card
2. Pass your turn
------------------
Choice: [input here]

[Player 2's Turn]
-----Round 1-----
1. Draw a card
2. Pass your turn 
------------------
Choice: [input here]

[Player 3's Turn]
-----Round 1-----
1. Draw a card
2. Pass your turn 
-----------------
Choice: [input here]

[Player 4's Turn]
-----Round 1-----
1. Draw a card
2. Pass your turn 
-----------------
Choice: [input here]

[Player 1] has drawn [card rank and suit here].
[Player 2] has drawn [card rank and suit here].
[Player 3] has drawn [card rank and suit here].
[Player 4] has drawn [card rank and suit here].

[Player __] has the highest card! 

The winner for Round 1 is: PLayer ___!

//then same for rounds 2 and 3

int main()
{
    int choice;
    while(true){
        cout << "-----MENU-----" << endl;
        cout << "1. Manual Play" << endl;
        cout << "2. Exit" <<endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice){
            case 1:
                gameStart(4, false);
                return 0;
            case 2:
                cout << "Exiting..." << endl;
                return 0;
            default:
                if(cin.fail()){
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Please enter a number only." << endl;
                } else {
                    cout << "Error. Please choose between 1 and 2 only." << endl;
                }
        }
    }
}
*/
