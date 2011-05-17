#if defined(WIN32)
#include <Windows.h>
#include <locale.h>
#endif
#include <gtk/gtk.h>

void on_menu_activate(GtkMenuItem* item, gpointer data);	// 回调函数申名
static GtkItemFactoryEntry		menu_items[]		= {		// 定义菜单
	{"/ 文件 (_F) ", NULL, NULL, 0, "<Branch>"}
	, {"/ 文件 (_F) /新建", NULL, on_menu_activate, "新建", "<StockItem>", GTK_STOCK_NEW}
	, {"/ 文件 (_F) /打开", NULL, on_menu_activate, "打开", "<StockItem>", GTK_STOCK_OPEN}
	, {"/ 文件 (_F) /保存", NULL, on_menu_activate, "保存", "<StockItem>", GTK_STOCK_SAVE}
	, {"/ 文件 (_F) /另存为", NULL, on_menu_activate, "另存为", "<StockItem>", GTK_STOCK_SAVE_AS}
	, {"/ 文件 (_F) /-", NULL, NULL, 0, "<Separator>"}
	, {"/ 文件 (_F) /退出", NULL, gtk_main_quit, 0, "<StockItem>", GTK_STOCK_QUIT}
	, {"/ 编辑 (_E) ", NULL, NULL, 0, "<Branch>"}
	, {"/ 编辑 (_E) /剪切", NULL, on_menu_activate, "剪切", "<StockItem>", GTK_STOCK_CUT}
	, {"/ 编辑 (_E) /复制", NULL, on_menu_activate, "复制", "<StockItem>", GTK_STOCK_COPY}
	, {"/ 编辑 (_E) /粘贴", NULL, on_menu_activate, "粘贴", "<StockItem>", GTK_STOCK_PASTE}
	, {"/ 编辑 (_E) /查找", NULL, on_menu_activate, "查找", "<StockItem>", GTK_STOCK_FIND}
	, {"/ 帮助 (_H) ", NULL, NULL, 0, "<Branch>"}
	, {"/ 帮助 (_H) /帮助", NULL, on_menu_activate, "帮助", "<StockItem>", GTK_STOCK_HELP}
	, {"/ 帮助 (_H) /关于...", NULL, on_menu_activate, "关于", "<StockItem>", GTK_STOCK_ABOUT}
};

void on_menu_activate(GtkMenuItem* item, gpointer data)
{
	g_print("Menu item %s is pressed.\n", (gchar *)data);
}

int main(int argc, char* argv[])
{
	GtkWidget*			window;
	GtkWidget*			box;
	GtkWidget*			menubar;
	GtkAccelGroup*		accel_group;
	GtkItemFactory*		item_factory;
	gint				n				= 15;

	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "添加菜单的快捷方法");
	g_signal_connect(G_OBJECT(window), "destroy"
		, G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
	
	accel_group = gtk_accel_group_new();

	box = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), box);
	gtk_widget_show(box);

	item_factory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<main>", accel_group);
	gtk_item_factory_create_items(item_factory, n, menu_items, NULL);
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);
	menubar = gtk_item_factory_get_widget(item_factory, "<main>");
	gtk_box_pack_start(GTK_BOX(box), menubar, FALSE, FALSE, 0);
	gtk_widget_show(menubar);

	gtk_widget_show(window);
	gtk_main();

    return 0;
}




















