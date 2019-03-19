typedef PCB;
int launcher(FILE *p, char *filename);
int countTotalPages(FILE *f);
FILE *findPage(int pageNumber, FILE *f);
int findFrame(FILE *page);
int findVictim(PCB *p);
int updateFrame(int frameNumber, int victimFrame, FILE*page);
int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame);