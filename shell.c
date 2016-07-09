#include <kernel.h>

WINDOW head_wnd = {0, 0, 83, 8, 0, 0, ' '};	// Dimensions for the header window. 
WINDOW pac_wnd = {60, 9, 23, 40, 50, 0, ' '};	// Dimensions for the pacman window. 
WINDOW shell_wnd = {0, 9, 55, 15, 0, 0, 0xDC};  // Dimensions for the shell window. 
WINDOW design_wnd = {0, 8, 80, 5, 0, 0, 0xDC}; // Dimensions for the design window. 
WINDOW head1_wnd = {29, 3, 0, 5, 0, 0, ' '};	// Dimensions for the window which shows the SWITCH CHANGE. 
WINDOW head2_wnd = {55, 3, 0, 5, 0, 0, ' '};	// Dimensions for the window which shows the SPEED CHANGE. 
WINDOW train_wnd = {0, 0, 80, 8, 0, 0,' '};	// Dimensions for the TRAIN OR PROBING window. 
WINDOW* twnd = &train_wnd;		// Pointer pointing to the Train window.
WINDOW* shell_ptr = &shell_wnd;			//Pointer pointing to the Shell window.


int size;
char input_buffer[50];


/**
 * This function compares two strings and returns 0 if they're equal.
 * It also ignores the blank spaces before a command.
 */
int str_cmp(char *str1, char *str2);


/**
 * This function clears the window.
 * @param wnd is the pointer to the window.
 */
void clear_cmd(WINDOW *wnd);


/**
 * This function matches the entered command and executes the corresponding action.
 */
void do_action();


/**
 * This function lists the commands supported by the shell. 
 */
void help();


/**
 * This function reads the input and checks for new line.
 */
void get_input();


/**
 * This function executes the Shell Process.
 * @param self is the shell process.
 * @param param is the parameters passed to this process.
 */
void shell_process (PROCESS self, PARAM param); 


/** 
 *This function creates the Shell process.
 */
void init_shell(); 



	void clear_cmd(WINDOW *wnd)
	{
		clear_window(wnd);			// Calling the API clear_window and passing the pointer to the window to be cleared.
	}



	void help()
	{
		wprintf(&shell_wnd, "\n~~ COMMANDS SUPPORTED BY TOS Shell: ~~\n");
		wprintf(&shell_wnd, "1. 'clear'    Clears the window.\n");
		wprintf(&shell_wnd, "2. 'help'     Lists the commands supported by TOS.\n");		//Lists the commands supported by TOS Shell.
		wprintf(&shell_wnd, "3. 'ps'       Prints out all the running processes.\n");
		wprintf(&shell_wnd, "4. 'pacman'   Display's Pacman Game.\n");
	}

	
	int str_cmp(char* str1, char* str2)
	{
		int i=0,flag=0,k=0;

		while (str1[i] == ' ')		// Ignores and keeps count of all the spaces before the input.
		{
	   		 i++;
	
		}
   
    		while(str1[i]!='\0' && str2[k]!='\0')
		{

            		if((str1[i] - str2[k])!=0)
			{
			flag=1;			//Makes flag=1 if the strings are not same.
			}	
       		i++;
		k++;
    		}

   		if (flag==0)			//Returns 0 if there is a match.
         	{
		return 0;
		}
	
	}


	void do_action()
	{

		char *input = &input_buffer;
			
			if (str_cmp(input, "clear") == 0) 
		   {
				clear_window(shell_ptr);
				wprintf(&shell_wnd, "TOS SHELL\n");

				wprintf(&shell_wnd, "=> ");
				get_input();
				do_action();
			} 

			else if (str_cmp(input, "help") == 0) 
			{
				help();
				
			}

	 		else if (str_cmp(input, "ps") == 0)
			{
				print_all_processes(&shell_wnd);
				

			} 

			else if (str_cmp(input, "train") == 0) 
			{
				init_train(twnd);
			}

			else if (str_cmp(input, "\013") == 0) 
			{
				wprintf(&shell_wnd, "\n");
			}


			else if (str_cmp(input, "pacman") == 0) 
			{
				init_pacman(&pac_wnd, 4);		//Executing the Pacman game.
			}

			else 
			{
				wprintf(&shell_wnd, "\n\nCommand not found! Type 'help'.\n");
			}  
	}


	void get_input()
	{
		char ch; 
		Keyb_Message msg;
		size = 0;

		while (ch != '\n' && ch != '\r' && size < 49) 
		{
			msg.key_buffer = &ch;
			send(keyb_port, &msg);
	
			
			if ( ch == '\b') 			// Implementing Backspace.
			{
				if( size != 0 ) 
				{
					input_buffer[size] = '\0';
					wprintf(&shell_wnd, "%c", ch);
					size--;
				}
			}
  
			else
			{
				
				input_buffer[size++] = ch;
				wprintf(&shell_wnd, "%c", ch);
				
			}

			} 
			input_buffer[size] = '\0';
			size = 0;			//Cleaning the buffer.
	}


	void shell_process (PROCESS self, PARAM param)
	{

		clear_window(&train_wnd);		// Clearing the train window.
		clear_window(&head_wnd);		// Clearing the heading window.
		clear_window(&head1_wnd);		// Clearing the switch change window.
		clear_window(&head2_wnd);		// Clearing the speed change window.
		clear_window(&design_wnd);		//Clears the design window.
		clear_window(&shell_wnd);		// Clears the shell window.
		wprintf(&head_wnd, "~~~~~~~~~~~~~~~~~~~~~~~~~~~Welcome to TOS Shell~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		wprintf(&head_wnd, "\n\n");
			
		wprintf(&design_wnd, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		wprintf(&shell_wnd, "TOS SHELL\n");
		
		init_switches();
			while (1)
		{
			wprintf(&shell_wnd, "=> ");
			get_input();
			do_action();

		}

	}


		void init_shell()
	{
		create_process(shell_process, 5, 0, "Shell Process");		// Assigning the shell process with priority 5.
		
	}

