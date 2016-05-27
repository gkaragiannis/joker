/***********************************************************************
* Project		:	Joker Game
* File Name		:	play_system_mode.c
* Version		:	final
* Milestone		:	Complete play_system_mode.c
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
#include "play_system_mode.h"


//------------------------------------------------------------------------------------------------------------------//
//												Functions Definitions												//
//------------------------------------------------------------------------------------------------------------------//

/***********************************************************************
* function	:	play_system()
* purpose	:	implements menu of "Play System" mode
* argument	:
*   — void
* return	:	0 if successful
* notes		:
*   —
***********************************************************************/
int play_system(){

	int exe;
	char input = '0';

	while ((input != 'r') && (input != 'R')){
		printf("\n\n Your options are :\n\tSimulate for (O)ne Draw\n\tSimulate for (M)ultiple Draws\n\t(R)eturn");
		printf("\n Give new choice :\t");
		fflush(stdin);
		scanf("%c", &input);
		getchar();

		if ((input != 'r') && (input != 'R')){
			switch (input){
			case 'O':
			case 'o':
				printf("\n\n SIMULATE ONE DRAW");
				exe = simulate(false);
				assert(exe == 0);
				break;
			case 'M':
			case 'm':
				printf("\n\n SIMULATE MULTIPLE DRAWS");
				exe = simulate(true);
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
* function	:	simulate()
* purpose	:	implement play system mode
* argument	:
*   — bool multiple
* return	:	0 if successful
* note		:
*   —
***********************************************************************/
int simulate(bool multiple){

	int max_col, min_col, col_num, gss_ctr, suc_ctr, arr_pos, tmp, exe;
	char max_col_str[BUFSIZ], min_col_str[BUFSIZ];
	bool vld_max = false, vld_min = false;
	draw_ptr draw_list = NULL, draw_aux;
	cpn_ptr cpn_list = NULL, cpn_aux;
	dates_ptr dt_bnd;
	time_t cur_time;
	FILE *nf_ptr;

	/* ask for desirable date(s) */
	dt_bnd = ask_dates(multiple);
	if (dt_bnd == NULL)
		return -1;

	/* search draws helded on date range */
	draw_list = search_draws(dt_bnd);
	if (draw_list == NULL)
		return -2;

	free(dt_bnd);

	/* ask for number of coupons */
	while (!(vld_max && vld_min)){
		printf("\n Please give ");
		if (!vld_max)
			printf("MAX");
		else
			printf("MIN");
		printf("imum number of coupons\t:\t");
		fflush(stdin);
		if (!vld_max)
			scanf("%s", &max_col_str);
		else
			scanf("%s", &min_col_str);
		getchar();
		if (!vld_max){
			vld_max = is_num(max_col_str);
			max_col = atoi(max_col_str);
		}
		else if (!vld_min){
			vld_min = is_num(min_col_str);
			min_col = atoi(min_col_str);
		}
		else
			printf("\a Invalid input");
	}
	if (min_col > max_col){
		tmp = max_col;
		max_col = min_col;
		min_col = tmp;
	}
	srand((unsigned)time(&cur_time));
	col_num = (rand() % (max_col - min_col + 1)) + min_col;
	assert((col_num >= min_col) && (col_num <= max_col));

	/* fill in coupons */
	cpn_list = fillin_cpns(col_num, false);
	if (cpn_list == NULL)
		return -3;

	/* create file to save data */
	nf_ptr = create_file("4");
	if (nf_ptr == NULL)
		return -4;

	/* save coupons' data on file and display them on screen */
	exe = print_cpns(nf_ptr, cpn_list);
	assert(exe == 0);

	/* derive results */
	fprintf(nf_ptr, "\n\n\n-------------------------------------------------------------------------------\n DRAWS:\n ID\tDate\t\tWining Numbers\t\t\t\tJoker\tWinners\n-------------------------------------------------------------------------------");
	printf("\n\n\n ------------------------------------------------------------------------------\n DRAWS:\n ID\tDate\t\tWining Numbers\t\t\t\tJoker\tWinners\n ------------------------------------------------------------------------------");
	//check every coupon for every draw
	draw_aux = draw_list;
	while (draw_list != NULL){		//for every draw in draw_list
		cpn_aux = cpn_list;
		while (cpn_aux != NULL){	//for every coupon in cpn_list
			//check for correct guesses in a coupon
			for (gss_ctr = 0, suc_ctr = 0; gss_ctr < WNAL; gss_ctr++){		//for every winning number of a draw
				for (arr_pos = 0; arr_pos < WNAL; arr_pos++){				//for every guess on a coupon
					if (draw_list->nums[gss_ctr] == cpn_aux->nums[arr_pos]){
						suc_ctr++;
					}
				}
			}

			if ((suc_ctr == WNAL) && (draw_list->joker == cpn_aux->joker)){
				draw_list->winners++;
			}

			cpn_aux = cpn_aux->next;
			/* free coupon-structs when no need anymore */
			if (draw_list->next == NULL){
				free(cpn_list);
				cpn_list = cpn_aux;
			}
		}

		/* print draw's data */
		fprintf(nf_ptr, "\n %d\t%s\t", draw_list->id, draw_list->date);
		printf("\n %d\t%s\t", draw_list->id, draw_list->date);

		for (arr_pos = 0; arr_pos<WNAL; arr_pos++){
			fprintf(nf_ptr, "%d\t", draw_list->nums[arr_pos]);
			printf("%d\t", draw_list->nums[arr_pos]);
		}

		fprintf(nf_ptr, "%d", draw_list->joker);
		printf("%d", draw_list->joker);

		if (draw_list->winners == 0){
			fprintf(nf_ptr, "\tjackpot");
			printf("\tjackpot");
		}
		else{
			fprintf(nf_ptr, "\t%d winner(s)", draw_list->winners);
			printf("\t%d winner(s)", draw_list->winners);
		}

		//go to next draw-struct and free previous 
		draw_list = draw_list->next;
		free(draw_aux);
		draw_aux = draw_list;
	}

	fprintf(nf_ptr, "\n-------------------------------------------------------------------------------");
	printf("\n ------------------------------------------------------------------------------");

	//close file
	errno = fclose(nf_ptr);
	if (errno == EOF){
		fprintf(stderr, "Value of errno: %d\n", errno);
		fprintf(stderr, "\n Error occured closing file");
		return -5;
	}

	return 0;
}