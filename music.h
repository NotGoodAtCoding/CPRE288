#ifndef MUSIC_H
#define MUSIC_H

#define IMERPIAL_MARCH 		1

/// Initialize the oi_t sensor for music
/** 
 * Allocates memory and initializes the sensor in preparation to play music.
 */
void init_music();

/// Loads and plays the Imperial March theme into the music sensor
/**
 * The song is loaded as two unsigned char arrays representing the notes and durations
 * The song is then immediately played.
 */
void load_songs();

#endif