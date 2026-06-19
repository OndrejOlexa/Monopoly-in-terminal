#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "monopoly.h"

// Color pairs
#define COLOR_BROWN 1
#define COLOR_SKYBLUE 2
#define COLOR_PURPLE 3
#define COLOR_ORANGE 4
#define COLOR_RED 5
#define COLOR_YELLOW 6
#define COLOR_GREEN 7
#define COLOR_BLUE 8
#define COLOR_HIGHLIGHT 9
#define COLOR_BORDER 10

// Window dimensions
#define BOARD_HEIGHT 20
#define BOARD_WIDTH 80
#define INFO_HEIGHT 10
#define INFO_WIDTH 40

// Initialize ncurses and colors
void ui_init(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);  // Hide cursor

    // Check if terminal supports colors
    if (has_colors()) {
        start_color();
        
        // Initialize color pairs (text_color, background_color)
        init_pair(COLOR_BROWN, COLOR_BLACK, COLOR_YELLOW);      // Brown-like
        init_pair(COLOR_SKYBLUE, COLOR_BLACK, COLOR_CYAN);       // Sky blue
        init_pair(COLOR_PURPLE, COLOR_WHITE, COLOR_MAGENTA);     // Purple
        init_pair(COLOR_ORANGE, COLOR_BLACK, COLOR_YELLOW);      // Orange-like
        init_pair(COLOR_RED, COLOR_WHITE, COLOR_RED);            // Red
        init_pair(COLOR_YELLOW, COLOR_BLACK, COLOR_YELLOW);      // Yellow
        init_pair(COLOR_GREEN, COLOR_WHITE, COLOR_GREEN);        // Green
        init_pair(COLOR_BLUE, COLOR_WHITE, COLOR_BLUE);          // Blue
        init_pair(COLOR_HIGHLIGHT, COLOR_WHITE, COLOR_BLACK);    // Highlight
        init_pair(COLOR_BORDER, COLOR_GREEN, COLOR_BLACK);       // Border
    }
}

// Clean up ncurses
void ui_cleanup(void) {
    endwin();
}

// Get ncurses color pair for property color
int get_color_pair(COLOR color) {
    switch (color) {
        case Brown:     return COLOR_BROWN;
        case Skyblue:   return COLOR_SKYBLUE;
        case Purple:    return COLOR_PURPLE;
        case Orange:    return COLOR_ORANGE;
        case Red:       return COLOR_RED;
        case Yellow:    return COLOR_YELLOW;
        case Green:     return COLOR_GREEN;
        case Blue:      return COLOR_BLUE;
        default:        return COLOR_HIGHLIGHT;
    }
}

// Draw a box with title
void draw_box(WINDOW *win, int height, int width, const char *title) {
    box(win, 0, 0);
    if (title) {
        mvwprintw(win, 0, 2, " %s ", title);
    }
    wrefresh(win);
}

