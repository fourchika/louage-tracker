#include "Louage.h"

Louage::Louage(int sv, int num, int id, string des, string dep) {
    serie_vehicule = sv;
    numero_louage = num;
    id_prop = id;
    destination = des;
    depart = dep;
}

void Louage::afficher() const {
    cout << "Louage " << serie_vehicule << "-" << numero_louage
         << " | De: " << depart << " -> " << destination << endl;
}

int Louage::getnumero_louage() const {
    return numero_louage;
}

// ======= getters you are missing for save/load =======

int Louage::getSerieVehicule() const {
    return serie_vehicule;
}

int Louage::getIdProp() const {
    return id_prop;
}

string Louage::getDestination() const {
    return destination;
}

string Louage::getDepart() const {
    return depart;
}
