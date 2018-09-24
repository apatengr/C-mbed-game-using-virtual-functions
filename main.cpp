
// Created by Animesh Patel   ECE 2036     11/06/2017


#include "mbed.h"
#include "uLCD_4DGL.h"
#include <iostream>
#include "Speaker.h"
#include "PinDetect.h"
#include <cmath>
#include "MMA8452.h"

Serial pc(USBTX,USBRX);
uLCD_4DGL uLCD(p13, p14, p29);
double x = 0, y = 0, z = 0;
Speaker mySpeaker(p22); //PWM out
// Define graphical sprites
// Asteroid information
#define ASTEROID_HEIGHT 12
#define ASTEROID_WIDTH 15
#define SPRITE_MAX 15
// earth and explosion information
#define EARTH_WIDTH 10
#define EARTH_HEIGHT 10
#define EXPLOSION1_WIDTH 20
// Screen info
#define SCREEN_MAX 125
#define SCREEN_MIN 1
#define NUM_ASTEROIDS 4

#define Q 0x808000 //OLIVE
#define I 0x008000 //GREEN
#define S 0xC0C0C0 //SILVER

#define C 0x17202A //UFO GLASS
#define D 0x797D7F //DARK GREY
#define L 0x00FF00 //LIME
MMA8452 acc(p9, p10, 40000);  //instantiate an acc object!
#define P 0xFF00FF //K
#define R 0xF1C40F //YELLOW
#define O 0xF39C12 //ORANGE

#define G 0xAAB7B8 //GREY
#define _ 0x000000 //BLACK
#define X 0xFFFFFF //WHITE
#define B 0x0000FF //BLUE
#define r 0xFF0000 //RED

PinDetect pb1(p16);
//set parameters -- use these and don't change
int spaceship_earth1[EARTH_WIDTH*EARTH_HEIGHT] = {
    _,_,S,S,S,S,S,S,_,_,
    _,S,I,I,I,I,I,I,S,_,
    S,I,I,I,I,I,I,I,I,S,
    S,I,I,I,I,I,I,I,I,S,
    S,I,I,I,I,I,I,I,I,S,
    S,I,I,I,I,I,I,I,I,S,
    S,I,I,I,I,I,I,I,I,S,
    S,I,I,I,I,I,I,I,I,S,
    S,I,I,I,I,I,I,I,S,_,
    _,S,S,S,S,S,S,S,_,_
};

int bomb[EARTH_WIDTH*EARTH_HEIGHT] = {
    R,r,r,r,r,r,r,r,r,R,
    r,R,r,r,r,r,r,r,R,r,
    r,r,R,r,r,r,r,R,r,r,
    r,r,r,R,r,r,R,r,r,r,
    r,r,r,r,R,R,r,r,r,r,
    r,r,r,r,R,R,r,r,r,r,
    r,r,r,R,r,r,R,r,r,r,
    r,r,R,r,r,r,r,R,r,r,
    r,R,r,r,r,r,r,r,R,r,
    R,r,r,r,r,r,r,r,r,R,
};

int asteroid_sprite_1[ASTEROID_HEIGHT * ASTEROID_WIDTH] = {
    _,_,_,_,X,X,X,X,X,X,X,X,_,_,_,
    _,_,_,X,_,_,_,_,_,_,_,_,X,_,_,
    _,_,X,_,_,_,_,_,_,_,_,_,X,_,_,
    _,X,_,_,_,_,_,_,_,_,_,_,X,_,_,
    X,X,X,X,_,_,_,_,_,_,_,_,_,X,_,
    _,_,_,X,_,_,_,_,_,_,_,_,_,X,_,
    _,_,X,_,_,_,_,_,_,_,_,_,_,X,_,
    _,X,_,_,_,_,_,X,_,_,_,_,_,X,_,
    X,_,_,_,_,_,X,X,_,_,_,_,X,_,_,
    _,X,_,_,_,X,_,X,_,_,_,_,X,_,_,
    _,_,X,_,X,_,_,X,_,_,_,X,_,_,_,
    _,_,_,X,_,_,_,X,X,X,X,_,_,_,_
};

