#include <iostream>
#include <cstdlib>
#include <functional>
#include <cmath>
#include <string>
#include <time.h>
#include <array>

struct node{
	size_t hashed;
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



std::size_t hash_str(size_t value_to_hash)
{
	std::hash<std::string> str_hash;
	std::string temp = std::to_string(value_to_hash);
	return str_hash(temp);	
}

std::size_t hash_combine(std::size_t h1, std::size_t h2)
{
	return h1 + h2;
}

void build_tree(int msgs[], tree *mt, int num_msgs)
{
	/* eg. 8 messages */	


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

	/* num_levels = {8 , 4 , 2 , 1} */


	
	/*HASH MESSAGES HERE FIRST*/
	for(int y = 0; y < num_msgs; y++)
	{
		nodes[y].hashed = hash_str((msgs[y])); /* Hash messages */	
		nodes[y].index = y;
	}
	
	
	int current = num_msgs; //8
	int child = 0;


	for(int level = 1; level < num_levels; level++)
	{
		for(int x = 0; x < num_at_level[level - 1]/2; x++)
		{	
			
			nodes[x + current].hashed = hash_str(hash_combine( nodes[2*x + current - num_at_level[level - 1]].hashed , nodes[2*x + current - num_at_level[level - 1] + 1].hashed));
			
			
			nodes[x + current].left = &nodes[2*x + current - num_at_level[level - 1]];
			nodes[x + current].right = &nodes[2*x + current -num_at_level[level - 1] + 1];


			nodes[x + current].index = x + current;
		

			if(current != num_nodes){
				nodes[2*x + current - num_at_level[level -1 ]].parent = &nodes[x + current]; // left gets parent
				nodes[2*x + current - num_at_level[level -1 ] + 1].parent = &nodes[x + current]; // right gets parent
			}

//			std::cout << nodes[2*x + current - num_at_level[level - 1]].index << " " << nodes[2*x + current - num_at_level[level - 1] + 1].index << " " << x + current << '\n';
	
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
		msg_arr[i] = rand() % 100;
	}
	return msg_arr;
}

node *find_path(tree mt ,int msg_num, int pathlen)
{
	node *path = new node[pathlen];

	if(msg_num % 2 == 0){path[0] = mt.nodeList[msg_num + 1];}
	else{path[0] = mt.nodeList[msg_num - 1];}

	node *itr = &(mt.nodeList[msg_num]);
	
	int n = 1;

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

int verify(tree mt, node *path, int pathLen ,int msg)
{
	node *nodes = mt.nodeList;

	size_t value = mt.nodeList[msg].hashed;


	for(int i = 0; i < pathLen; i++)
	{
		value = hash_combine(value, path[i].hashed);
		value = hash_str(value);
	}
	if (value == mt.root->hashed){return 1;}
	else{return 0;}
}


int main()
{
//	std::cout << RAND_MAX << '\n';	
	tree mt;
	tree *p_mt = &mt;
	int *msg_arr = create_msgs(4);
	build_tree(msg_arr, p_mt, 4);
	
	node *path = find_path(mt, 2, mt.numLevels - 1);
	
	for(int n = 0; n < mt.numLevels - 1; n++)
	{
		std::cout << path[n].index << '\n';	
	}

	std::cout << verify(mt, path, mt.numLevels - 1, 2) << '\n';
	//std::cout << mt.root->index <<'\n';
	return 0;
}










