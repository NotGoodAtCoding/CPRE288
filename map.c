 //System libraries and components
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "map.h"

///Converts degrees to radians
/**
 * Utility funciton to convert degrees to radians.
 * @param degrees an int representing the degrees to be converted.
 * @retrun radian representation of the degree measurement.
 */
  double deg_to_rad(int degrees){
  	return (((double) degrees) * PI) / 180;
  }

///Initializes the cartesian map
/**
 * Map initializer. The map is filled with whitespace chars ' ' as well as
 * forward distance markers and an -X- sprite representing the Rover and its width.
 * @param *m a pointer to the map to initialize
 */
  void init_map(map_t *m){
	  int i, j;
	  for(i=0; i<MAP_Y; i++){
		  for (j=0; j< MAP_X; j++) {
			  //Set marker for width of robo with '-'
			  (i == CENTER_Y && j>=CENTER_X-4 && j<=CENTER_X+4) ?
			   (m->map[i][j] = '-') : (m->map[i][j] = ' ');
		  }
	  }
	  //Center Marker
	  m->map[CENTER_Y][CENTER_X] = 'X';
	  m->map[30][CENTER_X]         = '2';
	  m->map[20][CENTER_X]         = '4';
	  m->map[10][CENTER_X]         = '6';
	  m->map[0][CENTER_X]          = '8';
  }

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
  void place_point(map_t *m, terrain t, float distance, int angle){
  	int x, y;
	
	x = (int) (CENTER_X + ((int)(distance / 2) * cos(deg_to_rad(angle))));
	y = (int) (CENTER_Y - ((int)(distance / 2) * sin(deg_to_rad(angle))));
		   	
	if (x >= 0 && x < MAP_X && y < MAP_Y && y >= 0) {
		//Double Mark denoted by 'X'
		(m->map[y][x] != ' ') ? (m->map[y][x] = 'X') :
		//IR denoted '#' , PING denoted '*'
		(t == IR) ? (m->map[y][x] = '#') : (m->map[y][x] = '*') ;
	}
  }
