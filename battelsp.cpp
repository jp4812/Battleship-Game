#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <memory>
using namespace std;

void createGrid(const vector<vector<char>>& grid, int size, const string& name, bool showShips);
void display_rule();
bool allShipsSunk(const vector<vector<char>>& grid);
void displayScoreboard(const string& name1, const string& name2, int hits1, int hits2);

class GamePlayer {
protected:
    string name;
    vector<vector<char>> grid;
    int gridSize;
    int hits_scored = 0;
public:
    GamePlayer(string name, int size) : name(name), gridSize(size) {
        grid.resize(gridSize, vector<char>(gridSize, 'X'));
    }
    
    virtual void placeShips(const vector<int>& shipSizes) = 0;
    virtual bool performAttack(GamePlayer& opponent) = 0;
    string getName() const { return name; }
    vector<vector<char>>& getGrid() { return grid; }
    const vector<vector<char>>& getGrid() const { return grid; }
    int getHits() const { return hits_scored; }
    void incrementHits() { hits_scored++; }
    bool isSunk() const { return allShipsSunk(grid); }
};

class HumanPlayer : public GamePlayer {
private:
    vector<vector<char>> viewGrid;
public:
    HumanPlayer(string name, int size) : GamePlayer(name, size) {
        viewGrid.resize(gridSize, vector<char>(gridSize, 'X'));
    }
    void placeShips(const vector<int>& shipSizes) override;
    bool performAttack(GamePlayer& opponent) override;
};

class BotPlayer : public GamePlayer {
private:
    vector<vector<char>> attackTracker;
public:
    BotPlayer(string name, int size) : GamePlayer(name, size) {
        attackTracker.resize(gridSize, vector<char>(gridSize, 'X'));
    }
    void placeShips(const vector<int>& shipSizes) override;
    bool performAttack(GamePlayer& opponent) override;
};

class Player {
private:
    string player1_name;
    string player2_name;
    void runGame(GamePlayer& p1, GamePlayer& p2) {
        cout << "Press 1 to see the rules of the game: ";
        int rule;
        cin >> rule;
        cin.ignore();
        if (rule == 1) display_rule();
        while (true) {
            cout << "\n--- " << p1.getName() << "'s Turn ---" << endl;
            if (p1.performAttack(p2)) {
                cout << "\n*** " << p1.getName() << " wins! All ships of " << p2.getName() << " are sunk. ***" << endl;
                displayScoreboard(p1.getName(), p2.getName(), p1.getHits(), p2.getHits());
                break;
            }
            displayScoreboard(p1.getName(), p2.getName(), p1.getHits(), p2.getHits());
            cout << "\n--- " << p2.getName() << "'s Turn ---" << endl;
            if (p2.performAttack(p1)) {
                cout << "\n*** " << p2.getName() << " wins! All ships of " << p1.getName() << " are sunk. ***" << endl;
                displayScoreboard(p1.getName(), p2.getName(), p1.getHits(), p2.getHits());
                break;
            }
            displayScoreboard(p1.getName(), p2.getName(), p1.getHits(), p2.getHits());
        }
    }
public:
    void getName();
    void play_with_friend();
    void play_with_bot();
};

void HumanPlayer::placeShips(const vector<int>& shipSizes) {
    string orientation;
    for (int size : shipSizes) {
        int x, y;
        bool placed = false;
        while (!placed) {
            createGrid(grid, gridSize, name, true);
            cout << "---" << name << ", enter the starting coordinates for your ship of size " << size << " (row col): ";
            cin >> x >> y;
            cout << "---Enter orientation (h for horizontal, v for vertical): ";
            cin >> orientation;
            bool valid = true;
            if (orientation == "h") {
                if (y + size > gridSize || x < 0 || x >= gridSize || y < 0) valid = false;
                else for (int i = 0; i < size; i++) if (grid[x][y + i] != 'X') valid = false;
            } else if (orientation == "v") {
                if (x + size > gridSize || y < 0 || y >= gridSize || x < 0) valid = false;
                else for (int i = 0; i < size; i++) if (grid[x + i][y] != 'X') valid = false;
            } else valid = false;
            if (!valid) {
                cout << "---Invalid placement. Try again." << endl;
                continue;
            }
            if (orientation == "h") for (int i = 0; i < size; i++) grid[x][y + i] = 'O';
            else for (int i = 0; i < size; i++) grid[x + i][y] = 'O';
            placed = true;
        }
    }
}

