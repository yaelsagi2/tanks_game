#pragma once
#include "../common/SatelliteView.h"
#include "MapData.h"
#include "CharMatrix.h"

class MapDataSatelliteView : public SatelliteView {
private:
    CharMatrix map_view; ///< 2D character matrix representing the board state
    int rows;             ///< Number of rows in the board
    int cols;             ///< Number of columns in the board

public:
    /**
     * @brief Constructs a satellite view for the given map data.
     * @param mapData Reference to the map data.
     */
    MapDataSatelliteView(MapData* mapData);

    /**
     * @brief Returns a character representing the object at the specified coordinates.
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     * @return Character representing the object at (x, y).
     */
    char getObjectAt(size_t x, size_t y) const override;

};