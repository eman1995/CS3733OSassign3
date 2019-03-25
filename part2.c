#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int clock = 0;
int pagefaults = 0;
typedef struct pt {
    int PT[32][2];
    int FT[8][2];    
}pageTable;


void initZero(pageTable *pt) {
    int i, j;

    for(i = 0; i < 32; i++) {
        for(j = 0; j <2; j++) {
            pt->PT[i][j] = 0;
            //printf("%d\t", pt->PT[i][j]);
        }
    }
    for(i = 0; i < 8; i++) {
        for(j = 0; j < 2; j++) {
            pt->FT[i][j] = 0;
        }
    }
}
//Page index is valid take frame from pagetable and update frame
void frameupdate(int p_num,int findex, pageTable *pt) {
    if(pt->FT[findex][0] == p_num) {
        pt->FT[findex][1] = clock;
        
    }
    else {
        pt->PT[p_num][0] = framelookup(p_num, pt);
    }
    
}
//p_num goes into the FT[i][0] spot thats free
//clock will go inot FT[i][1] spot
//i is equal to framenumber chosen
int framelookup(int p_num, pageTable *pt) {
    int i, min = 0, findex, temp;
    for(i = 1; i < 8; i++) {
        if(pt->FT[i][1] == 0) {
            pt->FT[i][0] = p_num;
            pt->FT[i][1] = clock;
            return i;
        }
    }

    for(i = 1; i < 8; i++) {
        if(min != 0 && min > pt->FT[i][1]){ //first run min is 0
            min = pt->FT[i][1];
            findex = i;
        }
        else {
            min = pt->FT[i][1];
            findex = i;
        }
    }
    temp = pt->FT[findex][0];
    pt->PT[temp][1] = 0;
    pt->PT[temp][0] = 0;
    pt->FT[findex][0] = p_num;
    pagefaults += 1;
    return findex;
}
//Have the pagenumber goto the index of the page number, which is p_num
//if the valid bit ([p_num][1] == 0) then lookup a free space on frame
int pagelookup(int p_num, pageTable *pt) {
    if(pt->PT[p_num][1] == 1) {
        frameupdate(p_num, pt->PT[p_num][0], pt);
        return pt->PT[p_num][0];
    }
    else {
        pt->PT[p_num][1] = 1;
        pt->PT[p_num][0] = framelookup(p_num, pt);
        return pt->PT[p_num][0];
    }
}

unsigned long translate(unsigned long buffer, pageTable *pt) {
    unsigned long p_num = buffer >> 7; //gives a PT index #
    unsigned long offset = buffer & 0x07f;

    unsigned long f_num = pagelookup(p_num, pt);//f_num = PT[p_num]
    unsigned long phyAddr = (f_num << 7) + offset;
    return phyAddr;
}

int main(int argc, char *argv[]) {
    pageTable PTable;
    initZero(&PTable);

    FILE *infile, *outfile;
    unsigned long buff, PA;

    infile = fopen(argv[1], "rb");
    outfile = fopen(argv[2], "wb+");
    
    while(fread(&buff, sizeof(buff), 1L, infile) == 1) {
        clock++;
        PA = translate(buff,&PTable);
        fwrite(&PA, sizeof(PA), 1L, outfile);
        
    }
    close(infile);
    close(outfile);
    printf("%d\n", pagefaults);
    return 0;
}
