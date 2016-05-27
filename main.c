/***********************************************************************
* Project		:	Joker Game
* File Name		:	main.c
* Version		:	final
* Milestone		:	Complete project
* Author		:	George Karagiannis
* Start Date	:	12 Jan. 2016
* End Date		:	28 Feb. 2016
***********************************************************************/

#define NDEBUG

//------------------------------------------------------------------------------------------------------------------//
//													Header Files													//
//------------------------------------------------------------------------------------------------------------------//
#include <assert.h>
#include <errno.h>
#include <stdio.h>

#include "adt_cv.h"
#include "globals.h"
#include "find_draw_mode.h"
#include "bizarre_draws_mode.h"
#include "play_game_mode.h"
#include "play_system_mode.h"


int main(int argc, char *argv[]){

	char input[BUFSIZ] = "0", test_input[BUFSIZ];
	int index = 1, num_of_args = argc - 1, exe;


	
	/* open "database" file */
	open_db();

	printf("\n Welcome to the 'Joker'!\n");
	while ((input[0] != 'q') && (input[0] != 'Q')){

		if (num_of_args != 0){
			strcpy(input, argv[index]);
			index++;
			num_of_args--;
		}
		else{
			printf("\n\n Your options are :\n\t (F)ind Draw\n\t Find (B)izarre Draws\n\t Play (G)ame\n\t Play (S)ystem\n\t (Q)uit");
			printf("\n Give new choice\t:\t");
			fflush(stdin);
			scanf("%s", &input);
			getchar();
		}
		if ((input[0] != 'q') && (input[0] != 'Q')){
			switch (input[0]){
			case 'F':
			case 'f':
				printf("\n\n FIND DRAW");
				exe = find_draw();
				assert(exe == 0);
				break;
			case 'B':
			case 'b':
				printf("\n\n FIND BIZARRE DRAWS");
				exe = bizarre_draws();
				assert(exe == 0);
				break;
			case 'G':
			case 'g':
				printf("\n\n PLAY GAME");
				exe = play_game();
				assert(exe == 0);
				break;
			case 'S':
			case 's':
				printf("\n\n PLAY SYSTEM");
				exe = play_system();
				assert(exe == 0);
				break;
			default:
				printf("\n\a Invalid input");
			}
		}
	}

	// close file
	close_db();

	printf("\n Thank you for playing!\n Bye bye!\n");

	return 0;
}



