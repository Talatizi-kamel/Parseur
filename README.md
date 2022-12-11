# Sprint-2-

Parseur d’articles scientifiques en format texte (carnet du produit)

pour executer le parseur il faut avoir avec le fichier main un dossier avec le nom Papier_a_traiter et a l'intérieur on met
les fichier pdf a convertire

# sprint-3-

l'ajout un nouveau type de sortie XML qui sont stocké directement dans un dossier résultatsXML qui sera ajour a chaque lancement du main avec l'argument -x

# main.cpp

Affichage de tous les fichiers PDF contenus dans le dossier Papiers a traiter.
Lecture de fichiers PDF choisit par l'utilisatuer on choisissant l'indice correspondant.
Conversion de PDF vers TXT en utilisant la commande pdf2txt.
Création du dossier ConversitonPdf pour mettre ces conversions.
\_lancement avec argument :

        -t ou rien "" pour la création du dossier résultats qui contient les fichier txt avec les séctions suivants : nom de fichier,Titre,Auteur avec leurs adresses mail,abstract,biblio,dans chaque ligne différente

        -x pour la création du dossier résultatsXML qui contient les fichier XML avec les séctions suivants : nom de fichier,Titre,Auteur avec leurs adresses mail,abstract,dans chaque balise corespondante

## Utilisation

Pour compiler et tester :

- g++ -std=c++11 main.cpp -o main
- ./main pour les résultats txt
- ./main -t pour les résultats xml
