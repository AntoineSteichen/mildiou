#ifndef WEEDIOU_COHORTE_H
#define WEEDIOU_COHORTE_H

class Cohorte {
 public:
  Cohorte(int , double , double , double , double ,
	  double , double , double , double , double ,
	  double , double , double , double , double ,
	  double , double , double , double , double ,
	  double , double , double , double , double,
	  double, double);

  virtual ~Cohorte() = default;

  void compute(double, double);

  double age() const { return AGE; }
  double spore() const { return SPORES; }
  double sporul() const { return SPORUL; }

 private:
  int ID;
  unsigned int count_AFFAIB;

  //paramètres du modèle de cohorte
  double p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14;
  double D0, D1, Dc;
  double CUM0, CUM1;
  double Topt, Tmin;
  double FACT, SSA;
  double seuil_HR;
  double seuil_Tmoy;

  //variables d'état du modèle de cohorte
  double TUDESPO; // Unité de développement des spores en 1
  // heure en fonction de la température horaire
  double CUMDDS;  // Evolution du développement des spores depuis
  // l'instant initiale (arrivée de l'inoculum
  // primaire). Il faut 100 unités pour que
  // commence la germinantion des spores. A 150
  // unités, toutes les spores ont germé.
  double SURVIE;  // Calcul de la survie des spores en fonction
  // de l'HR et de CUMDDS. Si HR > 90, toutes
  // les spores survivent ; si HR<90, la
  // mortalité des spores augmente au fur et à
  // mesure que  CUMDDS apporche de 100. Les
  // spores qui ont commencé à émettre un tube
  // germinantif meurent en 1 heure si HR < 90.
  double SPORES;  // C'est le nombre de spores au cours du temps
  // en fonction de la survie. Cette variable
  // donne à la fin (CUMDDS = 150) le nombre de
  // spores réellement prêtes pour germer après
  // la mortatilté due aus conditions climatiques.
  double GRAVI;   // Proportion de spores (parmi celles qui ont
  // survecu = SPORES) ayant germé en fonction de HR
  double POIDS;   // Cette variable donne le nombre de spores
  // contaminatrices, càd celles qui entrent en
  // période d'incubation et qui sont
  // susceptibles de sporuler en fonction des
  // conditions de développement et climatiques
  double RET;     // Les températures > 18 °C diminuent la
  // valeur de TINCUB. Cette variable permet de
  // calculer cette diminution.
  double TINCUB;  // Unité de développement horaire des
  // mycéliums pour provoquer les lésions (en
  // fonction de  la témpérature horaire)
  double AGE;     // C'est la valeur au cours du temps du
  // développement des mycéliums. 75 = nombre
  // d'unités nécessaire pour la période
  // d'incubation, ce qui correspond au début
  // sporulation si condition favorable 150 =
  // âge maximale de la lésion correspondant au
  // maximum de sporulation 225 = toute la
  // lésion est nécrosée, ce qui correspond à la
  // fin de la sporulation si condition favorable
  double KASPOTHEO; // C'est la capacité de sporulation
  // théorique d'une spore qui commence
  // uniquement après qu'on ait atteint AGE =
  // 75. Il augmente et atteint son maximum à
  // AGE =150 et diminue jusqu'à 0 vers AGE = 225.
  double TRED;    // Quand T > 18 °C, il y a réduction de KASPO,
  // cette variable quantifie cette diminution
  // en fonction de la température horaire.
  double AFFAIB;  // C'est la somme de TRED qui est calculée par
  // période de 12 heures.
  double KASPOREELLE; // C'est la capacité de sporulation réelle
  // après enlèvement des effets dus à AFFAIB
  double POSPO;   // C'est la capacité de sporulation de
  // l'ensemble des spores contaminatrices (POIDS)
  double SPOSPO;  // Potentiel de sporulation des lésions
  // (causées par toutes les spores
  // contaminatrices, tant que les conditions
  // sont favorables) jusqu'à ce qu'elles ne
  // sont plus en mesure de sporuler (necrose
  // des lésions).
  double TUSPORU; // Unité de développement de la sporulation en
  // fonction de la température horaire
  double CUMSPO;  // Degré de développement de la sporulation au
  // cours du temps.
  double ACTISPO; // Proportion de spores effectivement
  // produites par rapport à sporulation
  // potentielle. Il faut CUMSPO = 6 pour
  // débuter la sporulation réelle CUMSPO = 10,
  // le potentiel est atteint
  double SPORUL;  // C'est le nombre de spores réellement
  // produites et prêtes à être dispersées.
  double SURFMIL; // Conversion du nombre de spores en une
  // taille de lésion. Elle correspond à la
  // surface mildiousée.
};

#endif
