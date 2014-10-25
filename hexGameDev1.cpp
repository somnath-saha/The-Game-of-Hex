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
 * 
 */


#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <termios.h>
#include <unistd.h>   

using namespace std;

void display(char** grid, const int n);
bool play(char** grid, const int n);
void makeBoard();
char getch();

class graph
{
	class node
	{
		public:
			char data;
			bool visited;
			vector<short> link;
			node():data(0),visited(false){}
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
	}
	void fillboard()
	{
		std::vector<char> v;
		//int temp[]={1,0,1,0,1, 0,0,1,0,1, 1,1,1,0,0, 0,1,1,0,1, 0,0,0,1,0};
		//int temp[]={0,1,0,0,0, 1,1,1,0,0, 1,0,0,1,1, 1,0,1,0,0, 0,0,1,1,1};

		for(int i=0; i<n*n/2; ++i)
		{
			v.push_back('X');
		}
		for(int i=n*n/2; i<n*n; ++i)
		{
			v.push_back('O');
		}
		std::random_device rd;
		std::mt19937 engine(rd());
		std::shuffle(v.begin(), v.end(), engine);
		
		for(int i=0; i<n*n; ++i)
		{
			g[i].data=v[i];
		}
		
		
		//for(int i=0; i<n*n; ++i)
		//{
			//if(temp[i]) g[i].data='X';
			//else g[i].data='O';
		//}
		
		
	}
	void gdisplay()
	{
		char* grid[n];
		for(short i=0; i<n; ++i)
		{
			grid[i]= new char[n];
		}
		for(int i=0; i<n*n; ++i)
		{
			grid[i/n][i%n]=g[i].data;
		}
		display(grid,n);
		/*
		for(int i=0; i<n*n; ++i)
		{
			cout<<endl<<i<<": Data("<<g[i].data<<") :";
			for(auto j=g[i].link.begin(); j!=g[i].link.end(); ++j)
			{
				cout<<*j<<", ";
			}
		}
		*/
	}
	void update(int r, int c, char player)
	{
		r--; c--;
		g[r*n+c].data=player;
	}
	
	bool xWins()
	{
		vector<int> stack;
		for(int i=0, x=0;  i<n; ++i)
		{
			stack.clear();
			cout<<"\nChecking at i="<<i<<endl;
			if(g[i].visited==false && g[i].data=='X')
			{
				stack.push_back(i);
				while(stack.empty()!=true)
				{
					x=stack.back();
					stack.pop_back();
					g[x].visited=true;
					cout<<"\nx="<<x<<g[x].data<<" is visited.";
					if(x>=n*n-n && x<n*n)
						return true;
					else
					{
						for(auto data: g[x].link)
						{
							if(g[data].visited==false && g[data].data=='X')
							{
								g[data].visited=true;
								stack.push_back(data);
								cout<<"\n  Element "<<data<<"inserted";
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
		vector<int> stack;
		for(int i=0, x=0;  i<n*n; i+=n)
		{
			stack.clear();
			cout<<"\nChecking at i="<<i<<endl;
			if(g[i].visited==false && g[i].data=='O')
			{
				stack.push_back(i);
				while(stack.empty()!=true)
				{
					x=stack.back();
					stack.pop_back();
					g[x].visited=true;
					cout<<"\nx="<<x<<" is visited.";
					if((x+1)%n==0)
						return true;
					else
					{
						for(auto data: g[x].link)
						{
							if(g[data].visited==false && g[data].data=='O')
							{
								g[data].visited=true;
								stack.push_back(data);
								cout<<"\n  Element "<<data<<"inserted";
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
		//short *start= new short[n];
		//short *end= new short[n];
		
		//player top-bottom ie X
		//starts are from 0 to n-1 nodes
		//ends are from n*(n-1) to n*n-1 nodes
		if (xWins()||yWins())
			return true;
		else
			return false;
		/*
		//player left-right ie O
		//starts are from 0, n, 2n ... n(n-1)
		//ends are from n-1, 2n-1, 3n-1 ... n*n-1
		for(int i=0; i<n*n; i+=n)
		{
			if(g[i].data=='O'&&g[i].visited==false)
			{
				if (dfs(i, 'O')==true)
				{
					cout<<"\nplayer O wins.";
					return true;
				}
			}
		}
		* 
		*/
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
	
	~graph()
	{
		delete[] g;
	}
};

int main(int argc, char **argv)
{
	int n=5;
	/*
	cout<<"Enter size of board: ";
	do
	{
		cin>>n;
		if(n<2&&n>15) 
			cout<<"Enter a proper size of board[2-15]:";
	}while (n<2&&n>15);
	*/
	
	//Make a board of size n
	/*char* grid[n];
	for(short i=0; i<n; ++i)
	{
		grid[i]= new char[n];
	}
	for(int i=0; i<n; ++i)
	{
		for(int j=0; j<n; ++j)
		{
			grid[i][j]=0;
		}
	}*/
	//int t=7;
	//while(t-->0){
	graph board(n);
	
	int t=100;
	while(t-->0)
	{
		board.fillboard();
		board.gdisplay();
		
		if(board.xWins())
			cout<<"\nPlayer X wins.";
		else if(board.yWins())
			cout<<"\nPlayer 0 wins.";
		else
			cout<<"No one wins";
		getch();
	}
	//}
	//display(grid, n);
	//call play
	//play(grid, n);
	return 0;
}

bool play(char** grid, const int n)
{
	int r, c;
	graph board(n);
	while(board.end()!=true)
	{
		do
		{
			display(grid, n);		
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
		}while(!board.isValidMove(r,c));
		grid[r-1][c-1]='X';
		display(grid, n);
		board.update(r,c,'X');
		//board.display();
	}	
	return false;
}

void display(char** grid, const int n)//display a grid
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
				if (grid[i/2][j]!=0)
					cout<<static_cast<char>(grid[i/2][j]);
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
