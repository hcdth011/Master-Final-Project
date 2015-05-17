#include<iostream>
#include<string>
#include<vector>
using namespace std;

int out(vector<int> &iVec)
{  
	std::vector<int>::iterator it ;
    for(it=iVec.begin(); it != iVec.end(); it++)
        cout<<*it<<ends;
    cout<<endl;
    return 0;
}

int main()
{
    vector<int> iVec;
    vector<int>::iterator it;
    int i;
    for( i=0;i<10;i++)
        iVec.push_back(i);
        
    cout<<"vector size: "<< iVec.size()<<endl;
    cout<<"The Num(old):"<<endl;
    out(iVec);
    
    iVec.erase(iVec.begin());
    iVec.push_back(i);
    cout<<"-----------------------------"<<endl;
    cout<<"vector size: "<< iVec.size()<<endl;
    cout<<"The Num(new):";out(iVec);
    
    return 0;
}
