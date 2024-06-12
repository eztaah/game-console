/*
 * File:   main.c
 * Author:  
 */

#include "engine.h"

#define BEST_SCORE_ADDR 0x0001 // Exemple d'adresse, � ajuster selon votre application


//void write_flash(uint16_t address, uint8_t value){
//    INTCONbits.GIE = 0; // D�sactiver les interruptions
//    TBLPTRU = 0x00;  // Partie sup�rieure de l'adresse (pour une adresse de 16 bits, c'est 0)
//    TBLPTRH = (uint8_t)((address >> 8) & 0xFF);   // Partie haute de l'adresse
//    TBLPTRL = (uint8_t)(address & 0xFF);          // Partie basse de l'adresse
//    TABLAT = value;     // Valeur � �crire
//    asm("TBLWT*+");
//    EECON1bits.EEPGD = 1;    // Acc�s � la m�moire flash
//    EECON1bits.CFGS = 0;     // Acc�s � la m�moire de programme
//    EECON1bits.WREN = 1;     // Activation de l'�criture en m�moire flash
//    EECON2 = 0x55;
//    EECON2 = 0x0AA;
//    EECON1bits.WR = 1;       // D�clenche l'�criture
//    while (EECON1bits.WR){};
//    EECON1bits.WREN = 0;    // D�sactivation de l'�criture
//    INTCONbits.GIE = 1;     // R�activer les interruptions
//}
//
//int16_t read_flash(uint16_t address){
//    TBLPTRU = 0x00;  // Partie sup�rieure de l'adresse (pour une adresse de 16 bits, c'est 0)
//    TBLPTRH = (uint8_t)((address >> 8) & 0xFF);   // Partie haute de l'adresse
//    TBLPTRL = (uint8_t)(address & 0xFF);          // Partie basse de l'adresse
//    asm("TBLRD*+");     // Instruction d'assemblage pour lire la m�moire flash
//    return TABLAT;      // Retourne la valeur lue
//}

int16_t read_EEPROM(uint8_t address){
//    asm(
//        "MOVLW 0x06;"
//        "MOVWF EEADR;"         // Data Memory Address to read
//        "BCF EECON1, EEPGD;"   // Point to DATA memory
//        "BCF EECON1, CFGS;"    // Access EEPROM
//        "BSF EECON1, RD;"      // EEPROM Read
//        "MOVF EEDATA, W;"      // W = EEDATA
//    );    
    uint8_t data;
    EEADR = address;       // Adresse de m�moire � lire
    EECON1bits.EEPGD = 0;  // Pointez sur la m�moire DATA
    EECON1bits.CFGS = 0;   // Acc�der � l'EEPROM
    EECON1bits.RD = 1;     // Lecture de l'EEPROM
    data = EEDATA;         // Lire la valeur dans le registre EEDATA
    return data;
}

void write_EEPROM(){
//    asm(
//        "MOVLW 0x01;"
//        "MOVWF EEADR;"   // Data Memory Address to write
//        "MOVLW 0x03;"
//        "MOVWF EEDATA;"      // Data Memory Value to write
//        "BCF EECON1, EEPGD;" // Point to DATA memory
//        "BCF EECON1, CFGS;"  // Access EEPROM
//        "BSF EECON1, WREN;"  // Enable writes
//        "BCF INTCON, GIE;"   // Disable Interrupts
//        "MOVLW 55h;"
//        "MOVWF EECON2;"  // Write 55h
//        "MOVLW 0AAh;"
//        "MOVWF EECON2;"  // Write 0AAh
//        "BSF EECON1, WR;"    // Set WR bit to begin write
//        "BSF INTCON, GIE;"   // Enable Interrupts
//        "BCF EECON1, WREN;"  // Disable writes on write complete (EEIF set)
//    );
    EEADR = 0x01;
    EEDATA = 0x03;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    while (EECON1bits.WR);
    INTCONbits.GIE = 1;
    EECON1bits.WREN =0;
}

void main(void) {
    e_init_game_console();
    e_set_target_fps(20);
    
    // init font
    e_set_font(Courier_New_Bold_20);
    
    e_fill_screen(BLACK);
    
    int16_t cursor_position = 0;
    int16_t last_button_pressed = -1;
    
    int16_t score = 5;
    
    while (1) {
        if (e_is_button_pressed(BUTTON_UP)) {
            if (cursor_position == 0) {
                cursor_position = 2;
            } else {
                cursor_position--;
            }
            last_button_pressed = BUTTON_UP;
        }

        if (e_is_button_pressed(BUTTON_DOWN)) {
            if (cursor_position == 2) {
                cursor_position = 0;
            } else {
                cursor_position++;
            }
            last_button_pressed = BUTTON_DOWN;
        }


        if (e_is_button_pressed(BUTTON_A)) {
            switch (cursor_position) {
                case 0:
                    write_EEPROM();
                    score = read_EEPROM(0x01);
                    break;
                case 1:
                    score = read_EEPROM(0x01);
                    break;
                case 2:
                    score = read_EEPROM(0x02);
                    break;
            }
        }

        // DRAW
        e_draw_const_text("Choose a game : ", 50, 20, WHITE, BLACK);
        
        char score_text[20];
        sprintf(score_text, "Score: %d", score);
        e_draw_const_text(score_text, 110, 45, WHITE, BLACK);
        
        if (cursor_position == 0) {
           e_draw_const_text("pong", 110, 70, BLACK, WHITE);
           e_draw_const_text("tetris", 110, 100, WHITE, BLACK);
           e_draw_const_text("snake", 110, 130, WHITE, BLACK);
        }

        else if (cursor_position == 1) {
           e_draw_const_text("pong", 110, 70, WHITE, BLACK);
           e_draw_const_text("tetris", 110, 100, BLACK, WHITE);
           e_draw_const_text("snake", 110, 130, WHITE, BLACK);
        }

        else if (cursor_position == 2) {
           e_draw_const_text("pong", 110, 70, WHITE, BLACK);
           e_draw_const_text("tetris", 110, 100, WHITE, BLACK);
           e_draw_const_text("snake", 110, 130, BLACK, WHITE);
        }    
         
        if (last_button_pressed == BUTTON_DOWN) {
            while(e_is_button_pressed(BUTTON_DOWN)) {}
            last_button_pressed = -1;
        }
        else if (last_button_pressed == BUTTON_UP) {
            while(e_is_button_pressed(BUTTON_UP)) {}
            last_button_pressed = -1;
        }
    }  
}
