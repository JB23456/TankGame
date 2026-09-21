#include <stm32f030x6.h>
#include <stdlib.h>
#include <time.h>
#include "display.h"
#include "sound.h"
#include "musical_notes.h"
#include "serial.h"

// Included Functions
void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);

// Added Functions
int doMenu(const char *menu_items[],int count);
void helpScreen();
void playStartUp();
void prepScreen(int wall1[2][10], int wall2[2][10], int x1, int x2);
void winMusic();
int getRandomX(int, int);


volatile uint32_t milliseconds;
const char *choices[]={"Play","Help"};

// Assets
const uint16_t tank[]=
{
	40607,40607,40607,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,40607,40607,40607,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,40607,40607,40607,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,40607,40607,40607,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,40607,40607,40607,40607,54815,54815,54815,54815,54815,54815,54815,54815,54815,54815,40607,9293,9293,40607,40607,54815,54815,9293,9293,9293,9293,9293,54815,54815,54815,40607,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,54815,54815,54815,40607,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,54815,54815,54815,40607,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,54815,54815,54815,40607,9293,9293,40607,40607,54815,54815,9293,9293,9293,9293,9293,54815,54815,54815,40607,40607,40607,40607,40607,54815,54815,54815,54815,54815,54815,54815,54815,54815,54815,40607,40607,40607,40607,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,40607,40607,40607,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,40607,40607,40607,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,40607,40607,40607,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,61307,9293,
};

const uint16_t tankUp[]=
{
	40607,40607,40607,40607,40607,9293,9293,9293,9293,9293,40607,40607,40607,40607,40607,40607,40607,40607,40607,40607,9293,9293,9293,9293,9293,40607,40607,40607,40607,40607,40607,40607,40607,40607,40607,40607,9293,9293,9293,40607,40607,40607,40607,40607,40607,61307,61307,61307,61307,40607,40607,9293,9293,9293,40607,40607,61307,61307,61307,61307,9293,9293,9293,9293,54815,54815,9293,9293,9293,54815,54815,9293,9293,9293,9293,61307,61307,61307,61307,54815,54815,9293,9293,9293,54815,54815,61307,61307,61307,61307,9293,9293,9293,9293,54815,9293,9293,9293,9293,9293,54815,9293,9293,9293,9293,61307,61307,61307,61307,54815,9293,9293,9293,9293,9293,54815,61307,61307,61307,61307,9293,9293,9293,9293,54815,9293,9293,9293,9293,9293,54815,9293,9293,9293,9293,61307,61307,61307,61307,54815,9293,9293,9293,9293,9293,54815,61307,61307,61307,61307,9293,9293,9293,9293,54815,9293,9293,9293,9293,9293,54815,9293,9293,9293,9293,61307,61307,61307,61307,54815,54815,54815,54815,54815,54815,54815,61307,61307,61307,61307,9293,9293,9293,9293,54815,54815,54815,54815,54815,54815,54815,9293,9293,9293,9293,61307,61307,61307,61307,54815,54815,54815,54815,54815,54815,54815,61307,61307,61307,61307,9293,9293,9293,9293,40607,40607,40607,40607,40607,40607,40607,9293,9293,9293,9293,
};

const uint16_t bullet[]=
{
	33825,33825,33825,33825,33825,33825,33825,33825,33825,
};

