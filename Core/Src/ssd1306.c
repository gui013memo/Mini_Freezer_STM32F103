/**
 * original author:  Tilen Majerle<tilen@majerle.eu>
 * modification for STM32f10x: Alexander Lutsai<s.lyra@ya.ru>

 ----------------------------------------------------------------------
 Copyright (C) Alexander Lutsai, 2016
 Copyright (C) Tilen Majerle, 2015

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ----------------------------------------------------------------------
 */
#include "ssd1306.h"
#include "fonts.h"
#include "logo.h"

#include <stdio.h>


extern I2C_HandleTypeDef hi2c1;
/* Write command */
#define SSD1306_WRITECOMMAND(command)      ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x00, (command))
/* Write data */
#define SSD1306_WRITEDATA(data)            ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x40, (data))
/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))

/* SSD1306 data buffer */
uint8_t SSD1306_Buffer[1025];

/* Private SSD1306 structure */
//typedef struct {
//	uint16_t CurrentX;
//	uint16_t CurrentY;
//	uint8_t Inverted;
//	uint8_t Initialized;
//} SSD1306_t;
/* Private variable */

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define SSD1306_DEACTIVATE_SCROLL                    0x2E // Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F // Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 // Set scroll range

#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7

///////////////////		MY FUNCTIONS	////////////////////////////

//SSD1306_GetCurrent_X_Y()

void SSD1306_DrawMenus(uint8_t MenuNumber) {
	switch (MenuNumber) {
	case 1:
		SSD1306_GotoXY(64, 32);

		//char string[11] = "Menu 1";
		char string2[11] = "Menu 2";

		int valorfic = 523; ///////valor ficticio
		char msg[4];
		sprintf(msg, "%d", valorfic); //////////conversao de valor int para string

		SSD1306_DrawArrow(2, 2);

		SSD1306_GotoXY(10, 0);
		SSD1306_Puts(msg, &Font_7x10, 1);
		SSD1306_UpdateScreen();

		SSD1306_GotoXY(10, 20);
		SSD1306_Puts(string2, &Font_11x18, 1);
		SSD1306_UpdateScreen();
		break;

	case 0:
		SSD1306_DrawCircle(64, 32, 31, 1);
		SSD1306_UpdateScreen();

	}

}

void SSD1306_Draw_intrologo(const unsigned char *logo, uint8_t color) {
	SSD1306_DrawBitmap(25, 0, logo, 128, 64, color);
	//SSD1306_InvertDisplay(1);

	SSD1306_UpdateScreen();
	//Set_Contrast(0);
	for (int i = 0; i < 5; i++) {

		HAL_Delay(100);
		Set_Contrast(0);
		HAL_Delay(100);
		Set_Contrast(255);

	}
	HAL_Delay(1200);
	//SSD1306_InvertDisplay(0);
	SSD1306_Clear();
}

void SSD1306_DrawIntroFreezer(int delay) {

	SSD1306_Draw_intrologo(Logo_GO_bmp, 1);

	SSD1306_GotoXY(2, 0);
	SSD1306_Puts("Controlador", &Font_11x18, 1);

	SSD1306_GotoXY(4, 20);
	SSD1306_Puts("Freezer", &Font_16x26, 1);

	SSD1306_GotoXY(5, 48);
	SSD1306_Puts("v1.0 by Guilherme!", &Font_7x10, 1);

	SSD1306_UpdateScreen();
	HAL_Delay(delay);

	SSD1306_Clear();
}

