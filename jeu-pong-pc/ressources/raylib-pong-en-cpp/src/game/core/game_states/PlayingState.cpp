#include "PlayingState.hpp"
#include "globals.hpp"
#include <string>
#include <random>

////////////// UTILS /////////////////
int GetRandomNumber(int min, int max)
{
    // static is used to instantiate the random engine and distribution once only.
    // It then generates a new random number on subsequent calls.
    static std::random_device rd;
    static std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(min, max);

    return distr(eng);
}



PlayingState::PlayingState(Game* game)    // WHen the game changes state
    : _game(game),
      _ball(BLACK),
      _ghostBall(RED),
      _paddle1(30.0f),
      _paddle2(GAME_WIDTH - 30.0f),
      _ballsArray()
{
    // Set initial position for ball and ghost ball
    _ball.SetPosition({(GAME_WIDTH / 2.0f) - (_ball.GetSize() / 2.0f), (GAME_HEIGHT / 2.0f) - (_ball.GetSize() / 2.0f)});
    _ghostBall.SetPosition({(GAME_WIDTH / 2.0f) - (_ghostBall.GetSize() / 2.0f), (GAME_HEIGHT / 2.0f) - (_ghostBall.GetSize() / 2.0f)});
 
    // Setup the array
    _ballsArray.push_back(&_ball);
    _ballsArray.push_back(&_ghostBall);
}


void PlayingState::OnEnter()
{
    _ball.Reset();
    _ghostBall.Reset();
    _ghostBall.Activate();
    _ghostBall.SetSpeed({_ball.GetSpeed().x * 10.0f, _ball.GetSpeed().y * 10.0f});
    _paddle1.Reset(30.0f);
    _paddle2.Reset(GAME_WIDTH - 30.0f);
    _game->ResetScore();

    // Activate the ball
    _ball.Activate();
    _ghostBall.Activate();
    _ghostBall.SetSpeed({_ball.GetSpeed().x * 10.0f, _ball.GetSpeed().y * 10.0f});
}

void PlayingState::OnExit()
{

}

void PlayingState::Update()
{
    // Update paddle1 position
    _paddle1.Update();

    // Update paddle2 position
    _paddle2.Update(_botDefensePosition);

    // Update balls position
    for(Ball* elt : _ballsArray)
        elt->Update();

    _HandleCollisions();
}

void PlayingState::Render()
{
    // === Draw entities ===
    _paddle1.Render();
    _paddle2.Render();
    _ball.Render();
    //_ghostBall.Render();

    // === Draw score ===
    std::string scoreStr = std::to_string(_game->GetScore());   // Convert score to a string
    DrawText(scoreStr.c_str(), GetReelValue(670.0f), GetReelValue(20.0f), GetReelValue(40.0f), BLACK);     // Display the score
}




void PlayingState::_HandleCollisions()
{
    // === Balls-Vertical-Border collisions ===
    for(Ball* elt : _ballsArray)
    {
        if(elt->GetRectangle().y < 0.0f)
            elt->HandleBounceTop();
        else if(elt->GetRectangle().y > GAME_HEIGHT - elt->GetRectangle().height) 
            elt->HandleBounceBottom();
    }


    // === Ball-Horizontal-Border collisions ===
    if(_ball.GetRectangle().x <= 0.0f)
    {
        _game->SetGameOverState();
    }


    // === Ball-Paddle1 collisions ===
    if (_ball.GetSpeed().x < 0.0f)     // Here the ball will go into the paddle1
    {
        if (CheckCollisionRecs(_ball.GetRectangle(), _paddle1.GetRectangle()))
        {
            //float mismatch{_ball.GetRectangle().y - _paddle1.GetRectangle().y - (_paddle1.GetRectangle().height / 2)};
            _ball.HandleBounceLeft();

            _game->IncreaseScore();

            // Ghost ball
            _ghostBall.SetPosition(_ball.GetPosition());
            _ghostBall.Activate();
            _ghostBall.SetSpeed({_ball.GetSpeed().x * 10, _ball.GetSpeed().y * 10});
        };
    }

    // === Ball-Paddle2 collisions ===
    if(_ball.GetRectangle().x >= GAME_WIDTH - 30.0f - _ball.GetRectangle().width)
    {
        //mismatch = _ball.GetRectangle().y - _paddle2.GetRectangle().y - (_paddle2.GetRectangle().height / 2);
        _ball.HandleBounceRight();    // Prevent bot from losing
        
        _ghostBall.SetPosition({GAME_WIDTH - 35.0f, (GAME_HEIGHT / 2.0f) - (_ghostBall.GetSize() / 2.0f)});
    }


    // === Ghost-ball right wall collision
    if(_ghostBall.GetPosition().x >= GAME_WIDTH - _ghostBall.GetSize() - 30.0f)
    {
        _ghostBall.Desactivate();
        _ghostBall.SetPosition({_ghostBall.GetPosition().x - 5.0f, _ghostBall.GetPosition().y});
        const float ghostBallPositionY = _ghostBall.GetPosition().y + (_ghostBall.GetSize() / 2);
        float randomMismatch = GetRandomNumber(0, _paddle2.GetRectangle().height);
        _botDefensePosition = ghostBallPositionY - randomMismatch;
    }
}