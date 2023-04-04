#ifndef LCDfunctions_h
#define LCDfunctions_h

#include <TFT_HX8357.h> //Custom Lib by Bodmer for LCD

  //structs
struct Point2d
{
  int x=-90;
  int y=-90;
}
struct Line2d
{
  Point2d pt1;
  Point2d pt2;
}
struct Rect2d
{
  Point2d topRight;
  int h=1;
  int w=1;
}
struct Circle2d
{
  Point2d center;
  int radius=1;
}

//**********
//Name
//  dispSelect()
//Purpose
//  Deals with the selection control from controls
//  Adjusts parameter signal based on controls input
//Parameters
//  int selCur = current selection of controls
//  int state  = current screen state
//  bool caseStartSel   = toggle for if the start location of the case has been selected for readjustment
//  bool palletStartSel = toggle for if the start location of the pallet has been selected for readjustment
//  bool twoCaseMode    = toggle for two cases or one case mode
//  bool showStats      =
//  bool autoParameters =
//Returns
//  void
//**********
void dispSelect(int *selCur, int *state, bool *caseStartSel, bool *palletStartSel, bool *twoCubeMode, bool *showStats, bool *autoParameters);

//**********
//Name
//  
//Purpose
//  
//Parameters
//  
//Returns
//  
//**********

#endif
