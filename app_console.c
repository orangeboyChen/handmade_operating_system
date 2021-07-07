#include "app_console.h"

#include "widget_longTextField.h"

static char *prefix = "[user /]#";
static struct LongTextField *console;

static unsigned int canEditX;
static unsigned int canEditY;

static char command[512];

static struct CommandManager commandManager;

void consoleMain()
{
    struct Window *fatherWindow = createWindow(rootSheetManager.sheet, 5, 20, 300, 170, "Console");

    console = createLongTextFieldWithColor(fatherWindow->sheet, 2, 20, 300 - 2, 170 - 30, 3000, "", COL8_000000, COL8_FFFFFF);
    console->sheet->systemActionManager->onKeyPress = &onConsoleKeyDown;

    createCommand("mem", &consoleCommandMem);
    createCommand("cd", &consoleCommandCd);
    createCommand("hello", &consoleCommandHello);
    createCommand("echo", &consoleCommandEcho);

    printConsoleNewCommandLine();

    while (true)
    {
        //Timer
        struct FifoItem *item2 = getInFifo(systemTimerManager->fifo);

        if (item2->type == FIFO_TYPE_TIMER)
        {
            void (*callback)() = item2->pointer;
            callback();
        }
    }
}

void onConsoleKeyDown(struct Sheet *this, char c, unsigned int raw)
{
    struct LongTextField *longTextField = this->attribute[6];

    if (raw == 0x48)
    {
        longTextField->upButton->systemActionManager->onMouseLeftDown(longTextField->upButton, 0, 0);
    }
    else if (raw == 0x50)
    {
        longTextField->downButton->systemActionManager->onMouseLeftDown(longTextField->downButton, 0, 0);
    }
    else if (raw == 0x0e)
    {
        if (longTextField->currentCursorHeight * longTextField->charPerLine + longTextField->currentCursorWidth > canEditY * longTextField->charPerLine + canEditX)
        {
            if ((longTextField->currentCursorHeight == longTextField->currentDisplayHeight) && (longTextField->currentCursorWidth == longTextField->currentDisplayWidth))
            {
                deleteSingleCharInLongTextField(longTextField);
            }
            else
            {
                deleteSingleCharInLongTextFieldByCursor(longTextField);
            }
        }
    }
    else if (raw == 0x4b)
    {
        if (longTextField->currentCursorHeight * longTextField->charPerLine + longTextField->currentCursorWidth > canEditY * longTextField->charPerLine + canEditX)
        {
            leftMoveLongTextFieldCursor(longTextField);
        }
    }
    else if (raw == 0x4d)
    {
        rightMoveLongTextFieldCursor(longTextField);
    }
    else if (0x20 <= c && c <= 0x7e)
    {
        // char s[32];
        // sprintf(s, "%d %d %d %d", longTextField->currentCursorWidth, longTextField->currentCursorHeight, longTextField->currentDisplayWidth, longTextField->currentDisplayHeight);
        // setLabelText(statusLabel, s, COL8_000000);

        if ((longTextField->currentCursorHeight == longTextField->currentDisplayHeight) && (longTextField->currentCursorWidth == longTextField->currentDisplayWidth))
        {
            putSingleCharInLongTextField(longTextField, c);
        }
        else
        {
            putSingleCharInLongTextFieldByCurrentCursor(longTextField, c);
        }

        unsigned int currentCursorIndexInContentVram = longTextField->currentCursorHeight * 16 * longTextField->displaySheet->width + longTextField->currentCursorWidth * 8;

        if (currentCursorIndexInContentVram <= longTextField->displayHeightPointer * longTextField->displaySheet->width || currentCursorIndexInContentVram >= longTextField->displayHeightPointer * longTextField->displaySheet->width + (longTextField->displaySheet->height - 16) * longTextField->displaySheet->width)
        {
            moveLongTextFieldToBottom(longTextField);
        }
    }
    else if (c == '\n')
    {
        commitConsoleCommand();
    }
}

void printConsoleNewCommandLine()
{
    putStringInLongTextField(console, prefix);
    canEditX = console->currentCursorWidth;
    canEditY = console->currentCursorHeight;
}

void commitConsoleCommand()
{
    unsigned int index = getCharIndexInLongTextFieldContent(console, canEditX, canEditY);
    strcpy(command, console->content + index);
    putSingleCharInLongTextField(console, '\n');

    handleConsoleCommand();

    printConsoleNewCommandLine();
    moveLongTextFieldToBottom(console);
}

