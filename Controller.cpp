/*
 * Controller.cpp
 *
 * Manages user interaction: reads commands from stdin, dispatches them to the
 * appropriate handler (View, Model, or a specific Ship), and prints the time
 * prompt before each command.
 *
 * Dispatch rules (first token):
 *   "exit"                                         → stop the loop
 *   "default"|"size"|"zoom"|"pan"|"show"           → handleViewCommand()
 *   "status"|"go"|"create"                         → handleModelCommand()
 *   <known ship name>                              → handleShipCommand()
 *   anything else                                  → "Error: illegal command"
 */

#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "Freighter.h"
#include "Patrol.h"
#include "Cruiser.h"

#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <stdexcept>
#include <cmath>
using namespace std;

// Helper sets for quick keyword classification
static const set<string> VIEW_COMMANDS  = { "default", "size", "zoom", "pan", "show" };
static const set<string> MODEL_COMMANDS = { "status", "go", "create" };
static const set<string> SHIP_COMMANDS  = {
    "course", "position", "destination",
    "load_at", "unload_at", "dock_at",
    "attack", "refuel", "stop"
};

// Constructor / Destructor

Controller::Controller() {
    view_ptr = make_shared<View>();
}

Controller::~Controller() = default;

// run() — main event loop

void Controller::run() {
    string line;
    while (true) {
        cout << "Time " << Model::get().getTime() << ": Enter command: ";
        if (!getline(cin, line)) break;  // EOF
        if (!parseCommand(line)) break;  // "exit"
    }
}

// parseCommand() — top-level dispatcher

bool Controller::parseCommand(const string& line) {
    istringstream iss(line);
    string first;
    if (!(iss >> first)) return true;  // empty line — ignore

    if (first == "exit") return false;

    if (VIEW_COMMANDS.count(first))  { handleViewCommand(first, iss);       return true; }
    if (MODEL_COMMANDS.count(first)) { handleModelCommand(first, iss);      return true; }
    if (Model::get().shipExists(first)) { handleShipCommand(first, iss);    return true; }

    cerr << "Error: illegal command\n";
    return true;
}

// handleViewCommand()

/**
 * Commands:
 *   default              – restore defaults
 *   size <int>           – set grid size  (6..30)
 *   zoom <double>        – set nm/cell scale (> 0)
 *   pan  <double> <double> – set origin (x y)
 *   show                 – draw the map
 */
bool Controller::handleViewCommand(const string& cmd, istringstream& args) {
    try {
        if (cmd == "default") {
            view_ptr->setDefault();
            return true;
        }
        if (cmd == "show") {
            view_ptr->draw();
            return true;
        }
        if (cmd == "size") {
            string tok;
            if (!(args >> tok)) { cerr << "Error: size requires an integer argument\n"; return false; }
            // validate integer
            for (char c : tok) {
                if (!isdigit(c) && c != '-') {
                    cerr << "ERROR: Expected an integer.\n";
                    return false;
                }
            }
            int s = stoi(tok);
            view_ptr->setSize(s);   // throws invalid_argument if out of range
            return true;
        }
        if (cmd == "zoom") {
            string tok;
            if (!(args >> tok)) { cerr << "Error: zoom requires a numeric argument\n"; return false; }
            // try parsing as double; catch bad input
            size_t idx = 0;
            double z;
            try { z = stod(tok, &idx); }
            catch (...) { cerr << "ERROR: Expected a double.\n"; return false; }
            if (idx != tok.size()) { cerr << "ERROR: Expected a double.\n"; return false; }
            view_ptr->setScale(z);  // throws invalid_argument if <= 0
            return true;
        }
        if (cmd == "pan") {
            double px, py;
            if (!(args >> px >> py)) { cerr << "Error: pan requires two numeric arguments\n"; return false; }
            view_ptr->setOrigin(px, py);
            return true;
        }
    } catch (const invalid_argument& e) {
        cerr << "ERROR: " << e.what() << "\n";
        return false;
    }
    cerr << "Error: unknown view command '" << cmd << "'\n";
    return false;
}

// handleModelCommand()

/**
 * Commands:
 *   status – print status of all objects
 *   go                   – advance simulation one hour
 *   create <name> <type> (<x>,<y>) <stat> [<extra>]
 *       type: Freighter | Patrol_boat | Cruiser
 *       stat: resistance (Freighter/Patrol) or force (Cruiser)
 *       extra: maxContainers (Freighter) | attackRange (Cruiser) | omitted (Patrol)
 */
