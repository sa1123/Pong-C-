#include <iostream>
#include <raylib.h>

using namespace std;

int player_score = 0;
int cpu_score = 0;

class Ball{
    public:
    float x, y;
    int speed_x, speed_y;
    int radius;
    Color ball_color = ORANGE;

    void Draw(){
        DrawCircle(x, y, radius, ball_color);
    }

    void ResetBall(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;
        speed_x *= 1;
        // int speed_choices[2] = {-1, 1};
    }

    void Update(){
        x += speed_x;
        y += speed_y;

        if(y+radius >= GetScreenHeight() || y - radius <= 0){
            speed_y *= -1;
        }
        if(x+radius >= GetScreenWidth()){
            cpu_score++;
            ResetBall();
        }
        if(x - radius <= 0){
            player_score++;
            ResetBall();
        }
    }
};

class Paddle{
    public:
    float x, y;
    float width, height;
    int speed;

    void Draw(){
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update(){
        // cout<<"Y = "<<y<<"\n";
        if(IsKeyDown(KEY_UP) && y>0){
            y -= speed;
        }
        if(IsKeyDown(KEY_DOWN) && y + height < GetScreenHeight()){
            y += speed;
        }
    }
};

class CpuPaddle: public Paddle{

    // protected:

    // void LimitMovement(){

    // }

    public:

    void Update(int ball_y, int ball_x, int speed_x){
        if(ball_x < GetScreenWidth()/2 && speed_x < 0){
            if(y + height/2 > ball_y && y>0){
                y -= speed;
            }else{
                y += speed;
            }
        }
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main () {

    cout<<"Start"<<endl;

    const int screen_height = 800;
    const int screen_width = 1280;

    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(60);

    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.radius = 20;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 6;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height/2 - player.height/2;
    cpu.speed = 3; 

    while(WindowShouldClose() == false){
        BeginDrawing();

        //Updates
        ball.Update();
        player.Update();
        cpu.Update(ball.y, ball.x, ball.speed_x);

        // if(cpu.height < player.height){
        //     cpu.height = player_score*10 + 25;
        // }

        if(cpu.speed < 10){
            cpu.speed = player_score + 1;
        }

        //Collision detection
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})){
            ball.speed_x *= -1;
        }

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            ball.speed_x *= -1;
        }

        //Drawing
        ClearBackground(BLACK);
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", cpu_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3*screen_width/4 - 20, 20, 80, WHITE);

        EndDrawing();

        // cout<<"Player Score: "<<player_score<<" CPU Score: "<<cpu_score<<"\n";
    }

    CloseWindow();

    return 0;
}