#include "snake.h"

CSnake::CSnake(CRect r, char _c ):   /*=' '*/
  CFramedWindow(r, _c)
{
    start();
}

void CSnake::start(){   //początek gry i wywołanie węża
    snake.clear();
    srand(time(nullptr));
    CDesktop::speedness = 300;
    rand_point();
    level = 1;
    game_over = false;
    geom.topleft.x += x;
    geom.topleft.y += y;
    food.x += x;
    food.y += y;
    prevMove = 2;
    dir = CPoint(geom.topleft.x, geom.topleft.y);
    snake.push_front(CPoint(2,2) + dir);
    snake.push_front(snake.front()+CPoint(1,0));
    snake.push_front(snake.front()+CPoint(1,0));
}

void CSnake::paint() {
    CWindow::paint();
    int a = geom.topleft.x, b = geom.topleft.y;
    gotoyx(b,a);
    if(level == 10)
    {
        gotoyx(b+1,a+1);
        printl("YOU WON, result: %d",level);
        pause = 1;
        sleep(2);
        start();
    }
    if (game_over){
            gotoyx(b+1,a+1);
            printl("GAME OVER, result: %d",level);
            refresh();
            sleep(2);
            start();
    }
    if(pause)
    {
        if (game_over){
            gotoyx(b+1,a+1);
            printl("GAME OVER, result: %d",level);
            sleep(2);
            start();
        }
        else
        {
            gotoyx(geom.topleft.y-1,geom.topleft.x);
            printl("| LEVEL: %d |",level);
            gotoyx(geom.topleft.y+5,geom.topleft.x+5);
            printl("h - toggle help information");
            gotoyx(geom.topleft.y+6,geom.topleft.x+5);
            printl("p - toggle pause/play mode");
            gotoyx(geom.topleft.y+7,geom.topleft.x+5);
            printl("r - restart game");
            gotoyx(geom.topleft.y+8,geom.topleft.x+5);
            printl("arrows - move snake (in play mode) or");
            gotoyx(geom.topleft.y+9,geom.topleft.x+5);
            printl("         move window (in pause mode)");
        }
    }
    else{
        gotoyx(geom.topleft.y-1,geom.topleft.x);
        printl("| LEVEL: %d |",level);
        for (list<CPoint>::iterator i = snake.begin(); i != snake.end(); ++i) {
            gotoyx(i->y,i->x);
            if(i==snake.begin()){	
            printl("*");
		}
            else {
				printl("+");
			}
        }
        gotoyx(food.y,food.x);
        printl("O");
    }
}

void CSnake::move(CPoint a) {   //poruszanie wężem i przechodzenie przez ściany
    if(!pause) {
        int validate = validateMovement(a);
        if(!validate)
        {
            pause = 1;
            game_over = true;
            paint();
            return;
        }
        CPoint next = snake.front();
        next += a;

        if (snake.size() > 0) {
            if (next.x == food.x && next.y == food.y) {
                level++;
                snake.push_front(next);
                CDesktop::speedness -= 20;
                rand_point();
            } else {
                if (next.x == geom.topleft.x - 1) {
                    next.x = geom.topleft.x + geom.size.x - 1;
                } else if (next.y == geom.topleft.y - 1) {
                    next.y = geom.topleft.y + geom.size.y - 1;
                } else if (next.x == geom.topleft.x + geom.size.x) {
                    next.x = geom.topleft.x;
                } else if (next.y == geom.topleft.y + geom.size.y) {
                    next.y = geom.topleft.y;
                }
                snake.push_front(next);
                snake.pop_back();
            }
        }
    }
}

int CSnake::setPrevMove(CPoint a)
{
	if((a.x == 0 && a.y == -1 && prevMove == 3) || (a.x == 0 && a.y == 1 && prevMove == 1) || 
	(a.x == 1 && a.y == 0 && prevMove == 4) || (a.x == -1 && a.y == 0 && prevMove == 2))
	{
		return -1;
	}
	if(a.x == 0 && a.y == -1)
	{
		return 1;
	}
	else if(a.x == 0 && a.y == 1)
	{
		return 3;
	}
	else if(a.x == 1 && a.y == 0)
	{
		return 2;
	}
	else if(a.x == -1 && a.y == 0)
	{
		return 4;
	}
	return 0;
}

bool CSnake::handleMovement(CPoint a)
{
	if (pause == 1) {
		if(a.x == 1)
		{
			x++;
		}
		else if(a.x == -1)
		{
			x--;
		}
		else if(a.y == 1)
		{
			y++;
		}
		else if(a.y == -1)
		{
			y--;
		}
		CView::move(a);
	}
	else {
		int setPrev = setPrevMove(a);
		if(setPrev == -1)
		{
			return false;
		}
		prevMove = setPrev;
		move(a);
	}
	return true;
}

bool CSnake::handleSnakeReplacement(void)
{
	if (pause == 1)
	{
		pause = 0;
		for (list<CPoint>::iterator i = snake.begin(); i != snake.end(); ++i) {
			i->x += x;
			i->y += y;
		}
		food.x += x;
		food.y += y;
		x = 0;
		y = 0;
	}
	else
	{
		pause = 1;
	}
	return true;
}

bool CSnake::handleEvent(int key)
{
    switch (key)
    {
        case 'r': case 'R':
            start();
            break;
        case 'p': case 'P': case 'h': case 'H':
			return handleSnakeReplacement();
        case KEY_UP:
			return handleMovement(CPoint(0, -1));
        case KEY_DOWN:
			return handleMovement(CPoint(0, 1));
        case KEY_RIGHT:
			return handleMovement(CPoint(1, 0));
        case KEY_LEFT:
			return handleMovement(CPoint(-1, 0));
        default:
        
			if(!pause && !game_over)
			{
				switch(prevMove)
				{
					case 1:
						move (CPoint (0, -1));
						break;
					case 2:
						move (CPoint (1, 0));
						break;
					case 3:
						move (CPoint (0, 1));
						break;
					case 4:
						move (CPoint (-1, 0));
						break;
				}
				paint();
			}
			break;
    }
    return false;
}

void CSnake::rand_point(){
    int iterator = 0, counter=0;
    while(counter != 100000)
    {
        food.x = rand()%(geom.size.x - 1 - geom.topleft.x)+geom.topleft.x + 1;
        food.y = rand()%(geom.size.y - 1 - geom.topleft.y)+geom.topleft.y + 1;
        for (list<CPoint>::iterator i = snake.begin(); i != snake.end(); ++i) {
            if (i->x != food.x && i->y != food.y){
                iterator++;
            }
        }
        counter++;
        if(iterator == (int)snake.size())
            break;
        iterator=0;
    }

}

int CSnake::validateMovement(CPoint a)
{
    int xV = a.x, yV = a.y, iterator = 0;
    CPoint next = snake.front();
    for (list<CPoint>::iterator i = snake.begin(); i != snake.end(); ++i, ++iterator) {
        if (iterator != 0 && i->x == next.x + xV && i->y == next.y + yV){
            return 0;
        }
    }
    return 1;
}