bool Controller::handleModelCommand(const string& cmd, istringstream& args) {
    if (cmd == "status") {
        Model::get().printStatus();
        return true;
    }
    if (cmd == "go") {
        Model::get().go();
        return true;
    }
    if (cmd == "create") {
        //parse name
        string name;
        if (!(args >> name)) { cerr << "Error: create requires a name\n"; return false; }
        if (name.size() > 12) { cerr << "Error: ship name too long (max 12 chars)\n"; return false; }
        // name must be alphabetic
        for (char c : name) {
            if (!isalpha(c)) { cerr << "Error: ship name must be alphabetic\n"; return false; }
        }
        if (Model::get().nameExists(name)) {
            cerr << "Error: name '" << name << "' already exists\n";
            return false;
        }

        //parse type
        string type;
        if (!(args >> type)) { cerr << "Error: create requires a ship type\n"; return false; }

        //parse coordinates: "(x,y)" possibly split across tokens
        // Accept formats: "(x,y)" as one token, or "(x," and "y)" as two tokens
        double cx, cy;
        {
            string coordToken;
            if (!(args >> coordToken)) { cerr << "Error: create requires coordinates\n"; return false; }
            // If it doesn't end with ')' we need to read the second half
            if (coordToken.back() != ')') {
                string coordToken2;
                if (!(args >> coordToken2)) { cerr << "Error: create requires coordinates\n"; return false; }
                coordToken += coordToken2;
            }
            // coordToken should now be "(x,y)"
            // Strip parentheses and comma
            string inner;
            for (char c : coordToken) {
                if (c != '(' && c != ')') inner += c;
            }
            // replace comma with space
            for (char& c : inner) if (c == ',') c = ' ';
            istringstream coordSS(inner);
            if (!(coordSS >> cx >> cy)) {
                cerr << "Error: invalid coordinates\n";
                return false;
            }
        }

        //parse primary stat (resistance or force)
        int stat;
        if (!(args >> stat)) { cerr << "Error: create requires a resistance/force value\n"; return false; }

        //optional extra parameter
        int extra = 0;
        bool hasExtra = static_cast<bool>(args >> extra);

        //create the object
        try {
            if (type == "Freighter") {
                int maxContainers = hasExtra ? extra : 0;
                Model::get().addFreighter(name, cx, cy, stat, maxContainers);
            } else if (type == "Patrol_boat") {
                Model::get().addPatrol(name, cx, cy, stat);
            } else if (type == "Cruiser") {
                int range = hasExtra ? extra : 0;
                Model::get().addCruiser(name, cx, cy, stat, range);
            } else {
                cerr << "Error: unknown ship type '" << type << "'\n";
                return false;
            }
        } catch (const runtime_error& e) {
            cerr << "Error: " << e.what() << "\n";
            return false;
        }
        return true;
    }
    cerr << "Error: unknown model command '" << cmd << "'\n";
    return false;
}

// handleShipCommand()

/**
 * Sub-commands (all prefixed by a valid ship name):
 *   stop                          – halt the ship
 *   refuel                        – request refuel at current docked port
 *   course <heading> <speed>      – set compass course
 *   position (<x>,<y>) <speed>    – set destination by coordinates
 *   destination <port> <speed>    – set destination by port name
 *   load_at <port>                – set load port (Freighter only)
 *   unload_at <port> <count>      – set unload port and amount (Freighter only)
 *   dock_at <port>                – set dock destination (Freighter only)
 *   attack <target>               – queue attack for next step (Cruiser only)
 */