int asteroid_sprite_2[ASTEROID_HEIGHT * ASTEROID_WIDTH] = {
    _,_,_,_,X,_,_,_,_,_,_,_,_,_,_,
    _,_,_,X,_,X,_,_,_,_,_,_,_,_,_,
    _,_,X,_,_,_,X,X,X,X,X,X,X,_,_,
    _,X,_,_,_,_,_,_,_,_,_,_,_,X,_,
    X,_,_,_,_,_,_,_,_,_,_,_,X,_,_,
    _,X,_,_,_,_,_,_,_,_,_,X,_,_,_,
    _,_,X,_,_,_,_,_,_,_,_,_,X,_,_,
    _,X,_,_,_,_,_,_,_,_,_,_,_,X,_,
    X,_,_,_,_,_,_,_,_,_,_,_,_,_,X,
    _,X,_,_,_,_,_,_,_,_,_,_,_,X,_,
    _,_,X,_,_,_,X,X,X,X,X,X,X,_,_,
    _,_,_,X,X,X,_,_,_,_,_,_,_,_,_,
};

int asteroid_sprite_3[ASTEROID_HEIGHT * ASTEROID_WIDTH] = {
    _,_,_,_,_,X,X,X,X,_,_,_,_,_,_,
    _,_,_,_,_,X,_,_,_,X,X,X,X,_,_,
    _,_,_,_,_,_,X,_,_,_,_,_,X,X,_,
    _,X,X,X,X,X,X,_,_,_,X,X,_,_,_,
    X,_,_,_,_,_,_,_,_,X,_,_,_,_,_,
    X,_,_,_,_,_,_,_,_,X,_,_,_,_,_,
    X,_,_,_,_,_,_,_,_,_,X,X,_,_,_,
    X,_,_,_,_,_,_,_,_,_,_,_,X,X,_,
    _,X,_,_,_,_,_,X,X,_,_,_,_,X,X,
    _,_,X,_,_,_,X,_,_,X,_,_,_,X,_,
    _,_,_,X,_,X,_,_,_,_,X,_,X,_,_,
    _,_,_,_,X,_,_,_,_,_,_,X,_,_,_,
};   // end of array

int asteroid_sprite_4[ASTEROID_HEIGHT * ASTEROID_WIDTH] = {
    _,_,_,X,_,_,_,_,_,_,_,X,_,_,_,
    _,_,X,_,X,_,_,_,_,_,X,_,X,_,_,
    _,X,_,_,_,X,_,_,_,X,_,_,_,X,_,
    X,_,_,_,_,_,X,_,X,_,_,_,_,_,X,
    X,_,_,_,_,_,_,X,_,_,_,_,_,X,_,
    X,_,_,_,_,_,_,_,_,_,_,_,X,_,_,
    X,_,_,_,_,_,_,_,_,_,_,_,_,X,_,
    X,_,_,_,_,_,_,_,_,_,_,_,_,_,X,
    X,_,_,_,_,_,_,_,_,_,_,_,_,X,_,
    _,X,_,_,_,_,_,_,_,_,_,_,X,_,_,
    _,_,X,_,_,_,_,_,_,_,_,X,_,_,_,
    _,_,_,X,X,X,X,X,X,X,X,_,_,_,_,
};  // end of array

class ScreenObjects{
public:
    virtual void draw() = 0;
    virtual void update() = 0;
    int getX() {
        return x_pos;
    }
    
    int getY() {
        return y_pos;
    }
    
protected:
    int x_pos;
    int y_pos;
};

class AbstractAstroid:public ScreenObjects{
public:
    int deltaX;
    int deltaY;
};

class SpaceshipEarth:public ScreenObjects{
public:
    SpaceshipEarth() {
        x_pos = 63;
        y_pos = 63;
    }
    
    virtual void draw() {
        // spaceship_earth1 ()
        uLCD.BLIT(x_pos,y_pos, EARTH_WIDTH, EARTH_HEIGHT, spaceship_earth1);
    }
    
    virtual void update() {
        // erase the earth object
        uLCD.filled_rectangle(x_pos - 3,y_pos - 3, (x_pos+ EARTH_WIDTH), (y_pos + EARTH_HEIGHT),BLACK);
        // take the new measurement and make the new earth
        acc.readXYZGravity(&x,&y,&z); //notice this is passed by reference use pointers
        x_pos = (-1*x*50) + 63;
        y_pos = (-1*y*50) + 63;
    }
    
    int getX() {
        return x_pos;
    }
    
    int getY() {
        return y_pos;
    }
};

// going from most bottom (y = 125) and x random to upper left
class ConcreteAsteroid1: public AbstractAstroid{
public:
    // assign random positions for asteroids here in the constructor
    ConcreteAsteroid1() {
        x_pos = rand() % 110;
        ///y_pos = rand() % 110;
        y_pos = 125;
        
        // speed of the objects
        deltaX = rand() % 10;
        deltaY = rand() % 10;
    }
    
