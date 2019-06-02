#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	ifstream fichier_temperature("T.csv", ios::in);  // on ouvre le fichier en lecture
	ofstream fichier_data("data_temperature2016.csv", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
	
	if(fichier_temperature && fichier_data)  // si l'ouverture a réussi
	{
		// instructions
		int mois_precedent = 1;
		int jour_precedent = 1;
		int mois;
		int jour;
		int annee;
		string heure; 
		float humidite;
		float moyenne;
		float somme = 0;
		int compteur = 0;
		int compteur_jour = 0;
		
		while (compteur_jour < 250)
		{
			fichier_temperature >> mois >> jour >> annee >> heure >> humidite;
			if (mois_precedent == mois && jour_precedent == jour)
			{
				somme = somme + humidite;
				compteur ++;
			}
			else
			{
				compteur_jour++; 
				moyenne = somme / (float)compteur;
				mois_precedent = mois;
				jour_precedent = jour;
				compteur = 0;
				somme = 0;
				
				fichier_data << compteur_jour << " " << moyenne << endl;
			}
		}
		fichier_temperature.close();  // on ferme le fichier
		fichier_data.close();
	}
	else  // sinon
		cerr << "Impossible d'ouvrir les fichiers !" << endl;
	
	return 0;
}
