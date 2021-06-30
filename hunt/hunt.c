/* hunt.c
 * -----
 * Author : Geejbear
 * Created : May 29, 2021
 * Revision: T. Foster
 * Description : game
 *
 * TODO LIST
 * X keep the player from going off screen
 * - try somthing simple like place an item(s) the player collects
 * - use play() from play module
 * - write ascii.h with enum for 1-31 ascii glyph
 * - write a function clamp
 * TODO: simplify: one gem -> collect it
 * TODO: random location
 * TODO: try 3 gems
 * - use clamp()
 * TODO: item_t struct
 */

#include "play.h"
#include "utility.h"
#include "ascii.h"

#include <dos.h>
#include <conio.h>

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


// =============================================================================
// DEFINES

#define SCREEN_W    20
#define SCREEN_H    20

// note: minimum x and y is 1 in turbo C
#define MAX_X       SCREEN_W
#define MAX_Y       SCREEN_H

#define MIN_X           1
#define MIN_Y           1
#define MAX_NUM_ITEMS   3
// =============================================================================
// STRUCTS / TYPES / ENUMS


typedef struct
{
    int x;
    int y;
    
    // TODO: appearance
    int ch;
    int color;
    
    // TODO: entity's state
    bool active;
} entity_t;

// TODO: function that makes a new entity_t
// TODO: draw entity function

// =============================================================================
// DATA (state) global variables are initialized to 0 by default

entity_t player;

entity_t flat;
entity_t sharp;
entity_t natural;

entity_t entities[3];
bool item_collected = false; // this gets replaced with struct member (active)



// =============================================================================
// FUNCTIONS
void print_objective()
{
    gotoxy(MIN_X, MIN_Y);
    textcolor(YELLOW);
    cprintf("G%c", ASCII_UPARROW);
}

void sound_collect()
{
    const int tempo = 220; 
    const int note_value = 16; 
    const int octave = 4; 
    
    for (int i = 0; i < 2; i++) {
        play(NOTE_G, octave, note_value, tempo);
        play(NOTE_G_SHARP, octave, note_value, tempo);
    }

}

void draw_level()
{
    print_objective();
    
    gotoxy(player.x, player.y);
    textcolor(LIGHTGRAY);
    putch(ASCII_FACE1);

    if (!item_collected) { //could this be dryer like an array?
        
        gotoxy(flat.x, flat.y);
        textcolor(MAGENTA);
        putch('b');
        gotoxy(sharp.x, sharp.y);
        textcolor(RED);
        putch('#');
        gotoxy(natural.x, natural.y);
        textcolor(CYAN);
        putch(ASCII_SECTION);
    }     
}


void collect_gem()
{
    if ( !item_collected && sharp.x == player.x && sharp.y == player.y ) {  // player is on gem!
        // do collect gem stuff
        item_collected = true;
        sound_collect();
        gotoxy(player.x, player.y);
        textcolor(LIGHTGRAY);
        putch(ASCII_FACE1);
    } 
    
}

// returns a random coordinate between 1 and max_value
int random_coord(int max_value)
{
    return (rand() % max_value) + 1;
}

void draw_random_entities()
{
    // TODO: DRY
    player.x = (rand() % SCREEN_W) + 1;
    player.y = (rand() % SCREEN_H) + 1;
    
    // this is the new version
    for (int i = 0; i < 3; i++)
    {
        entities[i].x = random_coord(SCREEN_W);
        entities[i].y = random_coord(SCREEN_H);
    }
    
    // (old version, remove)
    flat.x = (rand() % SCREEN_W) + 1;
    flat.y = (rand() % SCREEN_H) + 1;
    
    sharp.x = (rand() % SCREEN_W) + 1;
    sharp.y = (rand() % SCREEN_H) + 1;
    
    natural.x = (rand() % SCREEN_W) + 1;
    natural.y = (rand() % SCREEN_H) + 1;
}

void key_hit() // this function: every time a hit is hit
{
    
    clrscr();
    int key = getch();
    
    switch (key) {
        case 'w': // TODO: optimize logic for wasd (clamp, from utility.h)
            --player.y;
            if (player.y < MIN_Y) {
                player.y = MIN_Y;
            }    
            break;
        case 's':
            ++player.y;
            if (player.y > MAX_Y) {
                player.y = MAX_Y;
            }    
            break;
        case 'a':
            --player.x;
            if (player.x < MIN_X) {
                player.x = MIN_X;
            }    
            break;
        case 'd':
            ++player.x;
            if (player.x > MAX_X) {
                player.x = MAX_X;
            }    
            break;
        default:
            break;
    }
    // the player has moved (potentially)
   
    draw_level(); // TODO: this is good, keep it
    collect_gem();
    
    if (item_collected && sharp.x != player.x && sharp.y != player.y) {
        item_collected = false;
        clrscr();
        draw_random_entities();
        draw_level();
    } 
    
}

int main()
{
    textmode(C40);
    setcursor(0);
    setscreensize(SCREEN_W, SCREEN_H);
    initdos();
    
    srand((unsigned)time(NULL));
    
    // TODO: set entities' appearance
    
    draw_random_entities();
    draw_level();
    
    // game loop structure:
    // 1. get and handle inputs (keyboard, mouse)
    // 2. update the game (logic stuff)
    // 3. redraw
    while (1) {
        
        if (kbhit()) {
            key_hit();
        }
        
        refresh();
    }    
    
    return 0;
}
