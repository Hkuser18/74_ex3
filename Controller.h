/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/
#pragma once
#include <memory>
#include <string>
#include <sstream>

// Forward-declare View to avoid circular includes; actual include in .cpp
class View;

class Controller {
public:
    Controller();
    ~Controller();

    // Creates View object, runs the program by accepting user commands, then destroys View object
    void run();

private:
    std::shared_ptr<View> view_ptr;

    /**
     * Parse a single input line and dispatch to the appropriate handler.
     * Returns false if the command was "exit" (caller should stop the loop),
     * true otherwise (even on error — errors are reported but the loop continues).
     */
    bool parseCommand(const std::string& line);

    /**
     * Handle view-group commands: default, size, zoom, pan, show.
     * @param cmd   The command keyword (already extracted).
     * @param args  The rest of the input line after the command word.
     * @return true on success, false on illegal command / bad arguments.
     */
    bool handleViewCommand(const std::string& cmd, std::istringstream& args);

    /**
     * Handle model-group commands: status, go, create.
     * @param cmd   The command keyword (already extracted).
     * @param args  The rest of the input line after the command word.
     * @return true on success, false on illegal command / bad arguments.
     */
    bool handleModelCommand(const std::string& cmd, std::istringstream& args);

    /**
     * Handle ship-specific commands: course, position, destination,
     * load_at, unload_at, dock_at, attack, refuel, stop.
     * @param shipName  The ship name (already extracted as the first token).
     * @param args      The rest of the input line after the ship name.
     * @return true on success, false on illegal command / bad arguments.
     */
    bool handleShipCommand(const std::string& shipName, std::istringstream& args);
};