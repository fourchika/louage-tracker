#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <string>
#include <iostream>
#include "Louage.h"
#include "Destination.h"
#include "Station.h"

using namespace std;

class System {
private:
    vector<Station> stations;

public:
    void ajouter_station(const Station& s);
    
    // Deux versions : non-const et const
    Station* findStation(const string& name);
    const Station* findStation(const string& name) const;
    
    void afficher_stations() const;

    void ajouter_destination(const string& stationName, const Destination& dest);
    
    // Deux versions : non-const et const
    Destination* findDestination(const string& destName, const string& stationName);
    const Destination* findDestination(const string& destName, const string& stationName) const;

    void ajouter_louage(const string& stationName, const string& destName, const Louage& l);
    Louage* findLouage(const Louage& l, const string& destName, const string& stationName);
    void remove_louage(const string& stationName, const string& destName, const Louage& l);

    void saveToFile(const string& filename);
    void loadFromFile(const string& filename);

    // NEW METHODS FOR WEB INTERFACE
    vector<string> getAllStationNames() const;
    vector<string> getDestinationsFromStation(const string& stationName) const;
    vector<Louage> searchLouages(const string& fromStation, const string& toDestination);
};

#endif