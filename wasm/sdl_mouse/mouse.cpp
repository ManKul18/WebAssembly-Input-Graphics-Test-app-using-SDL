#include <SDL2/SDL.h>
#include <emscripten.h>

using namespace std;

static bool quit = false;
bool show_l = false, show_r = false;
int x = 288;
int y = 208;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *spaceship;
SDL_Surface *missile;
SDL_Texture *spaceship_texture, *missile_texture;

/* Function Prototypes */
void PrintButtonInfo(SDL_MouseButtonEvent button);
void PrintMotionInfo(SDL_MouseMotionEvent motion);
void PrintWheelInfo(SDL_MouseWheelEvent wheel);
void loop();

/* main */
int main(int argc, char *argv[])
{
  /* Initialise SDL */
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "Could not initialise SDL: %s\n", SDL_GetError());
    exit(-1);
  }

  window = SDL_CreateWindow("Mouse Test App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
  renderer = SDL_CreateRenderer(window, -1, 0);

  spaceship = SDL_LoadBMP("spaceship.bmp");
  missile = SDL_LoadBMP("missile.bmp");
  spaceship_texture = SDL_CreateTextureFromSurface(renderer, spaceship);
  missile_texture = SDL_CreateTextureFromSurface(renderer, missile);
  SDL_FreeSurface(missile);
  SDL_FreeSurface(spaceship);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  /* Loop until an SDL_QUIT event is found */
  emscripten_set_main_loop(loop, 0, 1);

  return 0;
}

/* Emscripten loop */
void loop()
{
  SDL_Event event;
  if (!quit)
  {
    /* Poll for events */
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      /* Keyboard event */
      /* Pass the event data onto PrintKeyInfo() */
      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == 1)
        {
          show_l = true;
        }
        if (event.button.button == 3)
        {
          show_r = true;
        }
        PrintButtonInfo(event.button);
        break;
      case SDL_MOUSEBUTTONUP:
        if (event.button.button == 1)
        {
          show_l = false;
        }
        if (event.button.button == 3)
        {
          show_r = false;
        }
        PrintButtonInfo(event.button);
        break;
      case SDL_MOUSEMOTION:
        //mouse cordinates
        x = event.motion.x - 45;
        y = event.motion.y - 45;
        PrintMotionInfo(event.motion);
        break;
      case SDL_MOUSEWHEEL:
        PrintWheelInfo(event.wheel);
        break;

      /* SDL_QUIT event (window close) */
      case SDL_QUIT:
        quit = 1;
        break;

      default:
        break;
      }
    }
    //update graphics
    SDL_Rect dstrect = {x, y, 90, 90};
    SDL_Rect lm_dstrect = {x - 5, y - 15, 15, 60};
    SDL_Rect rm_dstrect = {x + 80-2, y - 15, 15, 60};

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, spaceship_texture, NULL, &dstrect);
    if (show_l)
    {
      SDL_RenderCopy(renderer, missile_texture, NULL, &lm_dstrect);
    }
    if (show_r)
    {
      SDL_RenderCopy(renderer, missile_texture, NULL, &rm_dstrect);
    }

    SDL_RenderPresent(renderer);
  }
  else
  {
    /* Clean up */
    emscripten_cancel_main_loop();
    SDL_DestroyTexture(spaceship_texture);
    SDL_DestroyTexture(missile_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
}

void PrintButtonInfo(SDL_MouseButtonEvent button)
{
  const char *btn_status = button.state ? "PRESSED" : "RELEASED";
  const char *btn_name;
  if (button.button == 1)
    btn_name = "LEFT";
  else if (button.button == 2)
    btn_name = "MIDDLE";
  else if (button.button == 3)
    btn_name = "RIGHT";
  printf("%s Button is %s [ ", btn_name, btn_status);
  printf("Timestamp : %d, Window ID : %d, Device ID : %d, Clicks : %d, x : %d, y : %d ]\n",
         button.timestamp, button.windowID, button.which, button.clicks, button.x, button.y);
}

void PrintMotionInfo(SDL_MouseMotionEvent motion)
{
  printf("Mouse moved [ ");
  printf("Timestamp : %d, Window ID : %d, Device ID : %d, x : %d, y : %d, Relative x : %d, Relative y : %d ]\n",
         motion.timestamp, motion.windowID, motion.which, motion.x, motion.y, motion.xrel, motion.yrel);
}

void PrintWheelInfo(SDL_MouseWheelEvent wheel)
{
  printf("Wheel rolled [ ");
  printf("Timestamp : %d, Window ID : %d, Device ID : %d, Wheel direction : %d, x : %d, y : %d ]\n",
         wheel.timestamp, wheel.windowID, wheel.which, wheel.direction, wheel.x, wheel.y);
}
