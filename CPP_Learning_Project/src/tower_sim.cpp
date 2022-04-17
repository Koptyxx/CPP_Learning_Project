#include "tower_sim.hpp"

#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;


TowerSimulation::~TowerSimulation()
{
    delete airport;
}

void TowerSimulation::create_aircraft()
{
    std::unique_ptr<Aircraft> aircraft = aircraft_factory.create_random_aircraft(airport);
    auto set = aircraft_factory.getAlreadyTakenFN();
    if(set.find(aircraft->get_flight_num()) != set.end()){
        std::cout<<"sorry this aircraft are already create !"<<std::endl;
        return;
    }
    aircraft_factory.add_name(aircraft->get_flight_num());
    aircraft_manager.add_aircraft(std::move(aircraft));
}

void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { create_aircraft(); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    GL::keystrokes.emplace('k', []() {GL::ticks_per_sec++; }); // up framerate
    GL::keystrokes.emplace('l', []() {(GL::ticks_per_sec > 1) ? GL::ticks_per_sec-- : GL::ticks_per_sec; }); // down framerate
    GL::keystrokes.emplace('p', []() {GL::is_paused = !GL::is_paused;}); // is_paused
    GL::keystrokes.emplace('0', [this]() { aircraft_manager.count_aircrafts_of_airlines(0);});
    GL::keystrokes.emplace('1', [this]() { aircraft_manager.count_aircrafts_of_airlines(1);});
    GL::keystrokes.emplace('2', [this]() { aircraft_manager.count_aircrafts_of_airlines(2);});
    GL::keystrokes.emplace('3', [this]() { aircraft_manager.count_aircrafts_of_airlines(3);});
    GL::keystrokes.emplace('4', [this]() { aircraft_manager.count_aircrafts_of_airlines(4);});
    GL::keystrokes.emplace('5', [this]() { aircraft_manager.count_aircrafts_of_airlines(5);});
    GL::keystrokes.emplace('6', [this]() { aircraft_manager.count_aircrafts_of_airlines(6);});
    GL::keystrokes.emplace('7', [this]() { aircraft_manager.count_aircrafts_of_airlines(7);});
    GL::keystrokes.emplace('m', [this]() {std::cout << "There is " << aircraft_manager.get_number_of_crash() << " aircrafts that has crashed !" << std::endl;});
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;
    for (const auto& [first,second] : GL::keystrokes)
    {
        std::cout << first  << ' ';
    }
    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport =
        new Airport { one_lane_airport, Point3D { 0.f, 0.f, 0.f },
                      new img::Image { one_lane_airport_sprite_path.get_full_path() }, aircraft_manager};

    GL::move_queue.emplace(airport);
}

void TowerSimulation::init_aircraftManager()
{
    GL::move_queue.emplace(&aircraft_manager);
}


void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }
    init_airport();
    init_aircraftManager();
    assert(airport != nullptr);
    GL::loop();
}