#include <iostream>

#include "../global.h"
#include "pong.h"

Pong::Pong()
{
}

void Pong::update()
{
    if (this->running && this->lost_count == 0)
    {
        if ((ball.x + ball_velocity_x) < (bounds.x))
        {
            restart();
            lost_count = 5;
            std::cout << "You Lost!\n";
        }
        else
        {
            if ((ball.x + ball_velocity_x >= paddle.x) && (ball.x + ball_velocity_x <= paddle.x + paddle.width))
            {
                if ((ball.y + ball_velocity_y >= paddle.y - ball.height*0.8) && (ball.y + ball_velocity_y <= paddle.y + paddle.height + ball.height*0.8))
                {
                    ball_velocity_x *= -1;
                }
            }   
            if ((ball.x + ball_velocity_x) > (bounds.x + bounds.width - ball.width))
            {
                ball_velocity_x *= -1;
            }
            if ((ball.y + ball_velocity_y) > (bounds.y + bounds.height - ball.height) || (ball.y + ball_velocity_y) < (bounds.y))
            {
                ball_velocity_y *= -1;
            }
            this->ball = cv::Rect(ball.x + ball_velocity_x, ball.y + ball_velocity_y, ball.width, ball.height);
        }
    }
    if (lost_count > 0)
        lost_count--;
}

void Pong::updatePaddle(int y)
{
    this->paddle = cv::Rect(paddle.x, y, paddle.width, paddle.height);
}

void Pong::restart()
{
    this->ball   = cv::Rect(start_ball.x, start_ball.y, start_ball.width, start_ball.height);
    this->paddle = cv::Rect(start_paddle.x, start_paddle.y, start_paddle.width, start_paddle.height);
    this->ball_velocity_x = start_ball_velocity_x;
    this->ball_velocity_y = start_ball_velocity_y;
}
