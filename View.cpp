/*
 * View.cpp
 *
 * Renders the simulation map as ASCII art.
 *
 * Map layout (matching view_console.dat reference):
 *   - Each row printed top-to-bottom (highest y first).
 *   - Every 3rd row (counting from bottom) gets a y-axis label (right-aligned, 4 chars).
 *     The remaining rows get 4 spaces of padding ("    ").
 *   - Each cell is two characters wide; an empty cell is ". " (dot + space).
 *   - A filled cell is the first two chars of the object's name (no trailing space).
 *   - If two objects map to the same cell the cell is shown as "* " to signal a collision.
 *   - The x-axis label line is printed below the last row.
 *   - Objects outside the map are listed as warnings before the grid.
 */

#include "View.h"
#include "Model.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
using namespace std;

// Construction / defaults

View::View()
    : size(DEFAULT_SIZE), scale(DEFAULT_SCALE),
      originX(DEFAULT_ORIGIN_X), originY(DEFAULT_ORIGIN_Y)
{}

void View::setDefault() {
    size    = DEFAULT_SIZE;
    scale   = DEFAULT_SCALE;
    originX = DEFAULT_ORIGIN_X;
    originY = DEFAULT_ORIGIN_Y;
}

// Parameter setters

void View::setSize(int newSize) {
    if (newSize < 6)  throw invalid_argument("New map size is too small.");
    if (newSize > 30) throw invalid_argument("New map size is too big.");
    size = newSize;
}

void View::setScale(double newScale) {
    if (newScale <= 0.0)
        throw invalid_argument("New map scale must be positive.");
    scale = newScale;
}

void View::setOrigin(double x, double y) {
    originX = x;
    originY = y;
}

// Coordinate conversion

/**
 * Convert world (wx, wy) to grid (col, row) indices.
 * Row 0 is the BOTTOM row of the grid; row (size-1) is the TOP row.
 * Returns true if the point is inside the map.
 */
bool View::worldToGrid(double wx, double wy, int& col, int& row) const {
    col = static_cast<int>(floor((wx - originX) / scale));
    row = static_cast<int>(floor((wy - originY) / scale));
    return col >= 0 && col < size && row >= 0 && row < size;
}

// draw()

void View::draw() const {
    // ---- header line ----
    cout << fixed << setprecision(2);
    cout << "Display size: " << size
         << ", scale: "  << scale
         << ", origin: (" << originX << ", " << originY << ")\n";

    // ---- build the grid ----
    // grid[row][col]: empty = "", occupied = first-two-chars label, collision = "*"
    // row 0 = bottom, row size-1 = top
    vector<vector<string>> grid(size, vector<string>(size, ""));

    // Collect objects from Model
    auto objects = Model::get().getAllObjects();
    vector<string> outsideNames;

    for (const auto& obj : objects) {
        double wx = obj->getLocation().first;
        double wy = obj->getLocation().second;
        int col, row;
        if (worldToGrid(wx, wy, col, row)) {
            string label = obj->getName().substr(0, 2);
            if (grid[row][col].empty()) {
                grid[row][col] = label;
            } else {
                grid[row][col] = "*"; // collision marker
            }
        } else {
            outsideNames.push_back(obj->getName());
        }
    }

    // ---- print outside-map warnings ----
    for (const auto& name : outsideNames) {
        cout << name << " is outside the map\n";
    }

    // ---- print rows top-to-bottom ----
    // y-axis: label appears on every row whose (bottom-edge y) is a multiple of (3*scale)
    // Matching the reference: label shown every 3 printed rows, starting from the bottom row.
    for (int r = size - 1; r >= 0; --r) {
        // y-coordinate of the bottom edge of this row
        double rowY = originY + r * scale;

        // Label every 3 rows from the bottom (r == 0, 3, 6, ...)
        if (r % 3 == 0) {
            // right-align in 4 chars
            ostringstream lbl;
            lbl << fixed << setprecision(0) << rowY;
            cout << setw(4) << lbl.str() << " ";
        } else {
            cout << "     ";
        }

        for (int c = 0; c < size; ++c) {
            const string& cell = grid[r][c];
            if (cell.empty()) {
                cout << ". ";
            } else if (cell == "*") {
                cout << "* ";
            } else {
                // Two-char label; no trailing space (occupies both char slots)
                cout << cell;
            }
        }
        cout << "\n";
    }

    // ---- x-axis labels ----
    // Print x coordinate labels below; one label per 5 columns, right-aligned in 6 chars
    cout << "   "; // offset to align with the grid body (5 spaces label + 1 space gap = 6 leading on rows)
    for (int c = 0; c < size; c += 5) {
        double xVal = originX + c * scale;
        ostringstream lbl;
        lbl << fixed << setprecision(0) << xVal;
        cout << setw(6) << lbl.str();
    }
    cout << "\n";
}
