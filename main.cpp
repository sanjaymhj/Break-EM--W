#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include<iostream>
#include<string>
int rand(void);
//made the variable glodal because we need to vary every variable in different functions
#define digit 6
bool isPlaying=true;//for remaining in the game
bool isRunning=false;//for playing game
bool left=false, right= false, up= false,down= false;
int screen_x=1300,screen_y=700;//for screen size
float cha_x,cha_y,cha_w=100,cha_h=15;
int playarea_x1,playarea_y1,playarea_x2,playarea_y2;
int ball_x,ball_y,ball_w=10,ball_h=10,ball_velo_x=0,ball_velo_y=0,ball_speed_factor=0;
bool ball_launched=false;
int level_counter=1;//for level counting
bool level_var;
int BRICKS;
int bric_counter;
unsigned int pad_texture = 0,brick_texture0 = 0,brick_texture1 = 0,background = 0, credit=0, ball_texture = 0, hisc=0,lev= 0, crsc=0, number0 = 0, number1 = 0,number2 = 0 , number3 = 0 , number4 = 0 , number5 = 0,number6 = 0,number7 = 0,number8 = 0,number9 = 0;
char high_score[digit]={'0','0','0','0','0','0'},current_score[digit]={'0','0','0','0','0'};
int cr_sc=0,hi_sc;
int score_coor_x,score_coor_y;




struct Brick //Brick structures which holds 4 elements
{
  //Elements are used for x and y position of the brick and its width and height
  float x;
  float y;
  float width;
  float height;
  bool alive; //Alive variable. We use to to store in which state the brick is (broken, not broken)
  int tpye;
}b_01[50];

GLuint loadTexture( const std::string &fileName )
{
  SDL_Surface *image = IMG_Load( fileName.c_str() );

  SDL_DisplayFormatAlpha(image);

  unsigned object(0);

  glGenTextures(1, &object);

  glBindTexture(GL_TEXTURE_2D, object);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

  //Free surface
  SDL_FreeSurface(image);

  return object;
}

bool checkCollision(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float Bw, float Bh) //Funcfion for checking collision
{
  if ( Ay+Ah < By ) return false; //if A is more to the left than B
  else if ( Ay > By+Bh ) return false; //if A is more to the right than B
  else if ( Ax+Aw < Bx ) return false; //if A is higher than B
  else if ( Ax > Bx+Bw ) return false; //if A is lower than B

  return true; //There is a collision because none of above returned false
}

void initializevar()//variable initialization
{
    isPlaying=true;
    isRunning=false;
    left=false;
    right= false;
    up= false;
    down= false;
    cha_x=screen_x/2;
    cha_y=screen_y-cha_h-15;
    cha_w=100;
    cha_h=15;
    playarea_x1=0.5*screen_x/100;
    playarea_y1=0.5*screen_y/100;
    playarea_x2=0.795*screen_x;
    playarea_y2=0.99*screen_y;
    ball_x=cha_x;
    ball_y=cha_y-10;
    ball_w=10;
    ball_h=10;
    ball_launched=false;
    ball_velo_x=0;
    ball_velo_y=0;
    ball_speed_factor=0;
    level_var=false;
    //current_score[digit]={'0','0','0','0','0','0'};
}


void initializeSDL()//initialize SDL
{

    //initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

    //caption if the window
    SDL_WM_SetCaption("Break EM!", NULL );//name of window

    //size of the window
    SDL_SetVideoMode(screen_x,screen_y,32, SDL_OPENGL );//size of window to play

    glClearColor(1,1,1,1);//RED,GREEN,BLUE,ALPHA
    glViewport(0,0,screen_x,screen_y);//upper right and lower left

    glShadeModel(GL_SMOOTH);//smooth coloring

    glMatrixMode(GL_PROJECTION);//for 2d
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);//depth checking

    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //std::cout<<"openGL is running\n";
    //std::cout<<"Main Loop has started\n";
}

