#include <iostream>
#include <cstdlib>
#include <functional>
#include <cmath>
#include <string>
#include <time.h>
#include <array>
#include "./picosha2.h"

struct node{
	std::string hashed;
	node *left;
	node *right;
	node *parent;
	int index;
};

struct tree{
	node *root;
	int numNodes;
	int numLevels;
	node *nodeList;
};



std::string hash(std::string value)
{
	return picosha2::hash256_hex_string(value);	
}

std::string hash_combine(std::string h1, std::string h2)
{
	return h1 + h2;
}

void Form(std::string publicKeys[], tree * mt, int num_msgs)
{

	int num_nodes = (num_msgs * 2) - 1; //15
	node* nodes = new node[num_nodes];
	int num_levels = 1 + floor(log2(num_nodes)); //4
	int *num_at_level = new int[num_levels]; 

	mt->numNodes = num_nodes;
	mt->numLevels = num_levels;
	mt->nodeList = nodes;


	int temp = num_nodes + 1; //16
	temp = temp/2; //8

	for(int k = 0; k < num_levels; k++)
	{
		num_at_level[k] = temp;
		temp = temp/2;
	}



	
	/*publicKeys HASHED ALREADY*/
	for(int y = 0; y < num_msgs; y++)
	{
		nodes[y].hashed = publicKeys[y]; /* Hash messages */	
		nodes[y].index = y;
	}
	
	
	int current = num_msgs; //8
	int child = 0;


	for(int level = 1; level < num_levels; level++)
	{
		for(int x = 0; x < num_at_level[level - 1]/2; x++)
		{	
			
			nodes[x + current].hashed = hash( nodes[2*x + current - num_at_level[level - 1]].hashed +  nodes[2*x + current - num_at_level[level - 1] + 1].hashed);
			
				
			
			nodes[x + current].left = &nodes[2*x + current - num_at_level[level - 1]];
			nodes[x + current].right = &nodes[2*x + current -num_at_level[level - 1] + 1];


			nodes[x + current].index = x + current;
		

			if(current != num_nodes){
				nodes[2*x + current - num_at_level[level -1 ]].parent = &nodes[x + current]; // left gets parent
				nodes[2*x + current - num_at_level[level -1 ] + 1].parent = &nodes[x + current]; // right gets parent
			}

	
			if(x == num_at_level[level] - 1){ current = current + num_at_level[level]; }			
		
		}		
		
	}
	mt->root = &(nodes[current - 1]);
	mt->root->index;
}


int * create_msgs(int num)
{
	srand(time(NULL));
	int *msg_arr = new int[num];
	for(int i = 0; i < num; i++)
	{
		msg_arr[i] = rand();
	}
	return msg_arr;
}

node * Path(tree mt, std::string hash2find  ,int pathlen)
{
	node *path = new node[pathlen];
	int msg_num = 0;

	for(int x = 0; x < (mt.numNodes + 1)/2; x++)
	{
		if(mt.nodeList[x].hashed.compare(hash2find) == 0){break;}	
		else{msg_num++;}
	}
	
//	std::cout << msg_num << '\n' << hash2find << '\n';
	path[0] = mt.nodeList[msg_num];

	if(msg_num % 2 == 0){path[1] = mt.nodeList[msg_num + 1];}
	else{path[1] = mt.nodeList[msg_num - 1];}

	node *itr = &(mt.nodeList[msg_num]);
	
	int n = 2;

	while(itr->parent != mt.root)
	{
		itr = itr->parent;
		if(itr->index % 2 == 0)
		{
			path[n] =  mt.nodeList[itr->index + 1];
		}
		else
		{
			path[n] = mt.nodeList[itr->index - 1];
		}	
		n++;
	}

	return path;
}

int Verify(tree mt, node *path, int pathLen)
{
	
	node swap;
	std::string value;
	value = path[0].hashed;	
	

	for(int i = 1; i < pathLen; i++)
	{
		if(path[i].index % 2 == 0)
		{
			value = path[i].hashed + value;		
		}
		else
		{
			value = value + path[i].hashed;
		}

		value = hash(value);
	}
	if (value == mt.root->hashed){return 1;}
	else{return 0;}
	
}












