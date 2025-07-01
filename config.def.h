#include <X11/XF86keysym.h>



#define STATUSBAR "dwmblocks"
 


/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  										= 1;        /* border pixel of windows */
static unsigned int snap      							= 32;       /* snap pixel */
static const unsigned int systraypinning 					= 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft 					= 1;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing 					= 2;   /* systray spacing */
static const int systraypinningfailfirst 					= 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray       	 							= 1;        /* 0 means no systray */
static int showbar            							= 1;        /* 0 means no bar */
static int topbar             							= 1;        /* 0 means bottom bar */
static const int extrabar           = 1;        /* 0 means no extra bar */
static const char statussep         = ';';      /* separator between statuses */
static const int showlayoutname										= 1;        /* 0 means no layout name in the bar */			

/* vanity gaps */
static const unsigned int gappih    							= 5;       /* horiz inner gap between windows */
static const unsigned int gappiv   			 					= 5;       /* vert inner gap between windows */
static const unsigned int gappoh    							= 5;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    							= 5;       /* vert outer gap between windows and screen edge */
static const int smartgaps          							= 0;        /* 1 means no outer gap when there is only one window */

// Xresources variables
static char defaultfont[] = "TeX Gyre Heros:style=regular:weight=80:size=12:pixelsize=12:antialias=true:autohint=true";
static char dmenufont[]       = "monospace:size=10";



// static const char *fonts[]          = { "TeX Gyre Heros:style=regular" };
static const char *fonts[] = {
	defaultfont,
  "JetBrainsMono Nerd Font:style=regular:weight=180:size=12:pixelsize=14:antialias=true:autohint=true"
};

/*
static const char col_gray1[]       = "#040404";
static const char col_gray2[]       = "#161616";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#fbfbfb";
static const char col_cyan[]        = "#080808";
*/
static char normbgcolor[]           = "#040404";
static char normbordercolor[]       = "#161616";
static char normfgcolor[]           = "#fbfbfb";
static char selfgcolor[]            = "#040404";
static char selbordercolor[]        = "#fbfbfb";
static char selbgcolor[]            = "#fbfbfb";
//static const char *colors[][3]      = {
	/*               fg         	bg         	border   */
//	[SchemeNorm] = { col_gray4, 	col_gray1, 	col_gray2 		},
//	[SchemeSel]  = { col_gray1, 	col_gray4,  col_gray4  		},
//};

