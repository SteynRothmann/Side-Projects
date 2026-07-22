#include <iostream>
#include <conio.h> // For _kbhit() and _getch() to detect keyboard inputs
#include <ctime>
#include <windows.h> // For Sleep() and PlaySound()
#include <mmsystem.h> // For PlaySound function
#include <fstream>
#include <sstream> // Reading from file with character detection
#include <algorithm> // For sort() and min() to display scores in scoreboard
#include <iomanip>

using namespace std;

/**** GLOBAL VARIABLES ****/
// Screen
const int WIDTH = 40;
const int HEIGHT = 20;

// Pipes
const char PIPE = '#';
const char SPACE = ' ';
int pipeX;
int gapY;
const int GAP_SIZE = 5;
const int PIPE_THICKNESS = 2;

// Bird
const char BIRD = '<>';
int birdY;
int birdVelocity;
const int GRAVITY = 1;

// Player data
int score;
string playerName;
int highScore = 0;
const int MAX_SCORES = 100;

// Computer
bool computerCrashed;
const char COMPUTER = 'X';

/**** STRUCTS ****/
struct PlayerScore {
    string name;
    int score;
};

// Function to compare PlayerScore objects (used for sorting)
bool CompareScores(const PlayerScore& a, const PlayerScore& b) {
    return a.score > b.score; // Sort in descending order
}

// Function to display "Scoreboard" text and load it line by line
void ASCIIScoreArt() {
    string scoreAsciiArt[] = {
         "======================================================",
        " _____                    _                         _ ",
        "/  ___|                  | |                       | |",
        "\\ `--.  ___ ___  _ __ ___| |__   ___   __ _ _ __ __| |",
        " `--. \\/ __/ _ \\| '__/ _ \\ '_ \\ / _ \\ / _` | '__/ _` |",
        "/\\__/ / (_| (_) | | |  __/ |_) | (_) | (_| | | | (_| |",
        "\\____/ \\___\\___/|_|  \\___|_.__/ \\___/ \\__,_|_|  \\__,_|",
        "======================================================"
    };

    // Code for loading line by line
    for (const auto& line : scoreAsciiArt) {
        cout << line << endl;
        Sleep(500); // Delay for 100 milliseconds between lines
    }
}

// One of the most important functions
// Used in every "form" to reset variables
void Setup() {
    birdY = HEIGHT / 2; // Reset bird position
    score = 0; // Reset score
    pipeX = WIDTH - 1; // Reset pipe position
    gapY = rand() % (HEIGHT - GAP_SIZE); // Initialize gap position
    birdVelocity = 0; // Reset bird velocity
    computerCrashed = false; // Initially, the computer hasn't crashed
}

// Funtion to give font colour based on integer value
// 10: Green
// 9: Blue
// 7: Gray
// 4: Red
void setColor(int color) {
    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // Set the text color
    SetConsoleTextAttribute(hConsole, color);
}

// Funtion to display player icon types with names
void PlayersText()
{
    // Player icon and name
    cout << playerName << ": ";
    setColor(10);
    cout << "O";
    setColor(7);
    cout << setw(25) << right << "COMP: ";

    // Computer icon and name
    setColor(9);
    cout << "X";
    setColor(7);
    cout << endl; // Add an extra line for spacing
}

// After welcome page, this funtion is called
// This funtion is mainly used for displaying the "game form" one line at a time
// Creating a so-called loading aesthetic
void LoadGameScreen() {
    system("cls"); // Clear console
    PlayersText(); // Display player types

    // Show game screen row by row
    for (int y = 0; y < HEIGHT; y++) { // y axis
        for (int x = 0; x < WIDTH; x++) { // x axis
            if (x == 0) cout << "|"; // Left wall
            if (x >= pipeX && x < pipeX + PIPE_THICKNESS) {

                // Draw pipe segments
                if (y < gapY || y >= gapY + GAP_SIZE) {
                    setColor(4);
                    cout << PIPE; // Draw pipe
                    setColor(7);
                } else {
                    cout << SPACE; // Draw gap
                }

                // Draw bird
            } else if (x == 1 && y == birdY) {
                setColor(10);
                cout << BIRD;
                setColor(7);

                // Draw computer
            } else if (x == 7 && y == birdY){
                setColor(9);
                cout << COMPUTER;
                setColor(7);

                // Draw empty space
            } else {
                cout << SPACE; // Empty space
            }
        }
        cout << "|" << endl; // Right wall
        Sleep(150); // Delay for the loading effect
    }
}

