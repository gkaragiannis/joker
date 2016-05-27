/***********************************************************************
* Project		:	Joker Game
* File Name		:	bizarre_draws_mode.h
* Version		:	final
* Milestone		:	Complete bizarre_draws_mode.h
* Author		:	George Karagiannis
* Start Date	:	12 Jan. 2016
* End Date		:	28 Feb. 2016
***********************************************************************/

#pragma once

#ifndef __BIZARRE__
#define __BIZARRE__

int bizarre_draws();
int find_bizzare_draws(char bzzr);
bool crit_cons(draw_ptr adraw);
bool crit_ten(draw_ptr adraw);
bool crit_wang(draw_ptr adraw);

#endif /*__BIZARRE__*/