    virtual  void draw() {
        // asteroid 1
        uLCD.BLIT(x_pos, y_pos, ASTEROID_WIDTH, ASTEROID_HEIGHT, asteroid_sprite_1);
    }
    virtual  void update() {
        // ERASE THE OBJECT
        uLCD.filled_rectangle(x_pos,y_pos, (x_pos+ ASTEROID_WIDTH), (y_pos + ASTEROID_HEIGHT),BLACK);
        
        // get the new random positions for the x and the y
        // deltaX and deltaY should be random values since we want the object movement to be random
        x_pos = x_pos - rand()%10;  // x_pos = x_pos - deltaX
        y_pos = y_pos - rand()%10;   // y_pos = y_pos - deltaY
        
        if ((x_pos > 5) && (y_pos > 5)) {
            // create an image/sprite on a new location
            uLCD.BLIT(x_pos, y_pos, ASTEROID_WIDTH, ASTEROID_HEIGHT, asteroid_sprite_1);
        } else {
            x_pos = rand() % 110;
            y_pos = 125;
            draw();
            // create a new asteroid of the same type
        }
    }
    
    int getXPos() {
        return x_pos;
    }
    
    int getYPos() {
        return y_pos;
    }
};

// extreme upper (y = 0) and x random to bottom right
class ConcreteAsteroid2: public AbstractAstroid{
public:
    ConcreteAsteroid2() {
        x_pos = rand() % 110;
        y_pos = 4;
        deltaX = rand()%2;
        deltaY = rand()%2;
    }
    virtual void draw() {
        // astroid 2
        uLCD.BLIT(x_pos, y_pos, ASTEROID_WIDTH, ASTEROID_HEIGHT, asteroid_sprite_2);
    }
    virtual  void update() {
        // ERASE THE OBJECT
        uLCD.filled_rectangle(x_pos,y_pos, (x_pos+ ASTEROID_WIDTH), (y_pos + ASTEROID_HEIGHT),BLACK);
        // get the new random positions for the x and the y
        // deltaX and deltaY should be random values since we want the object movement to be random
        x_pos = x_pos + rand()%10;  // x_pos = x_pos + deltaX
        y_pos = y_pos + rand()%10;   // y_pos = y_pos + deltaY
        
        if ((x_pos < 120) && (y_pos < 120))
            // create an image/sprite on a new location
            uLCD.BLIT(x_pos, y_pos, ASTEROID_WIDTH, ASTEROID_HEIGHT, asteroid_sprite_2);
        
        else {
            // create a new asteroid of the same type
            x_pos = rand() % 110;
            y_pos = 4;
            draw();
        }
    }
    
    int getX() {
        return x_pos;
    }
    
    int getY() {
        return y_pos;
    }
};

//// going from extreme most left and random y to upper right
class ConcreteAsteroid3: public AbstractAstroid{
public:
    ConcreteAsteroid3() {
        // current position of the entering asteroids
        x_pos = 0;
        y_pos = 4 + rand() % 110;
        // speed of the objects
        deltaX = rand()%2;
        deltaY = rand()%2;
    }
    virtual void draw() {
        // astroid 3
        uLCD.BLIT(x_pos, y_pos, ASTEROID_WIDTH, ASTEROID_HEIGHT, asteroid_sprite_3);
    }  // end of function
    
    virtual void update() {
        // ERASE THE OBJECT
        //uLCD.BLIT(x_pos, y_pos, ASTEROID_WIDTH, ASTEROID_HEIGHT, BLACK);
        uLCD.filled_rectangle(x_pos,y_pos, (x_pos+ ASTEROID_WIDTH), (y_pos + ASTEROID_HEIGHT),BLACK);
        // get the new random positions for the x and the y
        // deltaX and deltaY should be random values since we want the object movement to be random
        x_pos = x_pos + rand()%7;  // x_pos = x_pos + deltaX going left to right
        y_pos = y_pos - rand()%10;   // y_pos = y_pos - deltaY going down to up
        // x_pos + rand() %3 = going left to right
        // y_pos - rand() %3 = going down to up
        
        if ((x_pos < 115) && (y_pos > 5))
            // create an image/sprite on a new location
            uLCD.BLIT(x_pos, y_pos, ASTEROID_WIDTH, ASTEROID_HEIGHT, asteroid_sprite_3);
        
        else {
            // create a new asteroid of the same type
            x_pos = 0;
            y_pos = 4 + rand() % 110;
            draw();
        }
    }
    
