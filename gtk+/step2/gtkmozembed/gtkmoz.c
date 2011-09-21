#include<gtk/gtk.h>
#include"gtkmozembed.h"
/*

ubuntu下编译：
    $sudo apt-get install libxul-dev ##安装依赖
    $gcc -o gtkmoz gtkmoz.c `pkg-config --libs --cflags xulrunner-gtkmozembed gtk+-2.0`
    $./gtkmoz www.baidu.com
    $./gtkmoz mylocalweb.html

*/
static gchar *list_sig[]=
{
"link_message",//0
"js_status",//1
"location",//2
"title",//3
"open_uri",//4
NULL
};

gint open_uri_cb (GtkMozEmbed *embed, const char *uri, gpointer data)
{

    g_print("Got signal->%s:str->%s .\n",list_sig[4],uri);
//    gtk_moz_embed_load_url(embed,"http://www.baidu.com/s?wd=gh");
//    g_strlcpy((gchar*)uri,"http://www.baidu.com/s?wd=gh",0);
    return FALSE;
}

void do_signal_cb (GtkMozEmbed *embed, char *index)
{
    gint i = *index - '0';
    gchar *str = NULL;
    switch(i){
    case 0:
        str = gtk_moz_embed_get_link_message(embed);
    break;
    case 1:
        str = gtk_moz_embed_get_js_status(embed);
    break;
    case 2:
        str = gtk_moz_embed_get_location(embed);
    break;
    case 3:
        str = gtk_moz_embed_get_title(embed);
    break;
    default:
        str = g_strdup_printf("why index = %d.",i);
    break;
    }
    g_print("I get signal->%s:str->%s .\n",list_sig[i],str);
    g_free(str);
}

int main(int argc,char **argv)
{
    gtk_init(&argc,&argv);
    GtkWidget *window = NULL;
    GtkWidget *mozembed = NULL;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_moz_embed_set_comp_path("/tmp/mytest");
    mozembed = gtk_moz_embed_new ();
    /*handle event signal */
    g_signal_connect(mozembed,list_sig[0],G_CALLBACK(do_signal_cb),"0");
    g_signal_connect(mozembed,list_sig[1],G_CALLBACK(do_signal_cb),"1");
    g_signal_connect(mozembed,list_sig[2],G_CALLBACK(do_signal_cb),"2");
    g_signal_connect(mozembed,list_sig[3],G_CALLBACK(do_signal_cb),"3");
    g_signal_connect(mozembed,list_sig[4],G_CALLBACK(open_uri_cb),"4");
    /*  load an url */
    gtk_moz_embed_load_url(GTK_MOZ_EMBED(mozembed),argv[1]);
    gtk_container_add (GTK_CONTAINER(window),mozembed);

    g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
} 