bool HumanPlayer::performAttack(GamePlayer& opponent) {
    createGrid(viewGrid, gridSize, name + "'s view of " + opponent.getName(), false);
    int x, y;
    while (true) {
        cout << name << ", enter coordinates to attack " << opponent.getName() << "'s arena (row col): ";
        cin >> x >> y;
        if (x < 0 || y < 0 || x >= gridSize || y >= gridSize) {
            cout << "---Invalid input. Try again." << endl;
        } else if (viewGrid[x][y] != 'X') {
            cout << "---You already attacked this coordinate. Try again." << endl;
        } else {
            break;
        }
    }
    if (opponent.getGrid()[x][y] == 'O') {
        cout << "---It's a HIT!" << endl;
        viewGrid[x][y] = 'H';
        opponent.getGrid()[x][y] = 'H';
        incrementHits();
    } else {
        cout << "---It's a MISS." << endl;
        viewGrid[x][y] = 'M';
        if(opponent.getGrid()[x][y] == 'X') opponent.getGrid()[x][y] = 'M';
    }
    return opponent.isSunk();
}

void BotPlayer::placeShips(const vector<int>& shipSizes) {
    for (int size : shipSizes) {
        bool placed = false;
        while (!placed) {
            int x = rand() % gridSize;
            int y = rand() % gridSize;
            string orientation = (rand() % 2 == 0) ? "h" : "v";
            bool valid = true;
            if (orientation == "h") {
                if (y + size > gridSize) valid = false;
                else for (int i = 0; i < size; i++) if (grid[x][y + i] != 'X') valid = false;
            } else {
                if (x + size > gridSize) valid = false;
                else for (int i = 0; i < size; i++) if (grid[x + i][y] != 'X') valid = false;
            }
            if (valid) {
                if (orientation == "h") for (int i = 0; i < size; i++) grid[x][y + i] = 'O';
                else for (int i = 0; i < size; i++) grid[x + i][y] = 'O';
                placed = true;
            }
        }
    }
}

bool BotPlayer::performAttack(GamePlayer& opponent) {
    int x, y;
    do {
        x = rand() % gridSize;
        y = rand() % gridSize;
    } while (attackTracker[x][y] != 'X');
    cout << name << " attacks coordinates (" << x << ", " << y << ")..." << endl;
    attackTracker[x][y] = 'A';
    if (opponent.getGrid()[x][y] == 'O') {
        cout << "---It's a HIT! The bot hit your ship." << endl;
        opponent.getGrid()[x][y] = 'H';
        incrementHits();
    } else {
        cout << "---It's a MISS. The bot missed." << endl;
        if(opponent.getGrid()[x][y] == 'X') opponent.getGrid()[x][y] = 'M';
    }
    return opponent.isSunk();
}

void Player::getName() {
    cout << "---Enter the name of Player 1: ";
    getline(cin, player1_name);
    cout << "---Enter the name of Player 2: ";
    getline(cin, player2_name);
}

void Player::play_with_friend() {
    int gridChoice;
    cout << "\n---Choose the size of Arena." << endl;
    cout << "---1. 6x6\n---2. 8x8" << endl;
    cout << "---Enter your choice: ";
    cin >> gridChoice;
    cin.ignore();
    int gridSize = (gridChoice == 2) ? 8 : 6;
    vector<int> shipSizes = {2, 3, 4, 5};
    HumanPlayer p1(player1_name, gridSize);
    HumanPlayer p2(player2_name, gridSize);
    cout << "\n---" << p1.getName() << ", it's your turn to place your ships. Make sure " << p2.getName() << " is not looking!" << endl;
    p1.placeShips(shipSizes);
    cout << "\n---" << p2.getName() << ", it's your turn to place your ships. Make sure " << p1.getName() << " is not looking!" << endl;
    p2.placeShips(shipSizes);
    runGame(p1, p2);
}

