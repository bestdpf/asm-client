#include"xconfig.h"
#include<iostream>
#include<fstream>
#include<sstream>

using namespace std;

namespace StatModel {
XConfig::XConfig(string& filename)
{
    ifstream inp(filename.c_str()); 
  int i;
  for(i=0;i<X_NUM;i++){
    printf("read %d config\n",i);
    int c;
    inp>>c;
    cnt[i]=c;
    if(c==0)mask[i]=false;
    else{
      mask[i]=true;
      //cnt[i]=c;
      int j,point;
      p[i].clear();
      for(j=0;j<c;j++){
	inp>>point;
	p[i].push_back(point);
      }
      char a;
      inp>>a;
      if(a=='x')axis[i]=x;
      else axis[i]=y;
      string u;
      inp>>u;
      if(u=="es")unit[i]=ES;
      else if(u=="irisd")unit[i]=IRISD;
      else if(u=="ens")unit[i]=ENS;
      else if(u=="mns")unit[i]=MNS;
      else if(u=="mw")unit[i]=MW;
      else if(u=="au")unit[i]=AU;
      else cerr<<"illeagl unit"<<endl;
      string dir;
      inp>>dir;
      if(dir=="up")mdir[i]=UP;
      else if(dir=="down")mdir[i]=DOWN;
      else if(dir=="left")mdir[i]=LEFT;
      else if(dir=="right")mdir[i]=RIGHT;
      else cerr<<"illeagle direction"<<endl;
    }
  }
  sep=" ";
  initCnt=0;
  memset(avgx,0,sizeof(avgx));
  memset(avgy,0,sizeof(avgy));
}

int XConfig::getCnt(int idx)
{
  return cnt[idx];
}

bool XConfig::isUsed(int idx)
{
  return mask[idx];
}

vector< int >& XConfig::getPoints(int idx)
{
  return p[idx];
}

Axis XConfig::getAxis(int idx)
{
  return axis[idx];
}

Unit XConfig::getUnit(int idx)
{
  return unit[idx];
}
/*
vector< int >& XConfig::getValue()
{
  return value;
}
*/
const string XConfig::getRetStr()
{
  return retStr.str();
}

vector<Point_<int> > XConfig::chgOrder(vector<Point_<int> >& V){
  vector<Point_<int> > ret;
  ret.reserve(X_NUM*sizeof(Point_<int>));
  int i;
  for(i=0;i<X_NUM;i++){
    ret[i].x=0;
    ret[i].y=0;
    int j,len=getCnt(i);
    for(j=0;j<len;j++){
      ret[i].x+=V[p[i][j]].x;
      ret[i].y+=V[p[i][j]].y;
    }
    if(len>0){
      ret[i].x/=len;
      ret[i].y/=len;
    }
  }
  return ret;
}

bool XConfig::process(vector< Point_<int> >& V)
{
  if(V.size()!=INPUT_NUM){
    cerr<<"you should use muct76"<<endl;
    return false;
  }
  vector<Point_<int> > vec=chgOrder(V);
  initCnt++;
  int i;
  for(i=0;i<X_NUM;i++){
    retMask[i]=mask[i];
    if(vec[i].x==0&&vec[i].y==0)mask[i]=false;
  }
  retStr.clear();
  for(i=0;i<X_NUM;i++){
    if(mask[i]){
      avgx[i]=(initCnt-1)*avgx[i]+vec[i].x;
      avgx[i]/=initCnt;
      avgy[i]=(initCnt-1)*avgy[i]+vec[i].y;
      avgy[i]/=initCnt;
      valuex[i]=0.9*oldValuex[i]+0.1*vec[i].x;
      valuey[i]=0.9*oldValuey[i]+0.1*vec[i].y;
    }
  }
  for(i=0;i<X_NUM;i++){
    oldValuex[i]=valuex[i];
    oldValuey[i]=valuey[i];
  }
  if(initCnt<10)return false;//init 10 circls;
  else{
    unit_value[IRISD]=((vec[21-1].y-vec[19-1].y)+(vec[22-1].y-vec[20-1].y))/2;//irisd
    unit_value[ES]=vec[36-1].x-vec[31-1].x;//es
    unit_value[ENS]=V[41].y-vec[36-1].y;//ens
    unit_value[MNS]=vec[4-1].y-V[41].y;//mns
    unit_value[MW]=vec[59-1].x -vec[60-1].x;//mw
    unit_value[AU]=1e-5*1024;//au
    int i;
    for(i=0;i<6;i++)unit_value[i]/=1024;
    for(i=0;i<X_NUM;i++){
      retStr<<sep<<(mask[i])?"1":"0";
    }
    
    for(i=0;i<X_NUM;i++)if(mask[i]){
      int val;
      if(mdir[i]==DOWN||mdir[i]==RIGHT){//opencv's positive direction
	val=((axis[i]==x)?vec[i].x:vec[i].y-(axis[i]==x)?avgx[i]:avgy[i]);
      }
      else{
	val=(-((axis[i]==x)?vec[i].x:vec[i].y-(axis[i]==x)?avgx[i]:avgy[i]));
      }
      val/=unit_value[unit[i]];
      retStr<<sep<<val;
    }
    return true;
  }
}

}//end of StatModel namespace
