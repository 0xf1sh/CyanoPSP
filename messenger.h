#include <pspkernel.h>
#include <oslib/oslib.h>

int InitAdhocMessenger();
void joinServer();
void createServer();

char message[500] = "";	

void printInfo();
void clientConnected(struct remotePsp *aPsp);
void doClient();
void serverConnected(struct remotePsp *aPsp);
void doServer();
void activateOSK();
void newmessage();
int messenger();