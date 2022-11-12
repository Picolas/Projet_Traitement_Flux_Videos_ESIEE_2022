#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <string>


typedef struct
{
	uchar bit7 : 1;
	uchar bit6 : 1;
	uchar bit5 : 1;
	uchar bit4 : 1;
	uchar bit3 : 1;
	uchar bit2 : 1;
	uchar bit1 : 1;
	uchar bit0 : 1;
}st_bits;


// Convertie une valeur décimale en bit ( stocké dans un tableau de int[8] )
void DecToBit(int dec, int* bits) {

	for (int i = 7; i >= 0; i--)
	{
		// test si la valeur decimale est supérieur à 2^i
		if (dec < pow(2, i)) {
			bits[i] = 0;
		}
		else {
			// si la valeur moins 2^i est supérieur à 0 alors on peut le retirer -> 1 à cet emplacement
			if ((dec - pow(2, i)) > 0) {
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
int BitToDec(int bits[]) {

	int result = 0;

	// Pour chaque bit
	for (int i = 0; i < 8; i++)
	{
		// ajoute sa valeur en puissace de 2^i si elle est égale à 1
		if (bits[i] == 1) result += (int) pow(2, i);
	}

	return result;
}

// Calcul le hash pour effectuer la stéganographie
int Hash(int posHiddenbit, int nbLSB = 4) {

	return posHiddenbit % nbLSB;

}


st_bits convert(uchar data) {
	
	st_bits retour;
	for (int i = 7; i >= 0; i--) {
		if (data > pow(2, i)) {
			if (7 == i) retour.bit7 = 1;
			if (6 == i) retour.bit6 = 1;
			if (5 == i) retour.bit5 = 1;
			if (4 == i) retour.bit4 = 1;
			if (3 == i) retour.bit3 = 1;
			if (2 == i) retour.bit2 = 1;
			if (1 == i) retour.bit1 = 1;
			if (0 == i) retour.bit0 = 1;
			data -= (int) pow(2, i);
		}
		else {
			if (7 == i) retour.bit7 = 0;
			if (6 == i) retour.bit6 = 0;
			if (5 == i) retour.bit5 = 0;
			if (4 == i) retour.bit4 = 0;
			if (3 == i) retour.bit3 = 0;
			if (2 == i) retour.bit2 = 0;
			if (1 == i) retour.bit1 = 0;
			if (0 == i) retour.bit0 = 0;
		}
	}
	return retour;

}




int main(int argc, char* argv[]) {


	if (argc < 3) {
		std::cout << "erreur dans l'appel de l'executable !!" << std::endl;
		return 0;
	}
	// Création des matrices contenant chaque image utilisé pour ce programme
	cv::Mat img1 = cv::imread(argv[1]);
	cv::Mat img2 = cv::imread(argv[2]);
	cv::Mat img2_gray;
	cv::Mat stegano;

	// copie de lena dans le resultat -> initialise la taille de resultat
	img1.copyTo(stegano);
	// crée l'image en niveau de gris à insérer à partir de l'image 2
	cv::cvtColor(img2, img2_gray, cv::COLOR_BGR2GRAY);

	// Itération pour chaque lignes
	for (int i = 0; i < img2.rows; i++) {
		// Itérations pour chaque colonnes
		for (int j = 0; j < img2.cols; j++) {
			// récup les valeurs décimals de R G et B pour chaque pixel
			int valRGB[] = { img1.at<cv::Vec3b>(i, j)[0], img1.at<cv::Vec3b>(i, j)[1], img1.at<cv::Vec3b>(i, j)[2] };
			int val_img2 = (int) img2.at<cv::int8_t>(i, j);

			// Récupération des bits de l'image source
			int R[8], G[8], B[8];
			DecToBit(valRGB[0], R);
			DecToBit(valRGB[1], G);
			DecToBit(valRGB[2], B);

			// Récupération des bits de l'image stegano
			int bits[8];
			DecToBit(val_img2, bits);

			// Instantiation de tableau permettant de reconstruire chaque couleurs
			int Rbit[8];
			int Gbit[8];
			int Bbit[8];

			// Test si le bit à insérer est :
			for (int k = 0; k < 8; k++)
			{
				// dans la couleur Rouge
				if (0 <= k < 3) {
					int h = Hash(k);
					for (int l = 0; l < 8; l++)
					{
						if (l == h) {
							Rbit[l] = bits[k];
						}
						else
						{
							Rbit[l] = R[l];
						}
					}
				}
				// dans la couleur Bleu ( Green )
				if (3 <= k < 6) {
					int h = Hash(k);
					for (int l = 0; l < 8; l++)
					{
						if (l == h) {
							Gbit[l] = bits[k];
						}
						else
						{
							Gbit[l] = G[l];
						}
					}
				}
				// dans la couleur Bleu
				if (6 <= k < 8)	{
					int h = Hash(k);
					for (int l = 0; l < 8; l++)
					{
						if (l == h) {
							Bbit[l] = bits[k];
						}
						else
						{
							Bbit[l] = B[l];
						}
					}
				}



				// Transformation de bits en décimal et affectation de la valeur à chaque couleur
				int newR = BitToDec(Rbit);
				stegano.at<cv::Vec3b>(i, j)[0] = newR;

				int newG = BitToDec(Gbit);
				stegano.at<cv::Vec3b>(i, j)[1] = newG;

				int newB = BitToDec(Bbit);
				stegano.at<cv::Vec3b>(i, j)[2] = newB;

			}
		}
	}

	// affichage de chaque image ainsi que l'image finale
	cv::imshow("image 1", img1);
	cv::imshow("image 2", img2);
	cv::imshow("img2_gray", img2_gray);
	cv::imshow("result", stegano);

	// sauvegarde de l'image sur le stockage de l'ordinateur
	cv::imwrite("stegano.png", stegano);

	// delai afin de pouvoir regarder le résultat du traitement
	cv::waitKey(0);


}