﻿#include <iostream>
#include <fstream>

int readTextFile(std::string nameTf);
int createTextFile(std::string nameItf, std::string nameOtf);
int addTextToFile(std::string nameTf, int a);
bool isPrime(int num);

int main() {
	int code = readTextFile("A.txt");
	if (code == -1) {
		std::cout << "File not found";
		return code;
	}

	code = createTextFile("A.txt", "B.txt");
	if (code == -1) {
		std::cout << "File not created";
		return code;
	}
	readTextFile("B.txt");
	return 0;
}

int readTextFile(std::string nameTf) {
	int x;
	std::ifstream tf(nameTf);
	if (!tf.good())
		return -1;
	while (!tf.eof()) {
		tf >> x;
		std::cout << x << '\t';
	}
	std::cout << std::endl;
	tf.close();
	return 0;
}

int createTextFile(std::string nameItf, std::string nameOtf)
{
	std::ifstream itf(nameItf);
	if (!itf.good())
		return -1;
	std::ofstream otf(nameOtf);
	int x;
	while (itf >> x) {
		if (isPrime(x))
			otf << x << '\n';
	}
	itf.close();
	otf.close();
	return 0;
}

int addTextToFile(std::string nameTf, int a) {
	std::ofstream tf(nameTf, std::ios::app);
	if (!tf.good())
		return -1;
	tf << ' ' << a;
	tf.close();
	return 0;
}

bool isPrime(int num) {
	for (int i = 2; i <= num / 2; i++)
		if (num % i == 0)
			return false;
	return true;
}