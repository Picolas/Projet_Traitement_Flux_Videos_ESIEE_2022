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
					bits[i] = Bbit[j];
				}
			}
		}
	}
}

int recupDecAt(int i, int j, cv::Mat img) {

	// Recover the bits at 
	int valBGR[3];
	valBGR[0] = img.at<cv::Vec3b>(i, j)[0];
	valBGR[1] = img.at<cv::Vec3b>(i, j)[1];
	valBGR[2] = img.at<cv::Vec3b>(i, j)[2];

	int bitRecup[8] = { 0 };

	int B[8], G[8], R[8];
	decToBit(valBGR[0], B);
	decToBit(valBGR[1], G);
	decToBit(valBGR[2], R);

	recupBitWithHash(bitRecup, B, G, R);

	int value = bitToDec(bitRecup);

	return value;
}

int main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cout << "Erreur dans l'appel de l'executable !" << "\n" << "Veuillez spécifier que l'image duquel récupérer la stégano" << std::endl;
		return 0;
	}

	cv::Mat source = cv::imread(argv[1]);

	int offset = 2;

	int rowOffset = recupDecAt(0, 0, source);
	int colOffset = recupDecAt(0, 1, source);

	int row = 0;
	int col = 0;

	for (int i = offset; i < offset + rowOffset; i++) {
		row += recupDecAt(0, i, source);
	}

	for (int i = offset + rowOffset; i < offset + rowOffset + colOffset; i++) {
		col += recupDecAt(0, i, source);
	}

	cv::Mat recover = cv::Mat(row, col, CV_8U);

	for (int i = 0; i < row; i++) {
		if (0 == i) {
			for (int j = offset + rowOffset + colOffset; j < col; j++) {
				recover.at<cv::int8_t>(i, j + offset + rowOffset + colOffset) = recupDecAt(i, j + offset + rowOffset + colOffset, source);
			}
		}
		else {
			for (int j = 0; j < col; j++) {
				recover.at<cv::int8_t>(i, j) = recupDecAt(i, j, source);
			}
		}
		
	}

	cv::imshow("recovered image" , recover);

	cv::waitKey(0);


}