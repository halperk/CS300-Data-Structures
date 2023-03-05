//
//
// CS300 Homework 1 - Maze
// Huseyin Alper Karadeniz
// huseyinalper (00028029)
// Sabanci University 2022
//
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// The stack structure is adapted from the lecture slides.
template <class type>
class mazeStack {
public:
    mazeStack() {topOfStack = NULL;} // Default constructor for the mazeStack.
    mazeStack(const mazeStack &rhs) {topOfStack = NULL; *this = rhs;} // Copy constructor for the mazeStack.
    ~mazeStack() {makeEmpty();} // Destructor for the mazeStack.

    bool isEmpty() const {return (topOfStack == NULL);} // Checks if the stack is empty, or not.
    bool isFull() const {return false;} // Checks if the stack is full, or not.
    void makeEmpty() {while (!isEmpty()) pop();} // Deletes all the elements in the stack.

    void pop() { // Deletes the most recently inserted element from the stack.
        if(!isEmpty()){
            mazeNode* oldTop = topOfStack;
            topOfStack = topOfStack->next;
            delete oldTop;
        }
    }
    void push(const type &x) { // Inserts x to the top of the stack.
        topOfStack = new mazeNode(x, topOfStack);
    }
    type topAndPop() { // Deletes the most recently inserted element by also returning it.
        type topItem = top();
        pop();
        return topItem;
    }
    const type &top() const { // Returns the most recently inserted element.
		return topOfStack->element;
    }
    
    const mazeStack & operator=(const mazeStack &rhs) { // Copies the mazeStack as deep copy.
        if (this != &rhs)
        {
            makeEmpty();
            if (rhs.isEmpty())
                return *this;

            mazeNode* rptr = rhs.topOfStack;
            mazeNode* ptr  = new mazeNode(rptr->element);
            topOfStack = ptr;

            for (rptr = rptr->next; rptr != NULL; rptr = rptr->next)
                ptr = ptr->next = new mazeNode(rptr->element);
        }
        return *this;
    }
private:
    struct mazeNode {
        type element;
        mazeNode* next;
        mazeNode(const type &theElement, mazeNode* n = NULL) : element(theElement), next(n) { }
    };

    mazeNode* topOfStack; // List itself is the stack.
};

struct cell {
    int x, y; // Coordinates of the cell.
    char l, r, u, d; // Status of the walls around the cell.
    bool visited; // Status of visit for cell's itself.

	cell() {x = 0; y = 0; l = '1'; r = '1'; u = '1'; d = '1'; visited = false;} // Initializations of the values
};

void saveMazes(const vector<vector<vector<cell>>> &allMazeMatrices, int numberOfMazes, int numberOfRows, int numberOfCols) {
    // Saves all the mazes inside of the maze matrix one by one.
    for (int mazeNo = 0; mazeNo < numberOfMazes; mazeNo++) {
        string resultFileName = "maze_" + to_string(mazeNo + 1) + ".txt";
        ofstream resultFile(resultFileName); // Saves a file for each maze.
        resultFile << numberOfRows << " " << numberOfCols << endl;
        for (int rowNo = numberOfRows - 1; rowNo > -1; rowNo--) {
            for (int colNo = 0; colNo < numberOfCols; colNo++) {
                // Saves the coordinates and the wall details of each cell.
                cell targetCell = allMazeMatrices[mazeNo][rowNo][colNo];
                resultFile << "x=" << targetCell.x << " y=" << targetCell.y <<
                             " l=" << targetCell.l << " r=" << targetCell.r <<
                             " u=" << targetCell.u << " d=" << targetCell.d << endl;
            }
        }
        resultFile.close();
    }
}