void handleConsoleCommand()
{
    char *params[16];
    char commandTemp[128];
    strcpy(commandTemp, command);

    int currentParamsIndex = 0;

    char *temp = &commandTemp;
    char *lastTemp = &commandTemp;
    while (*temp != '\0')
    {
        if (*temp == ' ')
        {
            *temp = '\0';

            if (temp != lastTemp)
            {
                params[currentParamsIndex] = lastTemp;
                currentParamsIndex++;
            }

            temp++;
            lastTemp = temp;
            continue;
        }
        temp++;
    }
    params[currentParamsIndex] = lastTemp;

    // char s[32];
    // sprintf(s, "%s", params[0]);
    // setLabelText(statusLabel, s, COL8_000000);

    // if (strcmp(params[0], "mem") == 0)
    // {
    //     unsigned int leftMemoryByte = getUnusedMemoryTotal(getMemoryManager());
    //     char s[32];
    //     sprintf(s, "%d byte left.", leftMemoryByte);
    //     putStringInLongTextField(console, s);

    //     putSingleCharInLongTextField(console, '\n');
    // }
    // else if (strcmp(params[0], "clear") == 0)
    // {
    //     clearConsole();
    // }
    // else if (strcmp(params[0], "hello") == 0)
    // {
    //     putStringInLongTextField(console, "Hello!\n");
    // }
    // else
    // {
    //     char s[32];
    //     sprintf(s, "No command named %s", params[0]);
    //     putStringInLongTextField(console, s);
    //     putSingleCharInLongTextField(console, '\n');
    // }

    if (strcmp(params[0], "") == 0)
    {
        return;
    }

    if (strcmp(params[0], "clear") == 0)
    {
        clearConsole();
        return;
    }

    struct Command *runningCommand = NULL;
    struct Command *currentCommand = commandManager.firstCommand;
    while (currentCommand != NULL)
    {
        if (strcmp(currentCommand->commandName, params[0]) == 0)
        {
            runningCommand = currentCommand;
            currentCommand->handler(currentCommand, params, currentParamsIndex);
            break;
        }

        currentCommand = currentCommand->nextStoreCommand;
    }

    if (runningCommand == NULL)
    {
        char s[32];
        sprintf(s, "No command named %s\n", params[0]);
        putStringInLongTextField(console, s);
    }
}

void consoleCommandHello(struct Command *this, char *params[], unsigned int maxIndex)
{
    putStringInLongTextField(console, "Hello.\n");
}

void consoleCommandCd(struct Command *this, char *params[], unsigned int maxIndex)
{
    if (maxIndex < 1)
    {
        putStringInLongTextField(console, "Error input.\n");
        return;
    }

    if (strcmp(params[1], "/") != 0)
    {
        char s[32];
        sprintf(s, "No such directory called %s\n", params[1]);
        putStringInLongTextField(console, s);
    }
}

void consoleCommandMem(struct Command *this, char *params[], unsigned int maxIndex)
{
    unsigned int leftMemoryByte = getUnusedMemoryTotal(getMemoryManager());
    char s[32];
    sprintf(s, "%d byte left.\n", leftMemoryByte);
    putStringInLongTextField(console, s);
}

void consoleCommandEcho(struct Command *this, char *params[], unsigned int maxIndex)
{
    char *s = command;
    s += 4;
    while (*s != ' ')
    {
        s++;
    }

    if (*s != '\0')
    {
        s++;
        putStringInLongTextField(console, s);
        putSingleCharInLongTextField(console, '\n');
    }
}

void clearConsole()
{
    strcpy(console->content, "");
    console->displayHeightPointer = 0;
    console->currentDisplayHeight = 0;
    console->currentDisplayWidth = 0;
    console->currentCursorHeight = 0;
    console->currentCursorWidth = 0;
    canEditX = 0;
    canEditY = 0;
    updateLongTextFieldContent(console);
}

struct Command *getUnusedCommand()
{
    int i;
    for (i = 0; i < COMMAND_MAX; i++)
    {
        if (!commandManager.commandStore[i].isUsing)
        {
            commandManager.commandStore[i].isUsing = true;
            return &commandManager.commandStore[i];
        }
    }
    return NULL;
}

void createCommand(char *commandName, void (*handler)(struct Command *this, char params[]))
{
    struct Command *command = getUnusedCommand();

    command->commandName = commandName;
    command->handler = handler;

    if (commandManager.firstCommand == NULL)
    {

        commandManager.firstCommand = command;
    }
    else
    {
        command->nextStoreCommand = commandManager.firstCommand;
        commandManager.firstCommand = command;
    }
}