# ifndef MAP_H
# define MAP_H

#include <stdint.h>
#include "rover.h"

//Object detection constants
#define MAX_RANGE 90

//MAP CONSTANTS
#define MAP_X 80
#define MAP_Y 40
#define CENTER_X 39
#define CENTER_Y 39

typedef enum {IR, P} terrain;

typedef struct map {
  char map[MAP_Y][MAP_X];
} map_t;

///Initializes the cartesian map
/**
 * Map initializer. The map is filled with whitespace chars ' ' as well as
 * forward distance markers and an -X- sprite representing the Rover and its width.
 * @param *m a pointer to the map to initialize
 */
void init_map(map_t *m);

///Places a single point on the map
/** 
 * Calculates the [x, y] (cartesian) coordinates of the radial coordinates
 * given by the distance and angle parameters, then uses the terrain type to 
 * place a point on the map of the given map struct.
 * @param *m a pointer to the map struct on which the point is to be placed
 * @param t the terrain type of the point to be placed
 * @param distance the distance measurement to the point 
 * @param angle the angle from 0 of the point 
 */
void place_point(map_t *m, terrain t, float distance, int angle);

# endif