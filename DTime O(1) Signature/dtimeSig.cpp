#include "./picosha2.h"
#include <iostream>
#include <cstdlib>
#include <functional>
#include <cmath>
#include <string>
#include <time.h>
#include <array>
#include "dmerkle.cpp"
#include "dhors.cpp"

int main()
{
	clock_t start = clock();
	int success = 0;
	int t = 256;
	int k = 32;
	int d = 32;
	int st = 1;
	int z = 1234;
	
	tree mt;
	tree * p_mt = &mt;
	int *msg_arr = create_msgs(d);
	
	std::string * Vi = new std::string[t*d];
	std::string * Vij = new std::string[k];
	uint16_t * Ij = new uint16_t[k];
	keyGen(t, k, z, d, Vi);
	Form(Vi, p_mt, t*d);

//	std::cout << mt.nodeList[4092].hashed << " " <<  mt.nodeList[4093].hashed << '\n' << mt.root->hashed<< '\n';
	
	
	node ** path = new node*[k];

	for(int y = 0; y < k; y++)
	{
		path[y] = new node[mt.numLevels - 1];
	}
	

	for(st = 0; st < d; st++)
	{

		if(signLeaves(msg_arr[st], Vij, Ij , k, t, st + 1, z, d) == 0)
		{
			std::cout << "ABORT" << '\n';
			return 0;
		}
		for(int y = 0; y < k; y++)
		{
			path[y] = Path(mt, Vij[y] ,mt.numLevels);					

			if(verifyLeaves(msg_arr[st], Vij, Ij, k, t, st + 1, z ,d) == 0)
			{
				std::cout << "ABORT" << '\n';
				return 0;
			}

			 std::cout <<  Verify(mt ,path[y] ,mt.numLevels) << " : " << st << '\n';
			 success += 1;			
		}
	}
	clock_t end = clock();
	clock_t diff = end - start;
 	std::cout << (float(diff)) / CLOCKS_PER_SEC << '\n';	
}
