#ifndef MM_H
#define MM_H

#include <string>
#include <vector>
#include <array>
#include <map>

class MasterMind
{
public:
    MasterMind();
    ~MasterMind();

    void win();
    void play_game();
    void mm_game();
    void set_passcode();
    void get_guess_codes();
    void get_guess_flags();
    void print_mastermind_board();
    
private:
    bool win_flag;
    bool fail_flag;
    bool hard_flag;

    unsigned int chances;
    const unsigned int columns;

    int guess_buffer[4];
    unsigned int turn;

    std::vector<std::string> colours{"RED", "GREEN", "YELLOW", "BLUE", "BLACK", "ORANGE"};
    const std::map<int, std::string> colour_map {{1, "RED"},
                                                 {2, "GREEN"},
                                                 {3, "YELLOW"},
                                                 {4, "BLUE"},
                                                 {5, "BLACK"},
                                                 {6, "ORANGE"}};
    
    std::vector<std::string> show_passcode{"UNK", "UNK", "UNK", "UNK"};
    std::vector<std::string> passcode;

    std::vector<std::string> vectr{"-", "-", "-", "-"};
    std::vector<std::vector<std::string>> guess_codes;
    std::vector<std::vector<std::string>> guess_flags;

    std::vector<std::string> tokens;
    std::string user_data;
    
   
};

#endif