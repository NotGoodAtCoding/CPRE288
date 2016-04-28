/*
 * Mars_Rover.c
 *
 * Created: 4/13/2016 12:26:31 PM
 *  Author: 404 Group Not Found
 */ 

// System libraries and components
#include "rover.h"

// Initialize all necessary components
oi_t * init(){
	lcd_init();
	serial_init();
	sei();
	ADC_init();
	timer1_init();
	timer3_init();
	oi_t *sensor_data = oi_alloc();
	oi_init(sensor_data);
	
	serial_puts("Initialized.");
	return sensor_data;	
}

// Full 180 degree scan to populate and display map
void slowFullScan(map_t *m){
	print("Scanning...");
	int angle = 0;
	
	//Do a 180 degree sweep, populate amp
	while(angle <= 180)
	{
		move_servo(angle);

		place_point(m, IR, ADC_read(),  angle);
		place_point(m, P,  ping_read(), angle);
		
		angle +=2;
		wait_ms(75);
	}
	
	//Display Map
	int i, j;
	for(i=0; i < MAP_Y; i++){
		for(j=0; j < MAP_X; j++){
			serial_putc(m->map[i][j]);
		}
	}

	init_map(m);
}

// Do a short / RAPID forward scan from approx 45 - 135 to prevent collisions
//CHANGED: Lessened sweep angle, lessened detect distance to 15 cm
int rapidForwardScan(){
		float angle = 45;
		float pingAvg = 100;
		float irAvg = 100;
		
		//Do a 180 degree sweep
		while(angle <= 135)
		{
			move_servo(angle);
			
			//PING
			pingAvg = (ping_read() * 0.33) + (pingAvg * 0.66);
			
			//IR
			irAvg = (linearRegression(ADC_read()) * 0.33) + (irAvg * 0.66);
			
			//TODO: To 15
			if ( pingAvg < 25 || irAvg < 25 ) {
				print("Collision detected");
			}
			
			angle +=2;
		}
		
		return 0;
}

// Detect tape boundry with light sensor
int detectColoredBoundry(oi_t * sensor_data){
	int left_sensor = sensor_data->cliff_left_signal;
	int front_left_sensor = sensor_data->cliff_frontleft_signal;
	int front_right_sensor = sensor_data->cliff_frontright_signal;
	int right_sensor = sensor_data->cliff_right_signal;
	
	int left_reading = determineBoundryType(left_sensor);
	int front_left_reading = determineBoundryType(front_left_sensor);
	int front_right_reading = determineBoundryType(front_right_sensor);
	int right_reading = determineBoundryType(right_sensor);
	
	if ( left_reading == WHITE_BOUNDRY || front_right_reading == WHITE_BOUNDRY || front_left_reading == WHITE_BOUNDRY || right_reading == WHITE_BOUNDRY ) {
		if ( left_reading == WHITE_BOUNDRY ) {
			print("Left  Boundary detected");
		} else if ( front_left_reading == WHITE_BOUNDRY || front_right_reading == WHITE_BOUNDRY ) {
			print("Forward boundary detected");
		} else {
			print("Right boundary detected");
		}
		
		move(-100, -100, 50, sensor_data);
		return WHITE_BOUNDRY;
	}
	
	if ( left_reading == FINISH_BOUNDRY || front_right_reading == FINISH_BOUNDRY || front_left_reading == FINISH_BOUNDRY || right_reading == FINISH_BOUNDRY ) {
		print("Finish Detected.");
		winProcedure(sensor_data);
		return FINISH_BOUNDRY;
	}
	
	return NO_BOUNDRY;
}

int determineBoundryType(int reading){
	if ( reading < WHITE_BOUNDRY_LOWER ) {
		return NO_BOUNDRY;
	} else if ( reading >= WHITE_BOUNDRY_LOWER && reading < WHITE_BOUNDRY_UPPER ) {
		return WHITE_BOUNDRY;
	} else {
		return FINISH_BOUNDRY;
	}
}

// Crater detection
int detectCrater(oi_t * sensor_data){
	int left_cliff = sensor_data->cliff_left_signal;
	int front_left_cliff = sensor_data->cliff_frontleft_signal;
	int front_right_cliff = sensor_data->cliff_frontright_signal;
	int right_cliff = sensor_data->cliff_right_signal;
	
	// Naive cliff detection
	if ( !left_cliff || !right_cliff || !front_right_cliff || !front_left_cliff ) {
		if ( !left_cliff ) {
			print("Left Cliff detected");
		} else if ( !front_left_cliff || !front_right_cliff ) {
			print("Front Cliff Detected");
		} else {
			print("Right Cliff Detected");
		}
		
		move(-100,-100,100,sensor_data);
		return 1;
	}

	return 0;
}

