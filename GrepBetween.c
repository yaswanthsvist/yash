#include<stdio.h>
#include<string.h>

int main(int argc,char *argv[])
{
        int i=0,last=0;
        char buff[400]={0},s1[300]={0},s2[300]={0},flnm[400]={0},diffPrmtr[800][200]={0},temp[200];
        FILE *Parameters=NULL,*CDR=NULL;

        sprintf(s1,"%s%s",argv[1],argv[2]);
        sprintf(s2,"%s%s",argv[1],argv[3]);
        printf("S1=%s: S2=%s:\n",s1,s2);

        sprintf(buff,"ls %s* > GREP_BETWEEN_JUNK",argv[1]);
        system(buff);
        Parameters=fopen("GREP_BETWEEN_JUNK","r");
        while(NULL!=fgets(flnm,400,Parameters))
        {
                null_newline(flnm);
                //CDR=fopen(flnm,"r");
                if( ( 0>=strncmp( flnm,s1,strlen(s1) ) ) && ( 0<=strncmp( flnm,s2,strlen(s2) ) ) )
                {
                        puts(flnm);
                        /*ll_newline(buff);
                        if(0==strncmp(buff,argv[1],strlen(argv[1])))
                        {
                                copy(buff,temp);
                                for(i=0;i<=last;i++)
                                if(0==strcmp(temp,diffPrmtr[i]))
                                goto l;
                                printf("file=%s:%s\n",flnm,temp);
                                strcpy(diffPrmtr[i],temp);
                                last++;
l:                              i=0;
                        }
                        */
                }
                //fclose(CDR);
        }
//      for(i=0;i<=last;i++)
//      puts(diffPrmtr[i]);
        return 0;
}




int copy(char * src, char *dst)
{
//      puts("entered copy");
//      puts(src);
        int i=0;
        for(i=0;src[i]!=':';i++)
        if(src[i]==0)
        return -1;
        i++;
        int j=0;

        for(;src[i]!=0;)
        dst[j++]=src[i++];

        dst[j]=0;
//      puts("leaving copy");
        return 0;

}
int null_newline(char * string)
{
        int i;
        for(i=0;string[i]!='\n'&&string[i]!='\0';i++);
        string[i]='\0';
}


