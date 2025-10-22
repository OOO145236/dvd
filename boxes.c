#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <asm/termbits.h> 
#include <sys/ioctl.h>

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))


//create construct vector2 from 2 positions
struct Vector2{
int x;
int y;
};

//creating construct rectangle from 2 vector2`s
struct Rectangle{
struct Vector2 pos1;
struct Vector2 pos2;
};

//Create New Vector 2
struct Vector2 newVector2(int x,int y){
	struct Vector2 temp;
	temp.x = x;
	temp.y = y;
	return temp;
};

//Create New Rectangle
struct Rectangle newRectangle(struct Vector2 position1,struct Vector2 position2){
	struct Rectangle temp;
	temp.pos1 = position1;
	temp.pos2 = position2;
	return temp;
};

//get terminal size
struct Vector2 termSize(){
struct winsize ws;
ioctl(0, TIOCGWINSZ, &ws);
	return newVector2(ws.ws_col,ws.ws_row);
};

//sleep in miliseconds
void sleep_ms(int milliseconds){
usleep(milliseconds*1000);
};

//get lenght in array
int getLen(char str[]){
int len = 0;
while(str[len] != '\0') ++len;

return len;
};

//generate random number in selected range
int otherRand(int min, int max){
	int temp = rand();
	temp = temp % max;
	if (temp < min)
		temp = temp + (max-min);
	return temp;
}


//draw a box from rectangle struct
int drawBox(struct Rectangle Rectangle,char style){
	
int max_x,max_y;
int current_x, current_y;
char filled, empty, toprint;
struct Vector2 Start =  Rectangle.pos1;
struct Vector2 End =  Rectangle.pos2;
max_x = Rectangle.pos2.x - Rectangle.pos1.x;
max_y = Rectangle.pos2.y - Rectangle.pos1.y;

filled = style;
empty = ' ';

for(current_y = Start.y; current_y <= Start.y+max_y; current_y++){
	for(current_x = Start.x; current_x <= Start.x+max_x; current_x++){
		if((current_x == Start.x || current_x == End.x)||(current_y == Start.y || current_y == End.y)){
			toprint = filled;
		}	
		else{
			toprint = empty;
		}
		gotoxy(current_x,current_y);	
	
		putchar(toprint);
		fflush(stdout);
		//printf("%d,%d",current_x,current_y);
	}
}
	return 0;
}

//Vector2 sum
struct Vector2 sumVector2(struct Vector2 Vector1,struct Vector2 Vector2){
int x,y;
x = Vector1.x + Vector2.x;
y = Vector1.y + Vector2.y;
	struct Vector2 temp = newVector2(x,y);
	return temp;
}

//move rectangle
struct Rectangle moveRect(struct Rectangle Rectangle, struct Vector2 Vector2){
	struct Vector2 pos1,pos2;
	pos1 = sumVector2(Rectangle.pos1,Vector2);
	pos2 = sumVector2(Rectangle.pos2,Vector2);
	
	struct Rectangle temp = newRectangle(pos1,pos2);	
	return temp;
}

//checks collision of two rectangles returns 1 if coliding, and 0 if not 
bool colision(struct Rectangle Rect1, struct Rectangle Rect2){
	bool isColiding;
	if(Rect1.pos1.x == Rect2.pos2.x || Rect2.pos1.x ==Rect1.pos2.x || Rect1.pos1.y == Rect2.pos2.y || Rect2.pos1.y == Rect1.pos2.y)
		isColiding = true;
	else
		isColiding = false;

	return isColiding;
}


// [8]--[1]--[2]
//  |         |
// [7]  [0]  [3]
//  |         |
// [6]--[5]--[4]
// colision with side detection
int complexColision(struct Rectangle Rect1, struct Rectangle Rect2){
	int isColiding = 0;
	int temp;
	if(Rect1.pos1.x <= Rect2.pos2.x)
		isColiding = isColiding + 100;
	if(Rect1.pos2.x >= Rect2.pos1.x)
		isColiding = isColiding + 1;
	if(Rect1.pos1.y <= Rect2.pos2.y)
		isColiding = isColiding + 1000;
	if(Rect1.pos2.y >= Rect2.pos1.y)
		isColiding = isColiding + 10;

if (isColiding == 0) temp = 0;
else if (isColiding == 1000) temp = 1;
else if (isColiding == 1100) temp = 2;
else if (isColiding == 100) temp = 3;
else if (isColiding == 110) temp = 4;
else if (isColiding == 10) temp = 5;
else if (isColiding == 11) temp = 6;
else if (isColiding == 1) temp = 7;
else if (isColiding == 1001) temp = 8;

return temp;
}


//makes every coordinate in vector positive like -1 becomes 1 but if it initially equals 1 then it stays as 1
struct Vector2 positivise(struct Vector2 vector){
struct Vector2 temp;

if (vector.x < 0) temp.x = -vector.x;
else temp.x = vector.x;
if (vector.y < 0) temp.y = -vector.y;
else temp.y = vector.y;

return temp;
}

//mirrors vector based on side given by complexCollide()
struct Vector2 mirrorVector2(struct Vector2 vector, int side){
struct Vector2 temp;
int x,y;

if (side == 1 || side == 5) {x = vector.x; y = -vector.y;}
else if(side == 3 || side == 7) {x = -vector.x; y = vector.y;}
else if(side == 0){x = vector.x; y = vector.y;}
else {x = -vector.x; y = -vector.y;}

temp = newVector2(x,y);
return temp;
}


//main func
int main(){
	char gradient[] = "\\/";
	struct Vector2 size = termSize();
	//disable crsor
	printf("\e[?25l");
	clear();
	//create main rectangle and canvas
	struct Rectangle rect1, canvas; 
	canvas = newRectangle(size,newVector2(1,1));
	rect1 = newRectangle(newVector2(1,1),newVector2(10*2.4,10));
	rect1 = moveRect(rect1,newVector2(1,1));
	
	struct Vector2 test = newVector2(2,2);

	int colision;
	for(int i = 0; i < i+1; ++i){
	colision = complexColision(canvas,rect1);
	test = mirrorVector2(test,colision);
	rect1 = moveRect(rect1, test);
//	clear();

//debug gotoxy(0,0); printf("%d,%d,%d",rect1.pos1.y,rect1.pos1.x,colision);

	drawBox(rect1,gradient[(i%getLen(gradient))]);
	sleep_ms(1000/30);
	}
	return 0;	
}
