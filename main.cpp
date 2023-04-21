#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


void printCards(vector<std::pair<char,char>> &vecOfCards);
void readCharFile(string filePath, vector<std::pair<char , char>> &vecOfCards);

int main(int argc, char *argv[]) {
    vector<std::pair<char , char>> vecOfCards;
    readCharFile(argv[1], vecOfCards);
    printCards(vecOfCards);
    return 0;
}



void readCharFile(string filePath, vector<std::pair<char , char>> &vecOfCards) {
    ifstream in(filePath);

    char c = '\0';
    std::pair<char,char> card;

    if(in.is_open()) {
        while(in.good()) {
            in.get(c);
            card.first = c;
            in.get(c);
            card.second = c;
            vecOfCards.push_back(card);
            in.get(c);
        }

    }
    if(!in.eof() && in.fail())
        cout << "error reading " << filePath << endl;

    in.close();
}


void printCards(vector<std::pair<char,char>> &vecOfCards){
    for(int i = 0; i < size(vecOfCards);i++){
        cout << vecOfCards.at(i).first << vecOfCards.at(i).second << endl;
    }
}


void checkHighestValueHand(){

}