void initialmenu()//main menu
{
    SDL_Event event1;
    while( SDL_PollEvent(&event1))
    {
        if(event1.type == SDL_QUIT)
        {
            isPlaying= false;
            isRunning= false;
        }

        if((event1.type==SDL_KEYDOWN&&event1.key.keysym.sym==SDLK_SPACE))
        {
            isRunning=true;
            //SDL_Delay(500);//delay by 500 miliseconds
        }

        if(event1.type==SDL_KEYDOWN)//for game control keys
        {
            if(event1.key.keysym.sym==SDLK_LEFT)
            {
                left= true;
            }
            else if(event1.key.keysym.sym==SDLK_RIGHT)
            {
                right= true;
            }
            else if(event1.key.keysym.sym==SDLK_UP)
            {
                up= true;
            }
            else if(event1.key.keysym.sym==SDLK_DOWN)
            {
                down= true;
            }
        }
        if(event1.type==SDL_KEYUP)//for game control keys
        {
            if(event1.key.keysym.sym==SDLK_LEFT)
            {
                left= false;
            }
            else if(event1.key.keysym.sym==SDLK_RIGHT)
            {
                right= false;
            }
            else if(event1.key.keysym.sym==SDLK_UP)
            {
                up= false;
            }
            else if(event1.key.keysym.sym==SDLK_DOWN)
            {
                down= false;
            }
        }
    }
}

void launch_ball()//launching ball
{
    ball_velo_x=-3;
    ball_velo_y=-3;
}

void keypress()//check for key presses
{
    SDL_Event event;
    while( SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            isPlaying= false;
            isRunning= false;
        }
        //if a button was release and the button was escape, on release game will pause
        if(event.type==SDL_KEYUP && event.key.keysym.sym==SDLK_ESCAPE)
        {
            isRunning= false;
        }
        if((event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_SPACE))
        {
            //isRunning=true;
            if(ball_launched==false)
            {
                launch_ball();
                ball_launched=true;
            }
            //SDL_Delay(500);//delay by 500 miliseconds
        }
        if((event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_o))
        {
            ball_speed_factor+=0.5;
        }
        else if((event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_p))
        {
            ball_speed_factor-=0.5;
        }

        if((event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_LCTRL))
        {
            isRunning=false;
            initializevar();
        }
        if(event.type==SDL_KEYDOWN)//for game control keys
        {
            if(event.key.keysym.sym==SDLK_LEFT)
            {
                left= true;
            }
            else if(event.key.keysym.sym==SDLK_RIGHT)
            {
                right= true;
            }
            else if(event.key.keysym.sym==SDLK_UP)
            {
                up= true;
            }
            else if(event.key.keysym.sym==SDLK_DOWN)
            {
                down= true;
            }
        }
        if(event.type==SDL_KEYUP)//for game control keys
        {
            if(event.key.keysym.sym==SDLK_LEFT)
            {
                left= false;
            }
            else if(event.key.keysym.sym==SDLK_RIGHT)
            {
                right= false;
            }
            else if(event.key.keysym.sym==SDLK_UP)
            {
                up= false;
            }
            else if(event.key.keysym.sym==SDLK_DOWN)
            {
                down= false;
            }
        }
    }
}

void level_01()
{

    if(level_var==false)
    {
         bric_counter=BRICKS=12;
         for ( int n = 1, x = playarea_x1, y = playarea_y1; n <= BRICKS; n++, x+=40 ) //A for loop that goes throught the array so we can set the positions
        {
             b_01[n].width=40;
            b_01[n].height=20;
            b_01[n].tpye=1;
            b_01[n].alive=true;
      if ( x >= (playarea_x2-playarea_x1)-40 ) //If x is near the right edge of the screen
        {
          x = playarea_x1+5; //We start going from the left again
          y += 24; //And move a down a little
        }
      b_01[n].x = x; //Set currents bricks x position
      b_01[n].y = y; //Y position
      ball_speed_factor=0.2;
    }
        level_var=true;
    }


}

void level_02()
{

    if(level_var==false)
    { bric_counter=BRICKS=40;
        for(int j=1;j<=BRICKS/2;j++)
        {
            b_01[j].width=40;
            b_01[j].height=20;
            b_01[j].x=j*b_01[j].width;
            b_01[j].y=j*b_01[j].height;
            b_01[j].tpye=1;
            b_01[j].alive=true;

        }
        for(int j=BRICKS;j>=BRICKS/2;j--)
        {
            b_01[j].width=40;
            b_01[j].height=20;
            b_01[j].x=(48-j)*b_01[j].width;
            b_01[j].y=(j-11)*b_01[j].height;
            b_01[j].tpye=1;
            b_01[j].alive=true;
        }
        ball_speed_factor=0.4;
        level_var=true;
    }

}

void level_03()
{
   if(level_var==false)
    {  bric_counter=BRICKS=52;
        for(int j=1;j<=BRICKS;j++)
        {
            b_01[j].width=40;
            b_01[j].height=20;
            b_01[j].tpye=1;
            b_01[j].alive=true;
        }
        for ( int n = 1, x = playarea_x1, y = playarea_y1; n <= BRICKS; n++, x+=40 ) //A for loop that goes throught the array so we can set the positions
        {
      if ( x >= (playarea_x2-playarea_x1)-40 ) //If x is near the right edge of the screen
        {
          x = playarea_x1+5; //We start going from the left again
          y += 24; //And move a down a little
        }
      b_01[n].x = x; //Set currents bricks x position
      b_01[n].y = y+playarea_y2/2-50; //Y position
       ball_speed_factor=0.6;
    }


        level_var=true;
    }
}

void level_04()
{

    if(level_var==false)
    { bric_counter=BRICKS=24;
        for(int j=1;j<=BRICKS/2;j++)
        {
            b_01[j].width=40;
            b_01[j].height=20;
            b_01[j].x=b_01[j].width;
            b_01[j].y=j*b_01[j].height;
            b_01[j].tpye=1;
            b_01[j].alive=true;

        }
        for(int j=BRICKS;j>=BRICKS/2;j--)
        {
            b_01[j].width=40;
            b_01[j].height=20;
            b_01[j].x=(BRICKS-j)*b_01[j].width;
            b_01[j].y=b_01[j].height;
            b_01[j].tpye=1;
            b_01[j].alive=true;
        }
        ball_speed_factor=0.4;
        level_var=true;
    }

}
void pad_01()
{
    ball_velo_x=-2.5+ball_speed_factor;
    ball_velo_y=-1+ball_speed_factor;
}
void pad_11()
{
   ball_velo_x=2.5+ball_speed_factor;
    ball_velo_y=-1+ball_speed_factor;
}
void pad_02()
{
    ball_velo_x=-2+ball_speed_factor;
    ball_velo_y=-1.5+ball_speed_factor;
}
void pad_10()
{
    ball_velo_x=2+ball_speed_factor;
    ball_velo_y=-1.5+ball_speed_factor;
}

void pad_03()
{
    ball_velo_x=-1.5+ball_speed_factor;
    ball_velo_y=-2.5+ball_speed_factor;
}
void pad_09()
{
    ball_velo_x=1.5+ball_speed_factor;
    ball_velo_y=-2.5+ball_speed_factor;
}

void pad_04()
{
    ball_velo_x=-1+ball_speed_factor;
    ball_velo_y=-2.5+ball_speed_factor;
}
void pad_08()
{
     ball_velo_x=1+ball_speed_factor;
    ball_velo_y=-2.5+ball_speed_factor;
}

void pad_05()
{
    ball_velo_x=-0.5+ball_speed_factor;
    ball_velo_y=-2.5+ball_speed_factor;
}
void pad_07()
{
    ball_velo_x=0.5+ball_speed_factor;
    ball_velo_y=-2.5+ball_speed_factor;
}
void pad_06()
{
    ball_velo_x=0+ball_speed_factor;
    ball_velo_y=-3+ball_speed_factor;
}


