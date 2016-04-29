/*
 * Mars_Rover.c
 *
 * Created: 4/13/2016 12:26:31 PM
 *  Author: 404 Group Not Found
 */ 

// System libraries and components
#include "rover.h"
#include "map.h"

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

// Do a short / RAPID forward scan from approx 45 - 135 to prevent collisions
//CHANGED: Lessened sweep angle, lessened detect distance to 15 cm
int rapidForwardScan(){
		float angle = 40;
		float pingAvg = 100;
		float irAvg = 100;
		
		move_servo(angle);
		
		wait_ms(250);
		
		//Do a 180 degree sweep
		while(angle <= 140)
		{
			move_servo(angle);
			
			//PING
			pingAvg = (ping_read() * 0.33) + (pingAvg * 0.66);
			
			//IR
			irAvg = (linearRegression(ADC_read()) * 0.33) + (irAvg * 0.66);
			
			//TODO: To 15
			if ( pingAvg < 25 || irAvg < 25 ) {
				print("Collision detected");
				return angle;
			}
			
			angle +=2;
		}
		
		return 0;
}

int left_base = 0, front_left_base = 0, front_right_base = 0, right_base = 0;

// Detect tape boundary with light sensor
int detectColoredBoundry(oi_t * sensor_data){
	int left_sensor = sensor_data->cliff_left_signal;
	int front_left_sensor = sensor_data->cliff_frontleft_signal;
	int front_right_sensor = sensor_data->cliff_frontright_signal;
	int right_sensor = sensor_data->cliff_right_signal;
	
	int left_reading = determineBoundryType(left_sensor, left_base);
	int front_left_reading = determineBoundryType(front_left_sensor, front_left_base);
	int front_right_reading = determineBoundryType(front_right_sensor, front_right_base);
	int right_reading = determineBoundryType(right_sensor, right_base);

	//lprintf("%d %d %d", left_base, left_sensor, left_base * 4);

	if ( left_reading == WHITE_BOUNDRY || front_right_reading == WHITE_BOUNDRY || front_left_reading == WHITE_BOUNDRY || right_reading == WHITE_BOUNDRY ) {
		move(-100, -100, 50, sensor_data);
		
		if ( left_reading == WHITE_BOUNDRY ) {
			print("Left boundary detected");
			turn(-50, sensor_data);
		} else if ( front_left_reading == WHITE_BOUNDRY || front_right_reading == WHITE_BOUNDRY ) {
			print("Forward boundary detected");
			turn(100, sensor_data);
		} else {
			print("Right boundary detected");
			turn(50, sensor_data);
		}
		
		return WHITE_BOUNDRY;
	}
	
	if ( left_reading == FINISH_BOUNDRY || front_right_reading == FINISH_BOUNDRY || front_left_reading == FINISH_BOUNDRY || right_reading == FINISH_BOUNDRY ) {
		print("Finish Detected.");
		winProcedure(sensor_data);
		return FINISH_BOUNDRY;
	}
	
	return NO_BOUNDRY;
}

//Sketchy red/yellow
/*
int determineBoundryType(int reading, int base){
	if ( reading < base * 2 ) {
		return NO_BOUNDRY;
	} else if ( reading >= base * 2 && reading <  (base * 3) + (base / 2) ) {
		return WHITE_BOUNDRY;
	} else {
		return FINISH_BOUNDRY;
	}
} */

// For black circles
int determineBoundryType(int reading, int base){
	if ( reading < 100 && reading > 0 ) {
		return FINISH_BOUNDRY;
	} else if ( reading > 100 && reading < (base * 2) ) {
		return NO_BOUNDRY;
	} else {
		return WHITE_BOUNDRY;
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
		move(-100,-100, 50, sensor_data);
		
		if ( !left_cliff ) {
			print("Left Cliff detected");
			turn(-50, sensor_data);
		} else if ( !front_left_cliff || !front_right_cliff ) {
			print("Front Cliff Detected");
			turn(90, sensor_data);
		} else {
			print("Right Cliff Detected");
			turn(50, sensor_data);
		}
		
		return 1;
	}

	return 0;
}