char neighborPicker(const vector<vector<cell>> &maze, int xIndex, int yIndex) {
    // Picks a neighbor to visit according to the visited cells around a given cell.
    vector<char> possibilities;
    char resultDirection = 'n';
    int numberOfRows = int(maze.size());
    int numberOfCols = int(maze[0].size());
    
    // Adds the all visitable neigbor directions into a char vector.
    if ((xIndex != 0) && (!maze[yIndex][xIndex-1].visited)) {
        possibilities.push_back('l');}
    if ((xIndex != numberOfCols - 1) && (!maze[yIndex][xIndex+1].visited)) {
        possibilities.push_back('r');}
    if ((yIndex != 0) && (!maze[yIndex-1][xIndex].visited)) {
        possibilities.push_back('u');}
    if ((yIndex != numberOfRows - 1) && (!maze[yIndex+1][xIndex].visited)) {
        possibilities.push_back('d');}
    
    // Picks a random neigbor direction and then, returns the direction.
    if (possibilities.size() != 0) {
        int randomNumber = (rand() % possibilities.size());
        resultDirection = possibilities[randomNumber];
    }
    
    return resultDirection; // l for left, r for right, u for up, d for down, n for nowhere.
}

void resetVisits(vector<vector<vector<cell>>> &allMazeMatrices, int numberOfMazes, int numberOfRows, int numberOfCols) {
    // Resets all cells as unvisited to be used especially in the path finder.
    for (int mazeNo = 0; mazeNo < numberOfMazes; mazeNo++) {
        for (int rowNo = numberOfRows - 1; rowNo > -1; rowNo--) {
            for (int colNo = 0; colNo < numberOfCols; colNo++) {
                allMazeMatrices[mazeNo][rowNo][colNo].visited = false;
            }
        }
    }
}

char directionPicker(const vector<vector<cell>> &maze, int xIndex, int yIndex) {
    // Picks a direction to visit from the maze according to the walls.
    vector<char> possibilities;
    char resultDirection = 'n';
    cell currentCell = maze[yIndex][xIndex];
    
    // Adds the all visitable path directions into a char vector.
    if ((currentCell.l == '0') && (!maze[yIndex][xIndex-1].visited)) {
            possibilities.push_back('l');}
    if ((currentCell.r == '0') && (!maze[yIndex][xIndex+1].visited)) {
            possibilities.push_back('r');}
    if ((currentCell.u == '0') && (!maze[yIndex-1][xIndex].visited)) {
            possibilities.push_back('u');}
    if ((currentCell.d == '0') && (!maze[yIndex+1][xIndex].visited)) {
            possibilities.push_back('d');}
    
    // Picks a random path direction and then, returns the direction.
    if (possibilities.size() != 0) {
        int randomNumber = (rand() % possibilities.size());
        resultDirection = possibilities[randomNumber];
    }
    
    return resultDirection; // l for left, r for right, u for up, d for down, n for nowhere.
}

bool isPathFinished(const cell targetCell, int exitX, int exitY) {
    // Checks if the path finding operation is completed, or not.
    return (targetCell.x == exitX && targetCell.y == exitY);
}

void saveMazePath(const mazeStack<cell> &pathStack, int mazeNo, int entryX, int entryY, int exitX, int exitY) {
    // Saves the path found by reversing the order of the stack.
    string resultFileName = "maze_" + to_string(mazeNo) + "_path_" + to_string(entryX) + "_" + to_string(entryY) + "_" + to_string(exitX) + "_" + to_string(exitY) + ".txt";
    ofstream resultFile(resultFileName);
    mazeStack<cell> saverStack, tempStack;
    tempStack = pathStack;
    while(!tempStack.isEmpty()) { // Reverses the stack.
        cell topCell = tempStack.topAndPop();
        saverStack.push(topCell);
    }
    while(!saverStack.isEmpty()) { // Saves the path.
        cell topCell = saverStack.topAndPop();
        resultFile << topCell.x << " " << topCell.y << endl;
    }
    resultFile.close();
}