void addscore(int a)
{
    int r,q;
    if(a==1)
    {
        cr_sc+=a*5;
    }
        r=cr_sc;
        for(int aa=6;aa>=0;aa--)
        {
            q=r%10;
            r=r/10;
            current_score[aa]=48+q;
        }
}

void beat_score(int a)
{
    int q,r;
    r=a;
    for(int aa=digit;aa>=0;aa--)
        {
            q=r%10;
            r=r/10;
            high_score[aa]=48+q;
        }
}

void pad_movement()
{

    if(right==true)
    {
        cha_x=cha_x+5;
    }
    if(left==true)
    {
        cha_x=cha_x-5;
    }
    if(cha_x<=0.5*screen_x/100)
    {
        cha_x=0.5*screen_x/100;
    }
    else if(cha_x+cha_w>=0.795*screen_x)
    {
        cha_x=0.795*screen_x-cha_w;
    }
    if(ball_velo_x==0 && ball_velo_y==0)
    {
        ball_x=cha_x+20;
        ball_y=cha_y-10;
    }
}

void gamelogic()//main game
{
    pad_movement();
    ball_x=ball_x+ball_velo_x;

      for ( int n = 1; n <= BRICKS; n++ ) //Go throught the array of bricks
        {
          if ( b_01[n].alive == true ) //If the bricks is alive
            {
              if ( checkCollision(ball_x,ball_y,ball_w,ball_h,b_01[n].x,b_01[n].y,b_01[n].width,b_01[n].height) == true ) //Check for collision with the ball
                {
                  ball_velo_x = -ball_velo_x; //Change x velocity of the ball
                  b_01[n].alive = false; //Set the alive variable to false (brick is broken)
                  addscore(1);
                  break; //Stop checking for collision on x axis
                }
            }
        }
    ball_y=ball_y+ball_velo_y;
    for ( int n = 1; n <= BRICKS; n++ ) //Go throught the array of bricks
        {
          if ( b_01[n].alive == true ) //If the bricks is alive
            {
              if ( checkCollision(ball_x,ball_y,ball_w,ball_h,b_01[n].x,b_01[n].y,b_01[n].width,b_01[n].height) == true ) //Check for collision with the ball
                {
                  ball_velo_y = -ball_velo_y; //Change x velocity of the ball
                  b_01[n].alive = false; //Set the alive variable to false (brick is broken)
                  addscore(1);
                  break; //Stop checking for collision on y axis

                }
            }
        }

    if(checkCollision(ball_x,ball_y,ball_w,ball_h,0,0,playarea_x1,playarea_y2))
    {
        ball_velo_x=-ball_velo_x;
    }
    else if(checkCollision(ball_x,ball_y,ball_w,ball_h,0,0,playarea_x2,playarea_y1))
    {
        ball_velo_y=-ball_velo_y;
    }

    else if(checkCollision(ball_x,ball_y,ball_w,ball_h,playarea_x2,0,playarea_x2,playarea_y2))
    {
        ball_velo_x=-ball_velo_x;
    }

    else if(checkCollision(ball_x,ball_y,ball_w,ball_h,playarea_x1,playarea_y2,playarea_x2,5))//bottom of the window
    {
        level_counter=1;
        cr_sc=0;
        addscore(0);
        initializevar();
    }

    else if(checkCollision(ball_x,ball_y,ball_w,ball_h,cha_x,cha_y,cha_w,cha_h))
    {
        if(cha_x<=(ball_x+ball_h) && cha_x+0.09*cha_w*5>=ball_x+ball_h)
        {
           if(cha_x<=(ball_x+ball_h)&&cha_x+0.09*cha_w>=ball_x+ball_h)
           {
               pad_01();
           }
           else if(cha_x+0.09*cha_w<=(ball_x+ball_h)&&cha_x+0.09*cha_w*2>=ball_x+ball_h)
           {
               pad_02();
           }
           else if(cha_x+0.09*cha_w*2<=(ball_x+ball_h)&&cha_x+0.09*cha_w*3>=ball_x+ball_h)
           {
               pad_03();
           }
           else if(cha_x+0.09*cha_w*3<=(ball_x+ball_h)&&cha_x+0.09*cha_w*4>=ball_x+ball_h)
           {
               pad_04();
           }else if(cha_x+0.14*cha_w*4<=(ball_x+ball_h)&&cha_x+0.14*cha_w*5>=ball_x+ball_h)
           {
               pad_05();
           }


        }
        else if(cha_x+0.09*cha_w*6<=ball_x+ball_h&&cha_x+cha_w>=ball_x+ball_h)
        {
            if(cha_x+0.09*cha_w*6<=(ball_x+ball_h)&&cha_x+0.09*cha_w*7>=ball_x+ball_h)
           {
               pad_07();
           }
           else if(cha_x+0.09*cha_w*7<=(ball_x+ball_h)&&cha_x+0.09*cha_w*8>=ball_x+ball_h)
           {
               pad_08();
           }
           else if(cha_x+0.09*cha_w*8<=(ball_x+ball_h)&&cha_x+0.09*cha_w*9>=ball_x+ball_h)
           {
               pad_09();
           }
           else if(cha_x+0.09*cha_w*9<=(ball_x+ball_h)&&cha_x+0.09*cha_w*10>=ball_x+ball_h)
           {
               pad_10();
           }
           else if(cha_x+0.09*cha_w*10<=(ball_x+ball_h)&&cha_x+0.09*cha_w*11>=ball_x+ball_h)
           {
               pad_11();
           }
        }
        else
        {
            pad_06();
            ball_velo_x=+ball_velo_x;
        }
    }


    for(int I=1;I<=BRICKS;I++)
    {
        if(b_01[I].alive!=true)
        {
            bric_counter=bric_counter-1;
        }
    }


    if(bric_counter==0)
    {
          level_counter++;
          SDL_Delay(500);
          initializevar();
    }
    if(level_counter==1 && level_var==false)
    {
            level_01();
    }
    else if(level_counter==2 && level_var==false)
    {
        level_02();
    }
    else if(level_counter==3 && level_var==false)
    {
        level_03();
    }
    else if(level_counter==4 && level_var==false)
    {
        level_04();
    }
    bric_counter=BRICKS;

}