void SSD1306_DrawArrow(char x, char y) {
	//primeira coluna
	SSD1306_DrawPixel(x, y, 1);
	SSD1306_DrawPixel(x, y + 10, 1);

	//segunda coluna
	SSD1306_DrawPixel(x + 1, y + 1, 1);
	SSD1306_DrawPixel(x + 1, y + 9, 1);

	//terceira coluna
	SSD1306_DrawPixel(x + 2, y + 2, 1);
	SSD1306_DrawPixel(x + 2, y + 8, 1);

	//quarta coluna
	SSD1306_DrawPixel(x + 3, y + 3, 1);
	SSD1306_DrawPixel(x + 3, y + 7, 1);

	//quinta coluna
	SSD1306_DrawPixel(x + 4, y + 4, 1);
	SSD1306_DrawPixel(x + 4, y + 6, 1);

	//sexta coluna
	SSD1306_DrawPixel(x + 5, y + 5, 1);
	SSD1306_DrawPixel(x + 5, y + 5, 1);

}

/*----------- I2C -----------*/

void ssd1306_command(unsigned char Comando) {
	unsigned char DATA[2];

	DATA[0] = 0x00;
	DATA[1] = Comando;

	HAL_I2C_Master_Transmit(&hi2c1, 0x78, DATA, 2, 10000);

}
/*----------- I2C END -----------*/

///////////////////		MY FUNCTIONS END 	////////////////////////////
void SSD1306_ScrollRight(uint8_t start_row, uint8_t end_row, uint8_t speed) {
	SSD1306_WRITECOMMAND(SSD1306_RIGHT_HORIZONTAL_SCROLL);  // send 0x26
	SSD1306_WRITECOMMAND(0x00);  // send dummy
	SSD1306_WRITECOMMAND(start_row);  // start page address
	SSD1306_WRITECOMMAND(speed);  // time interval 5 frames
	SSD1306_WRITECOMMAND(end_row);  // end page address
	SSD1306_WRITECOMMAND(0X00); // estava no codigo original
	SSD1306_WRITECOMMAND(0XFF); //estava no codigo original
	SSD1306_WRITECOMMAND(SSD1306_ACTIVATE_SCROLL); // start scroll
}

void SSD1306_ScrollLeft(uint8_t start_row, uint8_t end_row) {
	SSD1306_WRITECOMMAND(SSD1306_LEFT_HORIZONTAL_SCROLL);  // send 0x26
	SSD1306_WRITECOMMAND(0x00);  // send dummy
	SSD1306_WRITECOMMAND(start_row);  // start page address
	SSD1306_WRITECOMMAND(0X00);  // time interval 5 frames
	SSD1306_WRITECOMMAND(end_row);  // end page address
	SSD1306_WRITECOMMAND(0X00);
	SSD1306_WRITECOMMAND(0XFF);
	SSD1306_WRITECOMMAND(SSD1306_ACTIVATE_SCROLL); // start scroll
}

