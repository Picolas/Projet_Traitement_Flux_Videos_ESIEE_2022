#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>


int Hash(int posHiddenbit, int nbLSB = 4) {

	return posHiddenbit % nbLSB;

}

// Convertie une valeur décimale en bit ( stocké dans un tableau de int[8] )
void decToBit(int dec, int* bits) {

	for (int i = 7; i >= 0; i--)
	{
		// test si la valeur decimale est supérieur à 2^i
		if (dec < pow(2, i)) {
			bits[i] = 0;
		}
		else {
			// si la valeur moins 2^i est supérieur à 0 alors on peut le retirer -> 1 à cet emplacement
			if ((dec - pow(2, i)) >= 0) {
				bits[i] = 1;
				dec = dec - pow(2, i);
			}
			// sinon 0
			else {
				bits[i] = 0;
			}
		}
	}

}

// Convertie un tableau de bit ( int[8] ) en une valeur décimale
int bitToDec(int bits[]) {

	int result = 0;

	// Pour chaque bit
	for (int i = 0; i < 8; i++)
	{
		// ajoute sa valeur en puissace de 2^i si elle est égale à 1
		if (bits[i] == 1) result += (int)pow(2, i);
	}

	return result;
}

// Recup des bit dans les bon emplacement via la caclul d'un hash
void recupBitWithHash(int bits[], int Bbit[], int Gbit[], int Rbit[]) {

	// Test si le bit à insérer est :
	for (int i = 0; i < 8; i++)
	{
		// dans la couleur Rouge
		if (i >= 0 && i < 3) {
			int h = Hash(i);
			for (int j = 0; j < 8; j++)
			{
				if (j == h) {
					bits[i] = Rbit[j];
				}
			}
		}
		// dans la couleur Bleu ( Green )
		if (i >= 3 && i < 6) {
			int h = Hash(i);
			for (int j = 0; j < 8; j++)
			{
				if (j == h) {
					bits[i] = Gbit[j];
				}
			}
		}
		// dans la couleur Bleu
		if (i >= 6 && i < 8) {
			int h = Hash(i);
			for (int j = 0; j < 8; j++)
			{
				if (j == h) {
					bits[j] = Bbit[j];
				}
			}
		}
	}

}


int main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cout << "Erreur dans l'appel de l'executable !" << "\n" << "Veuillez spécifier que l'image duquel récupérer la stégano" << std::endl;
		return 0;
	}

	cv::Mat source = cv::imread(argv[1]);

	// Recup length of row for the image to decipher
	int valBGR[3];
	valBGR[0] = source.at<cv::Vec3b>(0, 0)[0];
	valBGR[1] = source.at<cv::Vec3b>(0, 0)[1];
	valBGR[2] = source.at<cv::Vec3b>(0, 0)[2];

	int bitRecup[8] = {0};

	std::cout << " valeur dec R : " << valBGR[2] << std::endl;

	std::cout << " valeur dec G : " << valBGR[1] << std::endl;

	std::cout << " valeur dec B : " << valBGR[0] << std::endl;

	// Récupération des bits de l'image source
	int B[8], G[8], R[8];
	decToBit(valBGR[0], B);
	decToBit(valBGR[1], G);
	decToBit(valBGR[2], R);

	recupBitWithHash(bitRecup, B, G, R);

	int row = bitToDec(bitRecup);

	std::cout << "valeur de bit pour : ";
	std::cout << " bitREcup : ";
	for (int var : bitRecup)
	{
		std::cout << " " << var;
	}
	std::cout << std::endl;
	std::cout << "R : ";
	for (int var : R)
	{
		std::cout << " " << var;
	}
	std::cout << std::endl;
	std::cout << "G : ";
	for (int var : G)
	{
		std::cout << " " << var;
	}
	std::cout << std::endl;
	std::cout << "B : ";
	for (int var : B)
	{
		std::cout << " " << var;
	}
	std::cout << std::endl;

	std::cout << "valeur de rowoffset : " << row << std::endl;

	cv::waitKey(0);


}