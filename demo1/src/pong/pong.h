#ifndef __PONG_H_INCLUDED__
#define __PONG_H_INCLUDED__

#include <opencv2/core.hpp>

class Pong
{
private:
    cv::Rect start_ball   = cv::Rect(15, 10, 10, 10);
    cv::Rect start_paddle = cv::Rect(5, 10, 2, 40);
    cv::Rect ball   = cv::Rect(start_ball.x, start_ball.y, start_ball.width, start_ball.height);
    cv::Rect paddle = cv::Rect(start_paddle.x, start_paddle.y, start_paddle.width, start_paddle.height);
    cv::Rect bounds = cv::Rect(0, 0, 320, 240);
    int start_ball_velocity_x = 10;
    int start_ball_velocity_y = 10;
    int ball_velocity_x = start_ball_velocity_x;
    int ball_velocity_y = start_ball_velocity_y;
    bool running = false;
    int lost_count = 0;     // keeping track of how long "lost" text stays on screen

public:
    Pong();
    void update();
    void updatePaddle(int);
    void restart();
    void start() { this->running = true; }
    void stop() { this->running = false; }
    cv::Rect getBall() { return this->ball; }
    cv::Rect getPaddle() { return this->paddle; }
    bool isRunning() { return this->running; }
    bool isLost() { return (this->lost_count > 0); }
};

#endif
