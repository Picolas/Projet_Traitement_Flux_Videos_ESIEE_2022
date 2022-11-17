# Projet_Traitement_Flux_Video_ESIEE_2022
Projet final en traitement de flux vidéo, réalisé en C++ avec OpenCV

réalisé par Thomas AGNOLA, Raphaël DARMON, Alexis LI et Nicolas PIPLARD.

Mode d'emploi
==========================
Le projet est accessible en 3 méthodes :

- Le lien github
	- cloner le projet
	- choisir entre les deux méthodes suivantes

- Les exécutables et les fichiers associés dans le dossier Executable
	- lancer une invite de commande depuis le dossier courant
	- executer via les arguments suivants :
		- choux : choux.exe choux.mp4
		- stegano : stegano.exe lena.png opencv.jpg
		- decipher : decipher.exe stegano.png

- Le projet Visual Studio complet
	- Choisir la compilation en mode : Release x64
	- Exclure de la compilation les fichiers autres que le fichier à executer
	- Rentrer dans l'onglet Débogage de la solution les arguments associés au fichier à executer
	- lancer l'execution


Choux demande en argument soit rien soit le nom du fichier vidéo.

Stégano demande deux noms d'images, la première étant l'image originale, et la deuxième l'image à cacher.

Decipher demande le nom de l'image cryptée en argument.


Explications sur la solution logicielle mise en oeuvre
==========================

On a commencé par réaliser l'exercice sur les choux avant de s'attaquer à la stéganographie.

Chaque partie est séparée par un ou plusieurs fichiers .cpp (Choux, Stégano, Decipher).