void rendering()//rendering on the screen
{
    //rendering to the screen
    glClear(GL_COLOR_BUFFER_BIT);
    //render
    glPushMatrix();//start phase

    glOrtho(0,screen_x,screen_y,0,-1,1);//set the matrix
    glColor4ub(255,255,255,255);

    glEnable(GL_TEXTURE_2D);
    //What texture we are going to use
    glBindTexture(GL_TEXTURE_2D,background);
    //glColor4ub(25,150,5,255);
    glBegin(GL_QUADS);
    {
        glTexCoord2d(0,0); glVertex2f(0,0);
        glTexCoord2d(1,0); glVertex2f(screen_x,0);
        glTexCoord2d(1,1); glVertex2f(screen_x,screen_y);
        glTexCoord2d(0,1); glVertex2f(0,screen_y);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //wall of play area
    glBegin(GL_QUADS);
    {
        glColor4ub(200,200,200,255);
        glVertex2f(0,0);
        glVertex2f(0.5*screen_x/100,0);
        glVertex2f(0.5*screen_x/100,0.99*screen_y);
        glVertex2f(0,0.99*screen_y);

        glVertex2f(0,0);
        glVertex2f(0.8*screen_x,0);
        glVertex2f(0.8*screen_x,0.5*screen_y/100);
        glVertex2f(0,0.5*screen_y/100);

        glVertex2f(0.795*screen_x,0);
        glVertex2f(0.8*screen_x,0);
        glVertex2f(0.8*screen_x,0.99*screen_y);
        glVertex2f(0.795*screen_x,0.99*screen_y);
    }
    glEnd();
    glColor4ub(255,255,255,255);
    glEnable(GL_TEXTURE_2D);
    //breaking bar

    //What texture we are going to use


    glBindTexture(GL_TEXTURE_2D,pad_texture);
    glBegin(GL_QUADS);
    {

        glTexCoord2d(0,0); glVertex2f(cha_x,cha_y);
        glTexCoord2d(1,0); glVertex2f(cha_x+cha_w,cha_y);
        glTexCoord2d(1,1);  glVertex2f(cha_x+cha_w,cha_y+cha_h);
        glTexCoord2d(0,1); glVertex2f(cha_x,cha_y+cha_h);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    //What texture we are going to use
    glBindTexture(GL_TEXTURE_2D,ball_texture);
    glBegin(GL_QUADS);
    {
        glTexCoord2d(0,0); glVertex2f(ball_x,ball_y);
         glTexCoord2d(1,0); glVertex2f(ball_x+ball_w,ball_y);
        glTexCoord2d(1,1);  glVertex2f(ball_x+ball_w,ball_y+ball_h);
       glTexCoord2d(0,1);  glVertex2f(ball_x,ball_y+ball_h);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);

    //render bricks
    int x=0;
    glEnable(GL_TEXTURE_2D);
    //What texture we are going to use

    glBindTexture(GL_TEXTURE_2D,brick_texture0);
    glBegin(GL_QUADS);
    {
         /*if(x==0)
            {
                glBindTexture(GL_TEXTURE_2D,brick_texture0);
            }
            else if(x==1)
            {
                 glBindTexture(GL_TEXTURE_2D,brick_texture1);
            }*/
        for(int j=1;j<=BRICKS;j++)
        {


            if(b_01[j].alive==true)
            {
                glTexCoord2d(0,0); glVertex2f(b_01[j].x,b_01[j].y);
                glTexCoord2d(1,0); glVertex2f(b_01[j].x+b_01[j].width,b_01[j].y);
                glTexCoord2d(1,1);  glVertex2f(b_01[j].x+b_01[j].width,b_01[j].y+b_01[j].height);
                glTexCoord2d(0,1);  glVertex2f(b_01[j].x,b_01[j].y+b_01[j].height);
            }
        }
    //}
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //high score display
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,hisc);
    glBegin(GL_QUADS);
    {
        glTexCoord2d(0,0); glVertex2f(playarea_x2+60,playarea_y1+5);
         glTexCoord2d(1,0); glVertex2f(playarea_x2+180,playarea_y1+5);
        glTexCoord2d(1,1);  glVertex2f(playarea_x2+180,playarea_y1+45);
       glTexCoord2d(0,1);  glVertex2f(playarea_x2+60,playarea_y1+45);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);


    glEnable(GL_TEXTURE_2D);
    score_coor_x=playarea_x2+10;
    score_coor_y=playarea_y1+50;
    beat_score(hi_sc);
    int index=digit+1;
    for(int i=0;i<=digit;i++)
    {
        if(high_score[i]!='0')
        {
            index=i;
            i=digit+1;
        }
    }
    for(int i=index;i<=digit;i++)
    {
        switch(high_score[i])
        {
            case '0':
                glBindTexture(GL_TEXTURE_2D,number0);
                break;
            case '1':
                glBindTexture(GL_TEXTURE_2D,number1);
                break;
            case '2':
                glBindTexture(GL_TEXTURE_2D,number2);
                break;
            case '3':
                glBindTexture(GL_TEXTURE_2D,number3);
                break;
            case '4':
                glBindTexture(GL_TEXTURE_2D,number4);
                break;
            case '5':
                glBindTexture(GL_TEXTURE_2D,number5);
                break;
            case '6':
                glBindTexture(GL_TEXTURE_2D,number6);
                break;
            case '7':
                glBindTexture(GL_TEXTURE_2D,number7);
                break;
            case '8':
                glBindTexture(GL_TEXTURE_2D,number8);
                break;
            case '9':
                glBindTexture(GL_TEXTURE_2D,number9);
                break;
        }
        glBegin(GL_QUADS);
        {
            glTexCoord2d(0,0); glVertex2f(score_coor_x,score_coor_y);
            glTexCoord2d(1,0); glVertex2f(score_coor_x+30,score_coor_y);
            glTexCoord2d(1,1);  glVertex2f(score_coor_x+30,score_coor_y+60);
            glTexCoord2d(0,1);  glVertex2f(score_coor_x,score_coor_y+60);
        }
        glEnd();
        score_coor_x+=30;
    }
    glDisable(GL_TEXTURE_2D);

    //currentscore
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,crsc);
    glBegin(GL_QUADS);
    {
        glTexCoord2d(0,0); glVertex2f(playarea_x2+60,playarea_y1+110);
         glTexCoord2d(1,0); glVertex2f(playarea_x2+180,playarea_y1+110);
        glTexCoord2d(1,1);  glVertex2f(playarea_x2+180,playarea_y1+150);
       glTexCoord2d(0,1);  glVertex2f(playarea_x2+60,playarea_y1+150);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    score_coor_x=playarea_x2+10;
    score_coor_y=playarea_y1+150;
    addscore(0);
    glBindTexture(GL_TEXTURE_2D,background);
    index=digit;
    for(int i=0;i<=digit;i++)
    {
        if(current_score[i]!='0')
        {
            index=i;
            i=digit+1;
        }
    }

    for(int i=index;i<=digit;i++)
    {
        switch(current_score[i])
        {
            case '0':
                glBindTexture(GL_TEXTURE_2D,number0);
                break;
            case '1':
                glBindTexture(GL_TEXTURE_2D,number1);
                break;
            case '2':
                glBindTexture(GL_TEXTURE_2D,number2);
                break;
            case '3':
                glBindTexture(GL_TEXTURE_2D,number3);
                break;
            case '4':
                glBindTexture(GL_TEXTURE_2D,number4);
                break;
            case '5':
                glBindTexture(GL_TEXTURE_2D,number5);
                break;
            case '6':
                glBindTexture(GL_TEXTURE_2D,number6);
                break;
            case '7':
                glBindTexture(GL_TEXTURE_2D,number7);
                break;
            case '8':
                glBindTexture(GL_TEXTURE_2D,number8);
                break;
            case '9':
                glBindTexture(GL_TEXTURE_2D,number9);
                break;
        }
        glBegin(GL_QUADS);
        {
            glTexCoord2d(0,0); glVertex2f(score_coor_x,score_coor_y);
            glTexCoord2d(1,0); glVertex2f(score_coor_x+30,score_coor_y);
            glTexCoord2d(1,1);  glVertex2f(score_coor_x+30,score_coor_y+60);
            glTexCoord2d(0,1);  glVertex2f(score_coor_x,score_coor_y+60);
        }
        glEnd();
        score_coor_x+=30;
    }
    glDisable(GL_TEXTURE_2D);

    //level count display
    glEnable(GL_TEXTURE_2D);
    score_coor_x=playarea_x2+10;
    score_coor_y=playarea_y2/2;
    switch(level_counter)
        {
            case 0:
                glBindTexture(GL_TEXTURE_2D,number0);
                break;
            case 1:
                glBindTexture(GL_TEXTURE_2D,number1);
                break;
            case 2:
                glBindTexture(GL_TEXTURE_2D,number2);
                break;
            case 3:
                glBindTexture(GL_TEXTURE_2D,number3);
                break;
            case 4:
                glBindTexture(GL_TEXTURE_2D,number4);
                break;
            case 5:
                glBindTexture(GL_TEXTURE_2D,number5);
                break;
            case 6:
                glBindTexture(GL_TEXTURE_2D,number6);
                break;
            case 7:
                glBindTexture(GL_TEXTURE_2D,number7);
                break;
            case 8:
                glBindTexture(GL_TEXTURE_2D,number8);
                break;
            case 9:
                glBindTexture(GL_TEXTURE_2D,number9);
                break;
        }
        glBegin(GL_QUADS);
        {
            glTexCoord2d(0,0); glVertex2f(score_coor_x,score_coor_y);
            glTexCoord2d(1,0); glVertex2f(score_coor_x+30,score_coor_y);
            glTexCoord2d(1,1);  glVertex2f(score_coor_x+30,score_coor_y+60);
            glTexCoord2d(0,1);  glVertex2f(score_coor_x,score_coor_y+60);
        }
        glEnd();
    glDisable(GL_TEXTURE_2D);

    //level count display
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,lev);
    glBegin(GL_QUADS);
    {
        //glColor4ub(205,10,5,255);
            glTexCoord2d(0,0); glVertex2f(playarea_x2+60,playarea_y2/2-40);
            glTexCoord2d(1,0); glVertex2f(playarea_x2+180,playarea_y2/2-40);
            glTexCoord2d(1,1);  glVertex2f(playarea_x2+180,playarea_y2/2);
            glTexCoord2d(0,1);  glVertex2f(playarea_x2+60,playarea_y2/2);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);


    //credit display
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,credit);
    glBegin(GL_QUADS);
    {
        //glColor4ub(205,10,5,255);
        glTexCoord2d(0,0);  glVertex2f(playarea_x2, playarea_y2-200);
        glTexCoord2d(1,0);  glVertex2f(playarea_x2+250,playarea_y2-200);
        glTexCoord2d(1,1);  glVertex2f(playarea_x2+250,playarea_y2);
        glTexCoord2d(0,1);  glVertex2f(playarea_x2,playarea_y2);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();//end phase

    SDL_GL_SwapBuffers();

    SDL_Delay(5);//delay by 5 miliseconds

}
}

