
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/wait.h>

using namespace std;

int main()
{

	int pid;

	//getting # of nodes per level

	//Parent Level
	int numberOfLevelOne;
	if(cin>>numberOfLevelOne)
	{

		//check to ensure parent has a child
		if(numberOfLevelOne<1)
		{
			cout<<"Not a valid 3 level tree structure, Parent has not child"<<endl;
			_exit(0);

		}
	}
	else
	{
		//catch non integers
		cout<<"Found non valid input for parent"<<endl;
		_exit(0);
	}

	//Level One
	int levelOne[numberOfLevelOne];
	int x;
	int numberOfLevelTwo=0;
	bool validLevelOne=false;
	for(x=0;x<numberOfLevelOne;x++)
	{

		if(cin>>levelOne[x])
		{
			//at least one node has a child
			if(levelOne[x]>=1)
				validLevelOne=true;
			//summing total number of nodes for next level
			numberOfLevelTwo+=levelOne[x];
		}
		else
		{
			//catch non integers
			cout<<"Found non  valid input for level one"<<endl;
			_exit(0);
		}
	}
	//check to ensure level one has alteast one child
	if(!validLevelOne)
	{
		cout<<"Not a valid 3 level tree structure, Level one"<<endl;
		_exit(0);
	}

	//Level Two
	//checking rest of file to enure only a 3 level tree
	int y;
	int checkLevelTwo;
	for(y=0;y<numberOfLevelTwo;y++)
	{

		if(cin>>checkLevelTwo)
		{

			//ensure there are no children of level 2
			if(checkLevelTwo!=0)
			{
				cout<<"not a valid 3 level tree structure, Level two"<<endl;
				_exit(0);
			}
		}
		else
		{
			//catch non integers
			cout<<"Found non valid input for level two"<<endl;
			_exit(0);
		}
	}

	//Building of tree starts from here

	//list to save pids of level one
	int levelOnePidList[numberOfLevelOne];

	//Making Level one nodes
	int i;
	int k;
	for (i=0;i<numberOfLevelOne;i++)
	{
		pid=fork();
		if(pid==0)
		{
			break;
		}
		levelOnePidList[i]=pid;
	}

	//loop to match correct node with correct number of children
	if(pid==0)
	{

		//list to save PID of level two
		int levelTwoPidList[levelOne[i]];

		//loop to make level two nodes;
		for(int n=0;n<levelOne[i];n++)
		{
			pid=fork();
			if(pid==0)
			{
				cout<<"I'm a leaf process"<<endl;
				sleep(2);
				return(0);
			}
			levelTwoPidList[n]=pid;
		}

		//level one parent wating
		while(wait(0)!=-1);

		//closes level one nodes with no children to prevent creating file
		if(levelOne[i]<1)
			return(0);


//		cout<<"Creating File Pid: "<<getpid()<<endl;

		//Creating and printing list of childrent PIDs to parent files
		stringstream newConverter;
		newConverter<<"File:"<<(int)getpid()<<".txt";
		string parentFilename=newConverter.str();
		ofstream outputChild(parentFilename.c_str());
		//outputChild.open(parentFilename.c_str());
		if(outputChild.fail())
		{
			cerr<<"Could not create file"<<endl;
			_exit(0);
		}
		for(int n=0;n<levelOne[i];n++)
		{
			//cout<<LevelTwoPidList[n]<<endl;
			outputChild<<levelTwoPidList[n]<<endl;
		}
		outputChild.close();
//		cout<<"finished file: "<<parentFilename<<endl;
		return(0);
	}
	//tree complete

	//parent waiting
	while(wait(0)!=-1);

	//printing out final results
	cout<<endl<<"Root pid: "<<getpid()<<endl;
	for(int n=0;n<numberOfLevelOne;n++)
	{
		if(levelOne[n]>0)
		{
			ifstream finalInput;
			stringstream finalConverter;
			finalConverter<<"File:"<<levelOnePidList[n]<<".txt";
			string levelOneFilename = finalConverter.str();

//			cout<<"opening file: "<<levelOneFilename<<endl;

			finalInput.open(levelOneFilename.c_str());
			if(finalInput.fail())
			{
				cerr<<"Could not input open file"<<endl;
				_exit(0);
			}
			string temp;
			stringstream levelThreePrintString;
			while(getline(finalInput,temp))
			{
				levelThreePrintString<<temp<<endl;
			}
			cout<<"File:"<<levelOnePidList[n]<<endl<<levelThreePrintString.str();
			finalInput.close();
		}
	}
	return(0);
}
