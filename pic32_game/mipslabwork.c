/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <string.h>


int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  PORTE = 0b00000000;
  display_image(0, icon); //debug doges
  display_image(32, icon);  
  display_image(64, icon);
  display_image(96, icon);

  TRISE = TRISE & ~0xff;   /* Port E is used for the LED 
	Set bits 0 through 7 to 0 (output) */
  PORTE = 0b00000000;
  TRISD = TRISD | 0x03F0;

  return;
}



#define BALLDEFAULTX 64
#define BALLDEFAULTY 8
#define GRAVITY 4
#define PADDLELENGTH 12
#define AIRRESISTANCE 15
#define BALLMINIMUMSPEED 2
#define BALLMAXSPEED 5

//gravity is inverse. 20 is a little gravity. 1 is tons of gravity. same with air resistance.
//ballminimumspeed can be set to 99 to have no air resistance loss, for fun moments
/* This function is called repetitively from the main program */
void labwork( void )
{
  //U1BRG = calculate_baudrate_divider(80000000, 115200, 0);
  //all this code was meant for the communication through UART. but alas.
  /*ODCE = 0;
  U1BRG = 15200;
  U1STA = 0;
  U1STASET = 0x1400;
  U1MODE = 0x8000;
  int tmp;

  while (1)
  {
    while(!(U1STA & 0x1)); //wait for read buffer to have a value

        {tmp = U1RXREG & 0xFF; PORTE += 0b01;}
		while(U1STA & (1 << 9)); //make sure the write buffer is not full 
		U1TXREG = tmp;
		PORTE = tmp;
  }
  */


    int p1x = 8, p1y = 16; //x and y for paddle1
    int p2x = 128-8 , p2y = 16; //x and y for paddle2
    int p1s = 2, p2s = 2; //speed for paddle1 and paddle2; used after integrating gyro
    int bx = BALLDEFAULTX, by = BALLDEFAULTY; //x and y for ball
    int bxv = -1, byv = 1; //velocity of ball; how many pixels it jumps each frame
    int grav = 0, air = 0; //variable that counts up to GRAVITY to reduce ball y vel, same with air resistance
    int i, j; //variables with work with
    int v; //getsw variable
    int w; //getbtns variable, these are so we dont call them multiple times, just check the value
    
    
    int score1 = 0, score2 = 0; //scores for player1 and player2

    int select = 1;
    int selection = 0;

    // written by praanto samadder
    while (!selection) //TITLE SCREEN STUFF BEGINS
    {
      w = getbtns();
      delay(100);
      if (select == 1)
      {
        display_string(0,"--TENNIS 4 2--");
        display_string(1,">2V2");
        display_string(2," A.I.");
        display_string(3," CREDITS");
        display_update();
      }
      else if (select == 2)
      {
        display_string(0,"--TENNIS 4 2--");
        display_string(1," 2V2");
        display_string(2,">A.I.");
        display_string(3," CREDITS");
        display_update();
      }
      else if (select == 3)
      {
        display_string(0,"--TENNIS 4 2--");
        display_string(1," 2V2");
        display_string(2," A.I.");
        display_string(3,">CREDITS");
        display_update();
      }

      if (w & 0b0001)
        selection = select;
      if (w & 0b1000)
        select = 1;
      else if (w & 0b0100)
        select = 2;
      else if (w & 0b0010)
        select = 3;

      if (selection == 3)
      {
        display_string(0,"WEB/PHONE STUFF");
        display_string(1,"praanto");
        display_string(2,"GAME STUFF");
        display_string(3,"alin n praanto");
        display_update();
        delay (5000);
        selection = 0;
      }

    } //TITLE SCREEN STUFF ENDS

    while (1) //MAIN LOOP
    {


    
   
    v = getsw();
    w = getbtns();

    myMemCpy(arraydata, arraydatazero, sizeof(arraydata)); //clears the screen to render new stuff on it

 
    //this code handles scoring and ball hitting the edges of the map
    //written by alin-cristian serban
    if (bx <= 2)
    {
        score2++;
         bx = BALLDEFAULTX;
         by = BALLDEFAULTY;
         display_string(0, itoaconv(score1));
         display_string(1, "^ PLAYER1 SCORE");
         display_string(2, itoaconv(score2)); 
         display_string(3, "^ PLAYER2 SCORE");
         display_update();
        delay(3000);
         bxv = -1;
         byv = 1;
        //bx = 125;
    }
    if (bx >= 126)
    {
        score1++;
         bx = BALLDEFAULTX;
         by = BALLDEFAULTY;
         display_string(0, itoaconv(score1));
         display_string(1, "^ PLAYER1 SCORE");
         display_string(2, itoaconv(score2)); 
         display_string(3, "^ PLAYER2 SCORE");
         display_update();
        delay(3000);
         bxv = -1;
         byv = 1;

        //bx = 3;
    }
    if (by >= 31) //ball hits floor
    {

        by = 30;
        if (byv > 3)
          byv = (byv * (-1)) + 2; //ball jumps up a bit
        else
          byv = -1;
    }
    if (by <= 1) //ball hits top
    {
        by = 2;
        byv = 1; //ball jumps off top and goes down
    }




    if (bxv > BALLMAXSPEED)
      bxv = BALLMAXSPEED - 1;

    

    //commented code for receiving gyroscope data and appropriating it to paddle coord
    //praanto said he'd limit the paddle coordinate maxes anyway so no need to cap them.
    /*
    p1x = gyro1x;
    p1y = gyro1y;
    p1s = gyro1speed;

    p2x = gyro2x;
    p2y = gyro2y;
    p2s = gyro2speed;



    
    
    */





    
    // written by praanto samadder
    if ((selection == 1)) //temporary code for receiving movement input for the paddles
    {
        if ((w & 0b0001) && p2x < 128 - 4) //d
          p2x++;
        if ((v & 0b0001) && p2y < 32 - PADDLELENGTH)  //s
          p2y++;
        if ((w & 0b0010) && p2x > 64 + 4)//a
          p2x--;
        if ((v & 0b0010) && p2y > 2) //w
          p2y--;
    }
    
        if ((w & 0b0100) && p1x < 64 - 4) //d
          p1x++;
        if ((v & 0b0100) && p1y < 32 - PADDLELENGTH)  //s
          p1y++;
        if ((w & 0b1000) && p1x > 0 + 4) //a
          p1x--;
        if ((v & 0b1000) && p1y > 2) //w
          p1y--;
    

    //written by alin-cristian serban
    if (selection == 2) // code for a.i.
    {
      select++;

      if (select == 10)
      {
        if ((by - p2y) > 0 && p2y < 32 - PADDLELENGTH)
        {
          p2y++;
        }
        if ((by - p2y-4) < 0)
        {
          p2y--;
        }
        p2x = 120;
        select = 0;
      }
    }


    
    



    // written by alin-cristian serban
    for (i = 0; i < PADDLELENGTH ; i++) //displays paddle length, 8 pixels down from where the origin is. the top pixel of the paddle is the origin.
      {
        arraydata[p1y+i][p1x] = 1;
        arraydata[p2y+i][p2x] = 1;
      }

    bx += bxv;
    by += byv;


    //written by alin-cristian serban
    grav++; //how fast the ball slows down updown
    if (grav == GRAVITY)
    {
        byv++;
        grav = 0;
    }
    air++; //how fast the ball slows down sideways
    if (air == AIRRESISTANCE)
    {
        if (bxv >= BALLMINIMUMSPEED)
        {
          bxv--;
        }
        if (bxv <= (BALLMINIMUMSPEED * (-1)))
        {
          bxv++;
        }
        air = 0;
    }

    //written by praanto samadder
    for (i = 23 ; i < 32 ; i++) //NET DRAWING
    {
      if (i % 2)
        arraydata[i][64] = 1;
      else
        arraydata[i][63] = 1;
    }
    if ((63 <= bx) && (64 >= bx) && (22 < by)) //NET LOGIC
    {
      bx = BALLDEFAULTX;
      by = BALLDEFAULTY;
      p1x = 8; //default values for the paddles
      p1y = 16;
      p2x = 128-8;
      p2y = 16; 
      delay(1000);

    }


    //written by alin-cristian serban
    if ((p1x-4 < bx) && (p1x+2 > bx) && (p1y - 1 < by) && (p1y + PADDLELENGTH + 1 > by)) //hit paddle1
        {
            bxv = bxv * (-1);
            bxv = bxv + p1s; //adds paddle speed to ball

          
              byv = byv + 5;
            

            bx = p1x + 3;

        }
    
    if ((p2x-2 < bx) && (p2x+4 > bx) && (p2y - 1 < by) && (p2y + PADDLELENGTH + 1 > by)) //hit paddle2
        {
            bxv = bxv * (-1);
            bxv = bxv - p2s;

         
            byv = byv + 5;
      
          
            bx = p2x - 3;
        }


    
    
    for (i = -1 ; i <= 1 ; i++) //displays ball
      for (j = -1 ; j <= 1 ; j++)
        arraydata[by+i][bx+j] = 1;
    

    delay(10);
    //PORTE += 0b00000001; //debug counter to make sure the code's running
    image_to_screen(arraydata);
    display_full_image(screen);
    delay(10);
    }
  //display_image(0,icon);
  //display_image(32,icon);
  //display_image(64,icon);
  //display_image(96,icon);
}
