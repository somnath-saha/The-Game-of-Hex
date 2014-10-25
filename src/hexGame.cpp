/*
 * hexGame.cpp
 * Completed 2nd January 2013
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
#include <algorithm>
#include <ctime> 

using namespace std;

//Node that represents every hex of the hexBoard
//Data members: data for representing player character
//Connecting Links for the Hex Node via vector link
struct node
{
	char data;
	vector<short> link;
	node():data(0){}
};

//Implements the entire hexBoard and its functionality to play, computer-moves etc.
class hexBoard
{
	vector<node> g;
	int bdSize;
	char comp;
	char hum;
	bool compDrEW;

	public:
	
	hexBoard(int bdSize=5, char comp='O', char hum='X', bool compDrEW=true): 
		bdSize(bdSize), comp(comp), hum(hum), compDrEW(compDrEW)
	{
			
		//To construct graph for a hexboard of side bdSize		
		//Connections will be setup one by one for all the elements
		g.resize(bdSize*bdSize);
						
		//first element
		g[0].link.push_back(1);
		g[0].link.push_back(bdSize);		
		//first row except the last
		for(int i=1; i<bdSize-1; ++i)
		{
			g[i].link.push_back(i-1);
			g[i].link.push_back(i+1);
			g[i].link.push_back(i+bdSize);
			g[i].link.push_back(i+bdSize-1);	
		}
		//first row last element
		g[bdSize-1].link.push_back(bdSize-2);
		g[bdSize-1].link.push_back(2*bdSize-2);	
		g[bdSize-1].link.push_back(2*bdSize-1);
		//all elements except 1st & last row and 1st and last columns
		for(int i=bdSize+1; i<bdSize*bdSize-bdSize-1; ++i)
		{
			if(i%bdSize==0||(i+1)%bdSize==0)
				continue;
			g[i].link.push_back(i-bdSize);
			g[i].link.push_back(i-bdSize+1);
			g[i].link.push_back(i-1);
			g[i].link.push_back(i+1);
			g[i].link.push_back(i+bdSize-1);
			g[i].link.push_back(i+bdSize);	
		}
		//last row 1st element
		g[bdSize*bdSize-bdSize].link.push_back(bdSize*bdSize-2*bdSize);
		g[bdSize*bdSize-bdSize].link.push_back(bdSize*bdSize-2*bdSize+1);	
		g[bdSize*bdSize-bdSize].link.push_back(bdSize*bdSize-bdSize+1);
		//first column elements
		for(int i=bdSize; i<bdSize*bdSize-bdSize; i+=bdSize)
		{
			g[i].link.push_back(i-bdSize);
			g[i].link.push_back(i-bdSize+1);
			g[i].link.push_back(i+1);
			g[i].link.push_back(i+bdSize);
		}
		//last column elements
		for(int i=2*bdSize-1; i<bdSize*bdSize-bdSize; i+=bdSize)
		{
			g[i].link.push_back(i-bdSize);
			g[i].link.push_back(i-1);
			g[i].link.push_back(i+bdSize-1);
			g[i].link.push_back(i+bdSize);
		}		
		//last row elements
		for(int i=bdSize*bdSize-bdSize+1; i<bdSize*bdSize-1; i++)
		{
			g[i].link.push_back(i-bdSize);
			g[i].link.push_back(i-bdSize+1);
			g[i].link.push_back(i-1);
			g[i].link.push_back(i+1);
		}		
		//last element
		g[bdSize*bdSize-1].link.push_back(bdSize*bdSize-bdSize-1);
		g[bdSize*bdSize-1].link.push_back(bdSize*bdSize-2);	
		
	}
	void update(int pos, char player)
	{
		g[pos].data=player;
	}
	bool nsWins(char player) //returns true if player wins in making north-south path
	{
		//starts are from 0 to n-1 nodes
		//ends are from n*(n-1) to n*n-1 nodes
		//test for a path from starting nodes to ending nodes via dfs
		
		vector<int> stack;
		vector<bool> visited(bdSize*bdSize, false);
		
		for(int i=0, x=0;  i<bdSize; ++i)
		{
			stack.clear();
			if(visited.at(i)==false && g[i].data==player)
			{
				stack.push_back(i);
				while(stack.empty()!=true)
				{
					x=stack.back();
					stack.pop_back();
					visited.at(x)=true;
					if(x>=bdSize*bdSize-bdSize && x<bdSize*bdSize)
						return true;
					else
					{
						for(auto data: g[x].link)
						{
							if(visited.at(data)==false && g[data].data==player)
							{
								visited.at(data)=true;
								stack.push_back(data);
							}
						}
					}
				}
			}
		}
		return false;
	}
	bool ewWins(char player) //returns true if player wins in making east-west path
	{
		//starts are from 0, n, 2n ... n(n-1)
		//ends are from n-1, 2n-1, 3n-1 ... n*n-1
		//test for a path from starting nodes to ending nodes via dfs
		
		vector<int> stack;
		vector<bool> visited(bdSize*bdSize, false);
		
		for(int i=0, x=0;  i<bdSize*bdSize; i+=bdSize)
		{
			stack.clear();
			if(visited.at(i)==false && g[i].data==player)
			{
				stack.push_back(i);
				while(stack.empty()!=true)
				{
					x=stack.back();
					stack.pop_back();
					visited.at(x)=true;
					if((x+1)%bdSize==0)
						return true;
					else
					{
						for(auto data: g[x].link)
						{
							if(visited.at(data)==false && g[data].data==player)
							{
								visited.at(data)=true;
								stack.push_back(data);
							}
						}
					}
				}
			}
		}
		return false;
	}
	bool isValidMove(int r, int c)
	{
		r--; c--;
		if(g[r*bdSize+c].data==0)
			return true;
		else 
		{
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
		for(int i=0; i<bdSize; ++i)
		{
			if(i/10==0)
				cout<<"   "<<i+1;
			else
				cout<<"  "<<i+1; //2 digit no. need less space
		}
		cout<<"\n\n";
		for(int i=0, j=0, k; i<2*bdSize-1; ++i)
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
				for(j=0; j<bdSize; ++j) //display grid values
				{
					if (g[i/2*bdSize+j].data!=0)
						cout<<g[i/2*bdSize+j].data;
					else
						cout<<".";
					if(j==bdSize-1) break;
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
				for(j=0; j<(2*bdSize-1); ++j)
				{
					if(j%2==0) cout<<" \\";
					else cout<<" /";
				}
			}
			cout<<endl;
		}		
	}
	int computerMove(int m, char ans)
	{
		srand(time(NULL)); //for random_shuffle
		int pos=0, count=0, countMax=0;
		vector<char> savedBoard, randomFillArray(bdSize*bdSize);
		
		//odd and even board sizes cause the charcter distribution to be a little different
		//details omitted... find it out
		char firstPlayer=((ans=='N')?comp:hum); 
		
		//Save current board characters in a temporary vector 
		for(int j=0; j<bdSize*bdSize; ++j)
		{
			savedBoard.push_back(g[j].data);
		}
		
		//For every possible move on the board, find out the chance of winning
		//and remember only the maximum one and the corresponding move position
		for(int j=0; j<bdSize*bdSize; ++j)
		{
			if (g[j].data!=0) continue;
			else
			{
				int trials=1500;
				count=0;
				while(trials-->0)
				{
					//Play a move on jth position for this iteration
					g[j].data=comp;
					
					//Fill a character vector uniformly with the player characters
					for(int i=0; i<=(bdSize*bdSize/2-1); ++i)
					{
						randomFillArray.at(i)=hum;
					}
					if((bdSize*bdSize)%2!=0) randomFillArray.at(bdSize*bdSize/2)=firstPlayer;//first player character
					else
					{
						if(firstPlayer==comp)
							randomFillArray.at(bdSize*bdSize/2)=hum;//second player character
						else
							randomFillArray.at(bdSize*bdSize/2)=comp;//second player character

					}
					for(int i=(bdSize*bdSize/2+1); i<bdSize*bdSize; ++i)
					{
						randomFillArray.at(i)=comp;
					}		
								
					//Shuffle those characters which are to be filled in the board
					//With every move we have one char each of players reduced
					random_shuffle(randomFillArray.begin()+m, randomFillArray.end()-m);
										
					//Fill the board with shuffled characters
					int k=0;
					for(auto i=randomFillArray.begin()+m; i!=(randomFillArray.end()-m); ++i)
					{
						while(g[k].data!=0 && k<(bdSize*bdSize-1))	
						{
							k++;//inc k until it reaches position which is empty
						}							
						g[k].data=*i;
					}
					
					//Find if computer wins for the current distribution
					if (compDrEW==true && ewWins(comp))
							count++;
					else if (compDrEW==false && nsWins(comp))
							count++;
					
					//Restore board to its initial status
					for(int i=0; i<(bdSize*bdSize); ++i)
					{
						g[i].data=savedBoard.at(i);
					}
				}	
				//remember the maximum count and the corresponding move position
				if(count>countMax)
				{
					countMax=count;
					pos=j;
				}											
			}
		}
		return pos;					
	}
	int humanMove()
	{
		int r,c;
		do
		{
			cout<<"\n================================================="<<endl;
			cout<<"\nYour Move...\nEnter row number:";
			do
			{
				cin>>r;
				if(r<1||r>bdSize) 
					cout<<"Enter valid row[1-"<<bdSize<<"]:";
			}while (r<1||r>bdSize);
			cout<<"Enter column number:";
			do
			{
				cin>>c;
				if(c<1||c>bdSize) 
					cout<<"Enter valid column[1-"<<bdSize<<"]:";
			}while (c<1||c>bdSize);
		}while(!isValidMove(r,c));
		r--;
		c--;
		return (r*bdSize+c);		
	}
	char play()
	{
		int move=0, pos=0;
		
		//boolean variable pass controls the first move
		//by default true implies user makes first move
		bool pass=true;
		
		char ans;
		do
		{
			cout<<"Do you want to make first move? (y/n)";
			cin>>ans;
		}while(toupper(ans)!='Y'&&toupper(ans)!='N');
		if(toupper(ans)=='N')
			pass=false;
		while(1)
		{
			system("clear");
			if(pass==true)//true if user makes first move
			{
				display();
				pos=humanMove();
				move++;
				update(pos,hum);
			}					
			pos=computerMove(move, toupper(ans));
			update(pos,comp);	
			if((compDrEW==true&&nsWins(hum))||(compDrEW==false&&ewWins(hum)))
			{
				display();
				return hum;
			}
			else if((compDrEW==true&&ewWins(comp))||(compDrEW==false&&nsWins(comp)))
			{
				display();
				return comp;
			}
			pass=true;
		}	
		return '?';//hopefully this situation will never arise
	}
	~hexBoard()
	{
		g.resize(0);
	}
};

//playHexGame to play on HexBoard
class playHexGame
{
	int bdSize; 
	char comp, hum;
	bool compDrEW;
	
	public:
	
	playHexGame(int bdSize=5, char comp='X', char hum='O', bool compDrEW=true):
	bdSize(bdSize), comp(comp), hum(hum), compDrEW(compDrEW)
	{}
	
	char menu()
	{
		char choice;
		do
		{
			system("clear");
			cout<<"THE GAME OF HEX"<<endl
				<<endl<<"1. Play Game"
				<<endl<<"2. Change Board Size"
				<<endl<<"3. Change player character tokens"
				<<endl<<"4. Change direction (East-West) or (North-South)"
				<<endl<<"5. Set defaults"
				<<endl<<"6. Exit";
			cout<<"\n\nBoardSize="<<bdSize<<"  Computer="<<comp<<"  Human="
				<<hum<<"  YourDirection=";
			if(compDrEW==true)	cout<<"North to South";
			else                cout<<"East to West";			
			cout<<"\n=============================================================="<<endl;
			cout<<"\nEnter your choice: ";
			fflush(stdin);
			cin>>choice;	
		}while(choice<'1' || choice>'6');	
		return choice;
	}
	void play()
	{	
		char choice;
		char winner, ans;
		while(1)
		{
			choice=menu();
			hexBoard board(bdSize, comp, hum, compDrEW);
			switch(choice)
			{
				case '1':	winner=board.play();	
							if(winner==hum)
								cout<<"\nCongratulations...\nYou are the winner.";
							else if(winner==comp)
								cout<<"\nLoser...\nComputer wins.";
							else
								cout<<"Error in winner decision";
							break;
						
				case '2':	cout<<"Enter size of board: ";
							do
							{
								cin>>bdSize;
								if(bdSize<2&&bdSize>20) 
									cout<<"Enter a proper size of board[2-20]:";
							}while (bdSize<2&&bdSize>20);
							winner=0;
							break;
						
				case '3':	cout<<"Enter your character token: ";
							cin>>hum;
							cout<<"Enter character token for computer: ";
							cin>>comp;
							winner=0;
							break;
						
				case '4':	do
							{
								cout<<"Do you want to change your direction side? (y/n)";
								cin>>ans;
							}while(toupper(ans)!='Y'&&toupper(ans)!='N');
							if(toupper(ans)=='Y')
								compDrEW=!compDrEW;
							winner=0;
							break;
						
				case '5': 	bdSize=5;
							comp='X';
							hum='O';
							winner=0;
							break;
						
				case '6': 	exit(0);
							break;
						
				default: exit(1);					
			}		
			if (winner!=0)
			{
				do
				{
					cout<<"Do you want to play again? (y/n)";
					cin>>ans;
				}while(toupper(ans)!='Y'&&toupper(ans)!='N');
			}
		}	
	}	
};

int main(int argc, char **argv)
{
	playHexGame game;
	game.play();
	return 0;
}
