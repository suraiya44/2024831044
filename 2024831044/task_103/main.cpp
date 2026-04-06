#include <SDL2/SDL.h>
#include <stdio.h>
#include<math.h>

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 520

// Global variables
bool gameIsRunning = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Uint32 startTime;
Uint32 currentTime;

int radius = 20;
int radius2= 40;

int  centerX2 = SCREEN_WIDTH/2;
int  centerY2 = 0;
int speed= 5;

double centerX = 0;
double* centerX_ptr= &centerX;
int centerY= SCREEN_HEIGHT/2;

bool initializeWindow(void)
{
    // Initialize SDL with video support
    // Automatically initializes the Event Handling, File I/O and Threading subsystems
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }

    // Create an SDL window
    window = SDL_CreateWindow(
        "SDL Introduction",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);

    if (!window)
    {
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }

    // Create an SDL renderer for rendering graphics in the window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer)
    {
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }
    return true;
}

void process_input(void)
{
    // Poll SDL events (e.g., window close)
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            gameIsRunning = false; // Exit the game loop
            break;
         case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_UP:
                    if(centerY2== 0)
                        {
                            centerY2= 0;
                            break;
                        }
                    else
                    {
                        centerY2 -= 5;
                    } 
                    break;

                case SDLK_DOWN:
                    if(centerY2== SCREEN_HEIGHT)
                        {
                            centerY2= SCREEN_HEIGHT;
                            break;
                        }
                    else
                    {
                        centerY2 += 5;
                    }
                    break;

                case SDLK_LEFT:
                     if(centerX2== 0)
                        {
                            centerX2= 0;
                            break;
                        }
                        else
                        {
                            centerX2 -= 5;
                        }
                    break;

                case SDLK_RIGHT:
                    
                     if(centerX2== SCREEN_WIDTH)
                        {
                            centerX2= SCREEN_WIDTH;
                            break;
                        }
                        else
                        {
                            centerX2 += 5;
                        }
                    break;
            }
            break;

        default:
            break;
        }
       
    }
}

double setCircleCoords(double *centerX_ptr)
{
    *centerX_ptr = *centerX_ptr + 0.1;
    if (*centerX_ptr > SCREEN_WIDTH)
    {
        *centerX_ptr = 0;
    }
    return *centerX_ptr;
}


bool checkCollision(int centerX2, double* centerX_ptr)
{
    int distance_sqr= ((centerX2- *centerX_ptr)*(centerX2- *centerX_ptr)+ (centerY2- centerY)*(centerY2- centerY) );
    if(distance_sqr <= (radius+ radius2)*(radius+ radius2))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void drawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius)
{
    for (int w = -radius; w <= radius; w++)
    {
        for (int h = -radius; h <= radius; h++)
        {
            if (w*w + h*h <= radius*radius)
            {
                SDL_RenderDrawPoint(renderer, centerX + w, centerY + h);
            }
        }
    }
}


void draw()
{
    // Set the render draw color (R, G, B, A)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Clear the renderer with the specified draw color
    SDL_RenderClear(renderer);

    bool isColliding= checkCollision(centerX2, centerX_ptr);
    if(isColliding == 1)
    {
        //drawing circle no.1
       SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    centerX= setCircleCoords(&centerX); 
    drawCircle(renderer, centerX, SCREEN_HEIGHT/2, radius);
     
    // drawing circle no.2
     SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    drawCircle(renderer, centerX2, centerY2, radius2);

    }
    else
    {
        // drawing circle no.1
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        centerX= setCircleCoords(&centerX); 
        drawCircle(renderer, centerX, SCREEN_HEIGHT/2, radius);

        // drawing circle no.2
         SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        drawCircle(renderer, centerX2, centerY2, radius2);

    }

    // Present the renderer (draw the frame to the window)
    SDL_RenderPresent(renderer);
}

void destroyWindow(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char **argv)
{
    // The game loop control variable
    gameIsRunning = initializeWindow();
    startTime = SDL_GetTicks();

    // Game loop: keep the application running until 'running' is set to false
    while (gameIsRunning)
    {
        // Continuously polls for SDL events
        process_input();

        // Draw the rendered window
        draw();
    }

    // Clean up and exit the application
    destroyWindow();

    return 0;
}