static char *colors[][3] = {
  /*               fg           bg           border   */
  [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
  [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

static const char *const autostart[] = {
//  "killall", "picom", NULL,
  "/home/mawi/.wz", NULL,
	"vibrant-cli", "eDP-1", "2.125", NULL,
	"picom", "-b", "--config", "/home/mawi/.config/picom/dwm-picom.conf", NULL,
	"xinput", "disable", "10" , NULL,
	"light", "-S", "20", NULL,
	"numlockx", NULL,
  NULL /* terminate */
};


/* Special Key*/
static const char *volume_up[]       		= {"/home/mawi/.config/wm-scripts/zwm-scripts/zwm-volume", 					"high" , 		"4", 	NULL	};
static const char *volume_down[]     		= {"/home/mawi/.config/wm-scripts/zwm-scripts/zwm-volume", 					"low" , 		"4", 	NULL	};
static const char *volume_toggle[]   		= {"/home/mawi/.config/wm-scripts/zwm-scripts/zwm-volume", 					"toggle", 				NULL	};
static const char *light_up[]         	= {"/home/mawi/.config/wm-scripts/zwm-scripts/zwm-brightness", 			"up", 						NULL	};
static const char *light_down[]       	= {"/home/mawi/.config/wm-scripts/zwm-scripts/zwm-brightness", 			"down", 					NULL	};
static const char *mic_toggle[]      		= {"/home/mawi/.config/wm-scripts/zwm-scripts/zwm-mic",							 									NULL	};
static const char *audio_play[]         = {"playerctl", "play-pause", NULL};
static const char *audio_stop[]       	= {"playerctl", "stop", NULL};
static const char *audio_prev[]       	= {"playerctl", "previous", NULL};
static const char *audio_next[]       	= {"playerctl", "next", NULL};


/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };



static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title1
	 */
	/* class      									instance    								title                     tags mask     isfloating   monitor */
	{ NULL,     										"ZWM_SCREENSHOTTER",       	NULL,                     0,            1,           -1 },
	{ NULL,  							          NULL,       								"Picture-in-Picture",     0,       			1,           -1 },
	{ NULL,  							          "Navigator",       					NULL,     								1 << 0,       0,           -1 },
	{ "TERMINAL_SCRATCHPAD",  			NULL,       								NULL,     								1 << 8,       1,           -1 },
	{ "Nsxiv",     								  NULL,                      	NULL,                     0,            1,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;   /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function 			layout name*/ 
	{ " 󰀁 ",      tile,	 								"TILE"			},    /* first entry is default */
	{ "  ",      NULL,  								"FLOATING"	},    /* no layout function means floating behavior */
	{ "  ",      monocle, 							"MONOCLE"		},
};


// static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };


//static const Env envs[] = {
//	/* variable 	value */
//	{ "TERMINAL",	"zt" },
//	{ "PATH", "/home/mawi/.config/wm-scripts:/usr/local/bin:/usr/bin" },
//};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "zmenu_apps" };
static const char *termcmd[]  = { "st", NULL };
static const char *zwmscreenshootcmd[] = { "zwm-screenshotter", NULL };
static const char *clipmenu[] = { "clipmenu", NULL };

/*
 * Xresources preferences to load at startup
*/
ResourcePref resources[] = {
	{ "font",               STRING,  &defaultfont },
	{ "dmenufont",          STRING,  &dmenufont },
	{ "normbgcolor",        STRING,  &normbgcolor },
	{ "normbordercolor",    STRING,  &normbordercolor },
	{ "normfgcolor",        STRING,  &normfgcolor },
	{ "selbgcolor",         STRING,  &selbgcolor },
	{ "selbordercolor",     STRING,  &selbordercolor },
	{ "selfgcolor",         STRING,  &selfgcolor },
	{ "borderpx",          	INTEGER, &borderpx },
	{ "snap",          			INTEGER, &snap },
	{ "showbar",          	INTEGER, &showbar },
	{ "topbar",          		INTEGER, &topbar },
	{ "nmaster",          	INTEGER, &nmaster },
	{ "resizehints",       	INTEGER, &resizehints },
	{ "mfact",      	 			FLOAT,   &mfact },
};




static const Key keys[] = {
	/* modifier                     		key       		 										function        				argument */
// Special KeyBindings
  {0,             										XF86XK_AudioLowerVolume,   				spawn,          				{.v = volume_down}				},
	{0,             										XF86XK_AudioMute,          				spawn,          				{.v = volume_toggle }			},
	{0,             										XF86XK_AudioRaiseVolume,   				spawn,          				{.v = volume_up}					},
	{0,				      										XF86XK_MonBrightnessUp,		 				spawn,	         				{.v = light_up}						},
	{0,				      										XF86XK_MonBrightnessDown,					spawn,	        			 	{.v = light_down}					},
	{0,				      										XF86XK_AudioMicMute,							spawn,	        			 	{.v = mic_toggle}					},
	{0,				      										XF86XK_AudioPlay,									spawn,	        			 	{.v = audio_play}					},
	{0,				      										XF86XK_AudioStop,									spawn,	        			 	{.v = audio_stop}					},
	{0,				      										XF86XK_AudioPrev,									spawn,	        			 	{.v = audio_prev}					},
	{0,				      										XF86XK_AudioNext,									spawn,	        			 	{.v = audio_next}					},
  { MODKEY,            								XK_v, 														spawn,   						    {.v = clipmenu} 					},
	{ MODKEY,            								XK_n, 														toggleview, 						{.ui = 1 << 8 } 					},
	{ MODKEY|ShiftMask,             		XK_s,      												spawn,          				{.v = zwmscreenshootcmd } },
	{ MODKEY,                       		XK_r,      												spawn,          				{.v = dmenucmd } 					},
	{ MODKEY,                      			XK_Return, 												spawn,          				{.v = termcmd }						},
	{ MODKEY,                       		XK_b,      												togglebar,      				{0} },
	{ MODKEY,                       		XK_j,      												focusstack,     				{.i = +1 } 								},
	{ MODKEY,                       		XK_k,      												focusstack,     				{.i = -1 } 								},
	{ MODKEY,                       		XK_i,      												incnmaster,     				{.i = +1 } 								},
	{ MODKEY,                       		XK_d,      												incnmaster,     				{.i = -1 } 								},
	{ MODKEY,                       		XK_h,      												setmfact,       				{.f = -0.05} 							},
	{ MODKEY,                       		XK_l,      												setmfact,       				{.f = +0.05} 							},
	{ MODKEY,                       		XK_Return, 												zoom,           				{0} 											},
	{ MODKEY,                       		XK_Tab,    												view,           				{0} 											},
	{ MODKEY|ShiftMask,            		 	XK_c,      												killclient,     				{0} 											},
	{ MODKEY,                       		XK_t,      												setlayout,      				{.v = &layouts[0]} 				},
	{ MODKEY,                       		XK_f,      												setlayout,      				{.v = &layouts[1]} 				},
	{ MODKEY,                       		XK_m,      												setlayout,      				{.v = &layouts[2]} 				},
	{ MODKEY,                       		XK_space,  												setlayout,      				{0} 											},
	{ MODKEY|ShiftMask,             		XK_space,  												togglefloating, 				{0} 											},
	{ MODKEY,                       		XK_0,      												view,           				{.ui = ~0 } 							},
	{ MODKEY|ShiftMask,             		XK_0,      												tag,            				{.ui = ~0 } 							},
	{ MODKEY,                       		XK_comma,  												focusmon,       				{.i = -1 } 								},
	{ MODKEY,                       		XK_period, 												focusmon,       				{.i = +1 } 								},
	{ MODKEY|ShiftMask,             		XK_comma,  												tagmon,         				{.i = -1 } 								},
	{ MODKEY|ShiftMask,             		XK_period, 												tagmon,         				{.i = +1 } 								},
	/* Monocle focus windows */
	{ MODKEY,                       		XK_F1,      											monoclefocus,     			{.i = 0 } 								},
	{ MODKEY,                       		XK_F2,      											monoclefocus,     			{.i = 1 } 								},
	{ MODKEY,                       		XK_F3,      											monoclefocus,     			{.i = 2 } 								},
	{ MODKEY,                       		XK_F4,      											monoclefocus,     			{.i = 3 } 								},
	{ MODKEY,                       		XK_F5,      											monoclefocus,     			{.i = 4 } 								},
	{ MODKEY,                       		XK_F6,      											monoclefocus,     			{.i = 5 } 								},
	{ MODKEY,                       		XK_F7,      											monoclefocus,     			{.i = 6 } 								},
	{ MODKEY,                       		XK_F8,      											monoclefocus,     			{.i = 7 } 								},
	{ MODKEY,                 					XK_less,      										focusstack,     				{.i = +1 } 								},
	/* vanity gaps */
	{ MODKEY|Mod1Mask,              		XK_h,      												incrgaps,       				{.i = +1 } 								},
	{ MODKEY|Mod1Mask,              		XK_l,      												incrgaps,      	 				{.i = -1 }							 	},
	{ MODKEY|Mod1Mask|ShiftMask,    		XK_h,      												incrogaps,      				{.i = +1 } 								},					
	{ MODKEY|Mod1Mask|ShiftMask,    		XK_l,      												incrogaps,      				{.i = -1 } 								},
	{ MODKEY|Mod1Mask|ControlMask,  		XK_h,      												incrigaps,      				{.i = +1 } 								},
	{ MODKEY|Mod1Mask|ControlMask,  		XK_l,      												incrigaps,      				{.i = -1 } 								},
	{ MODKEY|Mod1Mask,              		XK_0,      												togglegaps,     				{0} 											},
	{ MODKEY|Mod1Mask|ShiftMask,    		XK_0,      												defaultgaps,    				{0} 											},
	{ MODKEY|Mod1Mask,                  XK_y,      												incrihgaps,     				{.i = +1 } 								},
	{ MODKEY|Mod1Mask,                  XK_o,      												incrihgaps,     				{.i = -1 } 								},
	{ MODKEY|ControlMask,           		XK_y,      												incrivgaps,     				{.i = +1 } 								},
	{ MODKEY|ControlMask,           		XK_o,      												incrivgaps,     				{.i = -1 } 						    },
	{ MODKEY|Mod4Mask,              		XK_y,      												incrohgaps,     				{.i = +1 }                },
	{ MODKEY|Mod4Mask,              		XK_o,      												incrohgaps,     				{.i = -1 }                },
	{ MODKEY|ShiftMask,             		XK_y,      												incrovgaps,     				{.i = +1 }                },
	{ MODKEY|ShiftMask,             		XK_o,     												incrovgaps,     				{.i = -1 } 							  },

	TAGKEYS(                        		XK_1,                      0)
	TAGKEYS(                        		XK_2,                      1)
	TAGKEYS(                        		XK_3,                      2)
	TAGKEYS(                        		XK_4,                      3)
	TAGKEYS(                        		XK_5,                      4)
	TAGKEYS(                        		XK_6,                      5)
	TAGKEYS(                        		XK_7,                      6)
	TAGKEYS(                        		XK_8,                      7)
//	TAGKEYS(                        		XK_9,                      8)
	{ MODKEY|ShiftMask,             		XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, 		XK_q,      quit,           {1} }, 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
//	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
//	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
//	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

