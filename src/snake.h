#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include "screen.h"

class CSnake:public CFramedWindow
{
  list <CPoint> snake;
  struct CPoint dir;
  struct CPoint food;
  int prevMove;
  bool game_over = false;
  int x=0, y=0;
  int level=1;
  void paint();
  bool pause = 1;
  

  void move(CPoint a);
  void start();
  void rand_point();
  bool handleEvent(int key);
  int validateMovement(CPoint a);
  bool handleMovement(CPoint a);
  bool handleSnakeReplacement(void);
  int setPrevMove(CPoint a);
public:
  CSnake(CRect r, char _c = ' ');
  
};

#endif
