#include "Station.h"
#include <iostream>

Station::Station(string nom, int num) {
    nom_station = nom;
    num_station = num;
}

void Station::ajouterDestination(const Destination &des) {
    destinations.push_back(des);
    cout << "  [+] Destination ajoutée à " << nom_station << ": " << des.getNom() << endl;
}

void Station::afficher() const {
    cout << "=== Station " << nom_station << " (N°" << num_station << ") ===" << endl;
    for (const auto &d : destinations)
        d.afficher();
}

string Station::getNom() const {
    return nom_station;
}
vector<Destination>& Station::getDestinations() {
    return destinations; 
}
int Station::getNumStation() const {
    return num_station;
}

const vector<Destination>& Station::getDestinations() const {
    return destinations;
}