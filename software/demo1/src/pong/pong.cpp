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
            lost_team = 0;
            std::cout << "Red Lost!\n";
        }
        else if ((ball.x + ball_velocity_x) > (bounds.x + bounds.width))
        {
            restart();
            lost_count = 5;
            lost_team = 1;
            std::cout << "Green Lost!\n";
        }
        else
        {
            if ((ball.x + ball_velocity_x >= paddle_l.x) && (ball.x + ball_velocity_x <= paddle_l.x + paddle_l.width))
            {
                if ((ball.y + ball_velocity_y >= paddle_l.y - ball.height*0.8) && (ball.y + ball_velocity_y <= paddle_l.y + paddle_l.height + ball.height*0.8))
                {
                    ball_velocity_x *= -1;
                }
            }   
            if ((ball.x + ball_velocity_x >= paddle_r.x) && (ball.x + ball_velocity_x <= paddle_r.x + paddle_r.width))
            {
                if ((ball.y + ball_velocity_y >= paddle_r.y - ball.height*0.8) && (ball.y + ball_velocity_y <= paddle_r.y + paddle_r.height + ball.height*0.8))
                {
                    ball_velocity_x *= -1;
                }
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

void Pong::updatePaddle(int y, unsigned char type)
{
    if (type == 0)
        this->paddle_l = cv::Rect(paddle_l.x, y, paddle_l.width, paddle_l.height);
    if (type == 1)
        this->paddle_r = cv::Rect(paddle_r.x, y, paddle_r.width, paddle_r.height);
}

void Pong::restart()
{
    this->ball     = cv::Rect(start_ball.x, start_ball.y, start_ball.width, start_ball.height);
    this->paddle_l = cv::Rect(start_paddle_l.x, start_paddle_l.y, start_paddle_l.width, start_paddle_l.height);
    this->paddle_r = cv::Rect(start_paddle_r.x, start_paddle_r.y, start_paddle_r.width, start_paddle_r.height);
    this->ball_velocity_x = start_ball_velocity_x;
    this->ball_velocity_y = start_ball_velocity_y;
}