    //virtual  void destroy() {
    //        uLCD.filled_rectangle(x_pos,y_pos, (x_pos+ ASTEROID_WIDTH), (y_pos + ASTEROID_HEIGHT),BLACK);
    //        wait(1);
    //        draw();
    //    }
    
    int getX() {
        return x_pos;
    }
    
    int getY() {
        return y_pos;
    }
    
}; // end of class

// going from extreme right x = 125 and y random to bottom left
class ConcreteAsteroid4: public AbstractAstroid{
public:
    ConcreteAsteroid4() {
        // current positions of the objects
        x_pos = 125;
        y_pos = 4 + rand() % 120;
        
        // speed of the objects
        deltaX = rand()%2;
        deltaY = rand()%2;
    }
    
    virtual  void draw() {
        // astroid 4
        uLCD.BLIT(x_pos, y_pos, ASTEROID_WIDTH, ASTEROID_HEIGHT, asteroid_sprite_4);
    }   // end of draw
    
    virtual void update() {
        // ERASE THE OBJECT
        //uLCD.BLIT(x_pos, y_pos, ASTEROID_WIDTH, ASTEROID_HEIGHT, BLACK);
        uLCD.filled_rectangle(x_pos,y_pos, (x_pos+ ASTEROID_WIDTH), (y_pos + ASTEROID_HEIGHT),BLACK);
        // get the new random positions for the x and the y
        // deltaX and deltaY should be random values since we want the object movement to be random
        x_pos = x_pos - rand()%10;  // x_pos = x_pos - deltaX
        y_pos = y_pos + rand()%10;   // y_pos = y_pos - deltaY
        
        if ((x_pos > 5) && (y_pos < 120))
            // create an image/sprite on a new location
            uLCD.BLIT(x_pos, y_pos, ASTEROID_WIDTH, ASTEROID_HEIGHT, asteroid_sprite_4);
        
        else {
            x_pos = 125;
            y_pos = 4 + rand() % 120;
            draw();
            // create a new asteroid of the same type
        }
    }
    
    int getX() {
        return x_pos;
    }
    
    int getY() {
        return y_pos;
    }
    
}; // end of class

bool overlap (ScreenObjects &earth, ScreenObjects &ast){
    int a =  ((earth).getX());
    uLCD.printf("%d",a);
    // check to see if any asteroid is in with collisiion with the earth
    // if the last pixel of rhe ast in collision with the first pixel of the earth
    int xP1 = earth.getX() - 14;
    int yP1 = earth.getY() -12;
    int xP2 = earth.getX() + 14;
    int yP2 = earth.getY() + 12;
    int astX = ast.getX();
    int astY = ast.getY();
    
    if ((astX >  xP1) && (astX < xP2) && (astY > yP1) && (astY < yP2))
        return true;
    else
        return false;
    // if the bottom pixel if the asteroid is in collision with top pixel of the earth
    
}
/*
 // add an intrupt here
 void pb1_hit_callback()
 {
 if (!Bombused) {
 Explodeallasterois = true;
 BombUsed = true;
 
 }
 }
 */
// make a function here which creates explosive sound when earth collides with an asteroid

//DigitalOut myled(LED1);

