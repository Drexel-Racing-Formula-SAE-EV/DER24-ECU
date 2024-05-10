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
#include <stdlib.h>

/**
* @brief Actual CLI task function
*
* @param arg App_data struct pointer converted to void pointer
*/
void cli_task_fn(void *arg);
int cli_handle_cmd(int argc, char *argv[]);
void cmd_not_found(int argc, char *argv[]);

int help(int argc, char *argv[]);
int id(int argc, char *argv[]);
int get_throttle(int argc, char *argv[]);
int get_brakelight(int argc, char *argv[]);
int get_brake(int argc, char *argv[]);
int get_time(int argc, char *argv[]);
int set_time(int argc, char *argv[]);
int get_faults(int argc, char *argv[]);
int ssa(int argc, char *argv[]);
int sd(int argc, char *argv[]);

char outline[CLI_LINESZ];
app_data_t *data;
cli_device_t *cli;
command_t cmds[] =
{
	{"help", &help, "print help menu"},
	{"id", &id, "identifies system"},
	{"throttle", &get_throttle, "get the throttle percentage"},
	{"brakelight", &get_brakelight, "get the brake light status"},
	{"brake", &get_brake, "get the brake percentage"},
	{"gtime", &get_time, "get the RTC"},
	{"stime", &set_time, "set the RTC. format: '1/2/24-17:38:50' for Jan. 2, 2024 at 5:38:50PM"},
	{"fault", &get_faults, "gets the faults of the system"},
	{"ssa", &ssa, "set the SSA light duty cycle"},
	{"sd", &sd, "print the shutdown circuit states"}
};

TaskHandle_t cli_task_start(app_data_t *data)
{
   TaskHandle_t handle;
   xTaskCreate(cli_task_fn, "CLI task", 256, (void *)data, CLI_PRIO, &handle);
   return handle;
}

