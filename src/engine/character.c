#include "character.h"

IMPLEMENT_LIST(CharacterList, CharacterSurface *, char *, CharacterSurface_copy, Str_copy, CharacterSurface_cmp, Str_cmp, CharacterSurface_free, Str_free);

CharacterSurface * CharacterSurface_new()
{
	CharacterSurface * tmp = NULL;
	tmp = calloc(sizeof(CharacterSurface), sizeof(CharacterSurface));
	tmp->loaded = false;

	// Right surface
	tmp->def_r    = NULL;
	tmp->jump_r   = NULL;
	tmp->walk_r   = NULL;
	tmp->slide_r  = NULL;
	tmp->crawl1_r = NULL;
	tmp->crawl2_r = NULL;

	// Left surface
	tmp->def_l    = NULL;
	tmp->jump_l   = NULL;
	tmp->walk_l   = NULL;
	tmp->slide_l  = NULL;
	tmp->crawl1_l = NULL;
	tmp->crawl2_l = NULL;

	// Initialize size
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
		// Free right surfaces
		SDL_FreeSurface(val->def_r);
		SDL_FreeSurface(val->jump_r);
		SDL_FreeSurface(val->walk_r);
		SDL_FreeSurface(val->slide_r);
		SDL_FreeSurface(val->crawl1_r);
		SDL_FreeSurface(val->crawl2_r);
		// Free left surfaces
		SDL_FreeSurface(val->def_l);
		SDL_FreeSurface(val->jump_l);
		SDL_FreeSurface(val->walk_l);
		SDL_FreeSurface(val->slide_l);
		SDL_FreeSurface(val->crawl1_l);
		SDL_FreeSurface(val->crawl2_l);
	}
	free(val);
}

int CharacterSurface_cmp(CharacterSurface * val1, CharacterSurface * val2)
{
	return memcmp(val1, val2, sizeof(CharacterSurface));
}

void CharacterSurface_copy(CharacterSurface ** dest, CharacterSurface ** src)
{
	*dest = calloc(sizeof(CharacterSurface), sizeof(CharacterSurface));
	memcpy(*dest, *src, sizeof(CharacterSurface));
}

bool CharacterSurface_load(CharacterSurface * cs, char * file)
{
	FILE * configFile   = NULL;
	char * buffer       = calloc(sizeof(char) * 255, sizeof(char));
	char * orig_buffer  = buffer;
	int    lineLen      = 0;
	char * charFilename = NULL;

	char        * currentAssetImage = NULL;
	SDL_Surface * assetImageSurface = NULL;

	char * key  = NULL;
	char * val  = NULL;
	char * posX = NULL;
	char * posY = NULL;

	SDL_Rect blitRect;

	bool inOptions = false;

	if(cs == NULL) return false;

	charFilename = calloc(sizeof(char) * (strlen(file)  + CHARACTER_DIR_LEN + 1), sizeof(char));
	sprintf(charFilename, "%s/%s", CHARACTER_DIR, file);
	configFile = fopen(charFilename, "r");
	if(configFile == NULL)
	{
		fprintf(stderr, "Unable to open %s : ", file);
		perror(NULL);
		free(charFilename);
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

						currentAssetImage = calloc(sizeof(char) * (lineLen + CHARACTER_DIR_LEN + 1), sizeof(char));
						sprintf(currentAssetImage, "%s/%s", CHARACTER_DIR, buffer);

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

					/*
					 * Right assets
					 */
					if(strcmp(key, "right.default") == 0)
					{
						cs->def_r = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->def_r, NULL);
					}
					else if(strcmp(key, "right.jump") == 0)
					{
						cs->jump_r = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->jump_r, NULL);
					}
					else if(strcmp(key, "right.walk") == 0)
					{
						cs->walk_r = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->walk_r, NULL);
					}
					else if(strcmp(key, "right.slide") == 0)
					{
						cs->slide_r = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->slide_r, NULL);
					}
					else if(strcmp(key, "right.crawl1") == 0)
					{
						cs->crawl1_r = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->crawl1_r, NULL);
					}
					else if(strcmp(key, "right.crawl2") == 0)
					{
						cs->crawl2_r = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->crawl2_r, NULL);
					}
					/*
					 * Left assets
					 */
					else if(strcmp(key, "left.default") == 0)
					{
						cs->def_l = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->def_l, NULL);
					}
					else if(strcmp(key, "left.jump") == 0)
					{
						cs->jump_l = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->jump_l, NULL);
					}
					else if(strcmp(key, "left.walk") == 0)
					{
						cs->walk_l = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->walk_l, NULL);
					}
					else if(strcmp(key, "left.slide") == 0)
					{
						cs->slide_l = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->slide_l, NULL);
					}
					else if(strcmp(key, "left.crawl1") == 0)
					{
						cs->crawl1_l = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->crawl1_l, NULL);
					}
					else if(strcmp(key, "left.crawl2") == 0)
					{
						cs->crawl2_l = SDL_CreateRGBSurface(0, cs->size.w, cs->size.h, 32, RMASK, GMASK, BMASK, AMASK);
						SDL_BlitSurface(assetImageSurface, &blitRect, cs->crawl2_l, NULL);
					}
					else fprintf(stderr, "Warning: parsing character '%s' : unknow state '%s'\n", file, key);
				}
			}
		}
	}

	if(cs->def_r    == NULL) fprintf(stderr, "Warning: character '%s', no right.default surface loaded\n", file);
	if(cs->jump_r   == NULL) fprintf(stderr, "Warning: character '%s', no right.jump surface loaded\n", file);
	if(cs->walk_r   == NULL) fprintf(stderr, "Warning: character '%s', no right.walk surface loaded\n", file);
	if(cs->slide_r  == NULL) fprintf(stderr, "Warning: character '%s', no right.slide surface loaded\n", file);
	if(cs->crawl1_r == NULL) fprintf(stderr, "Warning: character '%s', no right.crawl1 surface loaded\n", file);
	if(cs->crawl2_r == NULL) fprintf(stderr, "Warning: character '%s', no right.crawl2 surface loaded\n", file);

	if(cs->def_l    == NULL) fprintf(stderr, "Warning: character '%s', no left.default surface loaded\n", file);
	if(cs->jump_l   == NULL) fprintf(stderr, "Warning: character '%s', no left.jump surface loaded\n", file);
	if(cs->walk_l   == NULL) fprintf(stderr, "Warning: character '%s', no left.walk surface loaded\n", file);
	if(cs->slide_l  == NULL) fprintf(stderr, "Warning: character '%s', no left.slide surface loaded\n", file);
	if(cs->crawl1_l == NULL) fprintf(stderr, "Warning: character '%s', no left.crawl1 surface loaded\n", file);
	if(cs->crawl2_l == NULL) fprintf(stderr, "Warning: character '%s', no left.crawl2 surface loaded\n", file);

	cs->loaded = true;

	if(assetImageSurface != NULL)
		SDL_FreeSurface(assetImageSurface);

	if(currentAssetImage != NULL)
		free(currentAssetImage);

	free(charFilename);
	free(orig_buffer);
	fclose(configFile);
	return true;
}

