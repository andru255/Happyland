#include "character.h"

CharacterSurface * CharacterSurface_new()
{
	CharacterSurface * tmp = NULL;
	tmp = calloc(sizeof(CharacterSurface), sizeof(CharacterSurface));
	tmp->loaded = false;
	tmp->def    = NULL;
	tmp->jump   = NULL;
	tmp->walk   = NULL;
	tmp->slide  = NULL;
	tmp->crawl1 = NULL;
	tmp->crawl2 = NULL;
	tmp->size.x = 0;
	tmp->size.y = 0;
	tmp->size.w = 0;
	tmp->size.h = 0;
	return tmp;
}

void CharacterSurface_free(CharacterSurface * val)
{
	if(val->loaded)
	{
		SDL_FreeSurface(val->def);
		SDL_FreeSurface(val->jump);
		SDL_FreeSurface(val->walk);
		SDL_FreeSurface(val->slide);
		SDL_FreeSurface(val->crawl1);
		SDL_FreeSurface(val->crawl2);
	}
	free(val);
}

bool CharacterSurface_load(CharacterSurface * cs, char * file)
{
	FILE * configFile  = NULL;
	char * buffer      = calloc(sizeof(char) * 255, sizeof(char));
	char * orig_buffer = buffer;
	int    lineLen     = 0;

	char        * currentAssetImage = NULL;
	SDL_Surface * assetImageSurface = NULL;

	char * key  = NULL;
	char * val  = NULL;
	char * posX = NULL;
	char * posY = NULL;

	SDL_Rect blitRect;

	bool inOptions = false;

	if(cs == NULL) return false;

	configFile = fopen(file, "r");
	if(configFile == NULL)
	{
		fprintf(stderr, "Unable to open %s : ", file);
		perror(NULL);
		return false;
	}
	else
	{
		while(fgets(buffer, 255, configFile) != NULL)
		{
			trim(&buffer);

			if(buffer[0] != '#' && buffer[0] != '\0')
			{
				trimComment(&buffer);
				lineLen = strlen(buffer);

				if(buffer[0] == '[' && buffer[lineLen - 1] == ']')
				{
					// Extract zone name
					buffer[lineLen - 1] = '\0';
					buffer = buffer + 1;

					if(strcmp(buffer, "options") == 0)
					{
						inOptions = true;
					}
					else
					{
						inOptions = false;
						if(cs->size.h == 0 || cs->size.w == 0)
						{
							fprintf(stderr, "Error: parsing character '%s' : start tiles without set tile size.\n", file);
							return false;
						}

						blitRect.h = cs->size.h;
						blitRect.w = cs->size.w;

						if(currentAssetImage != NULL)
							free(currentAssetImage);

						currentAssetImage = calloc(sizeof(char) * (lineLen + ASSETS_DIR_LEN + 1), sizeof(char));
						sprintf(currentAssetImage, "%s/%s", ASSETS_DIR, buffer);

						if(assetImageSurface != NULL)
							SDL_FreeSurface(assetImageSurface);

						assetImageSurface = IMG_Load(currentAssetImage);
						if(assetImageSurface == NULL)
						{
							fprintf(stderr, "Error: parsing character '%s' : can't load file %s ; %s\n", file, currentAssetImage, IMG_GetError());
							return false;
						}

						SDL_SetColorKey(assetImageSurface, SDL_TRUE, SDL_MapRGB(assetImageSurface->format, 0, 0, 0));
					}
				}
				else if(inOptions)
				{
					key = buffer;
					val = cut(key, '=');
					trim(&key);
					trim(&val);

					     if(strcmp(key, "height") == 0) cs->size.h = atoi(val);
					else if(strcmp(key, "width")  == 0) cs->size.w = atoi(val);
					else fprintf(stderr, "Warning: parsing character '%s' : unknow option '%s = %s'\n", file, key, val);
				}
				else
				{
					key = buffer;
					val = cut(key, '=');
					trim(&key);
					trim(&val);

					// Extract position
					posX = val;
					posY = cut(val, ',');
					trim(&posX);
					trim(&posY);

					blitRect.x = atoi(posX);
					blitRect.y = atoi(posY);

					if(strcmp(key, "default") == 0)
					{
						cs->def = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->def, NULL);
					}
					else if(strcmp(key, "jump") == 0)
					{
						cs->jump = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->jump, NULL);
					}
					else if(strcmp(key, "walk") == 0)
					{
						cs->walk = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->walk, NULL);
					}
					else if(strcmp(key, "slide") == 0)
					{
						cs->slide = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->slide, NULL);
					}
					else if(strcmp(key, "crawl1") == 0)
					{
						cs->crawl1 = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->crawl1, NULL);
					}
					else if(strcmp(key, "crawl2") == 0)
					{
						cs->crawl2 = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->crawl2, NULL);
					}
					else fprintf(stderr, "Warning: parsing character '%s' : unknow state '%s'\n", file, key);
				}
			}
		}
	}

	if(cs->def    == NULL) fprintf(stderr, "Warning: character '%s', no default surface loaded\n", file);
	if(cs->jump   == NULL) fprintf(stderr, "Warning: character '%s', no jump surface loaded\n", file);
	if(cs->walk   == NULL) fprintf(stderr, "Warning: character '%s', no walk surface loaded\n", file);
	if(cs->slide  == NULL) fprintf(stderr, "Warning: character '%s', no slide surface loaded\n", file);
	if(cs->crawl1 == NULL) fprintf(stderr, "Warning: character '%s', no crawl1 surface loaded\n", file);
	if(cs->crawl2 == NULL) fprintf(stderr, "Warning: character '%s', no crawl2 surface loaded\n", file);

	cs->loaded = true;

	if(assetImageSurface != NULL)
		SDL_FreeSurface(assetImageSurface);

	if(currentAssetImage != NULL)
		free(currentAssetImage);

	free(orig_buffer);
	fclose(configFile);
	return true;
}

SDL_Surface * CharacterSurface_get(CharacterSurface * cs, CharacterState state)
{
	if(!cs->loaded) return NULL;

	switch(state)
	{
		case CS_JUMP:    return cs->jump;
		case CS_WALK:    return cs->walk;
		case CS_SLIDE:   return cs->slide;
		case CS_CRAWL_1: return cs->crawl1;
		case CS_CRAWL_2: return cs->crawl2;
		default:
		case CS_DEFAULT: return cs->def;
	}
}