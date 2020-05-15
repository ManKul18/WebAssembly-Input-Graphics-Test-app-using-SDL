#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>

#include <emscripten.h>

#include <stdio.h>

int i, num_js = 0;
SDL_Joystick *joystick = NULL;
SDL_bool quit = SDL_FALSE;

bool show_l = false, show_r = false;
int x = 288;
int y = 208;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *spaceship;
SDL_Surface *missile;
SDL_Texture *spaceship_texture, *missile_texture;

void process_buttons(SDL_Event evt)
{
  switch (evt.type)
  {
  case SDL_JOYBUTTONDOWN:
  {
    printf("Button code : %d, value : %d\n", evt.jbutton.button, evt.jbutton.state);
    switch (evt.jbutton.button)
    {
    case 12: //up
      if (y - 5 > 0)
        y = y - 5;
      break;
    case 13: //down
      if (y + 80 < 480)
        y = y + 5;
      break;
    case 14: //left
      if (x > 0)
        x = x - 5;
      break;
    case 15: //right
      if (x + 80 < 640)
        x = x + 5;
      break;
    case 4: //left fire
      show_l = true;
      break;
    case 5: //right fire
      show_r = true;
      break;
    }
  }
  break;
  case SDL_JOYBUTTONUP:
  {
    printf("Button code : %d, value : %d\n", evt.jbutton.button, evt.jbutton.state);
    switch (evt.jbutton.button)
    {
    case 4:
      show_l = false;
      break;
    case 5:
      show_r = false;
      break;
    }
  }
  break;

  case SDL_JOYHATMOTION:
  {
    int hat = SDL_JoystickGetHat(joystick, 0);
    printf("This block does not work for now\n");
  }
  break;

  case SDL_JOYAXISMOTION:
  {
    printf("Axis code : %d, value : %d\n", evt.jaxis.axis, evt.jaxis.value);
  }
  break;

  default:
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unrelated event type %d", evt.type);
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

void loop()
{
  if (!quit)
  {
    num_js = SDL_NumJoysticks();
    //SDL_Log("%d joystick(/s) found", num_js);
    //get number of joysticks
    if (joystick == NULL && num_js)
    {
      printf("%d gamepad(/s) found\n", num_js);
      joystick = SDL_JoystickOpen(0);
      printf("First gamepad info\n");
      printf("Name : %s\n", SDL_JoystickName(joystick));
      SDL_JoystickGUID guid = SDL_JoystickGetDeviceGUID(0);
      printf("GUID : %s\n", guid.data);
      printf("Event status : %s\n", SDL_JoystickEventState(SDL_QUERY) ? "ENABLED" : "IGNORED");
      printf("Gamepad status : %s\n", SDL_JoystickGetAttached(joystick) ? "OPENED" : "NOT OPENED");
      printf("Number of buttons : %d\n", SDL_JoystickNumButtons(joystick));
      printf("Number of hats : %d\n", SDL_JoystickNumHats(joystick));
      printf("Battery level : %d\n", SDL_JoystickCurrentPowerLevel(joystick));
    }
    else if (joystick && num_js)
    {
      SDL_Event e;
      while (SDL_PollEvent(&e))
      {
        process_buttons(e);
      }
    }
    else if (joystick && !num_js)
    {
      printf("Gamepad disconnected\n");
      SDL_JoystickClose(joystick);
      joystick = NULL;
    }
    SDL_Delay(30);
  }
  else
  {
    emscripten_cancel_main_loop();
    SDL_DestroyTexture(spaceship_texture);
    SDL_DestroyTexture(missile_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    if (joystick != NULL)
    {
      SDL_JoystickClose(joystick);
      joystick = NULL;
    }
    SDL_Quit();
  }
}

int main()
{
  //printf("WebAssembly Gamepad test app. Please connect a gamepad\n");
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL init failed : %s \n", SDL_GetError());
  }

  SDL_Window *window = SDL_CreateWindow("Gamepad Test App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
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

  emscripten_set_main_loop(loop, 0, 1);
  return 0;
}
