int benchmarkDebugActivate;
void onlineUpdater();
void wlanStatus(int n);
void loadConfig();
void saveConfig();
void changer(int set);
int getCpuClock();
int getBusClock();
void pspGetCpuBus();
void pspGetModel(int x, int y);
void aboutMenu();
void easterEgg();
void updatesMenu();
void performanceMenu();
void wait_release(unsigned int buttons);
unsigned int wait_press(unsigned int buttons);
void set_cpu_clock(int n);
void processorMenu();
void ramMenu();
void displayMenu();
void wifiMenu();
void build_path(char *output, const char *root, const char *path, int append);
void write_file(const char *read_loc, const char *write_loc, const char *name);
void DumpBootBin(void);
void dump_filesystem(const char *root, const char *write_loc);
void dump_memory(void);
void dumpMenu();
void developerMenu();
void settingsHighlight();
void settingsDeleteResources();
void settingsMenu();

