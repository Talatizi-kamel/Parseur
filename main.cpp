#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <regex>

using namespace std;

smatch m;
regex reg_discussion("([0-9I\\.\\s])*Discussion\\s*", regex_constants::icase);
/*

/// @Talbi aldja
/// @param nom
/// @param val1
/// @param val2

elle sert a remplacer une valeure val1 dans une chaine de caractéres par la valeur val2 dans la meme position que val1

*/
void remplacer_Val1_avec_Val2(string &nom, string val1, string val2)
{
	size_t position = nom.find(val1);
	while (position != -1)
	{
		nom.replace(position, val1.size(), val2);
		position = nom.find(val1, position + val2.size());
	}
}

// retourne la chaine passée en parametre en lowercase
string lower(string s)
{
	for_each(s.begin(), s.end(), [](char &c)
			 { c = ::tolower(c); });
	return s;
}
/*

/// @auteur Talatizi , Amiar , Talbi , Amara
/// @param  liste de string
/// @return  une liste qui contient les noms des fichiers convertis vers txt

elle sert a convertire des fichiers qui son passé en argument de pdf vers txt avec l outil pdf2txt
et les stocker dans répertoire  ConvertionToTxt

*/
vector<string> convertionPDFtoTxt(vector<string> liste)
{
	DIR *dossier;
	struct dirent *fichier;
	vector<string> listeTxt;

	dossier = opendir("./Papiers_a_traiter");
	if (!dossier)
	{
		cout << "Pas de dossier Papiers_a_traiter " << endl;
		return listeTxt;
	}
	int i = 0;
	while ((fichier = readdir(dossier)) != NULL)
	{
		for (int j = 0, length = liste.size(); j < length; j++)
		{
			if (liste[j] == fichier->d_name)
			{
				string tmpFichier = fichier->d_name;
				string nouvFichier = fichier->d_name;
				remplacer_Val1_avec_Val2(nouvFichier, "pdf", "txt");
				listeTxt.push_back(nouvFichier);
				remplacer_Val1_avec_Val2(nouvFichier, " ", "\\ ");
				remplacer_Val1_avec_Val2(tmpFichier, " ", "\\ ");
				string commande = "pdf2txt -o ";
				commande += "./ConvertionToTxt/";
				commande += nouvFichier.c_str();
				commande += " ";
				commande += "./Papiers_a_traiter/";
				commande += tmpFichier.c_str();
				system(commande.c_str());
			}
		}
		i++;
	}
	closedir(dossier);
	return listeTxt;
}

/*
/// @auteur Amiar Fazia
/// @param is
/// @param ligne
/// @return

*/
string Titre(istream &is, string &ligne)
{
	string titre;
	// REGEX pour les noms
	const string nom = "([A-ZÀÁÂÄÃÅĄĆČĖĘÈÉÊËÌÍÎÏĮŁŃÒÓÔÖÕØÙÚÛÜŲŪŸÝŻŹÑßÇŒÆČŠŽ∂ð][a-z´ˇàáâäãåąčćęèéêëėįìíîïłńòóôöõøùúûüųūÿýżźñçčšž]+)(\\-[A-ZÀÁÂÄÃÅĄĆČĖĘÈÉÊËÌÍÎÏĮŁŃÒÓÔÖÕØÙÚÛÜŲŪŸÝŻŹÑßÇŒÆČŠŽ∂ð][a-z´ˇàáâäãåąčćęèéêëėįìíîïłńòóôöõøùúûüųūÿýżźñçčšž]+)?";
	string nom_complet = "((" + nom + ")|([A-Z]\\.)+)\\s" + nom + "(\\s" + nom + ")?";
	string ligne_noms = "(" + nom_complet + "((\\s?.*\\s?)?,(\\s?.*\\s?)?(,\\s?.?\\s?)?)*)+(\\s*)";
	const regex regex_ligne_noms(ligne_noms);
	const regex regex_nom(nom_complet);
	const regex regex_titre("([0-9]|\\(|\\)|/|\\.)");

	while (getline(is, ligne) && !regex_match(ligne, m, regex_ligne_noms))
	{
		if (ligne.size() == 0 || titre.find(ligne) != -1 || regex_search(ligne, m, regex_titre))
			continue;
		titre += ligne + " ";
	}

	return titre;
}
/*
/// @auteur Amara Melyssa
/// @param ligne
/// @param abstract
elle sert a trouver le mot cle abstract dans le fichier
*/
void Abstract(string ligne, string &abstract)
{

	if (ligne.size() == 0)
	{
		abstract += "\n";
		return;
	}
	else if (ligne.size() != 0)
	{
		if (ligne.find("Abstract") != -1 || ligne.find("ABSTRACT") != -1)
		{
			if (ligne.size() > 8)
			{
				abstract = abstract + ligne.substr(9, ligne.size() - 1);
			}
			abstract += "\n";
			return;
		}
		if (ligne.substr(ligne.size() - 1, ligne.size()) == "-")
		{
			abstract = abstract + ligne.substr(0, ligne.size() - 1);
		}
		else
		{
			abstract = abstract + ligne + " ";
		}
	}
	abstract += "\n";
	return;
}

