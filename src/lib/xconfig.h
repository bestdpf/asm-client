#ifndef _XCONFIG_H
#define _XCONFIG_H
#include<cstdio>
#include<vector>
#include<string>
#include<cv.h>
#include<highgui.h>
#include<cvaux.h>
#include<sstream>
using namespace std;
using namespace cv;

namespace StatModel{
  enum Axis{
    x,y
  };
  enum Unit{
    IRISD,
    ES,
    ENS,
    MNS,
    MW,
    AU
  };
  enum Direction{
    UP,
    DOWN,
    LEFT,
    RIGHT
  };
  class XConfig{
  public:
    XConfig(string & filename);
    bool isUsed(int idx);
    vector<int>& getPoints(int idx);
    int getCnt(int idx);
    Axis getAxis(int idx);
    Unit getUnit(int idx);
    //vector<int>& getValue();
    const string getRetStr();
    bool process(vector< Point_<int> >& V);
  protected:
    vector<Point_<int> > chgOrder(vector<Point_<int> >& V);
  private:
    const static int X_NUM=68;
    const static int INPUT_NUM=76;
    bool mask[X_NUM];
    bool retMask[X_NUM];
    int cnt[X_NUM];
    enum Axis axis[X_NUM];
    enum Unit unit[X_NUM];
    enum Direction mdir[X_NUM];
    vector<int>p[X_NUM];
    int initCnt;
    float oldValuex[X_NUM];
    float oldValuey[X_NUM];
    float avgx[X_NUM];
    float avgy[X_NUM];
    float valuex[X_NUM];
    float valuey[X_NUM];
    stringstream retStr;
    string sep;
    float unit_value[6];
  };
}//end of StatModel namespace


#endif
