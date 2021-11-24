//Sovial Sonzeu
//Program 4: Twisty Passages
#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <ctime>

//if want to change to . instead of space, must have 2 spaces after it EX: "123" <-> ".12"
#define EDGES_AND_ROOMS  "   "
using namespace std;

//void function to print maze
void printMaze(vector<vector<string>> &maze, int mazeSize) {

    int mazeDimensions = mazeSize * 2 - 1;
    //     top border of maze
    for (int i = 0; i < mazeSize * 2 + 1; i++) {
        cout << "X  ";
    }
    cout << endl;

    //first prints each different Y index at the row and then shifts to next row
    for (int x = 0; x < mazeDimensions; x++) {
        cout << "X  ";
        for (int y = 0; y < mazeDimensions; y++) {
            cout << maze[x][y];
        }
        cout << "X" << endl;
    }

    // bottom border of maze
    for (int i = 0; i < mazeSize * 2 + 1; i++) {
        cout << "X  ";
    }

    cout << endl << endl;
}


vector<vector<string>> genRooms(int mazeSize) {
    cout << "Rooms Only" << endl;
    int mazeDimensions = mazeSize * 2 - 1;
    vector<vector<string>> maze(mazeDimensions, vector<string>(mazeDimensions));

    // top border of maze; want to exclude from actual maze vector
    for (int i = 0; i < mazeSize * 2 + 1; i++) {
        cout << "X  ";
    }
    cout << endl;

    //2 for loops - one acts as the rows, the other is columns

    // pre-prints  border of maze and changes values of vector with second for loop
    for (int y = 0; y < mazeDimensions; y++) {
        cout << "X  ";
        for (int x = 0; x < mazeDimensions; x++) {
            //rooms are always at an even index number for both x & y
            if (x % 2 == 0 && y % 2 == 0) {
                cout << EDGES_AND_ROOMS;
                maze[x][y] = EDGES_AND_ROOMS;

                //prints row of X's and stores it in maze if either x or y index value is an odd number
            } else {
                cout << "X  ";
                maze[x][y] = "X  ";
            }
        }

        cout << "X" << endl;
    }

    //bottom border of maze; want to exclude from maze vector
    for (int i = 0; i < mazeSize * 2 + 1; i++) {
        cout << "X  ";
    }

    cout << "\n----------------------------- \n";
    return maze;

}

//finds next room
pair<int, int> room(pair<int, int> currentRoom, vector<vector<string>> &maze, set<pair<int, int>> &roomsV,
                    stack<pair<int, int>> &roomS, const int mazeD) {
    pair<int, int> newRoom;
    int compX = currentRoom.first; //compares original X
    int compY = currentRoom.second; //compares original Y
    vector<pair<int, int>> tempVector = {}; //stores any possible candidates for next position
    int randNum;

    //checks if there is a room to the right and if it is unvisited
    if ((compX + 2) < mazeD) {
        if (!(roomsV.find({compX + 2, compY}) != roomsV.end())) {
            tempVector.emplace_back(compX + 2, compY); //inserts into temporary vector (IDE recommended emplace_back instead of push_back)
        }
    }
    //checks if there is a room to the right and if it is unvisited
    if ((compX - 2) >= 0) { //don't think i need the second part for if condition
        if (!(roomsV.find({compX - 2, compY}) != roomsV.end())){
            tempVector.emplace_back(compX - 2, compY);//inserts into temporary vector (IDE recommended emplace_back instead of push_back)
        }
    }
    //checks if there is a room above and if it is unvisited
    if ((compY + 2)  < mazeD) {
        if (!(roomsV.find({compX,compY + 2}) != roomsV.end())){
            tempVector.emplace_back(compX, compY + 2); //inserts into temporary vector (IDE recommended emplace_back instead of push_back)
        }
    }
    //checks if there is a room above and if it is unvisited
    if ((compY - 2) >= 0) {//don't think i need the second part for if condition
        if (!(roomsV.find({compX, compY - 2}) != roomsV.end())) {
            tempVector.emplace_back(compX, compY - 2); //inserts into temporary vector (IDE recommended emplace_back instead of push_back)
        }
    }
//checking which indices are not being observed correctly; FOR DEBUGGING
//    for (auto & itr : tempVector){
//        cout << itr.first << " " << itr.second << endl;
//    }

    //if there are possible candidates for next position
    if (!(tempVector.empty())) {
        randNum = rand() % tempVector.size();
        auto itr = tempVector.begin();
        advance(itr, randNum);
        newRoom = {itr->first, itr->second};
        roomS.push(make_pair(itr->first, itr->second));
        roomsV.insert(make_pair(itr->first, itr->second));

        //if original X index number is different, means it has gone either left or right, therefore make an edge between the original and new
        if (compX != newRoom.first) {
            if (compX > newRoom.first)
                maze[newRoom.first + 1][newRoom.second] = EDGES_AND_ROOMS;
            else
                maze[newRoom.first - 1][newRoom.second] = EDGES_AND_ROOMS;
        }

        //if original Y index number is different, means it has gone either above or below, therefore make an edge between the original and new
        if (compY != newRoom.second) {
            if (compY > newRoom.second)
                maze[newRoom.first][newRoom.second + 1] = EDGES_AND_ROOMS;
            else
                maze[newRoom.first][newRoom.second - 1] = EDGES_AND_ROOMS;
        }
        //printing for visual debugging; FOR DEBUGGING
        //printMaze(maze, 3);
        return newRoom;
    }

    //if tempVector is empty; no candidates available, go back to previous index position and check if there are possible valid candidates. Uses recursion
    else {
        roomS.pop();
        //makes currentRoom the most recent (top) room from stack
        newRoom = {roomS.top().first, roomS.top().second};
        return room(newRoom, maze, roomsV, roomS, mazeD);
    }
}

void genCompleteMaze(vector<vector<string>> &maze, int mazeSize) {
    cout << "Complete Maze" << endl;
    int mazeDimensions = mazeSize * 2 - 1; //size of maze
    int numOfRooms = mazeSize * mazeSize; //will always be size^2

    stack<pair<int, int>> roomStack = {};
    set<pair<int, int>> roomsVisited = {};

    //starting coordinates
    int x = mazeSize * 2 - 2;
    int y = 0;


    //initial position is bottom left (x =
    pair<int, int> currentRoom = {x, y};
    roomStack.push(currentRoom);
    roomsVisited.insert(currentRoom);

    srand(time(NULL));

    //only needs to be iterated for however many rooms there are - 1
    for (int i = 1; i < numOfRooms; i++) {
        currentRoom = room(currentRoom, maze, roomsVisited, roomStack, mazeDimensions);
    }

    printMaze(maze, mazeSize);
}


int main() {
    int mazeSize = 10;
    vector<vector<string>> maze = genRooms(mazeSize);
    genCompleteMaze(maze, mazeSize);
//        printMaze(maze, mazeSize);
}
