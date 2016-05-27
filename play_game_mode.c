/***********************************************************************
* Project		:	Joker Game
* File Name		:	play_game_mode.h
* Version		:	final
* Milestone		:	Complete play_game_mode.h
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
#include "play_game_mode.h"


//------------------------------------------------------------------------------------------------------------------//
//												Functions Definitions												//
//------------------------------------------------------------------------------------------------------------------//

/***********************************************************************
* function	:	play_game()
* purpose	:	implements menu of "Play Game" mode
* argument	:
*   — void
* return	:	0 if successful
* notes		:
*   —
***********************************************************************/
int play_game(){

	int exe;
	char input = '0';

	while ((input != 'r') && (input != 'R')){
		printf("\n\n Your options are :\n\tPlay (O)ne Game\n\tPlay (M)ultiple Games\n\t(R)eturn");
		printf("\n Give new choice :\t");
		fflush(stdin);
		scanf("%c", &input);
		getchar();

		if ((input != 'r') && (input != 'R')){
			switch (input){
			case 'O':
			case 'o':
				printf("\n\n PLAY ONE GAME\n");
				exe = game(false);
				assert(exe == 0);
				break;
			case 'M':
			case 'm':
				printf("\n\n PLAY MULTIPLE GAMES\n");
				exe = game(true);
				assert(exe == 0);
				break;
			default:
				printf("\n\a Invalid input.");
			}
		}
	}

	return 0;
}


/***********************************************************************
* function	:	game()
* purpose	:	implements play game mode
* argument	:
*   — bool multiple :	
* return	:	0 if successful
* notes		:
*   —
***********************************************************************/
int game(bool multiple){

	int arr_pos = 0, dar_pos, car_pos, suc_ctr, exe;
	draw_ptr draw_list = NULL, aux;
	cpn_ptr cpn = NULL;
	dates_ptr dt_bnd;
	FILE *nf_ptr;
	bool jo_suc;

	/***********************************************************************
	*@	bool type
	*	multiple		->	multiple games or not
	*	jo_suc			->	joker succeed
	*@	int type
	*	arr_pos			->	counter1
	*	dar_pos			->	counter2
	*	car_pos			->
	*	suc_ctr			->	success counter
	*@	draw_ptr type
	*	draw_list		->	pointer to list of structs represents draws
	*	aux				->	auxiliary pointer to struct draw
	***********************************************************************/
	/* ask dates */
	dt_bnd = ask_dates(multiple);
	if (dt_bnd == NULL)
		return -1;

	/* find draws */
	draw_list = search_draws(dt_bnd);
	if (draw_list == NULL)
		return -2;

	free(dt_bnd);

	/* ask user for guesses */
	cpn = fillin_cpns(1, true);
	if (cpn == NULL)
		return -3;

	/* create new file to save data */
	nf_ptr = create_file("3");
	if (nf_ptr == NULL)
		return -4;

	/* print coupons in file */
	exe = print_cpns(nf_ptr, cpn);
	if (exe != 0)
		return -5;

	fprintf(nf_ptr, "\n\n----------------------------------------------------------------\nID\tWining Numbers\t\t\t\tJoker\tCategory\n----------------------------------------------------------------");
	printf("\n\n ---------------------------------------------------------------\n ID\tWining Numbers\t\t\t\tJoker\tCategory\n ---------------------------------------------------------------");

	//check for wins
	//fix formated output with seek()
	aux = draw_list;
	while (draw_list != NULL){

		//print draw's data
		fprintf(nf_ptr, "\n%i\t", draw_list->id);
		printf("\n %d\t", draw_list->id);

		//check for correct guesses
		for (dar_pos = 0, car_pos = 0, suc_ctr = 0; dar_pos<WNAL; car_pos++){

			if (draw_list->nums[dar_pos] == cpn->nums[car_pos]){
				fprintf(nf_ptr, "(%d)\t", draw_list->nums[dar_pos]);
				printf("(%d)\t", draw_list->nums[dar_pos]);
				suc_ctr++;
				dar_pos++;
				car_pos = -1;
			}
			else if (car_pos == WNAL - 1){
				fprintf(nf_ptr, "%d\t", draw_list->nums[dar_pos]);
				printf("%d\t", draw_list->nums[dar_pos]);
				dar_pos++;
				car_pos = -1;
			}
		}

		if (draw_list->joker == cpn->joker){
			fprintf(nf_ptr, "(%d)\t", draw_list->joker);
			printf("(%d)\t", draw_list->joker);
			jo_suc = true;
		}
		else{
			fprintf(nf_ptr, "%d\t", draw_list->joker);
			printf("%d\t", draw_list->joker);
			jo_suc = false;
		}

		//print winining category if any
		if (jo_suc && (suc_ctr > 0)){
			fprintf(nf_ptr, "%d + 1", suc_ctr);
			printf("%d + 1", suc_ctr);
		}
		else if (suc_ctr > 2){
			fprintf(nf_ptr, "%d", suc_ctr);
			printf("%d", suc_ctr);
		}

		//free coupon-struct when no need anymore
		if (draw_list->next == NULL)
			free(cpn);

		//find next draw
		draw_list = draw_list->next;
		free(aux);
		aux = draw_list;
	}

	fprintf(nf_ptr, "\n----------------------------------------------------------------\n *Note: Founded numbers appear in parenthesis");
	printf("\n ---------------------------------------------------------------\n *Note: Founded numbers appear in parenthesis");

	errno = fclose(nf_ptr);
	if (errno == EOF){
		fprintf(stderr, "Value of errno: %d\n", errno);
		fprintf(stderr, "\n Error occured closing file");
		return -6;
	}

	return 0;
}


