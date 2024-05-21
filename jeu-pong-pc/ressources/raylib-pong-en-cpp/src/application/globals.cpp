#include "globals.hpp"


////////////////////// UTILITY //////////////////////////////////
float ASPECT_RATIO{1366.0f / 768.0f};
int WINDOW_WIDTH;
int WINDOW_HEIGHT;
int GAME_WIDTH;
int GAME_HEIGHT;
int MARGIN_X;
int MARGIN_Y;


float GetReelValue(const float value)
{
    float factor = static_cast<float>(GAME_WIDTH) / 1366.0f;
    return value * factor;
}