int moveCautiously(int cm, oi_t * sensorData){
	int currentDistanceTravelled = 0;
	int detectedAngle = 0;
	int fastScanSprint = -1, boundrySprint = -1, craterSprint = -1;
	
	while ( currentDistanceTravelled < cm) {
		// Scan every 10CM, prevent collisions
		if ( fastScanSprint > 100 || fastScanSprint == -1) {
			fastScanSprint = 0;
			detectedAngle = rapidForwardScan();
			if ( detectedAngle ) {
				int sign;
				sign = (detectedAngle < 90) ? 1 : -1;
				int correctedAngle = (60-abs(90-detectedAngle)) * sign;
				turn(correctedAngle, sensorData);
				break;
			}
		}
		
		// Scan every 2CM, prevent driving over boundaries
		if ( boundrySprint > 10 || boundrySprint == -1) {
			boundrySprint = 0;
			
			
			int boundaryType = detectColoredBoundry(sensorData);
			
			switch (boundaryType ) {
				case WHITE_BOUNDRY:
					break;
				case FINISH_BOUNDRY:
					return FINISH_BOUNDRY;
			}
		}
		
		// Scan ever 0.5CM, prevent falling in craters
		if ( craterSprint > 5 || craterSprint == -1) {
			craterSprint = 0;
			
			if ( detectCrater(sensorData) ) {
				break;
			}
		}
		
		//Move 0.5CM
		if ( move(125,125,5,sensorData) ) {
			break;
		}
		
		//Update scan sprint distances
		currentDistanceTravelled+=5;
		fastScanSprint+=5;
		boundrySprint+=5;
		craterSprint+=5;
	}
	
	return 0;
}

void winProcedure(oi_t * sensorData){
	
	int i = 0;
	for ( ; i < 8; i++ ) {
		move(500, 500, 50, sensorData);
		move(-500,-500, 50, sensorData);
	}
			
	unsigned char ImperialMarchNumNotes = 19;
	unsigned char ImperialMarchNotes[19]     = {55, 55, 55, 51, 58, 55, 51, 58, 55, 0,  62, 62, 62, 63, 58, 54, 51, 58, 55};
	unsigned char ImperialMarchDurations[19] = {32, 32, 32, 20, 12, 32, 20, 12, 32, 32, 32, 32, 32, 20, 12, 32, 20, 12, 32};
	oi_load_song(IMERPIAL_MARCH, ImperialMarchNumNotes, ImperialMarchNotes, ImperialMarchDurations);
	oi_play_song(IMERPIAL_MARCH);
	
	turn(1000, sensorData);
}

void setBaseLightSensors(oi_t * sensor_data){
	left_base = sensor_data->cliff_left_signal;
	front_left_base = sensor_data->cliff_frontleft_signal;
	front_right_base = sensor_data->cliff_frontright_signal;
	right_base = sensor_data->cliff_right_signal; 
	
	while ( !left_base || !front_left_base || !front_right_base || !right_base ) {
		oi_update(sensor_data);
		
		left_base = sensor_data->cliff_left_signal;
		front_left_base = sensor_data->cliff_frontleft_signal;
		front_right_base = sensor_data->cliff_frontright_signal;
		right_base = sensor_data->cliff_right_signal;
		
		lprintf("%d %d %d %d", left_base, front_left_base, front_right_base, right_base);
	}

}

void print(char* s){
	lprintf(s);
	serial_puts(s);
}

//Use to confirm servo calibration
void calibrateServo(){
	move_servo(0);
	wait_ms(750);
	move_servo(90);
	wait_ms(750);
	move_servo(180);
}

int main(void)
{
	oi_t * sensor_data = init();
	
	setBaseLightSensors(sensor_data);
	
	//calibrateServo();
	
	while (1) {
		if ( moveCautiously(400, sensor_data) ) {
			break;
		}
		
		//Color sensor calibration
		//detectColoredBoundry(sensor_data);
		
		oi_update(sensor_data);
	} 
	
	oi_free(sensor_data);

	return 0;
}
