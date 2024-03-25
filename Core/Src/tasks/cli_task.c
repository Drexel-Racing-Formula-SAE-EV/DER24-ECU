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

char outline[256];
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
    uint32_t entry;
    char buf[CLI_LINE_SIZE] = {0};
    char *tokens[MAXTOKS];
    int n;
	
	cli_putline("~~~~~~~~~~ DER ECU FW V0.1~~~~~~~~~~");
	cli_putline("Type 'help' for list of commands");

	for(;;)
	{
		entry = osKernelGetTickCount();
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
		osDelayUntil(entry + (1000 / CLI_FREQ));
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
	snprintf(outline, 256, "Command not found: \'%s\'", argv[0]);
	cli_putline(outline);
	cli_putline("Type 'help' for list of commands");
}

int help(int argc, char *argv[])
{
	int num_cmds;
	int i;

	cli_putline("---------- Help Menu ----------");
	num_cmds = sizeof(cmds) / sizeof(command_t);
	for(i = 0; i < num_cmds; i++)
	{
		snprintf(outline, 256, "%s - %s", cmds[i].name, cmds[i].desc);
		cli_putline(outline);
	}
	return 0;
}

int get_throttle(int argc, char *argv[])
{
	snprintf(outline, 256, "throttle: %3d%%", data->throttle);
	cli_putline(outline);
	return 0;
}

int get_brakelight(int argc, char *argv[])
{
	snprintf(outline, 256, "brakelight: %s", data->brakelight ? "ON" : "OFF");
	cli_putline(outline);
	return 0;
}

int get_brake(int argc, char *argv[])
{
	snprintf(outline, 256, "brake: %3d%%", data->brake);
	cli_putline(outline);
	return 0;
}

int get_time(int argc, char *argv[])
{
	read_time();
	snprintf(outline, 256, "RTC: %02d/%02d/%d-%02d:%02d:%02d",
			data->datetime.month,
			data->datetime.day,
			data->datetime.year,
			data->datetime.hour,
			data->datetime.minute,
			data->datetime.second);
	cli_putline(outline);
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

	snprintf(outline, 256, "Set RTC: %02d/%02d/%d-%02d:%02d:%02d",
			data->datetime.month,
			data->datetime.day,
			data->datetime.year,
			data->datetime.hour,
			data->datetime.minute,
			data->datetime.second
			);
	cli_putline(outline);
	return 0;
}

int get_faults(int argc, char *argv[])
{
	cli_putline("System faults:");
	snprintf(outline, 256, "hard:   %d", data->hard_fault);
	cli_putline(outline);
	snprintf(outline, 256, "  apps:   %d", data->apps_fault);
	cli_putline(outline);
	snprintf(outline, 256, "  bse:    %d", data->bse_fault);
	cli_putline(outline);
	snprintf(outline, 256, "soft:   %d", data->soft_fault);
	cli_putline(outline);
	snprintf(outline, 256, "  bppc:   %d", data->bppc_fault);
	cli_putline(outline);
	snprintf(outline, 256, "  acc:    %d", data->acc_fault);
	cli_putline(outline);
	snprintf(outline, 256, "  cli:    %d", data->cli_fault);
	cli_putline(outline);
	snprintf(outline, 256, "  canbus: %d", data->canbus_fault);
	cli_putline(outline);
	snprintf(outline, 256, "  dash: %d", data->dashboard_fault);
	cli_putline(outline);
	return 0;
}

