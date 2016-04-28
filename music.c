#include <avr/io.h>
#include "open_interface.h"
#include "music.h"

#include <stdio.h>

void init_music(){	
 oi_t* sensor = oi_alloc();
 oi_init(sensor);
}

void load_songs( ) {
	
	// Notes: oi_load_song takes four arguments
	// arg1 - an integer from 0 to 16 identifying this song
	// arg2 - an integer that indicates the number of notes in the song (if greater than 16, it will consume the next song index's storage space)
	// arg3 - an array of integers representing the midi note values (example: 60 = C, 61 = C sharp)
	// arg4 - an array of integers representing the duration of each note (in 1/64ths of a second)
	
	unsigned char ImperialMarchNumNotes = 19;
	unsigned char ImperialMarchNotes[19]     = {55, 55, 55, 51, 58, 55, 51, 58, 55, 0,  62, 62, 62, 63, 58, 54, 51, 58, 55};
	unsigned char ImperialMarchDurations[19] = {32, 32, 32, 20, 12, 32, 20, 12, 32, 32, 32, 32, 32, 20, 12, 32, 20, 12, 32};
	oi_load_song(IMERPIAL_MARCH, ImperialMarchNumNotes, ImperialMarchNotes, ImperialMarchDurations);
	oi_play_song(IMERPIAL_MARCH);
	
}