void cli_task_fn(void *arg)
{
    data = (app_data_t *)arg;
    cli = &data->board.cli;
    uint32_t entry;
    char buf[CLI_LINESZ] = {0};
    char *tokens[MAXTOKS];
    int n;
    int ret = 0;
	
	cli_printline(cli, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	cli_printline(cli, "Type 'help' for list of commands");

	for(;;)
	{
		entry = osKernelGetTickCount();
		if(cli->msg_pending == true)
		{
			taskENTER_CRITICAL();
			memcpy(buf, cli->line, strlen(cli->line) + 1);
			memset(cli->line, 0, strlen(cli->line) + 1);
			n = tokenize(buf, tokens, MAXTOKS, " \t");
			ret = cli_handle_cmd(n, tokens);
			taskEXIT_CRITICAL();
			data->cli_fault = ret;
			cli->msg_pending = false;
			cli->msg_proc++;
		}
		osDelayUntil(entry + (1000 / CLI_FREQ));
	}
}

int cli_handle_cmd(int argc, char *argv[])
{
	int i;
	int ret = 0;
	bool cmd_found = false;
	int num_cmds = sizeof(cmds) / sizeof(command_t);

	for(i = 0; i < num_cmds; i++)
	{
		if(!strncmp(cmds[i].name, argv[0], CLI_LINESZ))
		{
			ret = cmds[i].func(argc, argv);
			cli->msg_valid++;
			cmd_found = true;
			break;
		}
	}
	if(!cmd_found) cmd_not_found(argc, argv);
	cli->ret = ret;
	return ret;
}

void cmd_not_found(int argc, char *argv[])
{
	snprintf(outline, CLI_LINESZ, "Command not found: \'%s\'", argv[0]);
	cli_printline(cli, outline);
	cli_printline(cli, "Type 'help' for list of commands");
}

int help(int argc, char *argv[])
{
	int num_cmds;
	int i;

	cli_printline(cli, "---------- Help Menu ----------");
	num_cmds = sizeof(cmds) / sizeof(command_t);
	for(i = 0; i < num_cmds; i++)
	{
		snprintf(outline, CLI_LINESZ, "%s - %s", cmds[i].name, cmds[i].desc);
		cli_printline(cli, outline);
	}
	return 0;
}

int id(int argc, char *argv[])
{
    snprintf(outline, CLI_LINESZ, "DER ECU FW V%d.%d.%d", VER_MAJOR, VER_MINOR, VER_BUG);
	cli_printline(cli, outline);
	return 0;
}

int get_throttle(int argc, char *argv[])
{
	snprintf(outline, CLI_LINESZ, "throttle: %3d%%", data->throttle);
	cli_printline(cli, outline);
	return 0;
}

int get_brakelight(int argc, char *argv[])
{
	snprintf(outline, CLI_LINESZ, "brakelight: %s", data->brakelight ? "ON" : "OFF");
	cli_printline(cli, outline);
	return 0;
}

int get_brake(int argc, char *argv[])
{
	snprintf(outline, CLI_LINESZ, "brake: %3d%%", data->brake);
	cli_printline(cli, outline);
	return 0;
}

int get_time(int argc, char *argv[])
{
	read_time();
	snprintf(outline, CLI_LINESZ, "RTC: %02d/%02d/%d-%02d:%02d:%02d",
			data->datetime.month,
			data->datetime.day,
			data->datetime.year,
			data->datetime.hour,
			data->datetime.minute,
			data->datetime.second);
	cli_printline(cli, outline);
	return 0;
}

int set_time(int argc, char *argv[])
{
	int month, day, year, hour, minute, second;

	if(argc != 2)
	{
		cli_printline(cli, "ERROR: stime takes 1 argument");
		cli_printline(cli, "usage: 'stime 1/2/24-17:38:50' for Jan. 2, 2024 at 5:38:50PM");
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
		cli_printline(cli, "ERROR: set time format not readable");
		cli_printline(cli, "usage: 'stime 1/2/24-17:38:50' for Jan. 2, 2024 at 5:38:50PM");
		return 1;
	}

	data->datetime.month = month;
	data->datetime.day = day;
	data->datetime.year = year;
	data->datetime.hour = hour;
	data->datetime.minute = minute;
	data->datetime.second = second;
	write_time();

	snprintf(outline, CLI_LINESZ, "Set RTC: %02d/%02d/%d-%02d:%02d:%02d",
			data->datetime.month,
			data->datetime.day,
			data->datetime.year,
			data->datetime.hour,
			data->datetime.minute,
			data->datetime.second
			);
	cli_printline(cli, outline);
	return 0;
}

int get_faults(int argc, char *argv[])
{
	cli_printline(cli, "System faults:");
	snprintf(outline, CLI_LINESZ, "hard:   %d", data->hard_fault);
	cli_printline(cli, outline);
	snprintf(outline, CLI_LINESZ, "  apps:   %d", data->apps_fault);
	cli_printline(cli, outline);
	snprintf(outline, CLI_LINESZ, "  bse:    %d", data->bse_fault);
	cli_printline(cli, outline);
	snprintf(outline, CLI_LINESZ, "soft:   %d", data->soft_fault);
	cli_printline(cli, outline);
	snprintf(outline, CLI_LINESZ, "  bppc:   %d", data->bppc_fault);
	cli_printline(cli, outline);
	snprintf(outline, CLI_LINESZ, "  acc:    %d", data->acc_fault);
	cli_printline(cli, outline);
	snprintf(outline, CLI_LINESZ, "  cli:    %d", data->cli_fault);
	cli_printline(cli, outline);
	snprintf(outline, CLI_LINESZ, "  canbus: %d", data->canbus_fault);
	cli_printline(cli, outline);
	snprintf(outline, CLI_LINESZ, "  dash:   %d", data->dashboard_fault);
	cli_printline(cli, outline);
	snprintf(outline, CLI_LINESZ, "  mq:     %d", data->mq_fault);
	cli_printline(cli, outline);
	return 0;
}

int ssa(int argc, char *argv[])
{
	int ret;
	if(argc == 1)
	{
		snprintf(outline, CLI_LINESZ, "%d%%", (int)(TIM3->CCR4 * 100 / 65535));
		cli_printline(cli, outline);
	}
	else if(argc == 2)
	{
		int duty = atoi(argv[1]);
		if(duty >= 0 && duty <= 100)
		{
			snprintf(outline, CLI_LINESZ, "setting ssa to %d%%", duty);
			cli_printline(cli, outline);
			set_ssa(duty);
			ret = 0;
		}
		else
		{
			cli_printline(cli, "ERROR: ssa duty must be 0<= Duty <= 100");
			return 1;
		}
	}
	else
	{
		cli_printline(cli, "ERROR: too many arguments to ssa");
		return 1;
	}
	return ret;
}

int sd(int argc, char *argv[])
{
	int ret = 0;
	snprintf(outline, CLI_LINESZ, "bms fail:  %d", data->bms_fail);
	ret |= cli_printline(cli, outline);
	snprintf(outline, CLI_LINESZ, "imd fail:  %d", data->imd_fail);
	ret |= cli_printline(cli, outline);
	snprintf(outline, CLI_LINESZ, "bspd fail: %d", data->bspd_fail);
	ret |= cli_printline(cli, outline);
	snprintf(outline, CLI_LINESZ, "fw fail:   %d", !data->fw_state);
	ret |= cli_printline(cli, outline);
	return ret;
}
