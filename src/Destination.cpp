#include "Destination.h"
#include <iostream>

Destination::Destination(string n) { nom = n; }

void Destination::ajouter(const Louage &l) {
    louages.push_back(l);
    cout << "    [+] Louage #" << l.getnumero_louage() << " ajouté à destination: " << nom << endl;
}

void Destination::remove_louage(){
    if (!louages.empty()) {
        louages.erase(louages.begin());
    }
}

void Destination::afficher() const {
    if (louages.empty()) {
        cout << "Aucun louage vers " << nom << endl;
    } else {
        cout << "Louages vers " << nom << " (" << louages.size() << "):" << endl;
        for (const auto &l : louages)
            l.afficher();
    }
}

string Destination::getNom() const {
    return nom;
}

vector<Louage>& Destination::getLouages() {
    return louages;
}

const vector<Louage>& Destination::getLouages() const {
    return louages;
}