void getAuteursMail(string ligne, vector<string> &name, vector<string> &email)
{

	/*int aut_size =0;
	int mail_size = 0;*/

	string tmp;
	bool found;

	// REGEX pour les noms
	const string nom = "([A-ZÀÁÂÄÃÅĄĆČĖĘÈÉÊËÌÍÎÏĮŁŃÒÓÔÖÕØÙÚÛÜŲŪŸÝŻŹÑßÇŒÆČŠŽ∂ð][a-z´ˇàáâäãåąčćęèéêëėįìíîïłńòóôöõøùúûüųūÿýżźñçčšž]+)(\\-[A-ZÀÁÂÄÃÅĄĆČĖĘÈÉÊËÌÍÎÏĮŁŃÒÓÔÖÕØÙÚÛÜŲŪŸÝŻŹÑßÇŒÆČŠŽ∂ð][a-z´ˇàáâäãåąčćęèéêëėįìíîïłńòóôöõøùúûüųūÿýżźñçčšž]+)?";
	string nom_complet = "((" + nom + ")|([A-Z]\\.)+)\\s" + nom + "(\\s" + nom + ")?";
	string ligne_noms = "(" + nom_complet + "((\\s?.*\\s?)?,(\\s?.*\\s?)?(,\\s?.?\\s?)?)*)+(\\s*)";
	const regex regex_ligne_noms(ligne_noms);
	const regex regex_nom(nom_complet);

	// REGEX pour les adresses mail
	const regex regex_email("((([\\w\\-]+)(\\.)?([\\w\\-]+))|(\\{\\s*(([\\w\\-]+)(\\.)?([\\w\\-]+))\\s*(\\s*,\\s*(([\\w\\-]+)(\\.)?([\\w\\-]+)))\\s*\\}))@([\\w\\-]+)(\\.(\\w+))+", regex::icase);

	regex_token_iterator<string::iterator> rend; // regex vide pour la boucle
	// cout <<ligne<<endl;
	//  si la ligne est de la forme : nom [, nom [, nom..]]
	if (regex_match(ligne, regex_ligne_noms))
	{
		// cout<< "\nligne avec noms trouvée : "<<ligne<<endl;
		// recherche des noms
		regex_token_iterator<string::iterator> iter_noms(ligne.begin(), ligne.end(), regex_nom, 0);
		while (iter_noms != rend)
		{
			found = false;
			tmp = (*iter_noms++).str();
			// cout << " [" << tmp << "]";
			for (int i = 0, length = name.size(); i < length; i++) // verifier si le nom n'existe pas deja dans la liste des noms
				if (name[i] == tmp)
				{
					found = true;
					break;
				}
			if (!found)
				name.push_back(tmp);
		}
	}

	regex_token_iterator<string::iterator> iter_adr(ligne.begin(), ligne.end(), regex_email, 0);
	// ajout au tableau
	while (iter_adr != rend)
	{
		found = false;
		// cout << " [" << *b++ << "]";
		tmp = (*iter_adr++).str();
		tmp = lower(tmp);
		for (int i = 0, length = email.size(); i < length; i++) // verifier si l'adresse n'existe pas deja
			if (email[i] == tmp)
			{
				found = true;
				break;
			}
		if (!found)
			email.push_back(tmp);
	}
	return;
}

void getBiblio(string ligne, vector<string> &biblio)
{
	string tmpLigne;

	// cout << "on traite la ligne : " << ligne << endl;

	if (ligne.size() == 0 || ligne.find("References") != -1 || ligne.find("REFERENCES") != -1 || ligne.find("REFERENCE") != -1 || ligne.find("Reference") != -1)
	{
		return;
	}

	// cout << "on traite la ligne : " << ligne << endl;

	// Si le dernier caractère de la ligne est un tiret, on l'enlève, si non on ajoute un blanc
	if (ligne.substr(ligne.size() - 1, ligne.size()) == "-")
	{
		tmpLigne = ligne.substr(0, ligne.size() - 1);
	}
	else
	{
		tmpLigne = ligne + " ";
	}

	// cout << "tmp ligne : " << ligne << endl;
	// cout << "on traite la ligne : " << ligne << endl;

	// Si la ligne commence par [ (ex : [1] ) ou qu'on a 0 occurence, on crée une nouvelle réf
	if (ligne.substr(0, 1) == "[" || biblio.size() == 0)
	{
		biblio.push_back(tmpLigne);
	}
	else
	{
		// cout << "biblio size: " << biblio.size() << endl;
		biblio[biblio.size() - 1] += tmpLigne;
	}
}

