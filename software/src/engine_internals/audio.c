/*
 * File:   audio.c
 * Author: eztaah, MatteoPerez, MokhmadGUIRIEV and 21KEBY
 */

#include "../engine.h"

//==============================================================================
// This function stops the buzzer by turning off the PWM output.
//==============================================================================
void _e_stop_buzzer(void)
{
    PR2 = 0;
    CCPR2L = 0;
}

//==============================================================================
// This function initializes the buzzer for audio output by setting up PWM.
// Configures PWM settings and turns the buzzer off initially.
//==============================================================================
void _e_init_buzzer(void)
{
    TRISCbits.TRISC1 = 0;   // RC1 est la sortie PWM, sur laquelle est connect? le buzzer
    T2CON = 0b00000110;     // Postscaler = 1, Timer ON, Prescaler = 16
    CCP2CON = 0b00001100;   // mode PWM, avec 1 seule sortie sur RC1
    
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note A3 (La 3) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_A3(int16_t duration)
{
    PR2 = 180;
    CCPR2L = 90;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note B3 (Si 3) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_B3(int16_t duration)
{
    PR2 = 160;
    CCPR2L = 80;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note C4 (Do 4) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_C4(int16_t duration)
{
    PR2 = 151;
    CCPR2L = 75;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note D4 (R� 4) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_D4(int16_t duration)
{
    PR2 = 135;
    CCPR2L = 67;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note D#4 (R� di�se 4) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_D_4(int16_t duration)
{
    PR2 = 127;
    CCPR2L = 63;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note E4 (Mi 4) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_E4(int16_t duration)
{
    PR2 = 120;
    CCPR2L = 60;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note G4 (Sol 4) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_G4(int16_t duration)
{
    PR2 = 101;
    CCPR2L = 50;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note A#4 (La di�se 4) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_A_4(int16_t duration)
{
    PR2 = 86;
    CCPR2L = 43;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// Play the game start music.
//==============================================================================
void e_play_init_music(void)
{
    e_play_G4(400);
    e_play_G4(400);
    e_play_G4(400);
    e_play_D_4(150);
    e_play_A_4(100);
    e_play_G4(400);
    e_play_D_4(150);
    e_play_A_4(100);
    e_play_G4(400);
}

//==============================================================================
// Play a bip sound
//==============================================================================
void e_play_bip_sound(void)
{
    e_play_A3(50);
}

//==============================================================================
// Play game over music
//==============================================================================
void e_play_game_over_music(void){
    e_play_G4(400);
    e_play_G4(400);
    e_play_D_4(150);
}
