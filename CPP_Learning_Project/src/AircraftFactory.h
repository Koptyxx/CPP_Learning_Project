//
// Created by samyg on 28/02/2022.
//

#pragma once

#include "airport.hpp"

class AircraftFactory
{
private:
    const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
    AircraftType* aircraft_types[NUM_AIRCRAFT_TYPES] {};
    std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type,Airport* airport);
    std::set<std::string> already_taken_FN;
    void init_aircraft_types();

public:
    AircraftFactory()
    {
        init_aircraft_types();
    }
    std::unique_ptr<Aircraft> create_random_aircraft(Airport* airport);
    void add_name(const std::string& name);
    std::set<std::string> getAlreadyTakenFN() {return already_taken_FN;}
};