void Introduction(string &ligne, string &intro, ifstream &fichier, int &abs)
{
	regex reg_intro("([0-9I\\.\\s])*Introduction\\s*", regex_constants::icase);
	smatch m;
	regex point(".*\\.\\s*", regex_constants::icase);
	regex verif(".{0,2}(2|II)\\.?\\s+.*");
	regex vide("[0-9]*");
	//(ligne.find("2.") == -1 && ligne.find("II.") == -1 && ligne.find("2 ") == -1 && ligne.find("2 ") == -1  )
	// On cherche l'introduction du début
	if (regex_match(ligne, m, reg_intro))
	{
		int ligne_vide = 0; // 1; ligne se termine avec un point 2 : ligne vide et la ligne precedent se termine avec un point 0: aucun

		// La recherche de la fin de l'introduction
		while (!regex_match(ligne, m, verif) || ligne_vide != 2)
		{

			if (regex_match(ligne, m, point))
			{
				ligne_vide = 1;
			}
			else if ((ligne.size() == 0 || regex_match(ligne, m, vide)) && ligne_vide >= 1)
			{
				ligne_vide = 2;
			}
			/*else if((!regex_match(ligne,m,vide) || ligne.size() == 0) && ligne_vide == 0 ){
				ligne_vide = 2;
			}*/
			else
				ligne_vide = 0;

			intro = intro + ligne + "\n";
			if (!getline(fichier, ligne))
				break;
		}
		abs = 6;
	}
}

void Corps(string ligne, string &corps)
{
	if (ligne.size() == 0)
	{
		corps += "\n";
		return;
	}
	else if (ligne.size() != 0)
	{
		if (ligne.size() >= 2 && (ligne.substr(1, 2) == "2" || ligne.substr(0, 1) == "2" || ligne.substr(0, 2) == "2." || ligne.substr(1, 3) == "2." || ligne.substr(1, 3) == "II" || ligne.substr(0, 2) == "II"))
		{
			corps += "\n";
			return;
		}
		if (ligne.substr(ligne.size() - 1, ligne.size()) == "-")
		{
			corps = corps + ligne.substr(0, ligne.size() - 1);
		}
		else
		{
			corps = corps + ligne + " ";
		}
	}
	corps += "\n";
	return;
}

void Discussion(string ligne, string &discussion)
{

	if (ligne.size() != 0)
	{
		if (ligne.substr(ligne.size() - 1, ligne.size()) == "-")
		{
			discussion = discussion + ligne.substr(0, ligne.size() - 1);
		}
		else if (ligne.substr(ligne.size() - 1, ligne.size()) == ".")
		{
			discussion = discussion + ".\n" + ligne.substr(0, ligne.size());
		}
		else
		{
			discussion = discussion + ligne + " ";
		}
	}
	return;
}

void getConclusion(string ligne, string &conclusion)
{
	size_t Co = ligne.find("Conclusion");
	size_t CO = ligne.find("CONCLUSION");
	size_t Cos = ligne.find("Conclusions");
	size_t COS = ligne.find("CONCLUSIONS");
	if (ligne.size() == 0)
	{
		conclusion += "\n";
		return;
	}
	else if (ligne.size() != 0)
	{
		if (Co != -1 || CO != -1 || Cos != -1 || COS != -1)
		{
			conclusion += "\n";
			return;
		}

		if (ligne.substr(ligne.size() - 1, ligne.size()) == "-")
		{
			conclusion = conclusion + ligne.substr(0, ligne.size() - 1);
		}
		else
		{
			conclusion = conclusion + ligne + " ";
		}
	}
	conclusion += "\n";
	return;
}