bool Controller::handleShipCommand(const string& shipName, istringstream& args) {
    string subcmd;
    if (!(args >> subcmd)) {
        cerr << "Error: missing command for ship '" << shipName << "'\n";
        return false;
    }
    if (!SHIP_COMMANDS.count(subcmd)) {
        cerr << "Error: illegal command '" << subcmd << "' for ship '" << shipName << "'\n";
        return false;
    }

    try {
        auto ship = Model::get().getShip(shipName);

        //- stop-
        if (subcmd == "stop") {
            ship->stop();
            return true;
        }

        //- refuel-
        if (subcmd == "refuel") {
            if (ship->getState() != Docked) {
                cerr << "Error: '" << shipName << "' is not docked\n";
                return false;
            }
            // Find the port it is docked at (same location)
            auto objs = Model::get().getAllObjects();
            for (const auto& obj : objs) {
                auto loc = obj->getLocation();
                if (loc.first == ship->getCorX() && loc.second == ship->getCorY()) {
                    // Try to cast to Port
                    auto portPtr = dynamic_pointer_cast<Port>(obj);
                    if (portPtr) {
                        double needed = ship->getMaxFuel() - ship->getFuel();
                        double dispensed = portPtr->dispenseFuel(needed);
                        ship->refuel(dispensed);
                        return true;
                    }
                }
            }
            cerr << "Error: no port found at ship's current location\n";
            return false;
        }

        //- course <heading> <speed>-
        if (subcmd == "course") {
            double heading, speed;
            if (!(args >> heading >> speed)) {
                cerr << "Error: course requires heading and speed\n";
                return false;
            }
            if (speed <= 0 || speed > ship->getMaxSpeed()) {
                cerr << "Error: invalid speed for '" << shipName << "'\n";
                return false;
            }
            ship->setCourse(heading, speed);
            return true;
        }

        //- position (<x>,<y>) <speed>-
        if (subcmd == "position") {
            // parse coordinate token (same logic as create)
            string coordToken;
            if (!(args >> coordToken)) { cerr << "Error: position requires coordinates\n"; return false; }
            if (coordToken.back() != ')') {
                string t2;
                if (!(args >> t2)) { cerr << "Error: position requires coordinates\n"; return false; }
                coordToken += t2;
            }
            string inner;
            for (char c : coordToken) if (c != '(' && c != ')') inner += c;
            for (char& c : inner) if (c == ',') c = ' ';
            istringstream cs(inner);
            double px, py;
            if (!(cs >> px >> py)) { cerr << "Error: invalid coordinates\n"; return false; }

            double speed;
            if (!(args >> speed)) { cerr << "Error: position requires speed\n"; return false; }
            if (speed <= 0 || speed > ship->getMaxSpeed()) {
                cerr << "Error: invalid speed for '" << shipName << "'\n";
                return false;
            }
            ship->setDestination(px, py, speed);
            return true;
        }

        //- destination <portName> <speed>-
        if (subcmd == "destination") {
            string portName;
            double speed;
            if (!(args >> portName >> speed)) {
                cerr << "Error: destination requires port name and speed\n";
                return false;
            }
            // validate port exists
            shared_ptr<Port> port;
            try { port = Model::get().getPort(portName); }
            catch (...) { cerr << "Error: no port named '" << portName << "'\n"; return false; }

            if (speed <= 0 || speed > ship->getMaxSpeed()) {
                cerr << "Error: invalid speed for '" << shipName << "'\n";
                return false;
            }
            auto loc = port->getLocation();
            ship->setPortDestination(loc.first, loc.second, speed, portName);
            return true;
        }

        //- load_at <port>  (Freighter only)-
        if (subcmd == "load_at") {
            auto frtr = dynamic_pointer_cast<Freighter>(ship);
            if (!frtr) { cerr << "Error: load_at is only valid for Freighters\n"; return false; }
            string portName;
            if (!(args >> portName)) { cerr << "Error: load_at requires a port name\n"; return false; }
            try { Model::get().getPort(portName); } // validate
            catch (...) { cerr << "Error: no port named '" << portName << "'\n"; return false; }
            frtr->setLoadPort(portName);
            return true;
        }

        //- unload_at <port> <count>  (Freighter only)-
        if (subcmd == "unload_at") {
            auto frtr = dynamic_pointer_cast<Freighter>(ship);
            if (!frtr) { cerr << "Error: unload_at is only valid for Freighters\n"; return false; }
            string portName;
            int count;
            if (!(args >> portName >> count)) {
                cerr << "Error: unload_at requires port name and container count\n";
                return false;
            }
            try { Model::get().getPort(portName); }
            catch (...) { cerr << "Error: no port named '" << portName << "'\n"; return false; }
            frtr->setUnloadPort(portName, count);
            return true;
        }

        //- dock_at <port>  (Freighter only)-
        if (subcmd == "dock_at") {
            auto frtr = dynamic_pointer_cast<Freighter>(ship);
            if (!frtr) { cerr << "Error: dock_at is only valid for Freighters\n"; return false; }
            string portName;
            if (!(args >> portName)) { cerr << "Error: dock_at requires a port name\n"; return false; }
            shared_ptr<Port> port;
            try { port = Model::get().getPort(portName); }
            catch (...) { cerr << "Error: no port named '" << portName << "'\n"; return false; }
            auto loc = port->getLocation();
            // Dock immediately if already within 0.1 nm, otherwise move toward it
            double dx = frtr->getCorX() - loc.first;
            double dy = frtr->getCorY() - loc.second;
            double dist = sqrt(dx*dx + dy*dy);
            if (dist <= 0.1) {
                frtr->setCorX(loc.first);
                frtr->setCorY(loc.second);
                frtr->changeState(Docked);
            } else {
                frtr->setPortDestination(loc.first, loc.second, frtr->getMaxSpeed(), portName);
            }
            return true;
        }

        //attack <target>  (Cruiser only).
        if (subcmd == "attack") {
            auto crs = dynamic_pointer_cast<Cruiser>(ship);
            if (!crs) { cerr << "Error: attack is only valid for Cruisers\n"; return false; }
            string targetName;
            if (!(args >> targetName)) { cerr << "Error: attack requires a target ship name\n"; return false; }
            if (!Model::get().shipExists(targetName)) {
                cerr << "Error: no ship named '" << targetName << "'\n";
                return false;
            }
            auto target = Model::get().getShip(targetName);
            // Check target is not another cruiser
            if (dynamic_pointer_cast<Cruiser>(target)) {
                cerr << "Error: Cruisers cannot attack other Cruisers\n";
                return false;
            }
            crs->attack(target.get());
            return true;
        }

    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << "\n";
        return false;
    }

    cerr << "Error: unhandled ship command '" << subcmd << "'\n";
    return false;
}
