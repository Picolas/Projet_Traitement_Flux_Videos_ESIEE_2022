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

// Insertion des bit dans les bon emplacement via la caclul d'un hash
void InsertBitWithHash(int bits[], int Rbit[], int Gbit[], int Bbit[]) {

	// Test si le bit à insérer est :
	for (int i = 0; i < 8; i++)
	{
		// dans la couleur Rouge
		if (0 <= i < 3) {
			int h = Hash(i);
			for (int j = 0; j < 8; j++)
			{
				if (j == h) {
					Rbit[j] = bits[i];
				}
			}
		}
		// dans la couleur Bleu ( Green )
		if (3 <= i < 6) {
			int h = Hash(i);
			for (int j = 0; j < 8; j++)
			{
				if (j == h) {
					Gbit[j] = bits[i];
				}
			}
		}
		// dans la couleur Bleu
		if (6 <= i < 8) {
			int h = Hash(i);
			for (int j = 0; j < 8; j++)
			{
				if (j == h) {
					Bbit[j] = bits[i];
				}
			}
		}
	}

}

// Copie un tableau dans un autre
void TabCopy(int out[], int in[]) {

	for (int i = 0; i < 8; i++)
	{
		out[i] = in[i];
	}

}

void SteganoAt(int i, int j, int value, cv::Mat& img, cv::Mat& result, bool asOffset = false, int offset = 0) {

	// insertion du nombre de lignes
	int bits[8];
	DecToBit(value, bits);
	int ValRGB[3];
	if (asOffset) {
		ValRGB[0] = img.at<cv::Vec3b>(i, j - offset)[0];
		ValRGB[1] = img.at<cv::Vec3b>(i, j - offset)[1];
		ValRGB[2] = img.at<cv::Vec3b>(i, j - offset)[2];
	}
	else {
		ValRGB[0] = img.at<cv::Vec3b>(i, j)[0];
		ValRGB[1] = img.at<cv::Vec3b>(i, j)[1];
		ValRGB[2] = img.at<cv::Vec3b>(i, j)[2];

	}

	// Récupération des bits de l'image source
	int B[8], G[8], R[8];
	DecToBit(ValRGB[0], B);
	DecToBit(ValRGB[1], G);
	DecToBit(ValRGB[2], R);

	// Copy dans le tableau finale
	int Rbit[8], Gbit[8], Bbit[8];
	TabCopy(Bbit, B);
	TabCopy(Gbit, G);
	TabCopy(Rbit, R);

	// insertion de la valeur de row dans les bits de couleur de lena
	InsertBitWithHash(bits, Bbit, Gbit, Rbit);

	// Transformation de bits en décimal et affectation de la valeur à chaque couleur
	int newB = BitToDec(Bbit);
	if (asOffset) result.at<cv::Vec3b>(i, j - offset)[0] = newB;
	else result.at<cv::Vec3b>(i, j)[0] = newB;

	int newG = BitToDec(Gbit);
	if (asOffset) result.at<cv::Vec3b>(i, j - offset)[1] = newG;
	else result.at<cv::Vec3b>(i, j)[1] = newG;

	int newR = BitToDec(Rbit);
	if (asOffset) result.at<cv::Vec3b>(i, j - offset)[2] = newR;
	else result.at<cv::Vec3b>(i, j)[2] = newR;

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


	// Insertion des valeurs de Mat de l'image 2 dans image 1

	// création du offset pour insérer l'image à la suite
	// avec un offset par lignes/colonnes qui permet de dépasser la limite de 255 donner par un pixel
	int temp = 0;
	int offset = 4;
	int rowoffset = 1;
	int coloffset = 1;

	// récupération des lignes et des colonnes
	int row = img2.rows;
	int col = img2.cols;

	// calcul du nombre d'offset pour les lignes
	temp = row;
	while (temp > 255) {
		offset += 1;
		temp -= 255;
		rowoffset += 1;
	}

	// calcul du nombre d'offset pour les colonnes
	temp = col;
	while (temp > 255) {
		offset += 1;
		temp -= 255;
		coloffset += 1;
	}

	// Ajout des valeurs pour la quantité de lignes et de colonnes
	SteganoAt(0, 1, rowoffset, img1, stegano);
	SteganoAt(0, 2, coloffset, img1, stegano);

	// Ajout de la valeur pour les lignes
	temp = row;
	for (int i = 2; i <= 2 + rowoffset; i++)
	{
		if (temp > 255) SteganoAt(0, i, 255, img1, stegano);
		else {
			SteganoAt(0, i, temp, img1, stegano);
			temp -= 255;
		}
	}

	// Ajout de la valeur pour les colonnes
	temp = col;
	for (int i = 2 + coloffset; i <= 2 + coloffset + rowoffset; i++)
	{
		if (temp > 255) SteganoAt(0, i, 255, img1, stegano);
		else {
			SteganoAt(0, i, temp, img1, stegano);
			temp -= 255;
		}
	}

	// Insertion de l'image 2 dans 1
	
	// Itération pour chaque lignes
	for (int i = 0; i < row; i++) {
		// Itérations pour chaque colonnes
		for (int j = offset; j < col + offset; j++) {

			// récup la valeur décimal du gray pour chaque pixel
			int val_img2 = img2.at<cv::int8_t>(i, j - offset);

			// effectue la stégano en appelant avec les bon paramaètres la fonction suivante
			SteganoAt(i, j, val_img2, img1, stegano, true, offset);

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