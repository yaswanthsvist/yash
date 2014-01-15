/* example-start notebook notebook.c */
//gcc GtkNotebook.c -o GtkNotebook `pkg-config --cflags --libs gtk+-2.0` -lpthread
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include<string.h>

        int sockfd[4],numSockRecvSent[4];
        struct sockaddr_in servaddr[4],cliaddr[4];
        socklen_t len[4];
        char mesg[4][200]={0},sendCmnd[4][10]={0},prevMsg[4][5]={0},slable1_update[4][200]={0};

        int quit_gui=1;

        char mm[4][5]={"MM1","MM3","MM4","MM7"};
        char bufferf[32];
        int b[4]={0,1,2,3},giRun[4],giStop[4],giPause[4];
        char inner_tabs[3][10]={"STATUS","RESULT","FUTURE"};
        char zero[3]="0",one[3]="1",two[3]="2",three[3]="3";

        GtkWidget *table,*table1,*table2[4],*button[4][3];
        GtkWidget *runImg[4],*pauseImg[4],*stopImg[4],*waitImg[4],*fixed[4],*fixed_run[4],*fixed_pause[4],*fixed_skip[4];
        GtkWidget *button_run[4],*button_pause[4],*button_stop[4],*button_cntnu[4];
        GtkWidget *prsnt_fixed_img[4],*prsnt_fixed_run[4],*prsnt_fixed_pause[4];
        GtkWidget *label,*label1,*label_status[4];
        int gtkMain(void *);
        void show_Img(GtkWidget *,int);
        void show_run(GtkWidget *,int);
        void show_pause(GtkWidget *,int);


/* This function rotates the position of the tabs */
void rotate_book( GtkButton   *button,
                  GtkNotebook *notebook )
{
    gtk_notebook_set_tab_pos (notebook, (notebook->tab_pos +1) %4);
}
/* Add/Remove the page tabs and the borders */
void tabsborder_book( GtkButton   *button,
                      GtkNotebook *notebook )
{
    gint tval = FALSE;
    gint bval = FALSE;
    if (notebook->show_tabs == 0)
            tval = TRUE;
    if (notebook->show_border == 0)
            bval = TRUE;

    gtk_notebook_set_show_tabs (notebook, tval);
    gtk_notebook_set_show_border (notebook, bval);
}

/* Remove a page from the notebook */
void remove_book( GtkButton   *button,
                  GtkNotebook *notebook )
{
    gint page;

    page = gtk_notebook_get_current_page(notebook);
    gtk_notebook_remove_page (notebook, page);
    /* Need to refresh the widget --
     This forces the widget to redraw itself. */
    gtk_widget_draw(GTK_WIDGET(notebook), NULL);
}

gint delete( GtkWidget *widget,
             GtkWidget *event,
             gpointer   data )
{
        puts("in delete function");
        quit_gui=0;
//      sleep(10);
    gtk_main_quit();
    return(FALSE);
}


gint run( GtkWidget *widget,
             gpointer   data )
{
        int k=*(int *)data;
        printf("%d\n",k);

        strcpy(sendCmnd[k],"1");
        show_Img(waitImg[k],k);
        sleep(2);
}
gint stop( GtkWidget *widget,
             gpointer   data )
{
        int k=*(int *)data;
        printf("%d\n",k);
        strcpy(sendCmnd[k],"4");
        show_Img(waitImg[k],k);
        sleep(2);
}
int controlFunc(void * msg3)
{

        int k=*(int *)msg3;
        while (quit_gui==1)
        {

                len[k] = sizeof(cliaddr[k]);
                sendto(sockfd[k],sendCmnd[k],strlen(sendCmnd[k]),0,(struct sockaddr *)&servaddr[k],sizeof(servaddr[k]));
                printf("in %d waiting for response-----------------------------------\n",k);
                numSockRecvSent[k]=recvfrom(sockfd[k],mesg[k],1000,0,NULL,NULL);
                mesg[k][numSockRecvSent[k]]=0;
                puts(mesg[k]);
                printf("%d sending %s\n",k,sendCmnd[k]);
                sleep(2);
/*
                numSockRecvSent[k] = recvfrom(sockfd[k],mesg[k],1000,0,(struct sockaddr *)&cliaddr[k],&len[k]);
                mesg[k][numSockRecvSent[k]] = 0;
                printf("%s\n",mesg[k]);

                sendto(sockfd[k],sendCmnd[k],strlen(sendCmnd[k]),0,(struct sockaddr *)&cliaddr[k], sizeof(cliaddr[k]));
                sleep(1);

                puts(mesg[k]);
*/
                if(0!=strncmp(prevMsg[k],mesg[k],4))
                {
                        strncpy(prevMsg[k],mesg[k],4);
                        giRun[k]=mesg[k][0];
                        giPause[k]=mesg[k][1];
                        if(giRun[k]=='1')
                        {

                                show_run(button_stop[k],k);
                                if( giPause[k]=='1')
                                {
                                        show_pause(button_cntnu[k],k);
                                        show_Img(pauseImg[k],k);
                                }
                                else
                                {
                                        show_pause(button_pause[k],k);
                                        show_Img(runImg[k],k);
                                }

                        }
                        else if(giRun[k]=='0')
                        {
                                show_pause(NULL,k);

                                show_Img(stopImg[k],k);
                                show_run(button_run[k],k);
                        }
                        giStop[k]=mesg[k][3];
                        if(giStop[k]=='1')
                        {
                                show_pause(NULL,k);
                                show_run(button_run[k],k);
                                show_Img(stopImg[k],k);
                        }
                        /*lse if(giStop[k]=='0')
                        {
                                hideAll(k);
                                gtk_widget_show(button_stop[k]);
                                gtk_widget_show(runImg[k]);
                        }
                        */
                }

                if(0!=strcmp(slable1_update[k],&mesg[k][5]))
                {
                        strcpy(slable1_update[k],&mesg[k][5]);
                        gtk_label_set_text(GTK_LABEL(label_status[k]),slable1_update[k]);
                }


   }
        printf("%d control thread ended\n",k);
 return 0;

}


    
