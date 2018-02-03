/* See LICENSE file for copyright and license details. */
#include "movestack.c"

/* appearance */
static const char *fonts[] = {
  "FontAwesome:size=8",
  "Inconsolata:size=8",
};
static const char dmenufont[]       = "Inconsolata:size=10";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#005577";
static const char selbgcolor[]      = "#005577";
static const char selfgcolor[]      = "#eeeeee";
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6",  "8", "9", "0"};

#define NUMCOLORS         8
static const char colors[NUMCOLORS][MAXCOLORS][8] = {
  // border   foreground background
  { "#000033", "#dddddd", "#000033" },  // normal 1
  { "#000088", "#ffffff", "#000088" },  // selected 2
  { "#ff0000", "#000000", "#ffff00" },  // urgent/warning  (black on yellow) 3
  { "#ff0000", "#ffffff", "#ff0000" },  // error (white on red) 4 

  { "#ff0000", "#000000", "#ff0000" },  // red on black 5 (really bad)
  { "#000000", "#ff0000", "#000000" },  // black on red 6 (really bad)
  { "#00FF00", "#4169e1",  "#00FF00"},  // blue on green 7 (really good)
  { "#000033", "#ff0000", "#000033" },  // normal 8 (normal)

  // TODO add in more colors here
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
/* #define MODKEY Mod4Mask */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL }; 
static const char *firefoxcmd[]  = { "firefox", NULL }; 
static const char *emacscmd[]  = { "emacsclient", "-c", NULL }; 
static const char *sleepcmd[] = {"sudo", "/usr/sbin/s2ram", NULL};
static const char *lockcmd[] = {"slock", NULL};
static const char *scrotcmd[] = {"/home/dieraca/.Suckless/suckless-dwm-restivo/scrotscript", NULL};
// background

static Key keys[] = {
  /* modifier                     key        function        argument */

  // mouse addition/removal -- DO NOT TOUCH; segfaults 
  // TODO fix
	{ MODKEY|ControlMask,                       XK_x,      toggle_cursor,  {0} },

  // utilities and such commands
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_t,      spawn,          {.v = emacscmd} },
	{ MODKEY,                       XK_q,      spawn,          {.v = firefoxcmd} },
  { MODKEY|ControlMask,           XK_semicolon, spawn,      {.v = sleepcmd}},

  // TODO add in brightnessctl commands



	{ MODKEY,                       XK_l,      spawn,          {.v = lockcmd} },
	{ MODKEY,                       XK_p,      spawn,          {.v = scrotcmd} },



  // remove top bar
	{ MODKEY|ShiftMask,             XK_f,      togglebar,      {0} },

  // switch focus
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },

	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_p,      incnmaster,     {.i = -1 } },

	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
  { MODKEY|ShiftMask,             XK_v,      movestack,      {.i = +1 } },
  { MODKEY|ShiftMask,             XK_w,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_x,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_b,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },


  //sound
  // UNMUTE
	{ MODKEY|ControlMask,           XK_Up,   volctrl,        {.i = 1}},
    // MUTE
	{ MODKEY|ControlMask,           XK_Down,    volctrl,        {.i = 0}},
    // decrese
	{ MODKEY|ControlMask,           XK_Left,      volctrl,        {.i = 3}},
    // increase
	{ MODKEY|ControlMask,           XK_Right,    volctrl,        {.i = 2}},


  // tagging
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

  // close xserver
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
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

