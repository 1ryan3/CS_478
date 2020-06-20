/*
*	Simple Huffman Tree implementation for Network Security (ECE478) using a priority queue for tree construction
*
*	Author: Ryan Wood
*	Date: 5/18/18
*
*	Compile with "-std=c++11" flag
*   Run without arguments
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <climits>
#include <sstream>
#include <vector>
#include <cstring>

struct node{ //Store relevant information for creating the queue and tree
	int freq;//Frequency of symbol
	char letter;//Symbol
	node * left; //Connected nodes
	node * right;
	node();
	node(int, char, node *, node *); //Basic constructor
	node(node*, node*); //Create new parent nodes
};

node::node(){}

node::node(int f, char l, node * lft, node * rt)//initialize members
{
	freq = f;
	letter = l;
	left = lft;
	right = rt;
}

node::node(node * l, node * r) //create a parent node from two leaves or other parent nodes
{
	left = l;
	right = r;
	freq = l->freq + r->freq;
	letter = '~'; //Initialize to a standard symbol as the parents should not hold a symbol from the encoded string themselves
}


struct tree{ //Track tree sizes and root node, store string to encode etc.
	node * root; //store the root node and therefore the tree itself
	int size;	//tree size
	std::map<char, std::string> codedString; //Use a map here as it is an easy to use keypair data structure
	void encode(node *, int *, int); 
	void printCode(int *, int, char);
	void decodeString(std::string);
};

void tree::printCode(int code[], int n, char l)
{
	std::string temp;
	for(int i = 0; i < n; i++)
	{
//		std::cout << code[i];
		temp.append(std::to_string(code[i]));
	}
	codedString.insert(std::make_pair(l,temp));//store letter and encoding in map for visualization 
											   //The full string could also be created here.
//	std::cout << '\n';
}


void tree::encode(node * root, int code[], int top)
{
	if(root->left)//left traversals are 0
	{
		code[top] = 0;
		encode(root->left, code, top + 1);	//recurse until we reach a leaf

	}
	if(root->right)//right traversals are 1
	{
		code[top] = 1;
		encode(root->right, code, top + 1);	//recurse until we reach a leaf
	}

	if(root->letter != '~')//True when we are at a leaf 
	{
//		std::cout << root->letter << " : ";
		printCode(code, top, root->letter); //Print the code for each leaf as we go, and more importantly store it 
	}	
}

void tree::decodeString(std::string encoding)
{	

	std::vector<std::string> v;
	char * temp = strdup(encoding.c_str());
	char * tkn = std::strtok(temp, " ");//tokenize using " " as our delimiter
	node * temp_n;
	while(tkn != NULL)
	{
		v.push_back(std::string(tkn));
		tkn = strtok(NULL, " ");
	}
	
	for(int i = 0; i < v.size(); i++)
	{
		temp_n = root;
		for(int x = 0; x < v[i].length(); ++x)
		{
			if(v[i][x] == '1'){temp_n = temp_n -> right;}
			else if(v[i][x] == '0'){temp_n = temp_n -> left;}
		}
		std::cout << temp_n -> letter;
	}
	std::cout << '\n';
}


struct queue{
	node ** nodeArr; //store nodes in array
	int  index; 
	int  cap; //capacity of queue
	int  amt; //number of stored nodes	
	int addNode(node*);
	node * takeTop();
	void buildQueue(std::map<char, int>);
	void sortQueue();
	queue(int);//construct static+ly sized queue
};



queue::queue(int size)
{
	nodeArr = new node*[size]; //create an array of nodes 
	for(int i = 0; i < size; i++)
	{
		nodeArr[i] = NULL; //initialize them
	}
	cap = size;//track the capacity of our queue
	amt = 0; //initialize the number of items in queue to 0
}

int queue::addNode(node* newNode)
{
	if(amt == cap)
	{
		std::cout << "FULL" << '\n';//queue full
		return 0 ;
	} 
	
		
	amt++;//track absolute queue size
	nodeArr[amt - 1] = newNode;//store new node
	return 1;
	
}

node * queue::takeTop()
{
	node * temp = nodeArr[0];
	for(int i = 0; i < amt - 1; i++)
	{
		nodeArr[i] = nodeArr[i + 1];//move nodes forward
	}
	nodeArr[amt - 1] = NULL;//get rid of top
	amt--;//keep absolute queue size updated
	return temp;//return the top node
}

void queue::buildQueue(std::map<char, int> freqMap)
{
	std::map<char, int>::iterator it = freqMap.begin();//Iterator for map structure
	while(it != freqMap.end())
	{
		std::cout << it->first<< " :: " << it->second << '\n';//debug
		addNode(new node(it->second, it->first, NULL, NULL));//add node with no left or right connections
		it++;
	}	
}

void queue::sortQueue()
{
	node * temp;
	for(int x = 0; x < amt - 1; x++)	
	{
		int done = 1;
			for(int i = 0; i < amt - 1 - x; i++)
			{
				if(nodeArr[i]->freq > nodeArr[i+1]->freq)//sort based on frequency, low to high
				{
					temp = nodeArr[i];
					nodeArr[i] = nodeArr[i+1];
					nodeArr[i+1] = temp;
					done = 0;
				}
			}
		if(done == 1){return;}
	}	
}




int main()
{
	std::string input = "this string was compressed using huffman compression";
	int numUnique = 0;
	std::map<char, int> freqMap;
	char letters[27] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' '};//symbol alphabet, used for map creation.
	int code[100];//arbitrarily sized buffer to store encoding
	
	for(int i = 0; i < 27; i++)//loop through the alphabet we are using
	{
		int amt = std::count (input.begin(), input.end(), letters[i]);//use 'count' from algorithm library to get frequencies
		if(amt != 0)
		{	
			freqMap.insert(std::make_pair(letters[i], amt));//store pair
			numUnique++;//increment number of unique letters so we can size our queue properly
		}
	}

	queue huffQ(numUnique);//queue sized to match number of unique letters
	std::cout << "String to compress: " << input << "\n\n";//feedback/debug
	std::cout << "Creating queue of size: " << huffQ.cap << "\n\n" << "Here are the frequencies: \n";//
	huffQ.buildQueue(freqMap);//build queue from frequency map
	huffQ.sortQueue();//sort prior to tree construction 
	
	std::cout << '\n';
	

	int startAmt = huffQ.amt;//get the starting amount of queue members
	node * temp1 = new node;//Temporary variables for juggling nodes
	node * temp2 = new node;
	tree huffT;//create a tree to hold our nodes etc.



	for(int x = 0; x < startAmt; x++)//build the actual tree
	{
		if(huffQ.amt == 1)//catch a queue with only one member
		{
			huffT.root = huffQ.nodeArr[0];	
			break;
		}		
		
		temp1 = huffQ.takeTop();//Get the first two nodes
		temp2 = huffQ.takeTop();
		node * temp3 = new node(temp1, temp2);//create a parent node with frequency equal to the sum of those nodes
		huffQ.addNode(temp3);//add that new parent to the queue
		huffQ.sortQueue();//sort 
	}	//Repeat!
	std::cout << "\nHere are the encodings: \n";
	huffT.encode(huffT.root, code, 0);//Encode traversals and store in the buffer 'code'

	std::map<char, std::string>::iterator it = huffT.codedString.begin();
	while(it != huffT.codedString.end())//iterate through and print encoding pairs
	{
		std::cout << it->first<< " :: " << it->second << '\n';
		it++;
	}
	std::cout << '\n';	
	std::string fullString_e;//store encoding as full stream of tree traversal strings
	
	for(int y = 0; y < input.length(); y++)//reconstruct encoded string
	{
		fullString_e.append( huffT.codedString.find( input[y] )->second );
		fullString_e.append(" ");
	}
	
	std::cout << "Encoded string \n" << fullString_e << "\n\nDecoded string \n";	//Print encoded string
	huffT.decodeString(fullString_e);//decode and print the encoded string
	return 0;
}








