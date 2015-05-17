#include "ArgMin.h"
#include "stdio.h"
#define Q 5
#define ROWNUM 6

int main()
{
	int i=0;
	double solution[Q-1],eqt[Q][Q]={0};
	double a[ROWNUM][Q]={{3,-2,4,6,-11},{4,3,2,9,-2},{2,6,8,3,4},{2,4,5,3,3},
						  {0,0,8,6,-20},{3,4,5,6,0}};
	ArgMin(&a,Q,ROWNUM,Q,&solution);
	for(i=0;i<Q-1;i++) printf("%.14lf\n",solution[i]);
	return 0;
}
