#ifndef DESTINATION_H
#define DESTINATION_H

#include <vector>
#include <string>
#include "Louage.h"
using namespace std;

class Destination {
private:
    string nom;
    vector<Louage> louages;

public:
    Destination(string n);
    void ajouter(const Louage &l);
    void afficher() const;
    void remove_louage();
    string getNom() const;
    vector<Louage>& getLouages();         
    const vector<Louage>& getLouages() const;  

};

#endif
