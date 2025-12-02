#ifndef LOUAGE_H
#define LOUAGE_H

#include <string>
#include <iostream>
using namespace std;

class Louage {
private:
    int serie_vehicule;
    int numero_louage;
    int id_prop;
    string destination;
    string depart;

public:
    Louage(int sv, int num, int id, string des, string dep);
    void afficher() const;
    int getnumero_louage() const;

    // === getters needed for save/load ===
    int getSerieVehicule() const;
    int getIdProp() const;
    string getDestination() const;
    string getDepart() const;
};

#endif
