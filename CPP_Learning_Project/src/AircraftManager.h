//
// Created by samyg on 28/02/2022.
//

#pragma once

#include <vector>
#include <memory>
#pragma once

#include <vector>
#include <memory>
#include "aircraft.hpp"

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
    const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
    unsigned int number_of_crashed_aircrafts = 0;
public:
    void move() override;
    void add_aircraft(std::unique_ptr<Aircraft> aircraft);
    void count_aircrafts_of_airlines(const unsigned int& x);
    unsigned int get_required_fuel() const;
    unsigned int get_number_of_crash() const;
};


