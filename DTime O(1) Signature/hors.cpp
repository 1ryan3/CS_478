#include <iostream>
#include <cstdlib>
#include <functional>
#include <cmath>
#include <string>
#include <time.h>
#include <array>
#include <cmath>
#include "./picosha2.h"


void keyGen(int l, int t, int k, std::string * privateKey, std::string * publicKey)
{
	srand(time(NULL));	
	int temp = 0;

	for(int i = 0; i < t; i++)
	{
		temp = rand() % (1 << l);
		privateKey[i] = std::to_string(temp); 
		publicKey[i] = picosha2::hash256_hex_string(privateKey[i]);
	}
}


void sign(int msg, std::string *secretKey, std::string * signature, int k)
{


        std::string hex = picosha2::hash256_hex_string(std::to_string(msg));

	for(int i = 0; i < 32; i++)
	{
		signature[i] = secretKey[std::stoi(hex.substr(62 - i*2,2), nullptr, 16)];		
	}	
}

std::string verify(int msg, std::string * signature, std::string * publicKey, int k)
{
	
        std::string hex = picosha2::hash256_hex_string(std::to_string(msg));

	for(int i = 0; i < 32; i++)
	{

		if( picosha2::hash256_hex_string(signature[i] ) == (publicKey[std::stoi(hex.substr(62 - i*2,2), nullptr, 16)]) ){ continue;} 
		else{return "Reject";}			

	}

	return "Accept";
}

int main()
{
	int t = 256;
	int k = 32;
	int l = 16;
	int m = 12345678;
	
	std::string * privateKey = new std::string[t];
	std::string * publicKey = new std::string[t];	
	std::string * signature = new std::string[k];

	clock_t start = clock();
	int i = 32;
	while(i > 0)
	{
		keyGen(l, t, k, privateKey, publicKey); 
		sign(m, privateKey, signature, k);
		std::cout << verify(m, signature, publicKey, k) << '\n';
		i--;
	}
	clock_t end = clock();
	clock_t diff = end - start;
	std::cout << (float(diff))/CLOCKS_PER_SEC << '\n';


}


