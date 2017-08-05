/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vizualize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liabanzh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/01 21:15:11 by liabanzh          #+#    #+#             */
/*   Updated: 2017/08/01 21:15:13 by liabanzh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"
#include "../includes/op.h"
#include <math.h>

void	onKey(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(1);
	x = 0;
	y = 0;
}

void	drawUp(void)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	glColor4ub(255, 255, 255, 120);
	glVertex2f(-1.0, -1.0);
	glVertex2f(1.0, 1.0);
	glVertex2f(1.0, -1.0);
	glVertex2f(-1.0, 1.0);
	glEnd();

	glFinish();
}

void	vizualize(byte *memory, int size)
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("CoreWar v2.0");
	glutFullScreen();
	glutDisplayFunc(drawUp);
	glutKeyboardFunc(onKey);
	memory[0] = memory[0];
	size = 0;
}
