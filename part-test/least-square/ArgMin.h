/***************************************************************************
 *  ArgMin.h
 *  Author  
 *  Fri Aug  5 17:18:47 2005
 *  Copyright  2005  
 *  Email  
 ****************************************************************************/


#include "stdio.h"
#include "math.h"

/* Interface */
//solve the linear equations using "argmin" method
int ArgMin(double *inMtrx,int COLUMN,int rowNum,int colNum,double *solution);
//Solve a series of linear equations
int SolveLinearEqts(double *inMtrx,int COLUMN,int rowNum,int colNum,double *solution);
//Elimination of unknowns
int ReduceUnknowns(double *mtrx_tmp,int COLUMN,int rowNo,int colNo,int rowNum,int colNum);


/* Begin Coding */
//ArgMin using "solvelineareqts". 

int ArgMin(double *mtrx_tmp,int COLUMN,int rowNum,int colNum,double *solution)
{
  int k,l,j;  
  double *eqt;
  int *q;
  q=&colNum;
  eqt=(double *)malloc(colNum*colNum*sizeof(double));
  //printf("%d",*q);
  
  for(k=0;k<*q-1;k++)
    for(l=0;l<*q-1;l++)
    {
      eqt[k*COLUMN+l]=0;
      for(j=0;j<rowNum;j++)
        eqt[k*COLUMN+l]+=*(mtrx_tmp+COLUMN*j+k)*(*(mtrx_tmp+COLUMN*j+l));
    }
  for(k=0;k<*q-1;k++)
  {
    eqt[k*COLUMN+*q-1]=0;
    for(j=0;j<rowNum;j++)
      eqt[k*COLUMN+*q-1]+=*(mtrx_tmp+COLUMN*j+*q-1)*(*(mtrx_tmp+COLUMN*j+k));
  }
  
    /* show the contents of eqts */
    /*int m,n;
    printf("eqt:/n");
    for(m=0;m<*q-1;m++)
    {
      for(n=0;n<*q;n++)  
        printf("%.2lf/t",eqt[m*COLUMN+n]);
      printf("/n");
    }*/

  if(!SolveLinearEqts(eqt,COLUMN,(*q-1),*q,solution)) {
    return 0;
  }
  
  return 1;
}

//Solve the solution of a series of linear equations
int SolveLinearEqts(double *inMtrx,int COLUMN,int rowNum,int colNum,double *solution)
{
  int i,j;
  double tmpSum;
  if(rowNum!=(colNum-1)) {
    printf("Can't solve the equations because equation number ");
    printf("is not the same as unknow parameters!/n");
    return 0;
  }
  //reduce unknown parameters
  for(i=0;i<colNum-2;i++)
  {  
    if(!ReduceUnknowns(inMtrx,COLUMN,i,i,rowNum,colNum)) {
      printf("/nNeed more Equations to solve elements in matrix A/n");
      printf("(Tip: You can try setting a different \"theta\" value or ");
      printf("checking the data introduced to function\"CalculateCoeff\"!)/n");
      return 0;
    }
  }
  //Calculate the equation at the bottom to acquire value of the first 
  //variable, then Substitute the solved variables to the equations in 
  //order to solve more variables:
  for(i=rowNum-1;i>=0;i--)
  {
    tmpSum=0;
    for(j=i+1;j<rowNum;j++)
      tmpSum+=*(solution+j)*(*(inMtrx+i*COLUMN+j));
      
    if(fabs(inMtrx[i*COLUMN+i])<0.000001) {
      printf("/nNeed more Equations to solve elements in matrix A/n");
      printf("(Tip: You can try setting a different theta value, /n or ");
      printf("checking the data introduced to function CalculateCoeff! /n");
      return 0;      
    }
    *(solution+i)=(*(inMtrx+i*COLUMN+colNum-1)-tmpSum)/(*(inMtrx+i*COLUMN+i));
  }
  return 1;
}

/*Column number of Array is COLUMN_A, but actually the number of figures 
in each row is "colNum"! */
int ReduceUnknowns(double *mtrx_tmp,int COLUMN,int rowNo,int colNo,int rowNum,int colNum)
{
  int i,j,tmpInt;
  double tmpDbl1,tmpDbl2;
  double rowMax,colMax;
  double *p,*mp;

  /*Adjust the rowNo whose value is zero in matrix_tmp[rowNo][colNo] down to 
  a suitable site*/
  p=mtrx_tmp+rowNo*COLUMN+colNo;
  mp=mtrx_tmp+rowNo*COLUMN+colNo;
  //select main variable:
  /*rowMax=fabs(*p);
  for(j=colNo+1;j<colNum;j++)
  {
    p++;
    if(rowMax<fabs(*p)){
      rowMax=fabs(*p);
    }
  }    
  if(rowMax==0) return 0;
  p+=COLUMN-(colNum-colNo-1);
  colMax=fabs(*(mtrx_tmp+rowNo*COLUMN+colNo))/rowMax;
  tmpInt=rowNo;  
  for(i=rowNo+1;i<rowNum;i++)
  {  
    rowMax=fabs(*p);
    for(j=colNo+1;j<colNum;j++)
    {
      p++;
      if(rowMax<fabs(*p)){
        rowMax=fabs(*p);
      }
    }    
    p+=COLUMN-(colNum-colNo-1);
    if(rowMax==0) return 0;
    tmpDbl1=fabs(*(mtrx_tmp+i*COLUMN+colNo))/rowMax;
    if(colMax<tmpDbl1) {
      colMax=tmpDbl1;
      tmpInt=i;
    }
  }
  //change the whole row of "tmpInt" to "rowNo"
  for(j=colNo;j<colNum;j++)
  {
    tmpDbl1=*(mtrx_tmp+rowNo*COLUMN+j);
    *(mtrx_tmp+rowNo*COLUMN+j)=*(mtrx_tmp+tmpInt*COLUMN+j);
    *(mtrx_tmp+tmpInt*COLUMN+j)=tmpDbl1;
  }*/
    
  /*Transform the matrix_tmp using linear calculation methods*/
  tmpDbl1=*p;
  p+=COLUMN;
  for(i=rowNo+1;i<rowNum;i++)
  {
    if(fabs(*p)<0.000001) continue;
    tmpDbl2=*p;
    for(j=colNo;j<colNum;j++)
    {
      *p=*p-*mp*tmpDbl2/tmpDbl1;
      p++;
      mp++;
    }
    p+=COLUMN-(colNum-colNo);
    mp-=colNum-colNo;
  }
  return 1;
}
