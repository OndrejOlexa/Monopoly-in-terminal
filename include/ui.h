#ifndef Z4_UI_H
#define Z4_UI_H

#include "monopoly.h"

// Initialization and cleanup
void ui_init(void);
void ui_cleanup(void);

// Display functions
void ui_display_board(PLAYER hraci[], int pocet);
void ui_display_players(PLAYER hraci[], int pocet);
void ui_display_turn_info(int turn, int current_player, int dice_roll);
void ui_display_start_screen(int num_players);
void ui_display_move_info(PLAYER *player, int old_pos, int new_pos, int dice);

// Input functions
int ui_prompt_dice_roll(void);

// Message/Status functions
void ui_display_message(const char *message);
void ui_display_winner(int winner_number);
void ui_display_tie(void);

// Refresh function
void ui_refresh_all(PLAYER hraci[], int pocet, int turn, int current_player, int dice_roll);

#endif //Z4_UI_H