/*
 @auteur Talatizi Kamel

 c est pour que elle supperime les anciens répertoire a chaque lancement du parseur pour éviter de créer des copies
 et bien sur aprés chaque suppression on ajoute a nouveau les memes répertoires
*/
void suppressionRep()
{
	system("rm -r ConvertionToTxt");
	system("mkdir ConvertionToTxt");
	system("rm -r résultats");
	system("mkdir résultats");
	system("rm -r résultatsXml");
	system("mkdir résultatsXml");
}
/*
/// @auteur Amara Melyssa
/// @return vecteur de type string

elle sert a stocker des noms des fichier pdf  dans un vecteur de type string qui son dans un répertoire  donner
*/
vector<string> listeFichier()
{

	DIR *dossier;
	struct dirent *fichier;
	vector<string> liste;

	dossier = opendir("./Papiers_a_traiter");
	if (!dossier)
	{
		cout << "Pas de dossier Papers" << endl;
		return liste;
	}

	while ((fichier = readdir(dossier)) != NULL)
	{
		string test = fichier->d_name;
		if (test.length() > 4 && test.substr(test.length() - 4, test.length()) == ".pdf")
		{
			liste.push_back(test);
		}
	}
	closedir(dossier);
	return liste;
}
/*
/// @auteur Amara melyssa
/// @param val1
/// @return la liste des fichiers choisie pour la converstions
*/
vector<int> quelsFichiers(int val1)
{
	vector<int> nums;
	int num, taille = val1;
	cout << endl
		 << "Quels fichiers ? ([1 à " << taille << "], 0 pour sortir)" << endl;
	cout << "Entrez le chiffre : ";
	cin >> num;
	while (num != 0 && num > 0 && num <= taille)
	{
		if (find(nums.begin(), nums.end(), num) == nums.end())
		{
			nums.push_back(num);
		}
		cout << "Entrez le chiffre : ";
		cin >> num;
	}
	return nums;
}

/* retourne

/// @auteur Talbi Aldja
/// @param listeFichier
/// @param choix
/// @return un vecteur avec le nom des fichiers choisit

*/
vector<string> leChoix(vector<string> listeFichier, vector<int> choix)
{
	vector<string> final;
	for (int i = 0, length = choix.size(); i < length; i++)
	{
		final.push_back(listeFichier[choix[i] - 1]);
	}
	return final;
}
/*
/// @auteur Talatizi Kamel
/// @amiar
/// @param fichier
/// @param Titre
/// @param abstract
ca sert a la création de notre résultat lors d ouverture notre fichier
*/
void Creationresulats(string fichier, string Titre, string abstract, vector<string> auteurs, vector<string> emails, vector<string> biblio, string intro, string corps, string discussion, string conclusion)
{
	string fich = "./résultats/" + fichier;
	ofstream synth(fich.c_str());

	string temp = fichier;
	remplacer_Val1_avec_Val2(temp, "txt", "pdf");
	synth << "nom de fichier :" << temp << " convertis a txt" << endl;
	synth << "titre :" << Titre << endl;
	synth << "Les auteurs : " << endl;
	for (int i = 0, length = auteurs.size(); i < length; i++)
	{
		synth << auteurs[i] << endl;
	}
	synth << "Leurs adresses : " << endl;
	for (int i = 0, length = emails.size(); i < length; i++)
	{
		synth << emails[i] << endl;
	}
	synth << "Abstract : " << abstract << endl;
	synth << "Introduction : " << intro << endl;
	synth << "Corps : " << corps << endl;
	synth << "Conclusion : " << conclusion << endl;
	synth << "Discussion : " << discussion << endl;
	synth << "Biblio : " << endl;
	for (int i = 0; i < biblio.size(); i++)
	{
		synth << "Ref :" << biblio[i] << endl;
	}
}
// creation du notre fichier xml
void creationSyntheseXml(string fichier, string Titre, string abstract, vector<string> auteurs, vector<string> emails, vector<string> biblio, string intro, string corps, string discussion, string conclusion)
{
	string tmp = fichier;
	tmp = tmp.substr(0, tmp.size() - 3);
	tmp += "xml";
	string fich = "./résultatsXml/" + tmp;
	ofstream synth(fich.c_str());
	synth << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
	synth << "<article>" << endl;
	synth << "\t<preamble>" << fichier << "</preamble>" << endl;
	synth << "\t<titre>" << Titre << "</titre>" << endl;
	synth << "\t<auteurs>" << endl;
	for (int i = 0, length = auteurs.size(); i < length; i++)
	{
		synth << "\t\t<nom>" << auteurs[i] << "</nom>" << endl;
	}
	synth << "\t</auteurs>" << endl;
	synth << "\t<emails>" << endl;
	for (int i = 0, length = emails.size(); i < length; i++)
	{
		synth << "\t\t<mail>" << emails[i] << "</mail>" << endl;
	}
	synth << "\t</emails>" << endl;
	synth << "\t<abstract>" << abstract << "</abstract>" << endl;
	synth << "\t<introduction>" << intro << "</introduction>" << endl;
	synth << "\t<corps>" << corps << "</corps>" << endl;
	synth << "\t<conclusion>" << conclusion << "</conclusion>" << endl;
	synth << "\t<discussion>" << discussion << "</discussion>" << endl;
	synth << "\t<biblio>" << endl;
	for (int i = 0; i < biblio.size(); i++)
	{
		synth << "\t\t<ref>" << biblio[i] << "</ref>" << endl;
	}
	synth << "\t</biblio>" << endl;
	synth << "</article>" << endl;
}

