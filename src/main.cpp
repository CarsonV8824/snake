#include "raylib.h"
#include <deque>
#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <math.h>

class Player
{
private:
    bool isDown;
    bool isUp;
    bool isLeft;
    bool isRight;

public:
    Player()
    {
        isLeft = true;
        isRight = false;
        isUp = false;
        isDown = false;
    }
    void goUp()
    {
        isUp = true;
        isDown = false;
        isLeft = false;
        isRight = false;
    }
    void goDown()
    {
        isUp = false;
        isDown = true;
        isLeft = false;
        isRight = false;
    }
    void goLeft()
    {
        isUp = false;
        isDown = false;
        isLeft = true;
        isRight = false;
    }
    void goRight()
    {
        isUp = false;
        isDown = false;
        isLeft = false;
        isRight = true;
    }
    bool getIsUp() const
    {
        return isUp;
    }
    bool getIsDown() const
    {
        return isDown;
    }
    bool getIsLeft() const
    {
        return isLeft;
    }
    bool getIsRight() const
    {
        return isRight;
    }
};

Vector2 chooseRandomPos()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 14);

    short XrandomNumber = dist(gen);
    short YrandomNumber = dist(gen);

    float x_center = 13.5f;
    float y_center = 13.5f;

    float xCoords[14];
    float yCoords[14];
    for (short i = 0; i < 15; i++)
    {
        xCoords[i] = x_center + (i * 54);
        yCoords[i] = y_center + (i * 54);
    }

    float choosenX = xCoords[XrandomNumber];
    float choosenY = yCoords[YrandomNumber];
    Vector2 vec;
    vec.x = choosenX;
    vec.y = choosenY;
    return vec;
}

void DrawBoard(unsigned int squareSide=54)
{
    for (short i = 0; i < 15; i++)
    {
        for (short j = 0; j < 15; j++)
        {
            if (i % 2 == 0 && j % 2 == 0)
            {
                DrawRectangle(squareSide * j, squareSide * i, squareSide, squareSide, GREEN);
                continue;
            }
            if (i % 2 == 1 && j % 2 == 1)
            {
                DrawRectangle(squareSide * j, squareSide * i, squareSide, squareSide, GREEN);
                continue;
            }
            if (i % 2 == 0 && j % 2 == 1)
            {
                DrawRectangle(squareSide * j, squareSide * i, squareSide, squareSide, LIME);
                continue;
            }
            if (i % 2 == 1 && j % 2 == 0)
            {
                DrawRectangle(squareSide * j, squareSide * i, squareSide, squareSide, LIME);
                continue;
            }
        }
    }
}

void StartScreen()
{
    while (!WindowShouldClose())
    {
        DrawBoard();
        BeginDrawing();
        ClearBackground(RAYWHITE);

        std::string msg = "Welcome!";
        int msgWidth = MeasureText(msg.c_str(), 60);
        DrawText(msg.c_str(), GetScreenWidth()/2 - msgWidth/2, GetScreenHeight()/3, 60, DARKGRAY);

        EndDrawing();
        if (IsKeyDown(KEY_ENTER))
        {
            break;
        }
    }
}

