/*
 * main.cpp
 *
 * Entry point for simNautica.
 *
 * Usage:  simNautica <portfile>
 *
 * The port file contains one port per line in the format:
 *   <name> (<x>, <y>) <initialFuel> <fuelRate>
 *
 * On success the program enters the interactive command loop via Controller::run().
 * Any file or parse error is reported to stderr and the program exits with code 1.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "Model.h"
#include "Controller.h"

using namespace std;

int main(int argc, char* argv[]) {
    // 1. Validate command-line arguments
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <portfile>\n";
        return 1;
    }

    // 2. Open the port file
    ifstream portFile(argv[1]);
    if (!portFile.is_open()) {
        cerr << "Error: cannot open port file '" << argv[1] << "'\n";
        return 1;
    }

    // 3. Parse ports and load them into the Model
    //    Format per line:  <name> (<x>, <y>) <initialFuel> <fuelRate>
    //    Empty lines and lines consisting only of whitespace are skipped.
    string line;
    int lineNum = 0;
    try {
        while (getline(portFile, line)) {
            ++lineNum;

            // Skip blanks
            istringstream probe(line);
            string firstTok;
            if (!(probe >> firstTok)) continue;

            //name
            string name = firstTok;
            if (name.size() > 12) {
                cerr << "Error (line " << lineNum << "): port name too long: " << name << "\n";
                return 1;
            }

            // coordinates: "(x," and "y)" as two tokens
            string coordA, coordB;
            if (!(probe >> coordA >> coordB)) {
                cerr << "Error (line " << lineNum << "): expected coordinates after port name\n";
                return 1;
            }
            // Join and strip parentheses/comma
            string coordStr = coordA + coordB;
            string inner;
            for (char c : coordStr)
                if (c != '(' && c != ')') inner += c;
            for (char& c : inner)
                if (c == ',') c = ' ';
            istringstream coordSS(inner);
            double x, y;
            if (!(coordSS >> x >> y)) {
                cerr << "Error (line " << lineNum << "): invalid coordinates\n";
                return 1;
            }

            // initialFuel and fuelRate
            double initialFuel, fuelRate;
            if (!(probe >> initialFuel >> fuelRate)) {
                cerr << "Error (line " << lineNum << "): expected initialFuel and fuelRate\n";
                return 1;
            }
            if (initialFuel < 0 || fuelRate < 0) {
                cerr << "Error (line " << lineNum << "): fuel values must be non-negative\n";
                return 1;
            }

            // add to model (throws if name already exists)
            Model::get().addPort(name, x, y, initialFuel, fuelRate);
        }
    } catch (const runtime_error& e) {
        cerr << "Error (line " << lineNum << "): " << e.what() << "\n";
        return 1;
    }

    portFile.close();

    // 4. Hand control to the Controller
    Controller controller;
    controller.run();

    return 0;
}