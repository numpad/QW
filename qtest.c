#include <stdio.h>
#include <math.h>

#include "qw.h"

#define keypress(a) (qw_keydown(QW_KEY(a)))

int worldMap[24][24] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,1},
	{1,0,0,0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,1,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

int main() {
	/* Create a basic window */
	qw_screen(800, 600, 0, "Raycaster");
	
	double posX = 22, posY = 12;
	double dirX = -1, dirY = 0;
	double planeX = 0, planeY = 0.66;

	double oldTime = 0, time = 0;

	while (qw_running()) {
		qw_fill(33, 33, 33);

		for (int x = 0; x < qw_width; ++x) {
			double cameraX = 2 * x / (double)(qw_width) - 1;
			double rayPosX = posX,
				   rayPosY = posY,
				   rayDirX = dirX + planeX * cameraX,
				   rayDirY = dirY + planeY * cameraX;

			int mapX = (int)(rayPosX),
				mapY = (int)(rayPosY);

			double sideDistX, sideDistY;

			double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
			double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
			double perpWallDist;

			int stepX, stepY;
			
			int hit = 0;
			int side;

			if (rayDirX < 0) {
				stepX = -1;
				sideDistX = (rayPosX - mapX) * deltaDistX;
			} else {
				stepX = 1;
				sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
			}
			if (rayDirY < 0) {
				stepY = -1;
				sideDistY = (rayPosY - mapY) * deltaDistY;
			} else {
				stepY = 1;
				sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
			}
			
			while (hit == 0) {
				if (sideDistX < sideDistY) {
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				} else {
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}

				if (worldMap[mapX][mapY] > 0)
					hit = 1;
			}

			if (side == 0)
				perpWallDist = fabs((mapX - rayPosX + (1 - stepX) / 2) / rayDirX);
			else
				perpWallDist = fabs((mapY - rayPosY + (1 - stepY) / 2) / rayDirY);

			int lineHeight = abs((int)(qw_height / perpWallDist));

			int drawStart = -lineHeight / 2 + qw_height / 2;
			if (drawStart < 0)
				drawStart = 0;

			int drawEnd = lineHeight / 2 + qw_height / 2;
			if (drawEnd >= qw_height)
				drawEnd = qw_height - 1;

			int cr = 0, cg = 0, cb = 0;
			switch (worldMap[mapX][mapY]) {
				case 1:
					cr = 200;
					cg = 100;
					cb = 120;
					break;
				case 2:
					cr = 100;
					cg = 200;
					cb = 120;
					break;
				case 3:
					cr = 120;
					cg = 100;
					cb = 200;
					break;
			};

			if (side == 1) {
				cr /= 2;
				cg /= 2;
				cb /= 2;
			}
			
			qw_color(cr, cg, cb, 255);
			qw_drawline(x, drawStart, x, drawEnd);
			/*for (int y = drawStart; y < drawEnd; ++y) {
				qw_color(cr, cg, cb, 255);
				qw_drawpixel(x, y);
			}*/
		}
		
		oldTime = time;
		time = SDL_GetTicks();
		double frameTime = (time - oldTime) / 1000.0;
		qw_redraw();

		double moveSpeed = frameTime * 5.0;
		double rotSpeed = frameTime * 3.0;
		
		if (keypress(UP)) {
			if (worldMap[(int)(posX + dirX * moveSpeed)][(int)(posY)] == 0)
				posX += dirX * moveSpeed;
			if (worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)] == 0)
				posY += dirY * moveSpeed;
		}
		if (keypress(DOWN)) {
			if (worldMap[(int)(posX - dirX * moveSpeed)][(int)(posY)] == 0)
				posX -= dirX * moveSpeed;
			if (worldMap[(int)(posX)][(int)(posY - dirY * moveSpeed)] == 0)
				posY -= dirY * moveSpeed;
		}
		if (keypress(RIGHT)) {
			double oldDirX = dirX;
			dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
			dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
			double oldPlaneX = planeX;
			planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
			planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
		}
		if (keypress(LEFT)) {
			//both camera direction and camera plane must be rotated
			double oldDirX = dirX;
			dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
			dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
			double oldPlaneX = planeX;
			planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
			planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
		}
	}

	return 0;
}
