#ifndef __PONG_H_INCLUDED__
#define __PONG_H_INCLUDED__

#include <opencv2/core.hpp>
#include "../global.hpp" 

class Pong
{
private:
    cv::Rect start_ball   = cv::Rect(15, 10, 10, 10);
    cv::Rect start_paddle_l = cv::Rect(5, 10, 2, 40);
    cv::Rect start_paddle_r = cv::Rect(MAX_COL - 5 - 2, 10, 2, 40);
    cv::Rect ball   = cv::Rect(start_ball.x, start_ball.y, start_ball.width, start_ball.height);
    cv::Rect paddle_l = cv::Rect(start_paddle_l.x, start_paddle_l.y, start_paddle_l.width, start_paddle_l.height);
    cv::Rect paddle_r = cv::Rect(start_paddle_r.x, start_paddle_r.y, start_paddle_r.width, start_paddle_r.height);
    cv::Rect bounds = cv::Rect(0, 0, MAX_COL, MAX_ROW);
    int start_ball_velocity_x = BALL_VELOCITY;
    int start_ball_velocity_y = BALL_VELOCITY;
    int ball_velocity_x = start_ball_velocity_x;
    int ball_velocity_y = start_ball_velocity_y;
    bool running = false;
    int lost_count = 0;     // keeping track of how long "lost" text stays on screen
    int lost_team = 0;      // keeping track of how long "lost" text stays on screen

public:
    Pong();
    void update();
    void updatePaddle(int, unsigned char);
    void restart();
    void start() { this->running = true; }
    void stop() { this->running = false; }
    cv::Rect getBall() { return this->ball; }
    cv::Rect getPaddle(unsigned char type) { return (type == 0) ? this->paddle_l : this->paddle_r; }
    bool isRunning() { return this->running; }
    bool isLost() { return (this->lost_count > 0); }
    int getLostTeam() { return (this->lost_team); }
};

#endif
