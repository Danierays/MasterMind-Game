#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <vector>
#include <random>
#include <stdio.h>

#include "mm.h"
using namespace std;

//Constructor 
MasterMind::MasterMind()
    : turn(0), columns(4), chances(6), guess_buffer{0}, win_flag(false), fail_flag(false), 
      hard_flag(false), guess_codes(chances, vectr), guess_flags(chances, vectr)
{
    passcode.reserve(4);
}

//Destructor
MasterMind::~MasterMind()
{
    //empty destructor body
}

void MasterMind::set_passcode()
{   
    // Shuffle the colours
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(std::begin(colours), std::end(colours), rng);

    //Set the passcode
    for (int i = 0; i < columns; i++)
    {
        passcode.push_back(colours[i]);
    }
}

void MasterMind::get_guess_codes()
{
    // string intermediate to save tokens
    string intermediate;
INPUT:
    cout << "Enter your (4) choice:";
    getline(cin, user_data);
    cout << endl;

    // istringstream class check
    istringstream check(user_data);
     
    // Tokenizing usercode with whitespaces
    while(check >> intermediate)
    {
        tokens.push_back(intermediate);
    }

    //Tokenizing usercode without whitespaces
    if (tokens.size() == 1)
    {
        tokens.clear();
        std::vector<char> vec(user_data.begin(), user_data.end());
        
        for (char &ch: vec) {
            tokens.push_back(string(1, ch));
        } 
    }

    //Checking that the usercode is valid
    if (tokens.size() != 4)
    {
        cout << "Invalid input. Try again\n" << endl;
        tokens.clear();
        intermediate.clear();
        user_data.clear();
        goto INPUT;
    }
    
    //Storing the usercode in the buffer
    for(int i = 0; i < tokens.size(); i++)
    {
        guess_buffer[i] = stoi(tokens[i]);
    }

    //Is the stored usercode valid
    for (int i = 0; i < columns; i++)
    {
        if (guess_buffer[i] < 1 || guess_buffer[i] > 6)
        {
            cout << "Invalid input. Try again using the following format\n"
                 << "1 2 3 4 (w/ input: 1 - 6)" << endl;
            tokens.clear();
            user_data.clear();
            intermediate.clear();
            goto INPUT;
        }
        // Convert the user code to corresponding colours and store in guess_codes
        else
        {
            for (auto &itr : colour_map)
            {
                if (itr.first == guess_buffer[i])
                {
                    guess_codes[turn][i] = itr.second;
                }
            }
        }
    }
    //clear tokens
    tokens.clear();
    user_data.clear();
}

void MasterMind::win()
{   
    win_flag = true;
    print_mastermind_board();
    cout << "Congratulations!! YOU WIN!!!" << endl;
}

void MasterMind::get_guess_flags()
{
    std::vector<int> dummy_passcode;

    // Get corresponding integer value of passcode colours
    for (int i = 0; i < passcode.size(); i++)
    {
        for ( auto it = colour_map.begin(); it != colour_map.end(); ++it)
        {
            if (passcode.at(i) == it->second )
            {
                dummy_passcode.push_back(it->first); 
                break;
            }
        }
    }

    // Vector interator for guess_flags
    std::vector<int>::iterator it;
    unsigned int pos = 0;

    // Setting guess flags with respect to user code
    for (int index = 0; index < dummy_passcode.size(); index++)
    {
        it = find(dummy_passcode.begin(), dummy_passcode.end(), guess_buffer[index]);
        
        if (it != dummy_passcode.end())
        {
            if (index == (it - dummy_passcode.begin()))
            {
                guess_flags[turn][pos] = "R";
            }
            else
                guess_flags[turn][pos] = "W";
            pos += 1;
        }
        else 
        {
            guess_flags[turn][pos] = "-";
            pos += 1;
        }
    }
    
    if(hard_flag == true)
    {
        std::random_device rd;
        std::default_random_engine rng(rd());
        std::shuffle(std::begin(guess_flags[turn]), std::end(guess_flags[turn]), rng);

        if(turn == 1)
        {
            // time-based seed
            unsigned seed = 0;
            int arr[] = { 0, 1, 2, 3 };
            int n = sizeof(arr) / sizeof(arr[0]);
            // Shuffling our array
            
            shuffle(arr, arr + n,default_random_engine(seed));
            for (int i = 0; i < (passcode.size() - 2); i++)
                show_passcode.at(arr[i])= passcode.at(arr[i]);
        }

    }

    // Checking for Win condition 
    if (guess_codes[turn] == passcode)
    {
        win();
    }
    else
    {
        // Update the turn
        turn += 1;
        print_mastermind_board();
    }
}

void MasterMind::print_mastermind_board()
{
    cout << "\n-----------------------------------------\n"
         << "\t      MASTERMIND\n"
         << "-----------------------------------------\n"
         << "    |";
       
    for (int i = 0; i < show_passcode.size(); i++)
        {
            cout << "\t" << show_passcode[i].substr(0, 3) << " ";
        }
        cout << endl;

    for (int i = (chances - 1); i >= 0; i--)
    {
        cout << "-----------------------------------------\n"
             << guess_flags[i][0].substr(0, 3) << " " << guess_flags[i][1].substr(0, 3) << " |\n"

             << guess_flags[i][2].substr(0, 3) << " " << guess_flags[i][3].substr(0, 3) << " |";

        for (int j = 0; j < guess_codes[i].size(); j++)
        {
            cout << "\t" + guess_codes[i][j].substr(0, 3) << " ";
        }
        
        cout << endl;
        if (i == 0)
        {
            cout << "-----------------------------------------\n"
                 << "1- RED | 2- GRE | 3- YEL | 4-BLU | 5- BLA | 6- ORA\n"
                 << endl;
        }
    }
}

