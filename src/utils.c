#include "utils.h"

void pause()
{
	bool continuer = true;
	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
			continuer = false;
		}
	}
}
