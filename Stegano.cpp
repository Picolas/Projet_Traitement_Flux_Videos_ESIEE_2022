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


void DecToBit(int dec, int* bits) {

	for (int i = 7; i >= 0; i--)
	{
		// test si la valeur decimale est supérieur à 2^i
		if (dec < pow(2, i)) {
			bits[i] = 0;
		}
		else {
			// si la valeur moins 2^i est sup à 0 alors il ne peut pas être soustrait -> 0 à cet emplacement
			if ((dec - pow(2, i)) > 0) {
				bits[i] = 1;
				dec = dec - pow(2, i);
			}
			else {
				bits[i] = 0;
			}
		}
	}

}

int BitToDec(int bits[]) {

	int result = 0;

	for (int i = 0; i < 8; i++)
	{
		if (bits[i] == 1) result += pow(2, i);
	}

	return result;
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
			data -= pow(2, i);
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
	cv::Mat img1 = cv::imread(argv[1]);
	cv::Mat img2 = cv::imread(argv[2]);
	cv::Mat stegano;


	// ancien code 
	/*
	cv::cvtColor(img2, img2, cv::COLOR_BGR2GRAY);
	int img2size = img2.total();
	std::vector<st_bits> img2data;
	int size[2] = { img2.size[0],img.size[1] };
	// traitement ici
	for (int i = 0; i < img2.total(); i++) {
		uchar data = img2.at<uchar>(i / size[0], i % size[0]);
		img2data.at(i) = convert(data);
	}

	cv::imshow("Lena original", img);
	cv::imshow("Lena stegano", stegano);
	*/

	// nouveau code

	// copie de lena dans le resultat -> initialise la taille de resultat
	img1.copyTo(stegano);

	for (int i = 0; i < img2.rows; i++) {
		for (int j = 0; j < img2.cols; j++) {
			// récup les valeurs décimals de R G et B pour chaque pixel
			int temp[] = { img1.at<cv::Vec3b>(i, j)[0], img1.at<cv::Vec3b>(i, j)[1], img1.at<cv::Vec3b>(i, j)[2] };
			int temp2[] = { img2.at<cv::Vec3b>(i, j)[0], img2.at<cv::Vec3b>(i, j)[1], img2.at<cv::Vec3b>(i, j)[2] };

			// Récupération des bits de l'image source
			int R[8], G[8], B[8];
			DecToBit(temp[0], R);
			DecToBit(temp[1], G);
			DecToBit(temp[2], B);

			// Récupération des bits de l'image stegano
			int R2[8], G2[8], B2[8];
			DecToBit(temp2[0], R2);
			DecToBit(temp2[1], G2);
			DecToBit(temp2[2], B2);

			// Création des nouveaux bits pour R G et B
			int Rbit[8] = { R2[0], R[1], R[2], R[3], R[4], R[5], R[6], R[7] };
			int Gbit[8] = { G2[0], G[1], G[2], G[3], G[4], G[5], G[6], G[7] };
			int Bbit[8] = { B2[0], B[1], B[2], B[3], B[4], B[5], B[6], B[7] };

			// Transformation de bits en décimal et affectation de la valeur à chaque couleur
			int newR = BitToDec(Rbit);
			stegano.at<cv::Vec3b>(i, j)[0] = newR;

			int newG = BitToDec(Gbit);
			stegano.at<cv::Vec3b>(i, j)[1] = newG;

			int newB = BitToDec(Bbit);
			stegano.at<cv::Vec3b>(i, j)[2] = newB;

		}
	}

	// affichage de chaque image ainsi que l'image finale
	cv::imshow("image 1", img1);
	cv::imshow("image 2", img2);
	cv::imshow("result", stegano);

	cv::imwrite("stegano.png", stegano);

	cv::waitKey(0);


}