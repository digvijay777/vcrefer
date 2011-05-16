#if defined(WIN32)
#include <Windows.h>
#include <locale.h>
#endif
#include <gtk/gtk.h>

void on_menu_activate(GtkMenuItem* item, gpointer data)
{
	g_print("Menuitem %s is pressed.\n", (gchar*)data);
}

int main(int argc, char* argv[])
{
	GtkWidget*			window;
	GtkWidget*			box;
	GtkWidget*			menubar;
	GtkWidget*			menu;
	GtkWidget*			editmenu;
	GtkWidget*			helpmenu;
	GtkWidget*			rootmenu;
	GtkWidget*			menuitem;
	GtkAccelGroup*		accel_group;

	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "菜单测试程序");
	g_signal_connect(G_OBJECT(window), "destroy"
		, G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);
	
	box = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), box);

	menu = gtk_menu_new();
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	gtk_signal_connect(G_OBJECT(menuitem), "activate"
		, G_CALLBACK(on_menu_activate)
		, (gpointer)"Open");

	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(GTK_OBJECT(menuitem), "activate"
		, G_CALLBACK(on_menu_activate)
		, (gpointer)"Save");

	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate"
		, G_CALLBACK(on_menu_activate)
		, (gpointer)"Save As");

	menuitem = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate"
		, G_CALLBACK(gtk_main_quit), NULL);

	rootmenu = gtk_menu_item_new_with_label(" 文件 ");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu), menu);

	menubar = gtk_menu_bar_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), rootmenu);

	rootmenu = gtk_menu_item_new_with_label(" 编辑 ");
	editmenu = gtk_menu_new();

	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_CUT, accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate"
		, G_CALLBACK(on_menu_activate)
		, (gpointer)"Cut");

	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate"
		, G_CALLBACK(on_menu_activate)
		, (gpointer)"Copy");

	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE, accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate"
		, G_CALLBACK(on_menu_activate)
		, (gpointer)"Paste");

	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_FIND, accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate"
		, G_CALLBACK(on_menu_activate)
		, (gpointer)"Search");

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu), editmenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), rootmenu);

	rootmenu = gtk_menu_item_new_with_label(" 帮助 ");
	helpmenu = gtk_menu_new();

	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate"
		, G_CALLBACK(on_menu_activate)
		, (gpointer)"Help");

	menuitem = gtk_menu_item_new_with_label(" 关于... ");
	gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate"
		, G_CALLBACK(on_menu_activate)
		, (gpointer)"About");

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu), helpmenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), rootmenu);
	gtk_box_pack_start(GTK_BOX(box), menubar, FALSE, FALSE, 0);

	gtk_widget_show_all(window);
	gtk_main();
    return 0;
}



















