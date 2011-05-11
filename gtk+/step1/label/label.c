/*
 *	标签测试
 */
#include <gtk/gtk.h>

int main(int argc, char* argv[])
{
	GtkWidget*		window;
	GtkWidget*		box;
	GtkWidget*		label1;
	GtkWidget*		label2;
	GtkWidget*		label3;
	GtkWidget*		label4;
	GtkWidget*		frame1;
	GtkWidget*		frame2;
	GtkWidget*		frame3;
	GtkWidget*		frame4;
	gchar*			title;

	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "多种多样的标签");
	gtk_signal_connect(G_OBJECT(window), "destroy"
		, G_CALLBACK(gtk_main_quit), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 20);

	box = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), box);

	frame1 = gtk_frame_new("标签一");
	label1 = gtk_label_new("这是第一个标签， 居左的, This is the first label.");
	gtk_container_add(GTK_CONTAINER(frame1), label1);
	gtk_label_set_justify(GTK_LABEL(label1), GTK_JUSTIFY_LEFT);
	gtk_box_pack_start(GTK_BOX(box), frame1, FALSE, FALSE, 5);

	frame2 = gtk_frame_new("标签二");
	label2 = gtk_label_new("这是第二个标签， 它是多行的。 \n这还是第二个标签的内容，它是居右的.");
	gtk_container_add(GTK_CONTAINER(frame2), label2);
	gtk_label_set_justify(GTK_LABEL(label2), GTK_JUSTIFY_RIGHT);
	gtk_box_pack_start(GTK_BOX(box), frame2, FALSE, FALSE, 5);

	frame3 = gtk_frame_new("标签三");
	label3 = gtk_label_new(NULL);
	title = "<span foreground=\"red\"><big><i>这是第三个标签,\n它被格式化成红色的了， 并且字体也大了。</i></big></span>";
	gtk_label_set_markup(GTK_LABEL(label3), title);
	gtk_container_add(GTK_CONTAINER(frame3), label3);
	gtk_box_pack_start(GTK_BOX(box), frame3, FALSE, FALSE, 5);

	frame4 = gtk_frame_new("标签四");
	label4 = gtk_label_new("这也是一个多行标签，它的换行方式和上一个有所不同，主要是编程手段不一样了，请详细查看一下源码就会明白是怎么回事了。");
	gtk_container_add(GTK_CONTAINER(frame4), label4);
	gtk_label_set_line_wrap(GTK_LABEL(label4), TRUE);
	gtk_box_pack_start(GTK_BOX(box), frame4, FALSE, FALSE, 5);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}






