const uint16_t box[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,21306,21306,21306,21306,21306,21306,21306,21306,21306,21306,0,0,21306,21306,21306,21306,21306,21306,21306,21306,21306,21306,0,0,21306,21306,21306,21306,21306,21306,21306,21306,21306,21306,0,0,21306,21306,21306,21306,21306,21306,21306,21306,21306,21306,0,0,21306,21306,21306,21306,21306,21306,21306,21306,21306,21306,0,0,21306,21306,21306,21306,21306,21306,21306,21306,21306,21306,0,0,21306,21306,21306,21306,21306,21306,21306,21306,21306,21306,0,0,21306,21306,21306,21306,21306,21306,21306,21306,21306,21306,0,0,21306,21306,21306,21306,21306,21306,21306,21306,21306,21306,0,0,21306,21306,21306,21306,21306,21306,21306,21306,21306,21306,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t heart[]=
{
	30918,30918,40224,40224,30918,30918,40224,40224,30918,30918,30918,40224,40224,40224,40224,40224,40224,40224,40224,30918,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,30918,40224,40224,40224,40224,40224,40224,40224,40224,30918,30918,30918,40224,40224,40224,40224,40224,40224,30918,30918,30918,30918,30918,40224,40224,40224,40224,30918,30918,30918,30918,30918,30918,30918,40224,40224,30918,30918,30918,30918,
};

const uint16_t heartEmpty[]=
{
	30918,30918,61307,61307,30918,30918,61307,61307,30918,30918,30918,61307,61307,61307,61307,61307,61307,61307,61307,30918,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,61307,30918,61307,61307,61307,61307,61307,61307,61307,61307,30918,30918,30918,61307,61307,61307,61307,61307,61307,30918,30918,30918,30918,30918,61307,61307,61307,61307,30918,30918,30918,30918,30918,30918,30918,61307,61307,30918,30918,30918,30918,
};

// Main
int main()
{
	// Player 1 position and state
	int hinverted1 = 0;
	int toggle1 = 0;
	int hmoved1 = 0;
	uint16_t x1 = getRandomX(4, 109); // Random starting X
	uint16_t y1 = 12;
	uint16_t oldx1 = x1;
	uint16_t oldy1 = y1;

	// Player 1 shooting
	int canmove1 = 1;
	int fired1 = 0; // Used as a tick down for the shoot sound 
	uint16_t bx1; // Bullet x position
	uint16_t by1; // Bullet y position
	int bulletexists1 = 0; // Each player can have 1 travelling bullet
	int health1;
	int win1 = 0;
	

	// Player 2 position and state
	int hinverted2 = 0;
	int toggle2 = 0;
	int hmoved2 = 0;
	uint16_t x2 = getRandomX(4, 109); 
	uint16_t y2 = 133;
	uint16_t oldx2 = x2;
	uint16_t oldy2 = y2;
	
	// Player 2 shooting
	int canmove2 = 1;
	int fired2 = 0;
	uint16_t bx2; 
	uint16_t by2; 
	int bulletexists2 = 0;
	int health2;
	int win2 = 0;

	// Initial Setup
	initClock();
	initSysTick();
	setupIO();
	initSound();
	initSerial();
	fillRectangle(0,0,128,160,0); // clear screen

	//Main play loop
	while(1)
	{
		// 2D Wall Arrays, index = block 0 to 9, element = x position for drawing
		int wall1[2][10] = {{4,16,28,40,52,64,76,88,100,112},{4,16,28,40,52,64,76,88,100,112}};
		int wall2[2][10] = {{4,16,28,40,52,64,76,88,100,112},{4,16,28,40,52,64,76,88,100,112}};	

		// Startup Menu
		playStartUp();
		delay(200); // Short delay to allow the drawings to catch up
		prepScreen(wall1, wall2, x1, x2);

		while((win1 == 0) && (win2 == 0))
		{
			// Movement inputs
			hmoved1 = 0;
			hinverted1 = 0;
			hmoved2 = 0;
			hinverted2 = 0;
			if ((GPIOB->IDR & (1 << 0))==0 && canmove1 == 1) // left1 pressed
			{					
				if (x1 < 109)
				{
					x1 = x1 + 1;
					hmoved1 = 1;
					hinverted1=1;
				}						
			}
			if ((GPIOB->IDR & (1 << 4))==0 && canmove1 == 1) // right1 pressed
			{			
				
				if (x1 > 4)
				{
					x1 = x1 - 1;
					hmoved1 = 1;
					hinverted1=0;
				}			
			}
			if (((GPIOA->IDR & (1 << 8)) == 0) && (fired1 == 0) && (bulletexists1 == 0)) // shoot1 pressed and can shoot
			{
				canmove1 = 0;
				fired1 = 10;
				bulletexists1 = 1;
				bx1 = x1 + 7; // bullet start position
				by1 = y1 + 39;
				putImage(bx1, by1, 3, 3, bullet, 0, 0);
				eputs("P1 fired\n\n");
			}
			if ((GPIOA->IDR & (1 << 11)) == 0 && canmove1 == 1) // right2 pressed
			{
				if (x2 < 109)
				{
					x2 = x2 + 1;
					hmoved2 = 1;
					hinverted2=1;
				}		
			}
			if ((GPIOB->IDR & (1 << 5)) == 0 && canmove1 == 1) // left2 pressed
			{			
				if (x2 > 4)
				{
					x2 = x2 - 1;
					hmoved2 = 1;
					hinverted2 =0;
				}	
			}
			if (((GPIOB->IDR & (1 << 3)) == 0) && (fired2 == 0) && (bulletexists2 == 0)) // shoot2 pressed and can shoot
			{
				canmove2 = 0;	
				fired2 = 10;
				bulletexists2 = 1;
				bx2 = x2 + 7; // bullet start position
				by2 = y2 - 27;
				putImage(bx2, by2, 3, 3, bullet, 0, 0);
				eputs("P2 fired\n\n");
			}

			//  Redrawing 1 for movement or shooting
			if (fired1 > 0) // Shooting animation
			{
				if (fired1 == 10)  // Only redraw once when firing starts
				{
					fillRectangle(oldx1, oldy1, 15, 15, RGBToWord(245,228,156));
					putImage(x1, y1, 15, 15, tankUp, 0, 1);
					oldx1 = x1;
					oldy1 = y1;
				}
				fired1--;
				if(fired1 == 0) // Fire animation complete
				{
					canmove1 = 1;
				}
			}
			else if (hmoved1 == 1 && canmove1 == 1) // Moving
			{
				// only redraw if there has been some movement (reduces flicker)
				fillRectangle(oldx1,oldy1,15,15,RGBToWord(245,228,156));
				oldx1 = x1;
				oldy1 = y1;					
				if (hmoved1)
				{
					// Toggle is a remnant from sample code, noticed issues when it is refactored
					if (toggle1)
					{
						putImage(x1,y1,15,15,tank,hinverted1,0);
					}
					else
					{
						putImage(x1,y1,15,15,tank,hinverted1,0);
					}
					toggle1 = toggle1 ^ 1;
				}
			}
			if(bulletexists1 == 1) // Bullet travelling
			{
				fillRectangle(bx1, by1, 3, 3, RGBToWord(245,228,156)); // clear old bullet
				by1 = by1 + 4; // next y value
				putImage(bx1, by1, 3, 3, bullet, 0, 0); // draw bullet
				
				// Hit a box
				if(by1 >= 106) 
				{
					for(int i=0;i<2;i++) // Boxes
					{
						for(int j=0;j<10;j++)
						{
							if(isInside(wall2[i][j],109+(i*12),12,12,bx1+1,by1+3))
							{
								fillRectangle(wall2[i][j],109+(i*12),12,12,RGBToWord(245,228,156)); // Erase Box
								wall2[i][j] = -20; // Remove from array
								bulletexists1 = 0; // Can shoot again
								fillRectangle(bx1, by1, 3, 3, RGBToWord(245,228,156));
								eputs("P2 block destroyed\n\n");
							}
						}
					}
				}
				// Hit a tank
				if(isInside(x2,y2,15,15,bx1+1,by1+3))
				{
					eputs("P2 has been hit\n\n");
					health2--;
					// Redraw health
					if(health2 == 2)
					{
						putImage(45,149,10,10,heartEmpty,0,0);
					}
					else if(health2 == 1)
					{
						putImage(30,149,10,10,heartEmpty,0,0);
					}
					else if(health2 == 0)
					{
						// Player 1 wins
						win1 = 1;
					}
					bulletexists1 = 0; // Can shoot again
					fillRectangle(bx1, by1, 3, 3, RGBToWord(245,228,156));
				}
				// Out of bounds (would be on next cycle)
				if(by1+7 >= 150)
				{
					bulletexists1 = 0;
					fillRectangle(bx1, by1, 3, 3, RGBToWord(245,228,156));
				}
			}

			// Redrawing 2 for movement or shooting
			if (fired2 > 0) // Shooting animation
			{
				if (fired2 == 10)  // Only redraw once when firing starts
				{
					fillRectangle(oldx2, oldy2, 15, 15, RGBToWord(245,228,156));
					putImage(x2, y2, 15, 15, tankUp, 0, 0);
					oldx2 = x2;
					oldy2 = y2;
				}
				fired2--;
				if(fired2 == 0) // Fire animation complete
				{
					canmove2 = 1;
				}
			}	
			else if (hmoved2 == 1 && canmove2 == 1) // Moving
			{
				// only redraw if there has been some movement (reduces flicker)
				fillRectangle(oldx2,oldy2,15,15,RGBToWord(245,228,156));
				oldx2 = x2;
				oldy2 = y2;				
				if (hmoved2)
				{
					// Toggle is a remnant from sample code, noticed issues when it is refactored
					if (toggle2)
					{
						putImage(x2,y2,15,15,tank,hinverted2,0);
					}
					else
					{
						putImage(x2,y2,15,15,tank,hinverted2,0);
					}
					toggle2 = toggle2 ^ 1;
				}
			}
			if(bulletexists2 == 1) // Bullet travelling
			{
				fillRectangle(bx2, by2, 3, 3, RGBToWord(245,228,156)); // clear old bullet
				by2 = by2 - 4; // next y value
				putImage(bx2, by2, 3, 3, bullet, 0, 0); // draw bullet
				// Hit a box
				if(by2 <= 51) 
				{
					for(int i=0;i<2;i++) // Boxes
					{
						for(int j=0;j<10;j++)
						{
							if(isInside(wall1[i][j],27+(i*12),12,12,bx2+1,by2))
							{
								fillRectangle(wall1[i][j],27+(i*12),12,12,RGBToWord(245,228,156)); // Erase Box
								wall1[i][j] = -20; // Remove from array
								bulletexists2 = 0; // Can shoot again
								fillRectangle(bx2, by2, 3, 3, RGBToWord(245,228,156));
								eputs("P1 block destroyed\n\n");
							}
						}
					}
				}
				// Hit a tank
				if(isInside(x1,y1,15,15,bx2+1,by2))
				{
					eputs("P1 has been hit\n\n");
					health1--;
					// Redraw health
					if(health1 == 2)
					{
						putImage(73,1,10,10,heartEmpty,0,1);
					}
					else if(health1 == 1)
					{
						putImage(88,1,10,10,heartEmpty,0,1);
					}
					else if(health1 == 0)
					{
						// Player 2 wins
						win2 = 1;
					}
					bulletexists2 = 0; // Can shoot again
					fillRectangle(bx2, by2, 3, 3, RGBToWord(245,228,156));
				}
				// Out of bounds (would be on next cycle)
				if(by2-3 <= 11)
				{
					bulletexists2 = 0;
					fillRectangle(bx2, by2, 3, 3, RGBToWord(245,228,156));
				}
			}
			
			// Shot sound
			if(fired1 > 8 || fired2 > 8)
			{
				playNote(C5);
			}
			else
			{
				playNote(0);
			}
			
			delay(50);
		}

		// Win Screens
		fillRectangle(0,0,128,160,RGBToWord(100,100,100));
		if(win1 == 1)
		{
			printText("Player 1 Wins!",20,70,RGBToWord(255,0,0),RGBToWord(100,100,100));
			win1 = 0;
		}
		else
		{
			printText("Player 2 Wins!",20,70,RGBToWord(255,0,0),RGBToWord(100,100,100));
			win2 = 0;
		}
		// Win Music
		winMusic();

		// Reset Variables
		// Reset Health
		health1 = 3;
		health2 = 3;

		// Reset Positions
		x1 = getRandomX(4, 109);
		oldx1 = x1;
		x2 = getRandomX(4, 109);
		oldx2 = x2;
	}
	return 0;
}

void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
	milliseconds++;
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0); // P1 Right
	pinMode(GPIOB,5,0); // P2 Left
	pinMode(GPIOA,8,0); // P1 Left
	pinMode(GPIOA,11,0); // P2 Right
	pinMode(GPIOB,3,0); // P2 Shoot
	pinMode(GPIOB,0,0); // P1 Shoot
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
	enablePullUp(GPIOB,3);
	enablePullUp(GPIOB,0);
}

