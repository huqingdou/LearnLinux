#include<signal.h>
#include<pthread.h>
#include "udp_client.h"
#include "cwindow.h"

typedef struct client_info
{
	udp_client *cli;
	cwindow *win;
	string nick_name;
	string school;
}cinfo;

cinfo c_info;

static void quit(int sig)
{
	udp_client *clip = c_info.cli;

	udp_data data;
	data.nick_nme = c_info.nickname;
	data.school = c_info.school;
	data.msg = "None";
	data.type = "QUIT";

	string str;
	data.to_string(str);

	clip->send_data(str);
	exit(0);
}

void *run_header(void *arg)
{
	cinfo *wcli = (cinfo*)arg;
	cwindow *winp = wcli->win;
	win->draw_header();

	int max_y = 0;
	int max_x = 0;

	string mad = "Welcome To Chat~~";
	int i = 0;
	while(1)
	{
		getmaxyx(winp->header, max_y, max_x);
		winp->put_str_to_window(winp->header, max_y/2, i++, msg);
		winp->wfresh(winp->header);
		winp->delete_str_from_window(winp->header, max_y/2, 2);
		i %= max_x;
		usleep(100000);
	}
}

void *run_output_flist(void *arg)
{
	cinfo *wcli = (cinfo*)arg;
	cwindow *winp = wcli->win;
	udp_client *clip = wcli->cli;

	winp->draw_output();
	winp->draw_flist();

	string in_str;
	udp_data data;

	string user;
	string friends;
	int max_y, max_x;

	int line = 1;

	while(1)
	{
		getmaxyx(winp->output, max_y, max_x);
		clip->recv_data(in_str);
		data.to_value(in_str);
		user = data.nick_name;
		user += "-";
		user += data.school;

		friends = user;

		if(data.type = "QUIT")
		{
			clip->del_friend(friends);
		}
		else
		{
			clip->add_friend(friends);
		}

		user += "> ";
		user += data.msg;

		//show mag
		if(data.type != "QUIT")
		{
			winp->put_str_to_window(winp->output, line++, 3, user);
		}
		if(line > max_y)
		{
			line = 1;
			winp->delete_str_from_window(winp->output, 1, max_y-1);
		}

		//show flist
		int size = clip->flist.size();
		for(int i = 0; i < size; i++)
		{
			winp->put_str_to_window(winp->flist, i+1, 2, clip->flist[i]);
		}

		usleep(100000);
		winp->wfresh(winp->output);
		winp->wfresh(winp->flist);
	}
}

void *run_input(void *arg)
{
	cinfo *wcli = (cinfo*)arg;
	wcli->win->draw_input();
	cwindow *winp = wcli->win;
	udp_client *clip = wcli->cli;

	string tips = "Please Enter: ";

	int max_y = 0;
	int max_x = 0;

	udp_data data;
	string smsg;

	while(1)
	{
		data.nick.name = ecli->nick_name;
		data.schoo = wcli->school;
		data.type = "None";

		getmaxyx(winp->input, max_y, max_x);
		winp->put_str_to_window(winp->input, 1, 2, tips);
		winp->wfresh(winp->input);

		winp->get_msg_from_window(winp->input, 1, max_y-1);

		usleep(100000);
	}
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		cout<<"Usage:"<< argv[0]<<"ip,port"<<endl;
		return 1;
	}

	signal(2, quit);

	cout << "Please Enter nick_name: ";
	fflush(stdout);

	cin >> c_info.nick_name;

	cout << "Please Enter school: ";
	fflush(stdout);
	cin >> _info.school;

	udp_client net(argv[1], atoi(argv[2]));
	cwindow win;

	c_info.cli = &net;
	c_info.win = &win;

	pthread_t header, output_flist, input;
	pthread_create(&header, NULL, run_header, (void*)&c_info);
	pthread_create(&output_flist, NULL,run_output_flist, (void*)&c_info);
	pthread_create(&input, NULL, run_input, (void*)&c_info);

	pthread_join(header, NULL);
	pthread_join(output_flist, NULL);
	pthread_join(input, NULL);

	return 0;
}