void mainloop()//main loop
{
    //for handeling the game loop
    initialmenu();
    rendering();
    pad_movement();

    if(level_counter==1 && level_var==false)
    {
            level_01();
    }
    else if(level_counter==2 && level_var==false)
    {
        level_02();
    }
    else if(level_counter==3 && level_var==false)
    {
        level_03();
    }
    else if(level_counter==4 && level_var==false)
    {
        level_04();
    }
    while(isRunning==true)
    {
        keypress();//check for key press
        gamelogic();//game logic
        rendering();//rendering on screen
        if(cr_sc>=hi_sc)
        {
            hi_sc=cr_sc;
            beat_score(hi_sc);
           // addscore(0);
        }
    }

}


int main(int argc, char* args[])
{
    initializeSDL();
    initializevar();
    cha_x=screen_x/2;
    cha_y=screen_y-cha_h-15;
    //Load the image into the texture using the function
    pad_texture = loadTexture("ball.png");
    background = loadTexture("playarea.png");
    ball_texture = loadTexture("ball.png");
    brick_texture0 = loadTexture("brick0.png");
    brick_texture1 = loadTexture("brick1.png");
    hisc = loadTexture("hisc.png");
    crsc = loadTexture("yrsc.png");
    credit= loadTexture("credit.png");
    lev = loadTexture("lev.png");
    number0 = loadTexture("0.png");
    number1 = loadTexture("1.png");
    number2 = loadTexture("2.png");
    number3 = loadTexture("3.png");
    number4 = loadTexture("4.png");
    number5 = loadTexture("5.png");
    number6 = loadTexture("6.png");
    number7 = loadTexture("7.png");
    number8 = loadTexture("8.png");
    number9 = loadTexture("9.png");
    //std::cin>>hi_sc;
    while(isPlaying==true)
    {
        mainloop();
    }
    std::cout<<hi_sc;
    SDL_Quit();
    return 0;
}
