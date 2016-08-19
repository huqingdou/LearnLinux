#include "window.h"
#include<string.h>

cwindow::cwinodw()
	: header(NULL)
	  , output(NULL)
	  , flist(NULL)
	  , input(NULL)
{}

cwindow::~cwindow()
{
	pthread_mutex_destory(&lock);
	delwin(this->header);
	delwin(this->output);
	delwin(this->flist);
	delwin(this->input);
	endwin();
}

void cwindow::init()
{
	pthread_mutex_init(&lock, NULL);
	initscr();
}

WINDOW* cwindow::create_win(const int &h, const &int w, const int &y, const int &x)
{
	WINDOW* _win = newwin(h, w, y, x);

	return _win;
}

void cwindow::win_wfresh(WINDOW* _win)
{
	box(_win, 0, 0);
	pthread_mutex_lock(&lock);
	wrefresh(_win);
	pthread_mutex_unlock(&lock);
}

void cwindow::put_str_to_window(WINDOW *_win, int y, int x, const string &_str)
{
	mvwaddstr(_win, y, x, _str.c_str());
}

void cwindow::get_msg_from_window(WINDOW *_win, string &_out)
{
	char buf[G_BLOCK_SIZE];
	memset(buf, '\0', sizeof(buf));

	wgetnstr(_win, buf, sizeof(buf));
	_out = buf;
}

void cwindow::delete_str_from_window(WINDOW* _win, int begin, int num)
{
	while(num-- > 0)
	{
		wmove(_win, begin++, 0);
		wclrtoeol(_win);
	}
}

void cwindow::draw_header()
{
	int h = LINES / 5;
	int w = COLS;
	int y = 0;
	int x = 0;

	this->header = create_win(h, w, y, x);
}

void cwindow::draw_output()
{
	int h = LINES * 3 / 5;
	int w = COLS * 3 / 4;
	int y = LINES / 5;
	int x = 0;

	this->output = create_win(h, w, y, x);
}

void cwindow::draw_flist();
{
	int h = LINES * 3 / 5;
	int w = COLS / 4;
	int y = LINES / 5;
	int x = COLS * 3 / 4;

	this->flist = create_win(h, w, y, x);
}

void cwindow::draw_input()
{
	int h = LINES / 5;
	int w = COLS;
	int y = LINES * 4 / 5;
	INT X = 0;
}
