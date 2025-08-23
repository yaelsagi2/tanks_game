#include "MapDataSatelliteView.h"

// Constructor: initializes the satellite view with a reference to the board and the player's tank
MapDataSatelliteView::MapDataSatelliteView(MapData* mapData) {
    // Initialize the map_view with the dimensions from mapData
    map_view = CharMatrix(mapData->rows, mapData->cols,' '); // Fill with spaces
    for (int y = 0; y < mapData->rows; ++y) {
        for (int x = 0; x < mapData->cols; ++x) {
            char cell = mapData->grid[x][y]; // Access the character at (x, y)
            map_view.set(x, y, cell); // Set the character in the CharMatrix
        }
    }
    rows = mapData->rows; // Set the number of rows
    cols = mapData->cols; // Set the number of columns
}


// Returns a character representing the object at (x, y)
char MapDataSatelliteView::getObjectAt(size_t x, size_t y) const {

    if (x >= static_cast<size_t>(cols) || y >= static_cast<size_t>(rows)) { // Out of bounds check
        return '&';
    } 
    char cell = map_view.get(x, y); // Get the character from the CharMatrix
    return cell; // Return the character representing the object at (x, y)
}
