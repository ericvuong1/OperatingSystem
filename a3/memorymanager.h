struct PCB;
int launcher(FILE *p, char *filename);
int countTotalPages(FILE *f);
FILE *findPage(int pageNumber, FILE *f);
extern char *filePaths[10];

void initFilePaths();
int addToFilePaths(char *fileName);

int findFrame(FILE *page);
int findVictim(struct PCB *p);
int updateFrame(int frameNumber, int victimFrame, FILE*page);
int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame);