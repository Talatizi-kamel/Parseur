# Sprint-2-

Parseur d’articles scientifiques en format texte (carnet du produit)

pour executer le parseur il faut avoir avec le fichier main un dossier avec le nom Papier_a_traiter et a l'intérieur on met
les fichier pdf a convertire

# main.cpp

Affichage de tous les fichiers PDF contenus dans le dossier Papiers a traiter.
Lecture de fichiers PDF choisit par l'utilisatuer on choisissant l'indice correspondant.
Conversion de PDF vers TXT en utilisant la commande pdf2txt. .
Création du dossier ConversitonPdf pour mettre ces conversions.
et on création du dossier résultats qui contient les fichier txt avec les séctions suivants : nom de fichier,Titre,abstract dans chaque ligne

## Utilisation

Pour compiler et tester :

- g++ -std=c++11 main.cpp -o main
- ./main