// Function to display the scoreboard
void DisplayScoreboard() {
    PlayerScore scores[MAX_SCORES]; // Array for structure
    ifstream scoreFile("ScoreBoard.txt"); // Textfile with data
    string line;
    int count = 0; // For array

    // Check if the file opened successfully
    if (scoreFile.fail()) {
        cout << "Error opening file!" << endl;
        return;
    }

    // Read scores from file
    while (getline(scoreFile, line) && count < MAX_SCORES) {
        cout << "Reading line: " << line << endl; // Debug line
        cout << std::flush; // Force output to be displayed immediately

        // Variables for data in textfile
        stringstream ss(line);
        string name;
        int score;
        char comma;

        // Checks for " , " in text file, to seperate the name and score
        if (ss >> name >> comma && comma == ',') {
            ss >> score; // Read the score after the comma
            if (ss) {
                scores[count] = {name, score}; // Store in array
                cout << "Parsed: " << name << " - " << score << endl; // Debug line
                count++;
            } else {
                cout << "Failed to parse score for: " << line << endl; // Debug line
            }
        } else {
            cout << "Failed to parse line: " << line << endl; // Debug line
        }
    }

    // Sort the scores in descending order
    sort(scores, scores + count, CompareScores);

    system("cls"); // Clear the console

    // Display top 15 scores
    ASCIIScoreArt();
    cout << left << setw(10) <<
            "Rank" << setw(10) <<
            "Name" << setw(10) <<
            "Score" << endl;

    cout << "======================" << endl;

    for (int i = 0; i < min(15, count); i++) {
        cout << left << setw(10) << (i + 1) // Rank
             << setw(10) << scores[i].name // Name
             << setw(10) << scores[i].score << endl; // Score
    }

    cout << "\nPress 'Esc' to return to the game." << endl;

    // Wait for the user to press 'Esc' to return to the game
    while (true) {
        if (_kbhit()) { // If a keyboard button is pressed
            char current = _getch(); // Get that button
            if (current == 27) { // If escape key
                cout << "Returning to the game..." << endl;
                Sleep(1000); // Short delay for the message
                score = 0; // Reset score
                Setup(); // Reset game state for new session
                LoadGameScreen(); // Start a new game
                return; // Exit function and return to the game
            }
        }
    }
}

// Funtion to display the welcome text with game name
// Also line by line loading aesthetic
void ASCIIWelcomeArt() {
    string asciiArt[] = {
        "======================================================",
        "______ _                            ___           _ _ ",
        "|  ___| |                          / _ \\         (_|_)",
        "| |_  | | __ _ _ __  _ __  _   _  / /_\\ \\___  ___ _ _ ",
        "|  _| | |/ _  | '_ \\| '_ \\| | | | |  _  / __|/ __| | |",
        "| |   | | (_| | |_) | |_) | |_| | | | | \\__ \\ (__| | |",
        "\\_|   |_|\\__,_| .__/| .__/ \\__, | \\_| |_/___/\\___|_|_|",
        "              | |   | |     __/ |                     ",
        "              |_|   |_|    |___/                      ",
        "======================================================"
    };

    // Loading code
    for (const auto& line : asciiArt) {
        cout << line << endl;
        Sleep(100); // Delay for 100 milliseconds between lines
    }
    cout << endl;
}

// One of the main funtions
// This one draws the screen every time the program sleeps
// When the code is executed, it sleeps, and moves to the next position
void Draw() {
    system("cls"); // Clear console

    PlayersText(); // Player types

    for (int y = 0; y < HEIGHT; y++) { // y axis
        for (int x = 0; x < WIDTH; x++) { // x axis
            if (x == 0) cout << "|"; // Left wall

            if (x >= pipeX && x < pipeX + PIPE_THICKNESS) {
                // Draw pipe segments
                if (y < gapY || y >= gapY + GAP_SIZE) {
                    setColor(4);
                    cout << PIPE; // Draw pipe
                    setColor(7);
                } else {
                    cout << SPACE; // Draw gap
                }

                // Draw bird
            } else if (x == 1 && y == birdY) {
                setColor(10);
                cout << BIRD;
                setColor(7);

                // Draw computer
            } else if (x == 7 && !computerCrashed) {
                // Draw computer player on the right side of the wall
                int computerY = gapY + GAP_SIZE / 2; // Position the computer in the middle of the gap
                if (y == computerY) {
                    setColor(9);
                    cout << COMPUTER;
                    setColor(7);

                // Draw space
                } else {
                    cout << SPACE;
                }

            // Draw space
            } else {
                cout << SPACE;
            }
        }
        cout << "|" << endl; // Right wall
    }

    // Display text to help with how to play
    cout << "Press ";
    setColor(10);
    cout << "SPACE";
    setColor(7);
    cout << " to play" << endl;

    // Display live score and highscore
    cout << "Score: " << score << "   High Score: " << highScore << endl;

    // If the computer crashes
    if (computerCrashed) {
        cout << "Computer has crashed!" << endl;
    }
}

