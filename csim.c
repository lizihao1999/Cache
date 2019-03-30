#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned long ll;

typedef struct {
    int valid;
    ll tag;
    int recent;
}line;

typedef line** set;

typedef set* block;

typedef struct cache {
    block bloc;  
    int miss;
    int hit;
    int evict;
    int recent;
}cache;

line* new_line() {
    line* l=malloc(sizeof(line));
    l->valid=0;
    l->tag=0;
    l->recent=0;
    return l;
}

set new_set(int nline) {
    set s=malloc(sizeof(line*)*nline);
    for (int i=0; i<nline; i++) {
        s[i]=new_line();
    }
    return s;
}

block new_block(int nset, int nline) {
    block  b=malloc(sizeof(set)*(2<<nset));
    for (int i=0; i<(2<<nset); i++) {
        b[i]=new_set(nline);
    }
    return b;
}


cache* new_cache(int nset, int nline) {
    cache* c=malloc(sizeof(cache));
    c->bloc=new_block(nset, nline);
    c->miss=0;
    c->hit=0;
    c->evict=0;
    c->recent=0;
    return c;
}

ll hex(char* buf) {
	char* a=malloc(sizeof(char)*100);
	int i=0;
	while (buf[i]!=',') {
		a[i]=buf[i];
		i=i+1;
	}
	a[i]='\0';
	ll t = (ll) strtol(a, NULL, 16);
	free(a);
	return t;
}
void act(cache* c, ll addr, int nline, int nset, int nblock) {
    ll tag=addr>>(nset+nblock);
    int sindex=(addr>>nblock)%(1<<nset);
    block b=c->bloc;
    set s=b[sindex];
	c->recent=c->recent+1;
    int flag=1;
    for (int i=0; i<nline; i++) {
        if (s[i]->valid==0) {
            s[i]->valid=1;
            s[i]->tag=tag;
            s[i]->recent=c->recent;
            c->miss=c->miss+1;
            flag=0;
	    break;
        } else if (s[i]->tag==tag) {
            s[i]->recent=c->recent;
            c->hit=c->hit+1;
		flag=0;
            break;
        }
    }
    if (flag==1) {
        int j=0;
        for (int i=0; i<nline; i++) {
            if (s[i]->recent<s[j]->recent) {
            	j=i;
            }
        }
        s[j]->tag=tag;
        s[j]->recent=c->recent;
        c->miss++;
        c->evict++;        
    }
}

int main(int argc, char* argv[])
{
    int s=atoi(argv[2]);
    int e=atoi(argv[4]);    
    int b=atoi(argv[6]);
    FILE* fp;
    fp=fopen(argv[8],"r");
    cache* c=new_cache(s,e);
    if (fp==NULL) {
        printf("Error\n");
        exit(1);
    }
    char* flag=malloc(sizeof(char)*50);
    char* buf=malloc(sizeof(char)*100);

    while(fscanf(fp, " %c %s", flag, buf)!=EOF) {
	ll addr=hex(buf);	
	if (strcmp(flag, "L")==0) {
            act(c,addr,e,s,b);
        } else if (strcmp(flag,"S")==0) { 
            act(c,addr,e,s,b);
        } else if (strcmp(flag,"M")==0) {
            act(c,addr,e,s,b);
            act(c,addr,e,s,b);
        }     
    }
    printSummary(c->hit,c->miss,c->evict);
    fclose(fp);
    return 0;
}

    