void MasterMind::mm_game()
{
    set_passcode();
    print_mastermind_board();
    /* The GAME LOOP*/
    while (turn < chances)
    {
        // Accepting user/player's input code
        get_guess_codes();

        // Apply clues according to user's input code
        get_guess_flags();

        if (win_flag == true)
            break;

        if (turn == chances)
        {
            fail_flag = true;
            show_passcode.clear();
            
            for (int i = 0; i < passcode.size(); i++)
                show_passcode.push_back(passcode[i]);

            cout << "\nYOU LOSE :(((((( Try again next time!!!\n"
                 << endl;
            print_mastermind_board();
            break;
        }
    }
}

void MasterMind::play_game()
{
MAINMENU:
    /* The GAME MENU */
    cout << "\n-----------------------------------------\n"
         << "\t WELCOME TO MASTERMIND\n"
         << "-----------------------------------------\n"
         << "\t 1 - PLAY\n"
         << "\t 2 - GAME INFO\n"
         << "\t 3 - EXIT\n"
         << "-----------------------------------------\n"
         << endl;
    int x;
MENU:
    cin >> x;
    // Making sure that the newlines after cin are ignored
    cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

    switch (x)
    {
        case 1:
            cout << "\n-----------------------------------------\n"
                 << "  PLAY: SELECT DIFFICULTY LEVEL\n"
                 << "-----------------------------------------\n"
                 << "\t 1 - EASY\n"
                 << "\t 2 - MEDIUM\n"
                 << "\t 3 - HARD\n"
                 << "\t 4 - RETURN\n"
                 << "-----------------------------------------\n"
                 << "Difficulty Levels are specified as follows: \n"
                 << "EASY   : 5 Tries with no shuffle in clues\n"
                 << "MEDIUM : 3 Tries with no shuffle in clues\n"
                 << "HARD   : 4 Tries with shuffle in clues & 2 passcodes revealed after 2nd try\n"
                 << "-----------------------------------------\n"
                 << endl;
        int input;
    SUBMENU:
            cin >> input;
            cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

            switch (input)
            {
                case 1:
                    chances = 5;
                    break;
                case 2:
                    chances = 3;
                    break;
                case 3:
                    chances = 4;
                    hard_flag = true;
                    break;
                case 4: 
                    goto MAINMENU;
                default:
                    cout << "Invalid Input. Try again!" << endl;
                    goto SUBMENU;
            }
            break;
        
        case 2:
            cout << "\nMastermind is a code-breaking game. \n" 
                 << "The computer generates a secret code consisting of a combination of four(4) colors.\n" 
                 << "The user has to guess this passcode using clues provided.\n"
                 << "These clues are given on the left side of the screen as follows: \n"
                 << "R : the chosen colour is in the right position as in the secret code\n"
                 << "W : the chosen colour is present in the secret code, but its position in the user code is incorrect.\n"
                 << "- : the chosen colour is not present in the secret code\n"
                 << endl;
            cout << "\n-----------------------------------------\n"
                 << "Enter code using numbers.\n"
                 << "1 - RED       2 - GREEN\n" 
                 << "3 - YELLOW    4 - BLUE\n" 
                 << "5 - BLACK     6 - ORANGE\n\n"
                 << "Fx: RED YELLOW ORANGE BLACK ---> 1 3 6 5 | 1365\n"
                 << "-----------------------------------------\n"
                 << "1 - EXIT\n"
                 << "-----------------------------------------\n"
                 << endl;
            int infomenu;
        INFOMENU:
            cin >> infomenu;
            cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
                switch (infomenu)
                {
                case 1:
                    goto MAINMENU;
                default:
                    cout << "Invalid Input. Try again!" << endl;
                    goto INFOMENU;
                }
                
        case 3: 
            exit(0);
            break;
        default:
            cout << "Invalid input. Try again!!" << endl;
            goto MENU;
    }
    
    // Run the game loop
    mm_game();
    
    if (win_flag == true || fail_flag == true)
    {
        win_flag = false;
        fail_flag = false;
        hard_flag = false;
        cout << "Would you like to replay? (y/n)"
             << endl;
        char x;
    REPLAY:
        cin >> x;
        cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

        switch (x)
        {
        case 'y': case 'Y': 
            std::fill(guess_codes.begin(), guess_codes.end(), vectr);
            std::fill(guess_flags.begin(), guess_flags.end(), vectr);
            std::fill(show_passcode.begin(), show_passcode.end(), "UNK");
            passcode.clear();
            turn = 0;
            goto MAINMENU;
        
        case 'n': case 'N':
            exit(0);
        default:
            cout <<"Invalid input. Try again" << endl;
            goto REPLAY;
        }
    }
}

int main()
{
    MasterMind mmgame;
    mmgame.play_game();

    return 0;
}