#include <kernel.h>


WINDOW traint_wnd = {0, 0, 80, 8, 0, 0,' '};	// Dimensions for the train window. 

WINDOW* wnd;

/**
 * This function creates a Train Process.
 */

 void init_train(WINDOW* wnd);

/**
 * This function initializes the outer loop switches.
 */

void init_switches(void);

/**
 * This function sends the commands to the COM port.
 * @param cmd is the command which has to be passed.
 * @param buffer_length is the input buffer length.
 * @param buffer_input stores the return value from the COM port.
 */

void send_command(char* cmd, int buffer_length, char* buffer_input);

/**
 * This function Concatenates the strings.
 */

void make_string(char* str1, char* str2);

/**
 * This function is used to probe the track number. Returns "1" if there's something on track or "0" if nothing.
 * @param contact_number is the track number.
 */

char Probe(char* contact_number);


/**
 * Sets the speed for the Train.
 * @param speed is the speed of train between 0 to 5. 
 */

void set_speed_to(char* speed);

/**
 * This function sets the Switch to either R or G.
 * @param switch_number is the Switch Number.
 * @param color is either "R" or "G".
 */

 void set_switch_to(char* switch_number, char* color);

/**
* This function changes the train direction.
*/

void change_direction();

/**
* This function proceeds only when the Probe returns 1.
*/

void check_status();

/**
* This executes the train process.
*/

void train_process(PROCESS self, PARAM param);


	void init_switches()
	{
		wprintf(&traint_wnd, "\nAdjusting switches\n");		
		set_switch_to("1", "G");
		set_switch_to("4", "G");
		set_switch_to("5", "G");
		set_switch_to("8", "G");		
		set_switch_to("9", "R");
				
	}

	
	void set_switch_to(char* switch_number, char* color) 
	{
		char switch_array[6];
		char null;
		switch_array[0] = '\0';
		make_string(switch_array, "M");
		make_string(switch_array, switch_number);
		make_string(switch_array, color);
		send_command(switch_array, 0, &null);
		wprintf(&traint_wnd, "Changed switch %s to %s\n", switch_number, color );
	}


	void make_string(char *str1,char *str2)	
	{

	    while(*str1!='\0')
		str1++;

	    while(*str2!='\0')
	    {
		  *str1 = *str2 ;
		  str1++,str2++;
	    }

	    *str1='\0';
    
	}


	void send_command(char* cmd, int buffer_length, char* buffer_input)
	{
			
		COM_Message msg;
		char data_to_send[8];
		data_to_send[0] = '\0';
		make_string(data_to_send, cmd);
		make_string(data_to_send, "\015");	
		msg.output_buffer = data_to_send;
		msg.len_input_buffer = buffer_length;
		msg.input_buffer = buffer_input;
		sleep(15); 
		send(com_port, &msg);			//Using send()	
	}


	void set_speed_to(char* speed)
	{
		char speed_array[8];
		char null;
		speed_array[0] = '\0';
		make_string(speed_array, "L20S");
		make_string(speed_array, speed);
		send_command(speed_array, 0, &null);
		wprintf(&traint_wnd, "Changing speed to %s\n", speed);
	}


	void change_direction()
	{
		char direct[6];  
		char null;
		direct[0] = '\0';
		make_string(direct, "L20D");
		send_command(direct, 0, &null);
		wprintf(&traint_wnd, "Changing direction\n");

	}


	char Probe(char* contact_number) 
	{
	
		char null;
		char input[6];
		char return_array[3];
		send_command("R", 0, &null);
		input[0] = '\0';
		make_string(input, "C");
		make_string(input, contact_number);
		send_command(input, 3, return_array);
		return return_array[1];
							
	}


	void check_status(char* contact_number) 		//Checks if something is on the track
	{
		char value;
		int check_contact;
		check_contact = 0;
		while(check_contact == 0) 
		{
		value = Probe(contact_number);
		if(value == '1') 
			{		    
			break;
			}
		}
	}

	
	void config_one_two() 				// Config 1 and 2 without Zamboni.
	{			

		set_speed_to("4");
		set_switch_to("5", "R");
		check_status("6");
		set_switch_to("4", "R");
		set_switch_to("3", "G");
		
		check_status("1");
		set_speed_to("0");

		change_direction();
		set_speed_to("4");
		set_switch_to("5", "R");
		set_switch_to("6", "R");
		
		check_status("8");
		set_speed_to("0");
		clear_window(&traint_wnd);
		wprintf(&traint_wnd, "\nSuccessful!\n");
	}


	void config_one_zamboni() 
	{
		
		if ( Probe("8") == '1' && Probe("2") == '1') 		//Config 1 with Zamboni
		{		

		wprintf(&traint_wnd, "This is configuration 1 with Zamboni.\n");			

		check_status("10");
		set_switch_to("9", "R");
		set_switch_to("1", "R");
		set_switch_to("2", "R");
		set_switch_to("7", "R");
		config_one_two();
		clear_window(&traint_wnd);
		wprintf(&traint_wnd, "\nConfiguration 1 with zamboni successful!\n");
					
		}
			   
	}



	void config_two_zamboni() 
	{
	
		if ( Probe("8") == '1' && Probe("2") == '1') 			//Config 2 with Zamboni
		{	

		wprintf(&traint_wnd, "This is configuration 2 with Zamboni.\n");

		check_status("4");
		set_speed_to("4");
		set_switch_to("5", "R");
		set_switch_to("8", "R");
			
		check_status("6");
		set_switch_to("4", "R");
		set_switch_to("3", "G");
	
		check_status("1");
		set_speed_to("0");

		change_direction();
		set_speed_to("4");
		set_switch_to("5", "R");
		set_switch_to("6", "R");			

		check_status("8");
		set_speed_to("0");
		clear_window(&traint_wnd);
		wprintf(&traint_wnd, "\nConfiguration 2 with zamboni successful!\n");	
					
		}

				
	}


	int detect_config() 		// Determines the different configurations.
	{

		int i, j;
		
		for(i = 0; i <= 30; i++) 
		{

			wprintf(&traint_wnd, "Searching for zamboni...\n");
			if( Probe("4") == '1') 
			{
				
				for( j = 0; j <= 25; j++) 
				{

				if((Probe("6") == '1') || (Probe("7") == '1')) 
				{
					return 2;			// Zamboni going Clockwise
				}
				
				else if((Probe("3") == '1') || (Probe("15") == '1')) 
				{
					return 	3;			// Zamboni going counter-Clockwise
				}
				
				}			
			}
		}

		if ( Probe("8") == '1' && Probe("2") == '1') 		// Configuration 1 or 2 without Zamboni.
		{	
			return 1;
		}

		
	}


	void train_process(PROCESS self, PARAM param)
	{

		int value;
		clear_window(&traint_wnd);
		wprintf(&traint_wnd, "~~~~~~~~~~~~~~~~~~~~~~~~~~~Welcome to TOS train~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		wprintf(&traint_wnd, "Starting train application\n");

		init_switches();
		
		value = detect_config();
		
		if(value==1)
		{
			wprintf(&traint_wnd, "This is configuration 1 or 2 without zamboni\n");
			config_one_two();
			return;
		}
		else if(value==2)
		{
			config_one_zamboni();
			return;
		}
		else if(value==3)
		{			
			config_two_zamboni();
			return;
		}
				

		while(1) 
		{
			}
				 
	}


	void init_train(WINDOW* wnd)
	{
	
 		create_process(train_process, 4, (PARAM) wnd, "Train Process");
		clear_window(&traint_wnd);
		resign();

  	}