unsigned short Game()
{
    const unsigned short squareSide = 54;
    unsigned short lenSnake = 1;
    std::deque<Vector2> playerSnake; // the direction of each block will be stored in here and be moved constatly per frame if needed
    Vector2 position;
    position.x = 108 * 6;
    position.y = 108;
    Player player;
    const short SnakeVelocity = 6;

    std::vector<Vector2> fruitList;
    Vector2 firstFruitCoords = chooseRandomPos();
    fruitList.push_back(firstFruitCoords);

    bool gameOver = false;
    unsigned short scoreNum = 0;
    while (!WindowShouldClose())
    {
        DrawBoard();
        playerSnake.push_front(position);

        if (IsKeyDown(KEY_W) && ((int)playerSnake[0].x) % squareSide == 0 && !player.getIsDown())
        {
            player.goUp();
        }
        else if (IsKeyDown(KEY_S) && ((int)playerSnake[0].x) % squareSide == 0 && !player.getIsUp())
        {
            player.goDown();
        }
        else if (IsKeyDown(KEY_A) && ((int)playerSnake[0].y) % squareSide == 0 && !player.getIsRight())
        {
            player.goLeft();
        }
        else if (IsKeyDown(KEY_D) && ((int)playerSnake[0].y) % squareSide == 0 && !player.getIsLeft())
        {
            player.goRight();
        }
        if (player.getIsLeft())
        {
            position.x -= SnakeVelocity;
        }
        if (player.getIsRight())
        {
            position.x += SnakeVelocity;
        }
        if (player.getIsUp())
        {
            position.y -= SnakeVelocity;
        }
        if (player.getIsDown())
        {
            position.y += SnakeVelocity;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        unsigned short playerSnakeIndex = 0;
        for (Vector2 dir : playerSnake)
        {
            DrawRectangle(dir.x, dir.y, squareSide, squareSide, BLUE);
            if (playerSnake[0].x == dir.x && playerSnake[0].y == dir.y && playerSnakeIndex > 0)
            {
                gameOver = true;
            }
            playerSnakeIndex++;
        }

        short fruitIndex = 0;
        for (Vector2 fruit : fruitList)
        {

            DrawRectangle(fruit.x, fruit.y, squareSide / 2, squareSide / 2, RED);
            if (abs(fruit.x - playerSnake[0].x) <= squareSide / 2 && abs(fruit.y - playerSnake[0].y) <= squareSide / 2)
            {
                std::cout << "hit\n";
                lenSnake += squareSide / SnakeVelocity;
                Vector2 FruitCoords = chooseRandomPos();
                fruitList[fruitIndex] = FruitCoords;
                if (fruitList.size() <= 5)
                {
                    Vector2 newFruit = chooseRandomPos();
                    fruitList.push_back(newFruit);
                }
                continue;
            }

            fruitIndex++;
        }
        if (playerSnake[0].x < 0 || playerSnake[0].x + (squareSide - 1) > GetScreenWidth() || playerSnake[0].y < 0 || playerSnake[0].y + (squareSide - 1) > GetScreenHeight())
        {
            break;
        }

        scoreNum = (lenSnake - 1) / (squareSide / SnakeVelocity);
        std::string text = "Score: ";
        text += std::to_string(scoreNum);
        DrawText(text.c_str(), 0, 0, 24, WHITE);

        // has to be last or will get error

        if (playerSnake.size() >= lenSnake)
        {
            playerSnake.pop_back();
        }
        if (gameOver)
        {
            break;
        }

        EndDrawing();
    }
    return scoreNum;
}

bool ResultScreen(unsigned int score)
{
    while (!WindowShouldClose())
    {
        DrawBoard();
        BeginDrawing();
        ClearBackground(RAYWHITE);

        std::string msg = "Game Over!";
        int msgWidth = MeasureText(msg.c_str(), 60);
        DrawText(msg.c_str(), GetScreenWidth()/2 - msgWidth/2, GetScreenHeight()/3, 60, DARKGRAY);

        std::string text = "Score: " + std::to_string(score);
        int scoreWidth = MeasureText(text.c_str(), 40);
        DrawText(text.c_str(), GetScreenWidth()/2 - scoreWidth/2, GetScreenHeight()/2, 40, DARKGRAY);
        EndDrawing();
        if (IsKeyDown(KEY_ENTER))
        {
            return true;
        }
    }
    return false;
}

int main()
{
    const unsigned short screenWidth = 810;
    const unsigned short screenHeight = 810;

    InitWindow(screenWidth, screenHeight, "Snake");
    unsigned short score = 0;
    SetTargetFPS(60);
    StartScreen();
    score = Game();
    bool replay = ResultScreen(score);
    CloseWindow();
    if (replay)
    {
        main();
    }

    return 0;
}