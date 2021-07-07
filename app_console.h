#ifndef _APP_CONSOLE_H
#define _APP_CONSOLE_H

#include "timer.h"

#define COMMAND_MAX 32
struct Command
{
    char *commandName;
    void (*handler)(struct Command *this, char params[], unsigned int maxIndex);

    struct Command *nextStoreCommand;
    unsigned int isUsing;
};

struct CommandManager
{
    char prefix[64];
    char fullCommand[128];
    char *params[32];

    struct Command *firstCommand;
    struct Command commandStore[32];
};

void consoleMain();

void onConsoleKeyDown(struct Sheet *this, char c, unsigned int raw);
void printConsoleNewCommandLine();
void commitConsoleCommand();
void handleConsoleCommand();
void clearConsole();

void createCommand(char *commandName, void (*handler)(struct Command *this, char params[]));
struct Command *getUnusedCommand();

void consoleCommandCd(struct Command *this, char *params[], unsigned int maxIndex);
void consoleCommandMem(struct Command *this, char *params[], unsigned int maxIndex);
void consoleCommandHello(struct Command *this, char *params[], unsigned int maxIndex);
void consoleCommandEcho(struct Command *this, char *params[], unsigned int maxIndex);
#endif