void Player::play_with_bot() {
    cout << "---Enter your name: ";
    getline(cin, player1_name);
    int gridChoice;
    cout << "\n---Choose the size of Arena." << endl;
    cout << "---1. 6x6\n---2. 8x8" << endl;
    cout << "---Enter your choice: ";
    cin >> gridChoice;
    cin.ignore();
    int gridSize = (gridChoice == 2) ? 8 : 6;
    vector<int> shipSizes = {2, 3, 4, 5};
    HumanPlayer p1(player1_name, gridSize);
    BotPlayer p2("Bot", gridSize);
    cout << "\n---" << p1.getName() << ", it's your turn to place your ships." << endl;
    p1.placeShips(shipSizes);
    cout << "\n---The Bot is placing its ships..." << endl;
    p2.placeShips(shipSizes);
    cout << "---The Bot has placed its ships. The battle begins!" << endl;
    runGame(p1, p2);
}

void createGrid(const vector<vector<char>>& grid, int size, const string& name, bool showShips) {
    cout << "\n---Grid of " << name << " ---" << endl;
    cout << "   ";
    for (int j = 0; j < size; j++) cout << j << " ";
    cout << endl;
    for (int i = 0; i < size; i++) {
        cout << i << "  ";
        for (int j = 0; j < size; j++) {
            if (!showShips && grid[i][j] == 'O') cout << "X ";
            else cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

bool allShipsSunk(const vector<vector<char>>& grid) {
    for (const auto& row : grid)
        for (char cell : row)
            if (cell == 'O') return false;
    return true;
}

void display_rule() {
    cout << "\n---Rules of Battleship Game---" << endl;
    cout << "1. The game is played on a grid where each player places their ships.\n";
    cout << "2. Players take turns to call out coordinates to attack the opponent's ships.\n";
    cout << "3. If a ship occupies the called coordinate, it's a 'hit'; otherwise, it's a 'miss'.\n";
    cout << "4. The objective is to sink all of the opponent's ships.\n";
    cout << "5. The first player to sink all opponent's ships wins the game.\n";
    cout << "6. Ships can be placed either horizontally or vertically.\n";
}

void displayScoreboard(const string& name1, const string& name2, int hits1, int hits2) {
    const int totalShipParts = 14;
    cout << "\n+----------------- SCOREBOARD -----------------+" << endl;
    cout << "| " << name1 << "'s Hits: " << hits1 << endl;
    cout << "| " << name2 << "'s Hits: " << hits2 << endl;
    cout << "|--------------------------------------------|" << endl;
    cout << "| " << name1 << "'s remaining ship parts: " << (totalShipParts - hits2) << endl;
    cout << "| " << name2 << "'s remaining ship parts: " << (totalShipParts - hits1) << endl;
    cout << "+--------------------------------------------+" << endl;
}

int main() {
    srand(time(0));
    cout << "----------------- Welcome to Battleship Game !!! -----------------" << endl;
    cout << "---Press 1 to enter the game: ";
    int enter;
    cin >> enter;
    cin.ignore();
    if (enter == 1) {
        int choice;
        cout << endl << "-----Select an option-----" << endl;
        cout << "1. Play with Friend" << endl;
        cout << "2. Play with Bot" << endl;
        cout << "---Enter your choice: ";
        cin >> choice;
        cin.ignore();
        Player p;
        if (choice == 1) {
            p.getName();
            p.play_with_friend();
        } else if (choice == 2) {
            p.play_with_bot();
        } else {
            cout << "---Invalid choice. Exiting..." << endl;
        }
    } else {
        cout << "---Invalid input. Exiting..." << endl;
    }
    return 0;
}