// Display the game board
void ui_display_board(PLAYER hraci[], int pocet) {
    clear();
    
    WINDOW *board_win = newwin(BOARD_HEIGHT, BOARD_WIDTH, 0, 0);
    draw_box(board_win, BOARD_HEIGHT, BOARD_WIDTH, "MONOPOLY GAME BOARD");
    
    int line = 2;
    mvwprintw(board_win, line++, 2, "%-4s %-25s %-8s %-15s %s", 
              "Pos", "Property", "Price", "Color", "Owner");
    mvwprintw(board_win, line++, 2, "%-80s", "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    
    for (int i = 0; i < NUM_SPACES && line < BOARD_HEIGHT - 1; i++) {
        SPACE pole = game_board[i];
        char owner_str[20] = "Nobody";
        
        if (pole.type == Property && pole.property != NULL) {
            // Find owner
            for (int j = 0; j < pocet; j++) {
                for (int k = 0; k < hraci[j].num_properties; k++) {
                    if (pole.property == hraci[j].owned_properties[k]) {
                        snprintf(owner_str, sizeof(owner_str), "Player %d", hraci[j].number);
                        break;
                    }
                }
            }
            
            int color_pair = get_color_pair(pole.property->color);
            wattron(board_win, COLOR_PAIR(color_pair));
            mvwprintw(board_win, line, 2, "%-4d %-25s %-8d %-15s %s",
                      i + 1,
                      pole.property->name,
                      pole.property->price,
                      property_colors[pole.property->color],
                      owner_str);
            wattroff(board_win, COLOR_PAIR(color_pair));
        } else {
            mvwprintw(board_win, line, 2, "%-4d %s", i + 1, space_types[pole.type]);
        }
        line++;
    }
    
    wrefresh(board_win);
}

// Display player information
void ui_display_players(PLAYER hraci[], int pocet) {
    WINDOW *info_win = newwin(INFO_HEIGHT * pocet + 2, INFO_WIDTH, BOARD_HEIGHT, 0);
    draw_box(info_win, INFO_HEIGHT * pocet + 2, INFO_WIDTH, "PLAYERS INFO");
    
    for (int i = 0; i < pocet; i++) {
        int start_line = 2 + i * INFO_HEIGHT;
        PLAYER *p = &hraci[i];
        
        mvwprintw(info_win, start_line, 2, "Player %d:", p->number);
        mvwprintw(info_win, start_line + 1, 4, "Position: %d", p->space_number);
        mvwprintw(info_win, start_line + 2, 4, "Cash: %d", p->cash);
        mvwprintw(info_win, start_line + 3, 4, "Jail Pass: %d", p->num_jail_pass);
        mvwprintw(info_win, start_line + 4, 4, "In Jail: %s", p->is_in_jail ? "YES" : "NO");
        mvwprintw(info_win, start_line + 5, 4, "Properties: %d", p->num_properties);
        
        // Display owned properties on next lines
        int prop_line = start_line + 6;
        for (int j = 0; j < p->num_properties && prop_line < start_line + INFO_HEIGHT - 1; j++) {
            int color_pair = get_color_pair(p->owned_properties[j]->color);
            wattron(info_win, COLOR_PAIR(color_pair));
            mvwprintw(info_win, prop_line, 6, "%-16s", p->owned_properties[j]->name);
            wattroff(info_win, COLOR_PAIR(color_pair));
            prop_line++;
        }
    }
    
    wrefresh(info_win);
}

// Display turn information
void ui_display_turn_info(int turn, int current_player, int dice_roll) {
    WINDOW *turn_win = newwin(6, INFO_WIDTH, BOARD_HEIGHT + INFO_HEIGHT * 4, 0);
    draw_box(turn_win, 6, INFO_WIDTH, "TURN INFO");
    
    mvwprintw(turn_win, 2, 2, "Turn: %d", turn);
    mvwprintw(turn_win, 3, 2, "Current Player: P%d", current_player);
    mvwprintw(turn_win, 4, 2, "Last Dice Roll: %d", dice_roll);
    
    wrefresh(turn_win);
}

// Display game status and prompt for dice roll
int ui_prompt_dice_roll(void) {
    WINDOW *prompt_win = newwin(5, 40, 0, BOARD_WIDTH + INFO_WIDTH);
    draw_box(prompt_win, 5, 40, "DICE");
    
    mvwprintw(prompt_win, 2, 2, "Enter dice roll (1-6):");
    mvwprintw(prompt_win, 3, 2, "or 0 to end game:");
    
    wrefresh(prompt_win);
    
    int input;
    echo();
    mvwscanw(prompt_win, 3, 20, "%d", &input);
    noecho();
    
    delwin(prompt_win);
    return input;
}

// Display a message
void ui_display_message(const char *message) {
    WINDOW *msg_win = newwin(5, 50, LINES - 7, (COLS - 50) / 2);
    draw_box(msg_win, 5, 50, "MESSAGE");
    
    mvwprintw(msg_win, 2, 2, "%s", message);
    mvwprintw(msg_win, 3, 2, "Press any key to continue...");
    
    wrefresh(msg_win);
    getch();
    
    delwin(msg_win);
}

// Display winner announcement
void ui_display_winner(int winner_number) {
    WINDOW *winner_win = newwin(7, 50, (LINES - 7) / 2, (COLS - 50) / 2);
    wattron(winner_win, COLOR_PAIR(COLOR_HIGHLIGHT) | A_BOLD);
    draw_box(winner_win, 7, 50, "GAME OVER");
    
    mvwprintw(winner_win, 2, 5, "████████████████████████████████████");
    mvwprintw(winner_win, 3, 8, "PLAYER %d WINS!", winner_number);
    mvwprintw(winner_win, 4, 5, "████████████████████████████████████");
    mvwprintw(winner_win, 5, 2, "Press any key to exit...");
    
    wattroff(winner_win, COLOR_PAIR(COLOR_HIGHLIGHT) | A_BOLD);
    wrefresh(winner_win);
    getch();
    
    delwin(winner_win);
}

// Display a draw/tie
void ui_display_tie(void) {
    WINDOW *tie_win = newwin(7, 50, (LINES - 7) / 2, (COLS - 50) / 2);
    wattron(tie_win, COLOR_PAIR(COLOR_HIGHLIGHT) | A_BOLD);
    draw_box(tie_win, 7, 50, "GAME OVER");
    
    mvwprintw(tie_win, 2, 5, "████████████████████████████████████");
    mvwprintw(tie_win, 3, 12, "IT'S A TIE!");
    mvwprintw(tie_win, 4, 5, "████████████████████████████████████");
    mvwprintw(tie_win, 5, 2, "Press any key to exit...");
    
    wattroff(tie_win, COLOR_PAIR(COLOR_HIGHLIGHT) | A_BOLD);
    wrefresh(tie_win);
    getch();
    
    delwin(tie_win);
}

// Display game initialization screen
void ui_display_start_screen(int num_players) {
    clear();
    WINDOW *start_win = newwin(LINES, COLS, 0, 0);
    draw_box(start_win, LINES, COLS, "MONOPOLY - TERMINAL EDITION");
    
    int center_y = LINES / 2;
    int center_x = COLS / 2;
    
    mvwprintw(start_win, center_y - 3, center_x - 15, "GAME INITIALIZED");
    mvwprintw(start_win, center_y - 1, center_x - 18, "Number of Players: %d", num_players);
    mvwprintw(start_win, center_y + 1, center_x - 20, "Starting cash distributed.");
    mvwprintw(start_win, center_y + 3, center_x - 22, "Press any key to start the game...");
    
    wrefresh(start_win);
    getch();
    
    delwin(start_win);
}

// Display player status on move
void ui_display_move_info(PLAYER *player, int old_pos, int new_pos, int dice) {
    char msg[100];
    snprintf(msg, sizeof(msg), "Player %d rolled %d and moved to position %d", 
             player->number, dice, new_pos);
    ui_display_message(msg);
}

// Refresh all displays
void ui_refresh_all(PLAYER hraci[], int pocet, int turn, int current_player, int dice_roll) {
    clear();
    ui_display_board(hraci, pocet);
    ui_display_players(hraci, pocet);
    ui_display_turn_info(turn, current_player, dice_roll);
    refresh();
}