int doMenu(const char *menu_items[],int count)
{
	// Menu based on example code from lectures/labs
	int index=0;
	int choice=0;
	int oldchoice=-1;
	int done=0;
	while(index < count)
	{
		printText(menu_items[index],54,85+(index*10),RGBToWord(255,0,0),RGBToWord(100,100,100));
		index++;
	}
	while(!done)
	{
		if (oldchoice != choice)
		{
			fillRectangle(44,85+(oldchoice*10),8,8,RGBToWord(100,100,100));
			fillRectangle(44,85+(choice*10),8,8,RGBToWord(255,0,0));
			oldchoice = choice;
		}
		if((GPIOB->IDR & (1 << 3)) == 0) // P2 Shoot pressed
			done = 1;
		if ((GPIOB->IDR & (1 << 5)) == 0) // P2 Left pressed
		{
			choice = choice - 1;
			if (choice < 0)
				choice = 0;
			while((GPIOB->IDR & (1 << 5)) == 0);
		}
		if ((GPIOA->IDR & (1 << 11)) == 0) // P2 Right pressed
		{
			choice=choice+1;
			if (choice >= count)
				choice = count-1;
			while((GPIOA->IDR & (1 << 11)) == 0);
		}
	}
	return choice;
}

void helpScreen()
{
	// Help screen for 2 seconds
	fillRectangle(0,0,128,160,RGBToWord(100,100,100));
	printTextX2("Help", 38, 30, RGBToWord(255,0,0), RGBToWord(100,100,100));
	printText("Shoot your", 30, 70, RGBToWord(255,0,0), RGBToWord(100,100,100));
	printText("opponent's", 30, 90, RGBToWord(255,0,0), RGBToWord(100,100,100));
	printText("tank to win!", 30, 110, RGBToWord(255,0,0), RGBToWord(100,100,100));
	delay(2000);
}

