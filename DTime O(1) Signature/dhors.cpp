#include <iostream>
#include <cstdlib>
#include <functional>
#include <cmath>
#include <string>
#include <time.h>
#include <array>
#include <cmath>
#include "./picosha2.h"


/*ALL FUNCTIONS THAT ARE DONE SHOULD BE NOTED SO*/


void keyGen(int t, int k, int z, int d, std::string * Vi)//EDITED TO MATCH DTIME
{

	for(int i = 0; i < t*d; i++)
	{ 	 	 
		Vi[i] = picosha2::hash256_hex_string( picosha2::hash256_hex_string(std::to_string(z + i))) ;
	}
}


int signLeaves(int msg, std::string * Vij, uint16_t * Ij, int k, int t, int st, int z, int d)//EDITED TO MATCH DTIME; Only covers steps 1-5
{
	
	std::string Sij;

	if(st > d){return 0;}//return 0 = abort, return 1 = continue


        std::string hex = picosha2::hash256_hex_string(std::to_string(msg));

	for(int j = 0; j < k; j++)
	{
		Ij[j] = ( (st - 1) * t + std::stoi(hex.substr(62 - j*2, 2), nullptr, 16) ); 
		Sij = picosha2::hash256_hex_string( std::to_string(z + Ij[j]) );//H(Z||Ij)
		Vij[j] = picosha2::hash256_hex_string( Sij );
	}	

	return 1;
}



int verifyLeaves(int msg, std::string * Vij,uint16_t * Ij ,int k, int t, int st, int z, int d)//MATCH DTIME, Covers up to but not including actual verification
{
	
	if(st > d){return 0;}//return 0 = abort, return 1 = continue


        std::string hex = picosha2::hash256_hex_string(std::to_string(msg));
	std::string Sij;

	for(int j = 0; j < k; j++)
	{
		Ij[j] = ( (st - 1) * t + std::stoi(hex.substr(62 - j*2, 2), nullptr, 16) ); 
		Sij = picosha2::hash256_hex_string( std::to_string(z + Ij[j]) );//H(Z||Ij)
		Vij[j] = picosha2::hash256_hex_string( Sij );
	}	
	return 1;

}



