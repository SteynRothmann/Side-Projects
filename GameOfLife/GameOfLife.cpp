#include <iostream>
#include <ctime>
#include <chrono>
#include <windows.h>

using namespace std;

int** make2DArray(const int cols, const int rows)
{
    int** arr = new int*[cols];
    for (int i = 0; i < cols; ++i)
    {
        arr[i] = new int[rows];
    }
    return arr;
}

int getValue(int DoA)
{
    return DoA == 1 ? 1 : 0;
}

void DrawStart(const int Cols, const int Rows, int** gridArray)
{
    srand(time(NULL));
    for (int i = 0; i < Cols; i++)
    {
        for (int j = 0; j < Rows; j++)
        {
            int DoA = rand() % 5; // 1 in 20 chance of being 'alive'
            gridArray[i][j] = getValue(DoA);
        }
    }
}

int countNeighbors(int x, int y, int** grid, int Cols, int Rows)
{
    int sum = 0;
    for (int i  = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int neighborX = (x + i + Cols) % Cols;
            int neighborY = (y + j + Rows) % Rows;

            sum += grid[neighborX][neighborY];

            // Debugging output to see what cells are being checked
            /*cout << "Checking (" << neighborX << ", " << neighborY << ") with value: "
                 << grid[neighborX][neighborY] << endl;*/
        }
    }
    sum -= grid[x][y]; // Remove the center cell from the sum
    return sum;
}


void DrawNextGen(const int Cols, const int Rows, int** genUpdate, int** grid)
{
    for (int i = 0; i < Cols; i++)
    {
        for (int j = 0; j < Rows; j++)
        {
            int State = grid[i][j];
            if (i == 0 || i == Cols - 1 || j == 0 || j == Rows - 1)
            {
                // Keep the edges unchanged
                genUpdate[i][j] = State;
            }
            else
            {
                // Check neighbors for the middle cells
                int neighbors = countNeighbors(i, j, grid, Cols, Rows);

                // Apply the rules of the Game of Life
                if (State == 0 && neighbors == 3)
                    genUpdate[i][j] = 1; // Birth of a new cell
                else if (State == 1 && (neighbors < 2 || neighbors > 3))
                    genUpdate[i][j] = 0; // Cell dies due to under/over-population
                else
                    genUpdate[i][j] = State; // Cell stays the same
            }
        }
    }
}

void PrintGrid(const int Cols, const int Rows, int** grid)
{
    for (int i = 0; i < Cols; i++)
    {
        for (int j = 0; j < Rows; j++)
        {
            cout << (grid[i][j] == 1 ? "$" : " ");
        }
        cout << endl;
    }
}

int main()
{
    const int COLS = 25;
    const int ROWS = 50;

    // Create the current grid and the next generation grid
    int** grid = make2DArray(COLS, ROWS);
    int** nextGen = make2DArray(COLS, ROWS);

    DrawStart(COLS, ROWS, grid);

    while (true) // Infinite loop to keep the simulation going
    {
        // Print the current grid
        PrintGrid(COLS, ROWS, grid);

        // Calculate the next generation
        DrawNextGen(COLS, ROWS, nextGen, grid);

        // Copy the contents of nextGen into grid
        for (int i = 0; i < COLS; i++)
        {
            for (int j = 0; j < ROWS; j++)
            {
                grid[i][j] = nextGen[i][j];
            }
        }

        // Add a delay between generations (e.g., 500 milliseconds)
        Sleep(100);
        // Clear the screen (system-specific, this works on Windows, for Unix systems use "clear")
        system("cls");


    }

    // Free dynamically allocated memory
    for (int i = 0; i < COLS; ++i)
    {
        delete[] grid[i];
        delete[] nextGen[i];
    }
    delete[] grid;
    delete[] nextGen;

    return 0;
}