void playStartUp()
{
	// Title
	fillRectangle(0,0,128,160,RGBToWord(100,100,100));
	printTextX2("Take", 38, 30, RGBToWord(255,0,0), RGBToWord(100,100,100));
	printTextX2("Cover!", 30, 50, RGBToWord(255,0,0), RGBToWord(100,100,100));

	// Jingle
	playNote(E5);
	delay(200);
	playNote(F5);
	delay(100);
	playNote(D5);
	delay(100);
	playNote(G5);
	delay(700);
	playNote(0);

	// Display the menu
	int choice = doMenu(choices,2);
	if(choice == 0)	// Play was selected
	{
		eputs("Game Started\n\n");
		return;
	}
	else // Help was selected
	{
		delay(150);
		helpScreen();
		delay(150);
		playStartUp();
	}
}

void prepScreen(int wall1[2][10], int wall2[2][10], int x1, int x2)
{
	fillRectangle(4,0,120,11,30918); // P1 Health	
	putImage(103,1,10,10,heart,0,1); // P1 Heatlh icons
	putImage(88,1,10,10,heart,0,1);
	putImage(73,1,10,10,heart,0,1);
	printTextFlipped("1 reyalP", 14, 2, 0, 30918);
	drawLine(4,11,124,11,0); // P1 health line separator

	fillRectangle(4,149,120,11,30918); // P2 Health
	putImage(15,149,10,10,heart,0,0); // P2 Heatlh icons
	putImage(30,149,10,10,heart,0,0);
	putImage(45,149,10,10,heart,0,0);
	printText("Player 2", 60, 150, 0, 30918);
	drawLine(4,148,124,148,0); // P2 health line separator
	
	fillRectangle(4,12,120,137,RGBToWord(245,228,156)); // ground

	for(int i=0;i<2;i++) // Boxes
	{
		for(int j=0;j<10;j++)
		{
			putImage(wall1[i][j],27+(i*12),12,12,box,0,0);
			putImage(wall2[i][j],109+(i*12),12,12,box,0,0);
		}
	}

	putImage(x1,12,15,15,tankUp,0,1); // P1 tank
	putImage(x2,133,15,15,tankUp,0,0); // P2 tank
}

void winMusic()
{
	// Short win music
	playNote(D5);
	delay(200);
	playNote(0);
	delay(100);
	playNote(D5);
	delay(200);
	playNote(G5);
	delay(600);
	playNote(0);
	delay(1000);
}

int getRandomX(int min, int max)
{
	// Generates pseudo-random X  value for starting positions
	int random = rand() % (max-min + 1) + min;
	return random;
}