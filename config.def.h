// use ipc functionality
static bool ipc = true;
// initially hide all bars
static bool hidden = false;
// initially draw all bars at the bottom
static bool bottom = false;
// hide vacant tags
static bool hide_vacant = true;
// vertical pixel padding above and below text
static uint32_t vertical_padding = 0;
// allow in-line color commands in status text
static bool status_commands = true;
// center title text
static bool center_title = true;
// use title space as status text element
static bool custom_title = false;
// scale
static uint32_t buffer_scale = 1;
// font
static char *fontstr = "3270NerdFontMono:size=13";
// tag names
static char *tags_names[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

// set 16-bit colors for bar
// 8-bit color can be converted to 16-bit color by simply duplicating values e.g
// 0x55 -> 0x5555, 0xf1 -> 0xf1f1
static pixman_color_t active_fg_color =   { .red = 0xeeee, .green = 0xeeee, .blue = 0xeeee, .alpha = 0xeeee, };
static pixman_color_t active_bg_color =   { .red = 0x0000, .green = 0x5555, .blue = 0x7777, .alpha = 0xeeee, };
static pixman_color_t occupied_fg_color = { .red = 0xbbbb, .green = 0xbbbb, .blue = 0xbbbb, .alpha = 0xeeee, };
static pixman_color_t occupied_bg_color = { .red = 0x2222, .green = 0x2222, .blue = 0x2222, .alpha = 0xeeee, };
static pixman_color_t inactive_fg_color = { .red = 0xbbbb, .green = 0xbbbb, .blue = 0xbbbb, .alpha = 0xeeee, };
static pixman_color_t inactive_bg_color = { .red = 0x2222, .green = 0x2222, .blue = 0x2222, .alpha = 0xeeee, };
static pixman_color_t urgent_fg_color =   { .red = 0x2222, .green = 0x2222, .blue = 0x2222, .alpha = 0xeeee, };
static pixman_color_t urgent_bg_color =   { .red = 0xeeee, .green = 0xeeee, .blue = 0xeeee, .alpha = 0xeeee, };

/* string otputted if the supposed one isn`t valid */
#define UNDEFINEDSTR "n/a"

/* interval between updates (in s) */
const unsigned int interval = 1;

/*
 * function            description                     argument (example)
 *
 * battery_perc        battery percentage              battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_remaining   battery remaining HH:MM         battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_state       battery charging state          battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * cat                 read arbitrary file             path
 * cpu_freq            cpu frequency in MHz            NULL
 * cpu_perc            cpu usage in percent            NULL
 * datetime            date and time                   format string (%F %T)
 * disk_free           free disk space in GB           mountpoint path (/)
 * disk_perc           disk usage in percent           mountpoint path (/)
 * disk_total          total disk space in GB          mountpoint path (/)
 * disk_used           used disk space in GB           mountpoint path (/)
 * entropy             available entropy               NULL
 * gid                 GID of current user             NULL
 * hostname            hostname                        NULL
 * ipv4                IPv4 address                    interface name (eth0)
 * ipv6                IPv6 address                    interface name (eth0)
 * kernel_release      `uname -r`                      NULL
 * keyboard_indicators caps/num lock indicators        format string (c?n?)     // not supported yet
 *                                                     see keyboard_indicators.c
 * keymap              layout (variant) of current     NULL                     // not supported yet
 *                     keymap
 * load_avg            load average                    NULL
 * netspeed_rx         receive network speed           interface name (wlan0)
 * netspeed_tx         transfer network speed          interface name (wlan0)
 * num_files           number of files in a directory  path
 *                                                     (/home/foo/Inbox/cur)
 * ram_free            free memory in GB               NULL
 * ram_perc            memory usage in percent         NULL
 * ram_total           total memory size in GB         NULL
 * ram_used            used memory in GB               NULL
 * run_command         custom shell command            command (echo foo)
 * swap_free           free swap in GB                 NULL
 * swap_perc           swap usage in percent           NULL
 * swap_total          total swap size in GB           NULL
 * swap_used           used swap in GB                 NULL
 * temp                temperature in degree celsius   sensor file
 *                                                     (/sys/class/thermal/...)
 *                                                     NULL on OpenBSD
 *                                                     thermal zone on FreeBSD
 *                                                     (tz0, tz1, etc.)
 * uid                 UID of current user             NULL
 * uptime              system uptime                   NULL
 * username            username of current user        NULL
 *                                                     NULL on OpenBSD/FreeBSD
 * wifi_essid          WiFi ESSID                      interface name (wlan0)
 * wifi_perc           WiFi signal in percent          interface name (wlan0)
 */
static const struct arg args[] = {
	/* function     format          argument */
	{ battery_perc, "battery: %s%%", "BAT0"                   },
	{ cpu_perc,     " | cpu: %s%%",  NULL                     },
	{ ram_used,     " | used: %s",   NULL                     },
	{ ram_total,    "/%s",           NULL                     },
	{ ram_perc,     " (%s%%)",       NULL                     },
	{ netspeed_rx,  " | rx: %s",     "wlp58s0"                },
	{ netspeed_tx,  " / tx: %s",     "wlp58s0"                },
	{ load_avg,     " | load: %s",   NULL                     },
	{ datetime,     " | %s",         "%I:%M:%S | %a %d %b %g" },
};