void moveCautiously(int cm, oi_t * sensorData){
	int currentDistanceTravelled = 0;
	int detectedAngle = 0;
	int scanSprint = 0, boundrySprint = 0, craterSprint = 0;
	
	while ( currentDistanceTravelled < cm) {
		
		// Scan every 10CM, prevent collisions
		if ( scanSprint > 100 ) {
			scanSprint = 0;
			detectedAngle = rapidForwardScan();
			if ( detectedAngle ) {
				break;
			}
		}
		
		// Scan every 2.5CM, prevent driving over boundaries
		if ( boundrySprint > 25 ) {
			boundrySprint = 0;
			
			if ( detectColoredBoundry(sensorData) ) {
				break;
			}
		}
		
		// Scan ever 1CM, prevent falling in craters
		if ( craterSprint > 10 ) {
			craterSprint = 0;
			
			if ( detectCrater(sensorData) ) {
				break;
			}
		}
		
		//Move 1CM
		move(120,120,10,sensorData);
		
		//Update scan sprint distances
		currentDistanceTravelled+=10;
		scanSprint+=10;
		boundrySprint+=10;
		craterSprint+=10;
	}
}

void winProcedure(oi_t * sensorData){
	unsigned char mario1NumNotes = 49;
	unsigned char mario1Notes[49]    = {48, 60, 45, 57, 46, 58,  0, 48, 60, 45, 57, 46, 58,  0, 41, 53, 38, 50, 39, 51,  0, 41, 53, 38, 50, 39, 51,  0, 51, 50, 49, 48, 51, 50, 44, 43, 49, 48, 54, 53, 52, 58, 57, 56, 51, 47, 46, 45, 44 };
	unsigned char mario1Duration[49] = {12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12, 48,  8,  8,  8, 24, 24, 24, 24, 24, 24,  8,  8,  8,  8,  8,  8, 16, 16, 16, 16, 16, 16 };
	oi_load_song(MARIO_UNDERWORLD, mario1NumNotes, mario1Notes, mario1Duration);
	oi_play_song(MARIO_UNDERWORLD);
	
	turn(1000, sensorData);
}

int receiveCommand(){
	// Ignore garbage before command
	while ( (serial_getc() != '!') ) {}
	
	char command[10] = "\0\0\0\0\0\0\0\0\0\0";
	int commandCharIndex = 0;
	char currentChar;
	
	// Read command
	while ( (currentChar = serial_getc()) != ' ') {
		command[commandCharIndex] = currentChar;
		commandCharIndex++;
	}
	
	if ( !strcmp(command, "TURN") || !strcmp(command, "turn") ) {
		return TURN;
	}
	
	if ( !strcmp(command, "MOVE") || !strcmp(command, "move") ) {
		return MOVE;
	}
	
	if ( !strcmp(command, "WIN") || !strcmp(command, "win") ) {
		return WIN;
	}
	
	return NONE;
}

int receiveQuantity(){
	char quantity[10];
	int quantityCharIndex = 0;
	char currentChar;
	
	// Read quantity
	while ( (currentChar = serial_getc()) != '!') {
		quantity[quantityCharIndex] = currentChar;
		quantityCharIndex++;
	}
	
	if ( quantityCharIndex == 0 ) {
		return 0;
	}
	
	return atoi(quantity);
}

void executeCommand(int command, int quantity, oi_t * sensor_data){
	switch(command) {
		case MOVE:
			moveCautiously(quantity * 10, sensor_data);
			break;
		case TURN:
			turn(quantity, sensor_data);
			break;
		case WIN:
			winProcedure(sensor_data);
			break;
		default:
			break;
	}
}

void print(char* s){
	lprintf(s);
	serial_puts(s);
}

// AUTONOMY TODOS
// React to collisions:
// - Hit a boundary? Turn 90 degrees away from it
// - Hit a crater? Determine where it's at, back up and decided to bypass it left / right
// - Detect object collision? Adjust trajectory to move between objects or go elsewhere
// Finding the end:
// - Search for the smallest posts
// - Move to posts
// -- What if we hit the crater? (I've seen it setup so they often try and trick you to hit the craters)
// -- Position ourselves to move in the exact center of two posts
// -- Move straight through them. Detection does the rest

int main(void)
{
	oi_t * sensor_data = init();
	
	map_t map;
	init_map(&map);
	
	int command = -1, quantity = -1;
	
	
	while (1) {
		
		print("Scanning...");
		slowFullScan(&map);
		
		print("Receiving Command...");
		command = receiveCommand();
		
		print("Receiving Magnitude...");
		quantity = receiveQuantity();
		
		print("Executing Command...");
		executeCommand(command, quantity, sensor_data);
		
		oi_update(sensor_data);
	}
	
	oi_free(sensor_data);
	free(&map);
	
	return 0;
}
