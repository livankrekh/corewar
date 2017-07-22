/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vizualize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liabanzh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/07 15:15:40 by liabanzh          #+#    #+#             */
/*   Updated: 2017/07/07 15:15:41 by liabanzh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"
#include "../includes/op.h"
#include <math.h>

void	getImageBackground(SDL_Renderer *render, SDL_DisplayMode displayMode)
{
	SDL_Surface		*BMP_back;
	SDL_Texture		*back;
	SDL_Rect 		background;

	background.x = 30;
	background.y = 30;
	background.w = displayMode.w - 60;
	background.h = displayMode.h - 60;
	BMP_back = SDL_LoadBMP("8bit.bmp");
	back = SDL_CreateTextureFromSurface(render, BMP_back);
	SDL_FreeSurface(BMP_back);
	//SDL_RenderClear(render);
	SDL_RenderCopy(render, back, NULL, &background);
	SDL_DestroyTexture(back);
}

void	createAlpha(SDL_Renderer *render, SDL_DisplayMode displayMode)
{
	SDL_Rect 		rect;
	SDL_Rect 		panel;


	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(render, 0, 0, 0, 160);
	rect.x = 30;
	rect.y = 30;
	rect.w = displayMode.w - 830;
	rect.h = displayMode.h - 60;
	SDL_RenderFillRect(render, &rect);
	panel.x = displayMode.w - 800;
	panel.y = 30;
	panel.w = 770;
	panel.h = displayMode.h - 60;
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderFillRect(render, &panel);
	panel.w = 20;
	SDL_SetRenderDrawColor(render, 200, 200, 200, 255);
	SDL_RenderFillRect(render, &panel);
}

void	createFon(SDL_Renderer *render, SDL_DisplayMode displayMode)
{
	SDL_Rect 		back;

	back.x = 0;
	back.y = 0;
	back.w = displayMode.w;
	back.h = displayMode.h;
	SDL_SetRenderDrawColor(render, 200, 200, 200, 255);
	SDL_RenderFillRect(render, &back);
}

// int 	getSpace(SDL_DisplayMode displayMode, int size)
// {
// 	int 	space;
// 	int 	space2;

// 	displayMode.w -= 830;
// 	displayMode.h -= 60;
// 	space2 = (displayMode.w * displayMode.h) / size;
// 	space = sqrt(space2);
// 	return (space);
// }

void	printText(SDL_Renderer *render, char **text, SDL_DisplayMode displayMode, int size)
{
	TTF_Font	*Mac;
	SDL_Color 	textColor;
	SDL_Surface *Sur;
	SDL_Texture *message;
	SDL_Rect 	rect;
	int 		space;
	int 		x;
	int 		y;
	int 		i;

	TTF_Init();
	space = 12;
	x = space + 30;
	y = space + 30;
	i = 0;
	Mac = TTF_OpenFont("Font.ttf", 36);
	textColor.r = 255;
	textColor.g = 255;
	textColor.b = 255;
	rect.w = 16;
	rect.h = 16;
	// rect.w = getSpace(displayMode, size) / 2;
	// rect.h = getSpace(displayMode, size) / 2;
	while (y < displayMode.h - 80 && i < size && text[i] != NULL)
	{
		if (x >= displayMode.w - 830)
		{
			x = space + 30;
			y += space + rect.h;
		}
		Sur = TTF_RenderText_Blended(Mac, text[i], textColor);
		message = SDL_CreateTextureFromSurface(render, Sur);
		rect.x = x;
		rect.y = y;
		SDL_RenderCopy(render, message, NULL, &rect);
		// SDL_RenderCopy(render, message, NULL, &rect);
		x += space + rect.w;
		i++;
	}
	TTF_Quit();
}

void	outputText(SDL_Renderer *render, byte *memory, int size, SDL_DisplayMode displayMode)
{
	int 	i;
	char	**shit;
	char	*hex;

	i = 0;
	shit = (char**)malloc(sizeof(char*) * (size + 1));
	while (i < size)
	{
		shit[i] = ft_strnew(2);
		hex = ft_itoa_base(memory[i], 16);
		ft_strncpy(shit[i], hex, 2);
		if (ft_strlen(hex) == 1)
		{
			shit[i][1] = shit[i][0];
			shit[i][0] = '0';
		}
		i++;
		free(hex);
	}
	shit[i] = NULL;
	printText(render, shit, displayMode, size);
}

// void	getCursor(SDL_Renderer *render)
// {
// 	// SDL_Renderer 	*render;
// 	SDL_Rect 		cursor;

// 	// if ((render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == NULL)
// 	// 	return ;
// 	SDL_SetRenderDrawColor(render, 0, 180, 0, 160);
// 	cursor.x = 36;
// 	cursor.y = 36;
// 	cursor.w = 24;
// 	cursor.h = 24;
// 	SDL_RenderFillRect(render, &cursor);
// }

void	vizualize(byte *memory, int size)
{
	SDL_Window 		*win;
	SDL_DisplayMode	displayMode;
	SDL_Renderer	*render;
	t_bool			running;
	SDL_Event 		event;

	running = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return ;
	SDL_GetDesktopDisplayMode(0, &displayMode);
	if ((win = SDL_CreateWindow("CoreWar v1.0", 0, 0, displayMode.w, displayMode.h, SDL_WINDOW_FULLSCREEN)) == NULL)
		return ;
	if ((render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == NULL)
		return ;
	createFon(render, displayMode);
	getImageBackground(render, displayMode);
	createAlpha(render, displayMode);
	//outputText(render, memory, size, displayMode);
	// SDL_RenderPresent(render);
	// getCursor(render);
	SDL_RenderPresent(render);
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if ((SDL_QUIT == event.type) ||
				(SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
			{
				running = false;
				break;
			}
		}
	}
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(win);
	SDL_Quit();
	memory[size - 2] = memory[size - 2];
}