void SSD1306_Scrolldiagright(uint8_t start_row, uint8_t end_row) {
	SSD1306_WRITECOMMAND(SSD1306_SET_VERTICAL_SCROLL_AREA);  // sect the area
	SSD1306_WRITECOMMAND(0x00);   // write dummy
	SSD1306_WRITECOMMAND(SSD1306_HEIGHT);

	SSD1306_WRITECOMMAND(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
	SSD1306_WRITECOMMAND(0x00);
	SSD1306_WRITECOMMAND(start_row);
	SSD1306_WRITECOMMAND(0X00);
	SSD1306_WRITECOMMAND(end_row);
	SSD1306_WRITECOMMAND(0x01);
	SSD1306_WRITECOMMAND(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_Scrolldiagleft(uint8_t start_row, uint8_t end_row) {
	SSD1306_WRITECOMMAND(SSD1306_SET_VERTICAL_SCROLL_AREA);  // sect the area
	SSD1306_WRITECOMMAND(0x00);   // write dummy
	SSD1306_WRITECOMMAND(SSD1306_HEIGHT);

	SSD1306_WRITECOMMAND(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
	SSD1306_WRITECOMMAND(0x00);
	SSD1306_WRITECOMMAND(start_row);
	SSD1306_WRITECOMMAND(0X00);
	SSD1306_WRITECOMMAND(end_row);
	SSD1306_WRITECOMMAND(0x01);
	SSD1306_WRITECOMMAND(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_Stopscroll(void) {
	SSD1306_WRITECOMMAND(SSD1306_DEACTIVATE_SCROLL);
}

void SSD1306_InvertDisplay(int i) {
	if (i)
		SSD1306_WRITECOMMAND(SSD1306_INVERTDISPLAY);

	else
		SSD1306_WRITECOMMAND(SSD1306_NORMALDISPLAY);

}

void SSD1306_DrawBitmap(int16_t x, int16_t y, const unsigned char *bitmap,
		int16_t w, int16_t h, uint16_t color) {

	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t byte = 0;

	for (int16_t j = 0; j < h; j++, y++) {
		for (int16_t i = 0; i < w; i++) {
			if (i & 7) {
				byte <<= 1;
			} else {
				byte =
						(*(const unsigned char*) (&bitmap[j * byteWidth + i / 8]));
			}
			if (byte & 0x80)
				SSD1306_DrawPixel(x + i, y, color);
		}
	}
}

void Set_Contrast(unsigned char Level) {

	unsigned char DATA[4];

	DATA[0] = 0x00;
	DATA[1] = 0x81;
	DATA[2] = Level;

	HAL_I2C_Master_Transmit(&hi2c1, 0x78, DATA, 3, 10000);

	DATA[0] = 0x00;
	DATA[1] = 0x21;
	DATA[2] = 0;
	DATA[3] = 127;
	HAL_I2C_Master_Transmit(&hi2c1, (0x78 << 1) | 1, DATA, 4, 10000);

}

void OLED_INIT() {
	unsigned char vccstate = 0;

	///////// Seta todos os indices do MEMR para 0; /////////////
	/* for (unsigned char i = 1; i <= 1025; i++)
	 {

	 MEMR[i] = 0;

	 }*/

	// Init sequence
	ssd1306_command(0xAE);                    // 0xAE
	ssd1306_command(0xD5);            // 0xD5
	ssd1306_command(0x80);                           // the suggested ratio 0x80

	ssd1306_command(0xA8);                  // 0xA8
	ssd1306_command(64 - 1);

	ssd1306_command(0xD3);              // 0xD3
	ssd1306_command(0x0);                                   // no offset
	ssd1306_command(0x40 | 0x0);            // line #0
	ssd1306_command(0x8D);                    // 0x8D
	if (vccstate == 0x1) {
		ssd1306_command(0x10);
	} else {
		ssd1306_command(0x14);
	}
	ssd1306_command(0x20);                    // 0x20
	ssd1306_command(0x00);                                // 0x0 act like ks0108
	ssd1306_command(0xA0 | 0x1);
	ssd1306_command(0xC8);

	/*#if defined SSD1306_128_32
	 ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
	 ssd1306_command(0x02);
	 ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
	 ssd1306_command(0x8F);

	 #elif defined SSD1306_128_64
	 ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
	 ssd1306_command(0x12);
	 ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
	 if (vccstate == SSD1306_EXTERNALVCC)
	 { ssd1306_command(0x9F); }
	 else
	 { ssd1306_command(0xCF); }

	 #elif defined SSD1306_96_16
	 ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
	 ssd1306_command(0x2);   //ada x12
	 ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
	 if (vccstate == SSD1306_EXTERNALVCC)
	 { ssd1306_command(0x10); }
	 else
	 { ssd1306_command(0xAF); }

	 #endif
	 */
	ssd1306_command(0xD9);                  // 0xd9
	if (vccstate == 0x1) {
		ssd1306_command(0x22);
	} else {
		ssd1306_command(0xF1);
	}
	ssd1306_command(0xDB);                 // 0xDB
	ssd1306_command(0x40);
	ssd1306_command(0xA4);           // 0xA4
	ssd1306_command(0xA6);                 // 0xA6

	ssd1306_command(0x2E);

	//Inicia om X= 0 e Y = 0;

	Set_Contrast(0);

	//FIM

	ssd1306_command(0xAF);                 //--turn on oled panel

	//OLED_DISPLAY();
	SSD1306_UpdateScreen();

}

void SSD1306_UpdateScreen(void) {

	SSD1306_Buffer[0] = 0x40;

	HAL_I2C_Master_Transmit(&hi2c1, SSD1306_I2C_ADDR, SSD1306_Buffer, 1025,
			10000);

}
/*
 void OledSetPixel(unsigned char X, unsigned char Y, unsigned char bit, unsigned char invertido)
 {

 int cordenada = X + ((Y>>3)<<7);




 SSD1306_Buffer[cordenada] |= bit << (Y & 7);




 }
 */

void SSD1306_ToggleInvert(void) {
	uint16_t i;

	/* Toggle invert */
	SSD1306.Inverted = !SSD1306.Inverted;

	/* Do memory toggle */
	for (i = 0; i < sizeof(SSD1306_Buffer); i++) {
		SSD1306_Buffer[i] = ~SSD1306_Buffer[i];
	}
}

void SSD1306_Fill(SSD1306_COLOR_t color) {
	/* Set memory */
	memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF,
			sizeof(SSD1306_Buffer));
}

/*void OledSetPixel(unsigned char X, unsigned char Y, unsigned char bit, unsigned char invertido)
 {

 int cordenada = X + ((Y>>3)<<7);




 SSD1306_Buffer[cordenada] |= bit << (Y & 7);




 }*/

void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color) {

	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
		/* Error */
		return;
	}

	/* Check if pixels are inverted */
	if (SSD1306.Inverted) {
		color = (SSD1306_COLOR_t) !color;
	}

	/* Set color */
	if (color == SSD1306_COLOR_WHITE) {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y & 7);
	} else {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y & 7));
	}
}

