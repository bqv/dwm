/* See LICENSE file for copyright and license details. */

/* appearance */
static const char font[]            = "fixed";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#CCCCCC";
static const char selbordercolor[]  = "#222222";
static const char selbgcolor[]      = "#333333";
static const char selfgcolor[]      = "#2299CC";
static const unsigned int bottompx  = 16;       /* bottom pixel offset */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const Bool showsystray       = True;     /* False means no systray */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
static const char *tags[] = { "tty", "net", "wrk", "av", "im", "mon", "pin", "msc", "|" };
static const int   ilts[] = {0,  0,     2,     0,    1,    0,     0,     2,     1,   1  };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Skype",    NULL,       NULL,       1 << 4,       True,        -1 },
	{ "MPlayer",  NULL,       NULL,       1 << 3,       True,        -1 },
	{ "Emacs",    NULL,       NULL,       1 << 2,       False,       -1 },
	{ "Chromium", NULL,       NULL,       1 << 1,       False,       -1 },
	{ "Urxvt",    NULL,       NULL,       1 << 0,       True,        -1 },
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */
static const int layoutaxis[] = {
	1,    /* layout axis: 1 = x, 2 = y; negative values mirror the layout, setting the master area to the right / bottom instead of left / top */
	2,    /* master axis: 1 = x (from left to right), 2 = y (from top to bottom), 3 = z (monocle) */
	2,    /* stack axis:  1 = x (from left to right), 2 = y (from top to bottom), 3 = z (monocle) */
};

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "+++",      grid },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
};

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
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *webcmd[]   = { "chromium", NULL };
static const char *termcmd[]  = { "urxvt", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_c,      spawn,          {.v = webcmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Down,   moveresize,     {.v = (int []){ 0, 25, 0, 0 }}},
	{ MODKEY,                       XK_Up,     moveresize,     {.v = (int []){ 0, -25, 0, 0 }}},
	{ MODKEY,                       XK_Right,  moveresize,     {.v = (int []){ 25, 0, 0, 0 }}},
	{ MODKEY,                       XK_Left,   moveresize,     {.v = (int []){ -25, 0, 0, 0 }}},
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = (int []){ 0, 0, 0, 25 }}},
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = (int []){ 0, 0, 0, -25 }}},
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = (int []){ 0, 0, 25, 0 }}},
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = (int []){ 0, 0, -25, 0 }}},
	{ MODKEY|ShiftMask,             XK_m,      togglemaximize,        {0} },
	{ MODKEY|ShiftMask,             XK_v,      toggleverticalmax,     {0} },
	{ MODKEY|ShiftMask,             XK_h,      togglehorizontalmax,   {0} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, // Tiled
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} }, // Floating
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} }, // Monocle
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} }, // Grid
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ ControlMask,					0x1008ff13, incvol,        {.v = "Master", .i = +1 } },
	{ ControlMask,					0x1008ff11, incvol,        {.v = "Master", .i = -1 } },
	{ ShiftMask,                    0x1008ff13, incvol,        {.v = "Master", .i = +2 } },
	{ ShiftMask,                    0x1008ff11, incvol,        {.v = "Master", .i = -2 } },
	{ 0,							0x1008ff13, incvol,        {.v = "Master", .i = +4 } },
	{ 0,                            0x1008ff11, incvol,        {.v = "Master", .i = -4 } },
    { ShiftMask|ControlMask,        0x1008ff12, mutevol,       {.v = "Master", .i =  1 } }, /* definite unmute */
	{ ShiftMask,					0x1008ff12, mutevol,       {.v = "Master", .i =  0 } }, /* definite mute */
	{ 0,                            0x1008ff12, mutevol,       {.v = "Master", .i = -1 } }, /* toggle mute */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ControlMask,           XK_t,      rotatelayoutaxis, {.i = 0} },    /* 0 = layout axis */
	{ MODKEY|ControlMask,           XK_Tab,    rotatelayoutaxis, {.i = 1} },    /* 1 = master axis */
	{ MODKEY|ControlMask|ShiftMask, XK_Tab,    rotatelayoutaxis, {.i = 2} },    /* 2 = stack axis */
	{ MODKEY|ControlMask,           XK_Return, mirrorlayout,     {0} },
	{ MODKEY|ControlMask,           XK_h,      shiftmastersplit, {.i = -1} },   /* reduce the number of tiled clients in the master area */
	{ MODKEY|ControlMask,           XK_l,      shiftmastersplit, {.i = +1} },   /* increase the number of tiled clients in the master area */
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_r,      restart,        {0} },
	{ MODKEY|ShiftMask,             XK_x,      execute,        {.v = dmenucmd } },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