CharacterSurface * CharacterSurface_init(char * file)
{
	CharacterSurface * tmp = CharacterSurface_new();
	CharacterSurface_load(tmp, file);
	return tmp;
}

CharacterList * CharacterSurface_LoadList()
{
	CharacterList * characters   = NULL;
	FILE          * listFile     = NULL;
	char          * buffer       = calloc(sizeof(char) * 255, sizeof(char));
	char          * orig_buffer  = buffer;
	char          * charFilename = NULL;
	char          * charName      = NULL;

	listFile = fopen(CHARACTER_LIST_FILE, "r");

	if(listFile == NULL)
	{
		fprintf(stderr, "Unable to open %s : ", CHARACTER_LIST_FILE);
		perror(NULL);
	}
	else
	{
		characters = CharacterList_new();
		while(fgets(buffer, 255, listFile) != NULL)
		{

			trim(&buffer);
			if(buffer[0] != '#' && buffer[0] != '\0')
			{
				charFilename = buffer;
				charName = cut(charFilename, '=');
				trim(&charFilename);
				trim(&charName);

				// Load and add the map to the list
				CharacterList_add(characters, charName, CharacterSurface_init(charFilename));
			}
		}
	}

	free(orig_buffer);
	fclose(listFile);
	return characters;
}

SDL_Surface * CharacterSurface_get(CharacterSurface * cs, CharacterState state, CharacterOrientation orientation)
{
	if(!cs->loaded) return NULL;

	switch(state)
	{
		case CS_JUMP:    return orientation == CO_RIGHT ? cs->jump_r   : cs->jump_l;
		case CS_WALK:    return orientation == CO_RIGHT ? cs->walk_r   : cs->walk_l;
		case CS_SLIDE:   return orientation == CO_RIGHT ? cs->slide_r  : cs->slide_l;
		case CS_CRAWL_1: return orientation == CO_RIGHT ? cs->crawl1_r : cs->crawl1_l;
		case CS_CRAWL_2: return orientation == CO_RIGHT ? cs->crawl2_r : cs->crawl2_l;
		default:
		case CS_DEFAULT: return orientation == CO_RIGHT ? cs->def_r    : cs->def_l;
	}
}