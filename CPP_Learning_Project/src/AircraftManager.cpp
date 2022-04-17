//
// Created by samyg on 28/02/2022.
//

#include "AircraftManager.h"

void AircraftManager::move()
{
    auto aircrafts_sorted_by_fuel = [](const std::unique_ptr<Aircraft>& a,const std::unique_ptr<Aircraft>& b){
        assert(a != nullptr && b != nullptr);
        if(!a->has_terminal() && b->has_terminal()){
            return false;
        }
        if(a->has_terminal() && !b->has_terminal()){
            return true;
        }
        return a->get_fuel() < b->get_fuel();
    };

    std::sort(aircrafts.begin(), aircrafts.end(), aircrafts_sorted_by_fuel);

    std::function<bool(std::unique_ptr<Aircraft>&)> offsides_aircraft = [this](std::unique_ptr<Aircraft>& aircraft){
        assert(aircraft != nullptr);
        try
        {
            aircraft->move();
        }
        catch(const AircraftCrash &err){
            number_of_crashed_aircrafts++;
            std::cerr << err.what() << std::endl;
        }
        return aircraft->has_been_destroyed() || aircraft->get_not_fuel();
    };

    auto iter = std::remove_if(aircrafts.begin(),aircrafts.end(),offsides_aircraft);
    aircrafts.erase(iter,aircrafts.end());
}
void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> aircraft)
{
    assert(aircraft != nullptr);
    aircrafts.push_back(std::move(aircraft));
}

void AircraftManager::count_aircrafts_of_airlines(const unsigned int& x){
    assert(!(x <= 0 && x <= 7));
    std::function<bool(std::unique_ptr<Aircraft>&)> cpt = [x](std::unique_ptr<Aircraft>& aircraft){
        assert(aircraft != nullptr);
        return aircraft->get_indice() == x;
    };
    auto number_of_aircrafts = std::count_if(aircrafts.begin(),aircrafts.end(),cpt);
    std::cout << "There is " << number_of_aircrafts << " " << airlines[x] << " aircraft(s)" << std::endl;
}

unsigned int AircraftManager::get_required_fuel() const
{
    auto missing_fuel_sum = std::accumulate(aircrafts.begin(), aircrafts.end(), 0,[](unsigned int count, const std::unique_ptr<Aircraft>& aircraft){
                                           assert(aircraft != nullptr);
                                           if (aircraft->is_on_ground() && aircraft->is_low_on_fuel())
                                           {
                                               return count + (3000 - aircraft->get_fuel());
                                           }
                                           return count;
                                       });
    return missing_fuel_sum;
}

unsigned int AircraftManager::get_number_of_crash() const
{
    return number_of_crashed_aircrafts;
}