int main(){
    SpaceshipEarth myEarth;
    acc.setBitDepth(MMA8452::BIT_DEPTH_12);
    acc.setDynamicRange(MMA8452::DYNAMIC_RANGE_4G);
    acc.setDataRate(MMA8452::RATE_100);
    
    srand(time(0));
    DigitalOut myled(LED1);
    uLCD.baudrate(300000);
    wait(0.2);
    
    pb1.mode(PullUp);
    Speaker mySpeaker(p21);
    //ConcreteAsteroid1 Asteroid1;
    //ConcreteAsteroid2 Asteroid2;
    //ConcreteAsteroid3 Asteroid3;
    //ConcreteAsteroid4 Asteroid4;
    ScreenObjects* ast[NUM_ASTEROIDS];
    ast[0] = new ConcreteAsteroid1;
    ast[1] = new ConcreteAsteroid2;
    ast[2] = new ConcreteAsteroid3;
    ast[3] = new ConcreteAsteroid4;
    
    bool collision = false;
    //  overlap(myEarth,Asteroid2);
    int ct = 0;
    
    uLCD.cls();
    // Draw 4 asteroids
    myEarth.draw();
    ast[0]->draw();
    ast[1]->draw();
    ast[2]->draw();
    ast[3]->draw();
    
    time_t startTime;
    startTime = time(0);
    // make a time bar length of 120 pixels and width of 3 pixels
    uLCD.filled_rectangle(0,0, 120, 3, BLUE);
    
    while(1) {
        time_t timeElapsed = time(0)-startTime;
        // if (MusicOn)
        // mySpeaker.PlayNote(440.0+x*music_factor,0.25+0.2*y,0.05);
        if (!pb1 && ct == 0) {
            ct = 1;
            mySpeaker.PlayNote(423.0,0.45,0.7);
            //kill all the asteroids here;
            
            uLCD.BLIT(ast[0]->getX(), ast[0]->getY() , EARTH_WIDTH, EARTH_HEIGHT,bomb);
            uLCD.BLIT(ast[1]->getX(), ast[1]->getY() , EARTH_WIDTH, EARTH_HEIGHT,bomb);
            uLCD.BLIT(ast[2]->getX(), ast[2]->getY() , EARTH_WIDTH, EARTH_HEIGHT,bomb);
            uLCD.BLIT(ast[3]->getX(), ast[3]->getY() , EARTH_WIDTH, EARTH_HEIGHT,bomb);
            
            wait(1.00);
            uLCD.filled_rectangle(ast[0]->getX()-3,ast[0]->getY()-3,ast[0]->getX()+ASTEROID_WIDTH+3,ast[0]->getY()+ASTEROID_HEIGHT+3,BLACK );
            uLCD.filled_rectangle(ast[1]->getX()-3,ast[1]->getY()-3,ast[1]->getX()+ASTEROID_WIDTH+3,ast[1]->getY()+ASTEROID_HEIGHT+3,BLACK );
            uLCD.filled_rectangle(ast[2]->getX()-3,ast[2]->getY()-3,ast[2]->getX()+ASTEROID_WIDTH+3,ast[2]->getY()+ASTEROID_HEIGHT+3,BLACK );
            uLCD.filled_rectangle(ast[3]->getX()-3,ast[3]->getY()-3,ast[3]->getX()+ASTEROID_WIDTH+3,ast[3]->getY()+ASTEROID_HEIGHT+3,BLACK );
            
            delete ast[0];
            delete ast[1];
            delete ast[2];
            delete ast[3];
            
            ast[0] = new ConcreteAsteroid1;
            ast[1] = new ConcreteAsteroid2;
            ast[2] = new ConcreteAsteroid3;
            ast[3] = new ConcreteAsteroid4;
        }
        myEarth.draw();
        myEarth.update();
        
        ast[0]->draw();
        ast[1]->draw();
        ast[2]->draw();
        ast[3]->draw();
        
        ast[0]->update();
        ast[1]->update();
        ast[2]->update();
        ast[3]->update();
        
        
        collision = (overlap(myEarth,ast[0])|| overlap(myEarth,ast[1])||overlap(myEarth,ast[2])||overlap(myEarth,ast[3]));
        
        
        if (collision){
            mySpeaker.PlayNote(400.0,0.45,69);
            uLCD.filled_rectangle(ast[0]->getX()-3,ast[0]->getY()-3,ast[0]->getX()+ASTEROID_WIDTH+3,ast[0]->getY()+ASTEROID_HEIGHT+3,BLACK );//erase the asteroid
            uLCD.BLIT(myEarth.getX(), myEarth.getY(), EARTH_WIDTH, EARTH_HEIGHT, bomb);
            uLCD.printf("\n\n\n\n\n Game Over\n");
            break;
        }
        
        
        uLCD.filled_rectangle(0,0,timeElapsed*(110.00/37.00),2,BLACK);// decrease time
        if(!collision && timeElapsed >=30) {
            uLCD.printf("\n\n\n\n\n  You Won\n") ;
            mySpeaker.PlayNote(4450.0,0.45,69);
            break;
        }
        
        
        uLCD.filled_rectangle(120,0, 120-((4*timeElapsed)), 3, BLACK);
        
        ast[0]-> update();
        ast[1]-> update();
        ast[2]-> update();
        ast[3]-> update();
        myEarth.draw();
        myEarth.update();
        
        
        
    } // end of while loop
    
    // wait (4.0);
}
