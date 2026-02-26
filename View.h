/*
 * View.h
 *
 * Responsible for rendering the simulation map as ASCII art.
 * The map is a square grid; each cell represents a (scale x scale) nm² area.
 * Objects are drawn using their first two characters.
 *
 * Defaults: size=25, scale=2.0, origin=(-10, -10)
 * Constraints: 6 <= size <= 30, scale > 0
 */

#pragma once

class View {
public:
    // Construct with default parameters
    View();

    // Parameter commands
    // Restore all parameters to defaults
    void setDefault();

    // Set grid size (rows == cols). Throws std::invalid_argument if out of [6,30].
    void setSize(int newSize);

    // Set nm-per-cell scale. Throws std::invalid_argument if <= 0.
    void setScale(double newScale);

    // Set the (x,y) coordinates of the bottom-left corner of the map.
    void setOrigin(double x, double y);

    // Rendering
    /**
     * Draw the current map to stdout.
     * Fetches all objects from Model, places their 2-char labels in the grid,
     * and prints with y-axis labels on the left and x-axis labels on the bottom.
     * Objects outside the map boundary are noted with a warning message.
     */
    void draw() const;

private:
    int    size;    // grid dimension (size x size cells)
    double scale;   // nm per cell
    double originX; // x coordinate of leftmost column
    double originY; // y coordinate of bottom row

    // Default values
    static constexpr int    DEFAULT_SIZE   = 25;
    static constexpr double DEFAULT_SCALE  = 2.0;
    static constexpr double DEFAULT_ORIGIN_X = -10.0;
    static constexpr double DEFAULT_ORIGIN_Y = -10.0;

    // Convert world coordinate to grid column/row index.
    // Returns false if the coordinate falls outside the map.
    bool worldToGrid(double wx, double wy, int& col, int& row) const;
};
