/**
* @file cli_task.c
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-10-24
*
* @copyright Copyright (c) 2023
*
*/

#include "tasks/cli_task.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

/**
* @brief Actual CLI task function
*
* @param arg App_data struct pointer converted to void pointer
*/
void cli_task_fn(void *arg);
void cli_handle_cmd(int argc, char *argv[]);
void cmd_not_found(int argc, char *argv[]);

int help(int argc, char *argv[]);
int get_throttle(int argc, char *argv[]);
int get_brakelight(int argc, char *argv[]);
int get_brake(int argc, char *argv[]);
int get_time(int argc, char *argv[]);
int set_time(int argc, char *argv[]);
int get_faults(int argc, char *argv[]);

char line[256];
app_data_t *data;
command_t cmds[] =
{
	{"help", &help, "print help menu"},
	{"throttle", &get_throttle, "get the throttle percentage"},
	{"brakelight", &get_brakelight, "get the brake light status"},
	{"brake", &get_brake, "get the brake percentage"},
	{"gtime", &get_time, "get the RTC"},
	{"stime", &set_time, "set the RTC. format: '1/2/24-17:38:50' for Jan. 2, 2024 at 5:38:50PM"},
	{"fault", &get_faults, "gets the faults of the system"}
};

TaskHandle_t cli_task_start(app_data_t *data)
{
   TaskHandle_t handle;
   xTaskCreate(cli_task_fn, "CLI task", 256, (void *)data, 14, &handle);
   return handle;
}

void cli_task_fn(void *arg)
{
    data = (app_data_t *)arg;
	cli_device_t *cli = &data->board.cli;
    uint32_t taskNotification;
    char buf[CLI_LINE_SIZE] = {0};
    char *tokens[MAXTOKS];
    int n;
	
	cli_putline("DREV ECU Firmware Version 0.1");
	cli_putline("Type 'help' for help");

	for(;;)
	{
		xTaskNotifyWait(0, 0, &taskNotification, HAL_MAX_DELAY);
		if(cli->msg_pending == true)
		{
			taskENTER_CRITICAL();
			memcpy(buf, cli->line, strlen(cli->line) + 1);
			memset(cli->line, 0, strlen(cli->line) + 1);
			n = tokenize(buf, tokens, MAXTOKS, " \t");
			cli_handle_cmd(n, tokens);
			taskEXIT_CRITICAL();
			cli->msg_pending = false;
			cli->msg_proc++;
		}
	}
}

void cli_handle_cmd(int argc, char *argv[])
{
	cli_device_t *cli = &data->board.cli;
	int i;
	bool cmd_found = false;
	int num_cmds = sizeof(cmds) / sizeof(command_t);

	for(i = 0; i < num_cmds; i++)
	{
		if(!strncmp(cmds[i].name, argv[0], CLI_LINE_SIZE))
		{
			data->board.cli.ret = cmds[i].func(argc, argv);
			cli->msg_valid++;
			cmd_found = true;
			break;
		}
	}
	if(!cmd_found) cmd_not_found(argc, argv);
}

void cmd_not_found(int argc, char *argv[])
{
	snprintf(line, 256, "Command not found: \'%s\'", argv[0]);
	cli_putline(line);
}

int help(int argc, char *argv[])
{
	int num_cmds;
	int i;

	cli_putline("---------- Help Menu ----------");
	num_cmds = sizeof(cmds) / sizeof(command_t);
	for(i = 0; i < num_cmds; i++)
	{
		snprintf(line, 256, "%s - %s", cmds[i].name, cmds[i].desc);
		cli_putline(line);
	}
	return 0;
}

int get_throttle(int argc, char *argv[])
{
	double x = data->throttlePercent;
	snprintf(line, 256, "throttle: %6.2f%%", x);
	cli_putline(line);
	return 0;
}

int get_brakelight(int argc, char *argv[])
{
	snprintf(line, 256, "brakelight: %s", data->brakeLightState ? "ON" : "OFF");
	cli_putline(line);
	return 0;
}

int get_brake(int argc, char *argv[])
{
	double x = data->brakePercent;
	snprintf(line, 256, "brake: %6.2f%%", x);
	cli_putline(line);
	return 0;
}

int get_time(int argc, char *argv[])
{
	read_time();
	snprintf(line, 256, "RTC: %02d/%02d/%d-%02d:%02d:%02d",
			data->datetime.month,
			data->datetime.day,
			data->datetime.year,
			data->datetime.hour,
			data->datetime.minute,
			data->datetime.second);
	cli_putline(line);
	return 0;
}

int set_time(int argc, char *argv[])
{
	int month, day, year, hour, minute, second;

	if(argc != 2)
	{
		cli_putline("ERROR: stime takes 1 argument");
		cli_putline("usage: 'stime 1/2/24-17:38:50' for Jan. 2, 2024 at 5:38:50PM");
		return 1;
	}
	int ret = sscanf(argv[1], "%d/%d/%d-%d:%d:%d",
			&month,
			&day,
			&year,
			&hour,
			&minute,
			&second);

	if(ret != 6){
		cli_putline("ERROR: set time format not readable");
		cli_putline("usage: 'stime 1/2/24-17:38:50' for Jan. 2, 2024 at 5:38:50PM");
		return 1;
	}

	data->datetime.month = month;
	data->datetime.day = day;
	data->datetime.year = year;
	data->datetime.hour = hour;
	data->datetime.minute = minute;
	data->datetime.second = second;
	write_time();

	snprintf(line, 256, "Set RTC: %02d/%02d/%d-%02d:%02d:%02d",
			data->datetime.month,
			data->datetime.day,
			data->datetime.year,
			data->datetime.hour,
			data->datetime.minute,
			data->datetime.second
			);
	cli_putline(line);
	return 0;
}

int get_faults(int argc, char *argv[])
{
	cli_putline("System faults:");

	snprintf(line, 256, "hard: %d", data->hardSystemFault);
	cli_putline(line);

	snprintf(line, 256, "soft: %d", data->softSystemFault);
	cli_putline(line);

	snprintf(line, 256, "apps: %d", data->appsFaultFlag);
	cli_putline(line);

	snprintf(line, 256, "bse:  %d", data->bseFaultFlag);
	cli_putline(line);

	snprintf(line, 256, "bppc: %d", data->bppcFaultFlag);
	cli_putline(line);

	snprintf(line, 256, "acc: %d", data->accFaultFlag);
	cli_putline(line);

	snprintf(line, 256, "cli: %d", data->cliFaultFlag);
	cli_putline(line);
	return 0;
}