int main() {
    
    // Gets the inputs of the number of mazes, and the dimensions of them.
    int numberOfMazes = 0, numberOfRows = 0, numberOfCols = 0;
    cout << "Enter the number of mazes: ";
    cin >> numberOfMazes;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> numberOfRows >> numberOfCols;
    
    vector<vector<vector<cell>>> allMazeMatrices; // A cell matrix to store all mazes.
    
    // Initializes the mazes according to the inputs given.
    for (int mazeNo = 0; mazeNo < numberOfMazes; mazeNo++) {
        vector<vector<cell>> mazeMatrix;
        for (int rowNo = numberOfRows - 1; rowNo > -1; rowNo--) {
            vector<cell> rowMatrix;
            for (int colNo = 0; colNo < numberOfCols; colNo++) {
                // Creates an empty cell with coordinates, and then adds to the matrix.
                cell newCell; newCell.x = colNo; newCell.y = rowNo;
                rowMatrix.push_back(newCell); // S
            }
            mazeMatrix.push_back(rowMatrix);
        }
        allMazeMatrices.push_back(mazeMatrix);
    }
    
    // Generates some random mazes according to the inputs given.
    for (int i = 0; i < numberOfMazes; i++) {
        
        mazeStack<cell> mazeStack; // An empty mazeStack to be used to create a random maze.
        
        // Creates and then, adds the first cell (0,0) into the stack.
        int yIndex = numberOfRows - 1; int xIndex = 0;
        cell* currentCell = &allMazeMatrices[i][yIndex][xIndex];
        currentCell->visited = true;
        mazeStack.push(*currentCell);
        
        char nextCellDirection = neighborPicker(allMazeMatrices[i], xIndex, yIndex); // Picks a random neighbor to move.
        
        // Creates all the maze by using the mazeStack.
        bool finito = false; // Shows if the maze is completely created, or not.
        while (!finito) {
            // Knocks down a wall between two cells according to the given direction.
            if (nextCellDirection == 'l') {
                currentCell->l = '0'; // Knocks down the left wall of the cell.
                xIndex = xIndex - 1;
                currentCell = &allMazeMatrices[i][yIndex][xIndex]; // Moves to the left cell.
                currentCell->r = '0'; // Knocks down the right wall of the left cell.
                currentCell->visited = true;
                mazeStack.push(*currentCell); // Adds the cell into the stack.
                nextCellDirection = neighborPicker(allMazeMatrices[i], xIndex, yIndex); // Finds the next direction.
            } else if (nextCellDirection == 'r') {
                currentCell->r = '0'; // Knocks down the right wall of the cell.
                xIndex = xIndex + 1;
                currentCell = &allMazeMatrices[i][yIndex][xIndex]; // Moves to the right cell.
                currentCell->l = '0'; // Knocks down the left wall of the right cell.
                currentCell->visited = true;
                mazeStack.push(*currentCell); // Adds the cell into the stack.
                nextCellDirection = neighborPicker(allMazeMatrices[i], xIndex, yIndex); // Finds the next direction.
            } else if (nextCellDirection == 'd') {
                currentCell->d = '0'; // Knocks down the down wall of the cell.
                yIndex = yIndex + 1;
                currentCell = &allMazeMatrices[i][yIndex][xIndex]; // Moves to the down cell.
                currentCell->u = '0'; // Knocks down the up wall of the down cell.
                currentCell->visited = true;
                mazeStack.push(*currentCell); // Adds the cell into the stack.
                nextCellDirection = neighborPicker(allMazeMatrices[i], xIndex, yIndex); // Finds the next direction.
            } else if (nextCellDirection == 'u') {
                currentCell->u = '0'; // Knocks down the up wall of the cell.
                yIndex = yIndex - 1;
                currentCell = &allMazeMatrices[i][yIndex][xIndex]; // Moves to the up cell.
                currentCell->d = '0'; // Knocks down the down wall of the up cell.
                currentCell->visited = true;
                mazeStack.push(*currentCell); // Adds the cell into the stack.
                nextCellDirection = neighborPicker(allMazeMatrices[i], xIndex, yIndex); // Finds the next direction.
            } else if (nextCellDirection == 'n') { // Goes back since there is nowhere to move in the current cell.
                mazeStack.pop(); // Deletes the last cell in the stack so that goes back to the previous cell.
                if (mazeStack.isEmpty()) {
                    finito = true; // Finishes the loop if the all cells visited so that the stack is empty.
                } else {
                    cell previousCell = mazeStack.top();
                    xIndex = previousCell.x; yIndex = numberOfRows - previousCell.y - 1;
                    currentCell = &allMazeMatrices[i][yIndex][xIndex]; // Moves to the previous cell.
                    nextCellDirection = neighborPicker(allMazeMatrices[i], xIndex, yIndex); // Finds the next direction.
                }
            }
        }
    }
    
    saveMazes(allMazeMatrices, numberOfMazes, numberOfRows, numberOfCols); // Saves all the random mazes generated.
    
    cout << "All mazes are generated." << endl << endl;
    
    resetVisits(allMazeMatrices, numberOfMazes, numberOfRows, numberOfCols); // Resets the visited values of the mazes.
    
    // Gets inputs to find a path between the given coordinates in the chosen random maze.
    int mazeIndex, mazeID;
    cout << "Enter a maze ID between 1 to " << numberOfMazes << " inclusive to find a path: ";
    cin >> mazeID; mazeIndex = mazeID - 1;
    
    int entryX, entryY, exitX, exitY;
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> entryX >> entryY;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> exitX >> exitY;
    
    // Gets the maze chosen by the user to find a path.
    vector<vector<cell>> targetMaze;
    targetMaze = allMazeMatrices[mazeIndex];
    
    mazeStack<cell> finderStack; // An empty stack to find a path inside of the maze.
    
    // Adds the entry cell (x,y) chosen by the user into the stack.
    int xIndex = entryX; int yIndex = numberOfRows - entryY - 1;
    cell* currentCell = &targetMaze[yIndex][xIndex];
    currentCell->visited = true;
    finderStack.push(*currentCell);
    
    char nextCellDirection = directionPicker(targetMaze, xIndex, yIndex);  // Picks a random direction to move.
    
    // Finds a path according to the inputs given.
    bool bingo = false;; // Shows if the path is found, or not.
    while(!bingo) {
        if (nextCellDirection == 'l') { // Moves to the left cell.
            xIndex = xIndex - 1;
            currentCell = &targetMaze[yIndex][xIndex];
            currentCell->visited = true;
            finderStack.push(*currentCell); // Adds the left cell into the stack.
            if (isPathFinished(*currentCell, exitX, exitY)) {bingo = true;} // Checks if the path is found.
            else {nextCellDirection = directionPicker(targetMaze, xIndex, yIndex);} // Finds the next direction.
        } else if (nextCellDirection == 'r') { // Moves to the right cell.
            xIndex = xIndex + 1;
            currentCell = &targetMaze[yIndex][xIndex];
            currentCell->visited = true;
            finderStack.push(*currentCell); // Adds the right cell into the stack.
            if (isPathFinished(*currentCell, exitX, exitY)) {bingo = true;} // Checks if the path is found.
            else {nextCellDirection = directionPicker(targetMaze, xIndex, yIndex);} // Finds the next direction.
        } else if (nextCellDirection == 'd') { // Moves to the down cell.
            yIndex = yIndex + 1;
            currentCell = &targetMaze[yIndex][xIndex];
            currentCell->visited = true;
            finderStack.push(*currentCell); // Adds the down cell into the stack.
            if (isPathFinished(*currentCell, exitX, exitY)) {bingo = true;} // Checks if the path is found.
            else {nextCellDirection = directionPicker(targetMaze, xIndex, yIndex);} // Finds the next direction.
        } else if (nextCellDirection == 'u') { // Moves to the up cell.
            yIndex = yIndex - 1;
            currentCell = &targetMaze[yIndex][xIndex];
            currentCell->visited = true;
            finderStack.push(*currentCell); // Adds the up cell into the stack.
            if (isPathFinished(*currentCell, exitX, exitY)) {bingo = true;} // Checks if the path is found.
            else {nextCellDirection = directionPicker(targetMaze, xIndex, yIndex);} // Finds the next direction.
        } else if (nextCellDirection == 'n') { // Moves back since there is nowhere to go forward.
            finderStack.pop();
            if (!finderStack.isEmpty()) { // Goes back to the previous cell.
                cell previousCell = finderStack.top();
                xIndex = previousCell.x; yIndex = numberOfRows - previousCell.y - 1;
                currentCell = &targetMaze[yIndex][xIndex];
                if (isPathFinished(*currentCell, exitX, exitY)) {bingo = true;} // Checks if the path is found.
                else {nextCellDirection = directionPicker(targetMaze, xIndex, yIndex);} // Finds the next direction.
            }
        }
    }
    
    saveMazePath(finderStack, mazeID, entryX, entryY, exitX, exitY); // Saves the path found for the maze chosen.
    
    return 0;
}
