#pragma once

#include<iostream>
using namespace std;

#include<ncurses.h>
#include<string.h>
#include<pthread.h>
#include "comm.h"

class cwindow
{
public:
	cwinodw();
	~cwindow();
	void init();
	WINDOW* create_win(const int &h, const &int w, const int &y, const int &x);
	void win_wfresh(WINDOW* _win);
	void put_str_to_window(WINDOW *_win, int y, int x, const string &_str);
	void get_msg_from_window(WINDOW *_win, string &_out);
	void delete_str_from_window(WINDOW* _win, int begin, int num);

	void draw_header();
	void draw_output();
	void draw_flist();
	void draw_input();

	WINDOW* get_header()
	{
		return this->header;
	}

	WINDOW* get_output()
	{
		return this->output;
	}

	WINDOW* get_friends_list()
	{
		return this->flist;
	}

	WINDOW* get_input()
	{
		return this->input;
	}

private:
	WINDOW *header;
	WINDOW *output;
	WINDOW *flist;
	WINDOW *input;
	pthrea_mutex_t lock;
};
