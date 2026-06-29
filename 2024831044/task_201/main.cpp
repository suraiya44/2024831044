#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define CELL_SIZE 20

#define GRID_WIDTH (SCREEN_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / CELL_SIZE)

#define MAX_SNAKE_LENGTH 500


// Snake Structure

typedef struct
{
    int x[MAX_SNAKE_LENGTH];
    int y[MAX_SNAKE_LENGTH];

    int length;

    int dx;
    int dy;

} Snake;


// Global Variables

Snake snake;

int foodX;
int foodY;

int score = 0;

bool gameOver = false;
bool running = true;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


// Function Declarations

void initializeSnake();
void generateFood();
void handleInput();
void moveSnake();
void checkCollision();
void renderGame();
void restartGame();
void cleanup();


// Initialize Snake

void initializeSnake()
{
    snake.length = 3;

    snake.x[0] = 10;
    snake.y[0] = 10;

    snake.x[1] = 9;
    snake.y[1] = 10;

    snake.x[2] = 8;
    snake.y[2] = 10;

    snake.dx = 1;
    // Moving it right initially
    snake.dy = 0;
}


// Generate Food

void generateFood()
{
    bool validPosition = false;

    while(validPosition== false)
    {
        validPosition = true;

        foodX = rand() % GRID_WIDTH;
        foodY = rand() % GRID_HEIGHT;

        // Prevent food spawning on snake
        for(int i = 0; i < snake.length; i++)
        {
            if(foodX == snake.x[i] &&
               foodY == snake.y[i])
            {
                validPosition = false;
                break;
            }
        }
    }
}


// Handle Input

void handleInput()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            running = false;
        }

        if(event.type == SDL_KEYDOWN)
        // checks if a key was pressed 
        {
            SDL_Keycode key = event.key.keysym.sym;

            // Prevent direct reverse movement

            if(key == SDLK_UP && snake.dy == 0)
            {
                snake.dx = 0;
                snake.dy = -1;
            }

            else if(key == SDLK_DOWN && snake.dy == 0)
            {
                snake.dx = 0;
                snake.dy = 1;
            }

            else if(key == SDLK_LEFT && snake.dx == 0)
            {
                snake.dx = -1;
                snake.dy = 0;
            }

            else if(key == SDLK_RIGHT && snake.dx == 0)
            {
                snake.dx = 1;
                snake.dy = 0;
            }

            // Restart game
            if(gameOver && key == SDLK_r)
            // game over and then we press SDLK_r
            {
                restartGame();
            }

            // Quit game
            if(key == SDLK_ESCAPE)
            {
                running = false;
            }

        }
    }
}


// Move Snake

void moveSnake()
{
    // Move body backward
    for(int i = snake.length - 1; i > 0; i--)
    {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }

    // Move head
    // moving right initially

    snake.x[0] += snake.dx;
    snake.y[0] += snake.dy;

    // Food collision
    if(snake.x[0] == foodX &&
       snake.y[0] == foodY)
    {
        if(snake.length < MAX_SNAKE_LENGTH)
        {
            snake.length++;
        }

        score += 10;

        printf("Score: %d\n", score);

        generateFood();
    }
}


// Check Collision

void checkCollision()
{
    // Wall collision
    if(snake.x[0] < 0 ||
       snake.x[0] >= GRID_WIDTH ||
       snake.y[0] < 0 ||
       snake.y[0] >= GRID_HEIGHT)
    {
        gameOver = true;

        printf("\nGAME OVER!\n");
        printf("Final Score: %d\n", score);
        printf("Press R to Restart\n");
    }

    // Self collision
    for(int i = 1; i < snake.length; i++)
    {
        if(snake.x[0] == snake.x[i] &&
           snake.y[0] == snake.y[i])
          // x[0] y[0] (HEAD)== x[ith grid] y[ith grid]
        {
            gameOver = true;

            printf("\nGAME OVER!\n");
            printf("Final Score: %d\n", score);
            printf("Press R to Restart\n");

            break;
        }
    }
}


// Render Game

void renderGame()
{
    // creates a Black background before every new frame
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw snake
    for(int i = 0; i < snake.length; i++)
    {
        SDL_Rect segment;
        segment.x = snake.x[i] * CELL_SIZE;
        segment.y = snake.y[i] * CELL_SIZE;
        segment.w = CELL_SIZE;
        segment.h = CELL_SIZE;
        // creates a rectangle called segment

        if(i == 0)
        {
            // Snake head color
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }

        else
        {
             // Snake body color
            SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255);
        }

        SDL_RenderFillRect(renderer, &segment);
        // fills the segment rectangle with current color

    }

    // Draw food

    SDL_Rect food;
    food.x = foodX * CELL_SIZE;
    food.y = foodY * CELL_SIZE;
    food.w = CELL_SIZE;
    food.h = CELL_SIZE;
    // creates a rectangle called food

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_RenderFillRect(renderer, &food);
    // fills the food rectangle with current color

    // presents the newest frame on the black background
    SDL_RenderPresent(renderer);
}


// Restart Game

void restartGame()
{
    score = 0;
    gameOver = false;

    initializeSnake();
    generateFood();

    printf("\nGame Restarted!\n");
}


// Cleanup

void cleanup()
{
    if(renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }

    if(window != NULL)
    {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}


// Main Function

int main(int argc, char* argv[])
{
    srand(time(NULL));

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize!\n");
        return 1;
    }

    // Create window
    window = SDL_CreateWindow(
        "Snake Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if(window == NULL)
    {
        printf("Window could not be created!\n");
        return 1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if(renderer == NULL)
    {
        printf("Renderer could not be created!\n");
        return 1;
    }

    initializeSnake();

    generateFood();

    printf("Snake Game Started!\n");
    printf("Use Arrow Keys to Move\n");
    printf("Press ESC to Quit\n");

    // Main Game Loop
    while(running)
    {
        // handles input
        handleInput();

        if(!gameOver)
        {
        // moving the snake
            moveSnake();

            checkCollision();
        }

        renderGame();

        SDL_Delay(200);
    }

    cleanup();

    return 0;
}