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

void cmd_not_found(char *cmd);
void help(char *arg);
void get_throttle(char *arg);
void get_brakelight(char *arg);
void get_brake(char *arg);
void get_time(char *arg);
void get_faults(char *arg);

char line[256];
struct app_data *ad;
command cmds[] = 
{
	{"help", &help, "print help menu"},
	{"get throttle", &get_throttle, "get the throttle percentage"},
	{"get brakelight", &get_brakelight, "get the brake light status"},
	{"get brake", &get_brake, "get the brake percentage"},
	{"get time", &get_time, "get the current time"},
	{"get fault", &get_faults, "gets the faults of the system"}
};

TaskHandle_t cli_task_start(struct app_data *data){
   TaskHandle_t handle;
   xTaskCreate(cli_task_fn, "CLI task", 128, (void *)data, 10, &handle);
   return handle;
}

void cli_task_fn(void *arg){
    struct app_data *data = (struct app_data *)arg;
	ad = data;
	struct cli_device *cli = &data->board.cli;
    uint32_t taskNotification;
	int num_cmds;
	int i;
	
	num_cmds = sizeof(cmds) / sizeof(command);

	cli_putline("DREV ECU Firmware Version 0.1");
	cli_putline("Type 'help' for help");

	while(1){
		xTaskNotifyWait(0, 0, &taskNotification, HAL_MAX_DELAY);
		if(cli->msg_pending == true){
			for(i = 0; i < num_cmds + 1; i++){
				if(i == num_cmds){
					cmd_not_found(cli->line);
					break;
				}
				if(!strncmp(cmds[i].name, cli->line, strlen(cmds[i].name))){
					cmds[i].func(cli->line);
					break;
				}
			}
			cli->msg_pending = false;
		}
	}
}

void cmd_not_found(char *cmd){
	snprintf(line, 256, "Command not found: \'%s\'", cmd);
	cli_putline(line);
	/*
	for(int i = 0; i < strlen(cmd); i++){
		snprintf(line, 256, "[%c(%d)]", cmd[i], cmd[i]);
		cli_putline(line);
	}
	*/
}

void help(char *arg) {
	int num_cmds;
	int i;

	cli_putline("---------- Help Menu ----------");
	num_cmds = sizeof(cmds) / sizeof(command);
	for(i = 0; i < num_cmds; i++){
		snprintf(line, 256, "%s - %s", cmds[i].name, cmds[i].desc);
		cli_putline(line);
	}
}

void get_throttle(char *arg){
	float x = ad->throttlePercent;
	snprintf(line, 256, "throttle: %6.2f%%", x);
	cli_putline(line);
}

void get_brakelight(char *arg){
	snprintf(line, 256, "brakelight: %s", ad->brakeLightState ? "ON" : "OFF");
	cli_putline(line);
}

void get_brake(char *arg){
	float x = ad->brakePercent;
	snprintf(line, 256, "brake: %6.2f%%", x);
	cli_putline(line);
}

void get_time(char *arg){
	cli_putline("time to get a watch");
}

void get_faults(char *arg){
	cli_putline("System faults:");

	snprintf(line, 256, "hard: %d", ad->hardSystemFault);
	cli_putline(line);

	snprintf(line, 256, "soft: %d", ad->softSystemFault);
	cli_putline(line);

	snprintf(line, 256, "apps: %d", ad->appsFaultFlag);
	cli_putline(line);

	snprintf(line, 256, "bse:  %d", ad->bseFaultFlag);
	cli_putline(line);

	snprintf(line, 256, "bppc: %d", ad->bppcFaultFlag);
	cli_putline(line);
}