// Funtion to save the score to the textfile
void SaveScore() {
    ofstream scoreFile("ScoreBoard.txt", ios::app); // Open the scoreboard file in append mode
    if (scoreFile.is_open()) {
        scoreFile << playerName << " , " << score << endl; // Save player name and high score
        scoreFile.close(); // Close the file
    } else {
        cout << "Unable to open ScoreBoard file." << endl;
    }
}

// Also one of the most important funtions
// Purpose is to move the pipe to the left, update the bird y-position, and check if computer player crashed
// After, update the sleep
void Logic() {

    /**** Pipe logic ****/
    if (pipeX > 0) { // If x-position of pipe is not 0
        // Move the pipe to the left
        pipeX--;
    } else { // If x-position of pipe is 0
        pipeX = WIDTH - 1; // Reset pipe to the right
        gapY = rand() % (HEIGHT - GAP_SIZE); // Randomize the gap position
        PlaySound(TEXT("Sounds/Point.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play point sound
        score++; // Increase score

    /**** Computer player logic ****/
        // Randomly determine if the computer will crash after passing the pipe
        if (!computerCrashed) {
            int randomCrash = rand() % 10; // 1 in 10 chance to crash
            if (randomCrash == 0) {
                computerCrashed = true; // Mark as crashed
            }
        }
    }

    /**** Bird logic ****/
    // Check if bird collides with the pipe
    if (pipeX == 1 && (birdY < gapY || birdY >= gapY + GAP_SIZE)) { // If bird is under or over gap, and gap is at x-position 1
        PlaySound(TEXT("Sounds/Death_Sound.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play crash sound
        cout << "Game Over!" << endl;

        // New highscore
        if (score > highScore) {
            highScore = score; // Update high score if the current score is higher
        }
        SaveScore(); // Save the current score

        // Display options after game over
        cout << "\n'R' to Restart" << endl;
        cout << "'Esc' to return to the main menu" << endl;
        cout << "'Z' to show scoreboard" << endl;

        // Wait for user input to restart or return to the menu
        while (true) {
            if (_kbhit()) { // If keybind is pressed
                char current = _getch(); // Determine type
                // Restart
                if (current == 'r' || current == 'R') {
                    Setup(); // Reset game state for a new game
                    return; // Exit Logic function

                // Exit
                } else if (current == 27) { // Escape key pressed
                    cout << "Returning to the main menu..." << endl;
                    Sleep(1000); // Short delay before returning
                    playerName.clear(); // Clear player name for new session
                    highScore = 0; // Reset high score
                    Setup(); // Reset game state for a new session
                    return; // Exit Logic function to return to the menu

                // Scoreboard
                } else if (current == 'z' || current == 'Z') {
                    DisplayScoreboard(); // Show the scoreboard
                    return; // Exit Logic function after displaying
                }
            }
        }
    }
}

// Imput logic for bird jumping
void Input() {
    if (_kbhit()) { // Check if a key is pressed
        char current = _getch(); // Get keybind
        if (current == ' ') { // If space, jump
            birdVelocity = -2; // Set flap velocity
        }
    }
}

// Welcome funtion at start of program
void DisplayWelcomeMessage() {
    system("cls"); // Clear console
    PlaySound(TEXT("Sounds/Background.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // Play Background sound, looping
    ASCIIWelcomeArt(); // Display ASCII art
    cout << "Enter your name (First 5 letters will be used): ";
    cin >> playerName; // Input player name

    // Validate player name
    if (playerName.length() > 5) {
        playerName = playerName.substr(0, 5); // Cut only fisrt 5 letters of name
    }
    for (char& c : playerName) { // Go through each character in string
        c = toupper(c); // Convert to uppercase
    }

    // Show message before starting game
    string loading = "Loading...";

    // Load message one character at a time
    for (int i = 0; i < 10; i ++)
    {
        cout << loading[i];
        Sleep(200); // Loading screen
    }

    // Stop the background music before loading the game screen
    PlaySound(NULL, 0, 0); // Stop background music
}

int main() {
    srand(time(0)); // Seed for random number generator
    Setup(); // Reset
    DisplayWelcomeMessage(); // Show welcome message before the game starts

    // Start the game loop
    while (true) {
        LoadGameScreen(); // Load game screen with animation
        while (true) { // Game loop
            Input();
            Logic();
            Draw();
            Sleep(70); // Game speed
            birdY += birdVelocity; // Apply bird velocity
            birdVelocity += GRAVITY; // Apply gravity

            // Keep the bird within the screen bounds
            if (birdY >= HEIGHT) {
                birdY = HEIGHT - 1; // Prevent falling off the screen
            }
            if (birdY < 0) {
                birdY = 0; // Prevent flying off the top of the screen
            }

            // Check if the player has chosen to return to the menu
            if (playerName.empty()) {
                break; // Exit game loop if returning to the welcome screen
            }
        }

        // If the player is returning to the welcome screen, show it again
        DisplayWelcomeMessage();
        Setup(); // Reinitialize game state
    }
    return 0;
}
