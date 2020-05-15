#include <SDL2/SDL.h>
#include <emscripten.h>

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
void PrintKeyInfo(SDL_KeyboardEvent *key);
void PrintModifiers(Uint16 mod);
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

  SDL_Window *window = SDL_CreateWindow("Keyboard Test App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                        640, 480, SDL_WINDOW_SHOWN);
  if (window == NULL)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", SDL_GetError());
  }

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
      SDL_Delay(30);
      switch (event.type)
      {
      /* Keyboard event */
      /* Pass the event data onto PrintKeyInfo() */
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_LEFT:
          if (x > 0)
            x = x - 5;
          break;
        case SDLK_RIGHT:
          if (x + 80 < 640)
            x = x + 5;
          break;
        case SDLK_UP:
          if (y - 5 > 0)
            y = y - 5;
          break;
        case SDLK_DOWN:
          if (y + 80 < 480)
            y = y + 5;
          break;
        case SDLK_a:
          show_l = true;
          break;
        case SDLK_d:
          show_r = true;
          break;
        }
        PrintKeyInfo(&event.key);
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym)
        {
        case SDLK_a:
          show_l = false;
          break;
        case SDLK_d:
          show_r = false;
          break;
        }
        PrintKeyInfo(&event.key);
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
    SDL_Rect rm_dstrect = {x + 80 - 2, y - 15, 15, 60};

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

/* Print all information about a key event */
void PrintKeyInfo(SDL_KeyboardEvent *key)
{
  /* Print the name of the key */
  printf("\n%s ", SDL_GetKeyName(key->keysym.sym));

  /* Is it a release or a press? */
  if (key->type == SDL_KEYUP)
    printf("RELEASED :- ");
  else
    printf("PRESSED :- ");

  /* Print the hardware scancode first */
  printf("Scancode: 0x%02X", key->keysym.scancode);
  /* If repeated key ? */
  printf(", Repeated: %d", key->repeat);

  printf("\n");

  /* Print modifier info */
  PrintModifiers(key->keysym.mod);
}

/* Print modifier info */
void PrintModifiers(Uint16 mod)
{
  printf("Modifers: ");

  /* If there are none then say so and return */
  if (mod == KMOD_NONE)
  {
    printf("None\n");
    return;
  }

  /* Check for the presence of each SDLMod value */
  /* This looks messy, but there really isn't    */
  /* a clearer way.                              */
  if (mod & KMOD_NUM)
    printf("NUMLOCK ");
  if (mod & KMOD_CAPS)
    printf("CAPSLOCK ");
  if (mod & KMOD_LCTRL)
    printf("LCTRL ");
  if (mod & KMOD_RCTRL)
    printf("RCTRL ");
  if (mod & KMOD_RSHIFT)
    printf("RSHIFT ");
  if (mod & KMOD_LSHIFT)
    printf("LSHIFT ");
  if (mod & KMOD_RALT)
    printf("RALT ");
  if (mod & KMOD_LALT)
    printf("LALT ");
  if (mod & KMOD_CTRL)
    printf("CTRL ");
  if (mod & KMOD_SHIFT)
    printf("SHIFT ");
  if (mod & KMOD_ALT)
    printf("ALT ");
  printf("\n");
}