/*
/// @auteur Talatizi Kamel
/// @param fichier
/// @param Titre
/// @param abstract
ca sert a examiner notre fichier et a récupérer les déffirents séction
*/

void ouvertureFichiers(vector<string> liste, string param)
{
	for (int i = 0, length = liste.size(); i < length; i++)
	{
		string concat = "./ConvertionToTxt/" + liste[i];
		ifstream fichier(concat.c_str());
		cout << "Fichier en cours = " << liste[i] << endl;
		string ligne;

		string titre = Titre(fichier, ligne);
		string abstract;
		int abs = 0;

		vector<string> names;
		vector<string> emails;
		vector<string> biblio;

		string intro;
		string corps;
		string discussion;
		string conclusion;

		do
		{

			if (abs == 0 && ligne.find("Abstract") != -1 || ligne.find("ABSTRACT") != -1)
				abs = 1;
			if (abs == 1 && ligne.find("Introduction") != -1 || ligne.find("INTRODUCTION") != -1)
				abs = 2;
			if (abs == 4 && ligne.find("Disscution") != -1 || ligne.find("DISSCUTION") != -1 || ligne.find("Discutions") != -1 || ligne.find("DISCUTIONS") != -1)
				abs = 3;
			if (abs == 6 && ligne.find("Conclusion") != -1 || ligne.find("CONCLUSION") != -1 || ligne.find("Conclusions") != -1 || ligne.find("CONCLUSIONS") != -1)
				abs = 4;

			if (abs == 4 && ligne.find("References") != -1 || ligne.find("REFERENCES") != -1 || ligne.find("REFERENCE") != -1 || ligne.find("Reference") != -1)
				abs = 5;
			if (abs == 2 && ligne.size() >= 2 && (ligne.substr(1, 2) == "2" || ligne.substr(0, 1) == "2" || ligne.substr(0, 2) == "2." || ligne.substr(1, 3) == "2." || ligne.substr(1, 3) == "II" || ligne.substr(0, 2) == "II"))
				abs = 6;

			switch (abs)
			{
			case 0:
				getAuteursMail(ligne, names, emails);
				break;
			case 1:
				Abstract(ligne, abstract);
				break;
			case 2:
				Introduction(ligne, intro, fichier, abs);
				corps += ligne;
				break;

			case 3:
				Discussion(ligne, discussion);
				break;
			case 4:
				getConclusion(ligne, conclusion);
				break;
			case 5:
				getBiblio(ligne, biblio);
				break;
			case 6:
				Corps(ligne, corps);
				break;
			}
		} while (getline(fichier, ligne));
		fichier.close();
		if (param == "-t" || param == "")
		{
			Creationresulats(liste[i], titre, abstract, names, emails, biblio, intro, corps, discussion, conclusion);
		}
		else if (param == "-x")
		{
			creationSyntheseXml(liste[i], titre, abstract, names, emails, biblio, intro, corps, discussion, conclusion);
		}
	}
}

int main(int argc, char **argv)
{
	// poure maittre a jour le repertoire a chaque lancement du main
	suppressionRep();
	vector<string> fichier = listeFichier();
	// affiche tout les fichiers avec un indice

	for (int i = 0, length = fichier.size(); i < length; i++)
	{
		cout << i + 1 << ": " << fichier[i] << endl;
	}
	// le choix des fichiers

	vector<int> liste = quelsFichiers(fichier.size());
	// Liste des fichier choisit

	vector<string> listeFinale = leChoix(fichier, liste);

	// l'affichage des des fichier choisit

	for (int i = 0, length = listeFinale.size(); i < length; i++)
	{
		cout << listeFinale[i] << endl;
	}

	vector<string> listeFinaleTxt = convertionPDFtoTxt(listeFinale);

	if (argc == 2)
	{
		ouvertureFichiers(listeFinaleTxt, argv[1]);
		return 0;
	}
	ouvertureFichiers(listeFinaleTxt, "");
	return 0;
}