/*
 * hexGame.cpp
 * 
 * Copyright 2013 somnath <somnath@saasbook>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA. 
 * 
 */

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <termios.h>
#include <unistd.h>  
#include <ctime> 
using namespace std;

char getch();

class graph
{
	class node
	{
		public:
			char data;
			vector<short> link;
			node():data(0){}
	};
	node *g;
	int n;

	public:
	
	graph(int n): n(n)
	{
		//To construct graph for a hexboard of side n		
		//Connections will be setup one by one for all the elements
		g= new node[n*n];
						
		//first element
		g[0].link.push_back(1);
		g[0].link.push_back(n);		
		//first row except the last
		for(int i=1; i<n-1; ++i)
		{
			g[i].link.push_back(i-1);
			g[i].link.push_back(i+1);
			g[i].link.push_back(i+n);
			g[i].link.push_back(i+n-1);	
		}
		//first row last element
		g[n-1].link.push_back(n-2);
		g[n-1].link.push_back(2*n-2);	
		g[n-1].link.push_back(2*n-1);
		//all elements except 1st & last row and 1st and last columns
		for(int i=n+1; i<n*n-n-1; ++i)
		{
			if(i%n==0||(i+1)%n==0)
				continue;
			g[i].link.push_back(i-n);
			g[i].link.push_back(i-n+1);
			g[i].link.push_back(i-1);
			g[i].link.push_back(i+1);
			g[i].link.push_back(i+n-1);
			g[i].link.push_back(i+n);	
		}
		//last row 1st element
		g[n*n-n].link.push_back(n*n-2*n);
		g[n*n-n].link.push_back(n*n-2*n+1);	
		g[n*n-n].link.push_back(n*n-n+1);
		//first column elements
		for(int i=n; i<n*n-n; i+=n)
		{
			g[i].link.push_back(i-n);
			g[i].link.push_back(i-n+1);
			g[i].link.push_back(i+1);
			g[i].link.push_back(i+n);
		}
		//last column elements
		for(int i=2*n-1; i<n*n-n; i+=n)
		{
			g[i].link.push_back(i-n);
			g[i].link.push_back(i-1);
			g[i].link.push_back(i+n-1);
			g[i].link.push_back(i+n);
		}		
		//last row elements
		for(int i=n*n-n+1; i<n*n-1; i++)
		{
			g[i].link.push_back(i-n);
			g[i].link.push_back(i-n+1);
			g[i].link.push_back(i-1);
			g[i].link.push_back(i+1);
		}		
		//last element
		g[n*n-1].link.push_back(n*n-n-1);
		g[n*n-1].link.push_back(n*n-2);	
		for(int i=0; i<n*n; ++i)
		{
			cout<<i<<":"<<g[i].data<<" ";
		}
	}
	void update(int r, int c, char player)
	{
		r--; c--;
		g[r*n+c].data=player;
	}
	bool xWins()
	{
		//player top-bottom ie X
		//starts are from 0 to n-1 nodes
		//ends are from n*(n-1) to n*n-1 nodes
		
		vector<int> stack;
		vector<bool> visited(n*n, false);
		
		for(int i=0, x=0;  i<n; ++i)
		{
			stack.clear();
			//cout<<"\nChecking at i="<<i<<endl;
			if(visited.at(i)==false && g[i].data=='X')
			{
				stack.push_back(i);
				while(stack.empty()!=true)
				{
					x=stack.back();
					stack.pop_back();
					visited.at(x)=true;
					//cout<<"\nx="<<x<<g[x].data<<" is visited.";
					if(x>=n*n-n && x<n*n)
						return true;
					else
					{
						for(auto data: g[x].link)
						{
							if(visited.at(data)==false && g[data].data=='X')
							{
								visited.at(data)=true;
								stack.push_back(data);
								//cout<<"\n  Element "<<data<<"inserted";
							}
						}
					}
				}
			}
		}
		return false;
	}
	bool yWins()
	{
		//player left-right ie O
		//starts are from 0, n, 2n ... n(n-1)
		//ends are from n-1, 2n-1, 3n-1 ... n*n-1
		
		vector<int> stack;
		vector<bool> visited(n*n, false);
		
		for(int i=0, x=0;  i<n*n; i+=n)
		{
			stack.clear();
			//cout<<"\nChecking at i="<<i<<endl;
			if(visited.at(i)==false && g[i].data=='O')
			{
				stack.push_back(i);
				while(stack.empty()!=true)
				{
					x=stack.back();
					stack.pop_back();
					visited.at(x)=true;
					//cout<<"\nx="<<x<<" is visited.";
					if((x+1)%n==0)
						return true;
					else
					{
						for(auto data: g[x].link)
						{
							if(visited.at(data)==false && g[data].data=='O')
							{
								visited.at(data)=true;
								stack.push_back(data);
								//cout<<"\n  Element "<<data<<"inserted";
							}
						}
					}
				}
			}
		}
		return false;
	}
	bool end()
	{	
		if (xWins()||yWins())
			return true;
		else
			return false;
	}
	bool isValidMove(int r, int c)
	{
		r--; c--;
		if(g[r*n+c].data==0)
			return true;
		else 
		{
			system("clear");
			cout<<"\nInvalid Move. Press any key to continue...";
			return false;
		}
	}
	void display()
	{	
		std::system("clear");
		cout<<"\n================================================="<<endl;
		cout<<"              THE GAME OF HEX					"<<endl;
		cout<<"================================================="<<endl;
		cout<<endl<<"        Columns --->\n"<<"     ";
		for(int i=0; i<n; ++i)
		{
			if(i/10==0)
				cout<<"   "<<i+1;
			else
				cout<<"  "<<i+1; //2 digit no. need less space
		}
		cout<<"\n\n";
		for(int i=0, j=0, k; i<2*n-1; ++i)
		{
			k=i;
			if(i%2==0)
			{			
				if((i/2+1)/10==0)
					cout<<"Row "<<i/2+1<<"    ";
				else
					cout<<"Row "<<i/2+1<<"   "; //2 digit no. need less space
				
				while(k-->0)
				{
					cout<<" ";//display starting spaces
				}
				for(j=0; j<n; ++j) //display grid values
				{
					if (g[i/2*n+j].data!=0)
						cout<<g[i/2*n+j].data;
					else
						cout<<".";
					if(j==n-1) break;
					else cout<<"---";
				}
			}
			else
			{
				cout<<"         ";
				while(k-->0)
				{
					cout<<" ";//display starting spaces
				}
				cout<<"\b";//display edges as ascii \ and /
				for(j=0; j<(2*n-1); ++j)
				{
					if(j%2==0) cout<<" \\";
					else cout<<" /";
				}
			}
			cout<<endl;
		}
		
	}
	int computerMove(int m)
	{
		srand(time(NULL));
		int pos=0;
		vector<char> vtemp, v('X', n*n);
		vector<int> count(n*n,0);
		for(int j=0; j<n*n; ++j)
		{
			vtemp.push_back(g[j].data);
		}
		for(int j=0; j<n*n; ++j)
		{
			if (g[j].data!=0) continue;
			else
			{
				int trials=1000;
				while(trials-->0)
				{
					//cout<<"\nExecuting trial no. "<<trials;					
					g[j].data='O';
					
					for(int i=0; i<=(n*n/2-1); ++i)
					{
						v.at(i)='X';
					}
					if((n*n)%2!=0) v.at(n*n/2)='X';//first player character
					else
					{
						v.at(n*n/2)='O';//second player character
					}
					//cout<<"\nat n*n/2 we have "<<v.at(n*n/2);
					for(int i=(n*n/2+1); i<n*n; ++i)
					{
						v.at(i)='O';
					}
					
					random_shuffle(v.begin()+m, v.end()-m);
					int k=0;
					for(auto i=v.begin()+m; i!=(v.end()-m); ++i)
					{
						//cout<<"\nk="<<k;
						while(g[k].data!=0 && k<(n*n-1))	
						{
							//cout<<"\ng[k].data="<<g[k].data<<" (k="<<k<<")";
							k++;
						}							
						g[k].data=*i;
						//cout<<" accessing g["<<k<<"].data set to"<<*i;
					}
					//cout<<"\nFor trial "<<trials;
					//vdisplay(v);
					//display();
					//getch();
					if(!xWins())
						count.at(j)++;						
					for(int i=0; i<(n*n); ++i)
					{
						g[i].data=vtemp.at(i);
					}
				}												
			}
		}
		//vdisplay(count);
		for(int i=0; i<(n*n); ++i)
		{
			g[i].data=vtemp.at(i);
		}
		
		for(int i=0, max=0; i<(n*n); ++i)
		{
			if (count.at(i)>max)
			{
				max=count.at(i);
				pos=i;
			}
		}
		return pos;					
	}
	char play()
	{
		int r, c, move=0, pos=0;
		while(end()!=true)
		{
			do
			{
				system("clear");
				display();
				cout<<"\n================================================="<<endl;
				cout<<"\nEnter row number:";
				do
				{
					cin>>r;
					if(r<1||r>n) 
						cout<<"Enter valid row[1-"<<n<<"]:";
				}while (r<1||r>n);
				cout<<"Enter column number:";
				do
				{
					cin>>c;
					if(c<1||c>n) 
						cout<<"Enter valid column[1-"<<n<<"]:";
				}while (c<1||c>n);
			} while(!isValidMove(r,c));
			move++;
			//grid[r-1][c-1]='X';
			update(r,c,'X');		
			pos=computerMove(move);	
			update(pos/n+1,(pos%n)+1,'O');	
			display();
			if(xWins())
			{
				return 'X';
				
			}
			else if(yWins())
			{
				return 'O';
			}
		}	
		return '?';
	}
	~graph()
	{
		delete[] g;
	}
};

int main(int argc, char **argv)
{
	int n=5;
	
	cout<<"Enter size of board: ";
	do
	{
		cin>>n;
		if(n<2&&n>15) 
			cout<<"Enter a proper size of board[2-15]:";
	}while (n<2&&n>15);	
	
	graph board(n);
	char winner=board.play();	
	if(winner=='X')
		cout<<"\nCongratulations...\nX is the winner.";
	else if(winner=='O')
		cout<<"\nLoser...\nO is the winner.";
	else
		cout<<"Error in winner decision";
	
	return 0;
}

char getch()
{
    /*#include <unistd.h>   //_getch*/
    /*#include <termios.h>  //_getch*/
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    //printf("%c\n",buf);
    return buf;
 }
