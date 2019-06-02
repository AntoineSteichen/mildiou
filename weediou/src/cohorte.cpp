#include "cohorte.h"

#include <cmath>

Cohorte::Cohorte(int id, double InocPrim, double P1, double P2, double P3,
                 double P4, double P5, double P6, double P7, double P8,
                 double P9, double P10, double P11, double P12, double P13,
                 double P14, double d0, double d1, double dc, double cum0,
                 double cum1, double topt, double tmin, double fact,
                 double ssa, double seuilHR, double seuilTmoy) :
    ID(id), p1(P1), p2(P2), p3(P3), p4(P4), p5(P5), p6(P6), p7(P7), p8(P8),
    p9(P9), p10(P10), p11(P11), p12(P12), p13(P13), p14(P14),
    D0(d0),  D1(d1),  Dc(dc), CUM0(cum0), CUM1(cum1), Topt(topt), Tmin(tmin),
    FACT(fact), SSA(ssa), seuil_HR(seuilHR), seuil_Tmoy(seuilTmoy),
    SPORES(InocPrim)
{
    count_AFFAIB = 0;
    TUDESPO = 0.0;
    CUMDDS = 0.0;
    SURVIE = 1.0;
    GRAVI = 0.0;
    POIDS = 0.0;
    RET = 0.0;
    TINCUB = 0.0;
    AGE = 0.0;
    KASPOTHEO = 0.0;
    TRED = 0.0;
    AFFAIB = 0.0;
    KASPOREELLE = 0.0;
    POSPO = 0.0;
    SPOSPO = 0.0;
    TUSPORU = 0.0;
    CUMSPO = 0.0;
    ACTISPO = 0.0;
    SPORUL = 0.0;
    SURFMIL = 0.0;
}

void Cohorte::compute(double Tmoy, double HR)
{
    if (CUMDDS < D1) { //Compartiment Survie et Contamination
        //TUDESPO
        if (Tmoy < seuil_Tmoy) { //Tmoy < 18.0
            TUDESPO = p1 * Tmoy + p2;
        } else {
            TUDESPO = p3;
        }

        //CUMDDS
        CUMDDS += TUDESPO;

        //SURVIE
        if (HR > seuil_HR) { //HR > 90.0
            SURVIE = 1.0;
        } else if (CUMDDS <= Dc) {
            SURVIE = 1 - CUMDDS / Dc;
        } else {
            SURVIE = 0.0;
        }

        //SPORES
        SPORES *= SURVIE;

        //GRAVI
        if (HR < seuil_HR) { // HR < 90.0
            GRAVI = 0.0;
        } else {
            GRAVI = (CUMDDS - D0) / (D1 - D0);
        }

        //POIDS
        POIDS = SPORES * GRAVI;
    } else { //Compartiment Incubation et Sporulation potentielle & réelle
        //RET
        if (Tmoy <= Topt) {
            RET = 0.0;
        } else {
            RET = p5 * std::pow(Tmoy - Topt, p6);
        }

        //TINCUB
        if (Tmoy <= seuil_Tmoy) { //Tmoy <= 18.0
            TINCUB = p4 * Tmoy;
        } else {
            TINCUB = p4 * Tmoy - RET;
        }

        //AGE
        AGE += TINCUB;

        if (AGE > p7) {
            //KASPOTHEO
            if (AGE < p8) {
                KASPOTHEO = FACT * (AGE - p7) / p7;
            } else if (AGE < p9) {
                KASPOTHEO = FACT * (p9 - AGE) / p9;
            } else {
                KASPOTHEO = 0.0;
            }

            //TRED
            if (Tmoy <= seuil_Tmoy) { // Tmoy <= 18.0
                TRED = 0.0;
            } else {
                TRED = FACT * p10 * std::pow(Tmoy - Topt, p6);
            }

            //AFFAIB
            count_AFFAIB++;
            if (count_AFFAIB % 12 != 0) {
                AFFAIB += TRED;
            } else {
                AFFAIB = TRED; // Remise a 0 de AFFAIB toutes les 12 heures
            }
            //KASPOREELLE
            KASPOREELLE = KASPOTHEO - AFFAIB;

            //POSPO
            POSPO = POIDS * KASPOREELLE;

            //SPOSPO
            SPOSPO += POSPO;

            //TUSPORU
            if (HR <= seuil_HR) { // (HR <= 90.0)
                TUSPORU = 0.0;
            } else {
                TUSPORU = p11 * std::pow(Tmoy - Tmin, p12) *
                          (p13 - p14 * (Tmoy - Tmin));
            }

            //CUMSPO
            CUMSPO += TUSPORU;

            //ACTISPO
            if (CUMSPO < CUM0) {
                ACTISPO = 0.0;
            } else if (CUMSPO < CUM1) {
                ACTISPO = (CUMSPO - CUM0) / (CUM1 - CUM0);
            } else {
                ACTISPO = 1.0;
            }

            //SPORUL
            SPORUL = ACTISPO * SPOSPO;

            //SURFMIL
            SURFMIL = SPORUL * SSA;
        }
    }
}
