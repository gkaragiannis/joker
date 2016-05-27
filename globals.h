/***********************************************************************
* Project		:	Joker Game
* File Name		:	globals.h
* Version		:	final
* Milestone		:	Complete globals.h
* Author		:	George Karagiannis
* Start Date	:	12 Jan. 2016
* End Date		:	28 Feb. 2016
***********************************************************************/

#pragma once

#ifndef __GLOBALS__
#define __GLOBALS__

void open_db();
void close_db();
dates_ptr ask_dates(bool multiple);
int *winnums_to_int(char *str);
int valid_date(int day, int month, int year);
int date_to_int(char *str);
draw_ptr find_draws(dates_ptr dates);
FILE *create_file(char mode[1]);
bool is_num(char *str);
cpn_ptr fillin_cpns(int col_num, bool manual);
cpn_ptr fill_manual(bool *nums, int cpn_id);
cpn_ptr fill_auto(int cpn_id, bool *nums);
cpn_ptr cpn_gen(int cpn_id, bool *nums, int joker);
int print_cpns(FILE *f_ptr, cpn_ptr cpn_list);

#endif /*__GLOBALS*/