void SSD1306_GotoXY(uint16_t x, uint16_t y) {
	/* Set write pointers */
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

char SSD1306_Putc(char ch, FontDef_t *Font, SSD1306_COLOR_t color) {
	uint32_t i, b, j;

	/* Check available space in LCD */
//	if (
//		SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
//		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight)
//	) {
//		/* Error */
//		return 0;
//	}
	/* Go through font */
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i),
						(SSD1306_COLOR_t) color);
			} else {
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i),
						(SSD1306_COLOR_t) !color);
			}
		}
	}

	/* Increase pointer */
	SSD1306.CurrentX += Font->FontWidth;

	/* Return character written */
	return ch;
}

char SSD1306_Puts(char *str, FontDef_t *Font, SSD1306_COLOR_t color) {
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (SSD1306_Putc(*str, Font, color) != *str) {
			/* Return error */
			return *str;
		}

		/* Increase string pointer */
		str++;
	}

	/* Everything OK, zero should be returned */
	return *str;
}

void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
		SSD1306_COLOR_t c) {
	int16_t dx, dy, sx, sy, err, e2, i, tmp;

	/* Check for overflow */
	if (x0 >= SSD1306_WIDTH) {
		x0 = SSD1306_WIDTH - 1;
	}
	if (x1 >= SSD1306_WIDTH) {
		x1 = SSD1306_WIDTH - 1;
	}
	if (y0 >= SSD1306_HEIGHT) {
		y0 = SSD1306_HEIGHT - 1;
	}
	if (y1 >= SSD1306_HEIGHT) {
		y1 = SSD1306_HEIGHT - 1;
	}

	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;
	err = ((dx > dy) ? dx : -dy) / 2;

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}

		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			SSD1306_DrawPixel(x0, i, c);
		}

		/* Return from function */
		return;
	}

	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}

		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			SSD1306_DrawPixel(i, y0, c);
		}

		/* Return from function */
		return;
	}

	while (1) {
		SSD1306_DrawPixel(x0, y0, c);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}

void SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
		SSD1306_COLOR_t c) {
	/* Check input parameters */
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
		/* Return error */
		return;
	}

	/* Check width and height */
	if ((x + w) >= SSD1306_WIDTH) {
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT) {
		h = SSD1306_HEIGHT - y;
	}

	/* Draw 4 lines */
	SSD1306_DrawLine(x, y, x + w, y, c); /* Top line */
	SSD1306_DrawLine(x, y + h, x + w, y + h, c); /* Bottom line */
	SSD1306_DrawLine(x, y, x, y + h, c); /* Left line */
	SSD1306_DrawLine(x + w, y, x + w, y + h, c); /* Right line */
}

void SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
		SSD1306_COLOR_t c) {
	uint8_t i;

	/* Check input parameters */
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
		/* Return error */
		return;
	}

	/* Check width and height */
	if ((x + w) >= SSD1306_WIDTH) {
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT) {
		h = SSD1306_HEIGHT - y;
	}

	/* Draw lines */
	for (i = 0; i <= h; i++) {
		/* Draw lines */
		SSD1306_DrawLine(x, y + i, x + w, y + i, c);
	}
}

void SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
		uint16_t x3, uint16_t y3, SSD1306_COLOR_t color) {
	/* Draw lines */
	SSD1306_DrawLine(x1, y1, x2, y2, color);
	SSD1306_DrawLine(x2, y2, x3, y3, color);
	SSD1306_DrawLine(x3, y3, x1, y1, color);
}

void SSD1306_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2,
		uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color) {
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, yinc1 =
			0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
			curpixel = 0;

	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay) {
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		SSD1306_DrawLine(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	SSD1306_DrawPixel(x0, y0 + r, c);
	SSD1306_DrawPixel(x0, y0 - r, c);
	SSD1306_DrawPixel(x0 + r, y0, c);
	SSD1306_DrawPixel(x0 - r, y0, c);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		SSD1306_DrawPixel(x0 + x, y0 + y, c);
		SSD1306_DrawPixel(x0 - x, y0 + y, c);
		SSD1306_DrawPixel(x0 + x, y0 - y, c);
		SSD1306_DrawPixel(x0 - x, y0 - y, c);

		SSD1306_DrawPixel(x0 + y, y0 + x, c);
		SSD1306_DrawPixel(x0 - y, y0 + x, c);
		SSD1306_DrawPixel(x0 + y, y0 - x, c);
		SSD1306_DrawPixel(x0 - y, y0 - x, c);
	}
}

void SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r,
		SSD1306_COLOR_t c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	SSD1306_DrawPixel(x0, y0 + r, c);
	SSD1306_DrawPixel(x0, y0 - r, c);
	SSD1306_DrawPixel(x0 + r, y0, c);
	SSD1306_DrawPixel(x0 - r, y0, c);
	SSD1306_DrawLine(x0 - r, y0, x0 + r, y0, c);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		SSD1306_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, c);
		SSD1306_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, c);

		SSD1306_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, c);
		SSD1306_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, c);
	}
}

void SSD1306_Clear(void) {
	SSD1306_Fill(0);
	SSD1306_UpdateScreen();
}
void SSD1306_ON(void) {
	SSD1306_WRITECOMMAND(0x8D);
	SSD1306_WRITECOMMAND(0x14);
	SSD1306_WRITECOMMAND(0xAF);
}
void SSD1306_OFF(void) {
	SSD1306_WRITECOMMAND(0x8D);
	SSD1306_WRITECOMMAND(0x10);
	SSD1306_WRITECOMMAND(0xAE);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  _____ ___   _____
// |_   _|__ \ / ____|
//   | |    ) | |
//   | |   / /| |
//  _| |_ / /_| |____
// |_____|____|\_____|
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void ssd1306_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t *data,
		uint16_t count) {
	uint8_t dt[256];
	dt[0] = reg;
	uint8_t i;
	for (i = 0; i < count; i++)
		dt[i + 1] = data[i];
	HAL_I2C_Master_Transmit(&hi2c1, address, dt, count + 1, 10);
}

void ssd1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data) {
	uint8_t dt[2];
	dt[0] = reg;
	dt[1] = data;
	HAL_I2C_Master_Transmit(&hi2c1, address, dt, 2, 10);
}

