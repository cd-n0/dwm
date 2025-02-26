/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "mono:size=10", "JoyPixels:size=8" };
static const char dmenufont[]       = "mono:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *    WM_CLASS(STRING) = instance, class
     *    WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    { "Gimp",     NULL,       NULL,       0,            1,           -1 },
    { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
    { "steam",    NULL,       NULL,       1 << 7,       0,            0 },
    { "obs",      NULL,       NULL,       0,            0,            1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "TTT",      bstack },
    { "H[]",      deck },
    { "[M]",      monocle },
    { "HHH",      grid },
    { "###",      nrowgrid },
    { "===",      bstackhoriz },
    { "---",      horizgrid },
    { "[@]",      spiral },
    { "[\\]",     dwindle },
    { ":::",      gaplessgrid },
    { "|M|",      centeredmaster },
    { ">M>",      centeredfloatingmaster },
    { "><>",      NULL },    /* no layout function means floating behavior */
    { NULL,       NULL },
};

static const int default_monitor_layouts[] = {
    0, /* Monitor 0 */
    1, /* Monitor 1 */
    /* ... */
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
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };
#define TERMINAL "st"

static const Key keys[] = {
    /* modifier                     key                 function        argument */
    { MODKEY,                       XK_d,               spawn,          {.v = dmenucmd } },
    { MODKEY,                       XK_Return,          spawn,          {.v = termcmd } },
    { MODKEY,                       XK_b,               togglebar,      {0} },
    { MODKEY,                       XK_j,               focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,               focusstack,     {.i = -1 } },
    { MODKEY,                       XK_o,               incnmaster,     {.i = +1 } },
    { MODKEY|ShiftMask,             XK_o,               incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_h,               setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,               setmfact,       {.f = +0.05} },
    { MODKEY,                       XK_space,           zoom,           {0} },
    { MODKEY,                       XK_Tab,             view,           {0} },
    { MODKEY,                       XK_q,               killclient,     {0} },
    { MODKEY|ControlMask,           XK_space,           setlayout,      {0} },
    { MODKEY|ShiftMask,             XK_space,           togglefloating, {0} },
    { MODKEY,                       XK_0,               view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,               tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,           focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period,          focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,           tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period,          tagmon,         {.i = +1 } },
    { MODKEY,                       XK_s,               togglesticky,   {0} },
    TAGKEYS(                        XK_1,                               0)
    TAGKEYS(                        XK_2,                               1)
    TAGKEYS(                        XK_3,                               2)
    TAGKEYS(                        XK_4,                               3)
    TAGKEYS(                        XK_5,                               4)
    TAGKEYS(                        XK_6,                               5)
    TAGKEYS(                        XK_7,                               6)
    TAGKEYS(                        XK_8,                               7)
    TAGKEYS(                        XK_9,                               8)

    /* Vanity gaps */
    { MODKEY,                       XK_t,               setlayout,      {.v = &layouts[0]} },
    { MODKEY|ShiftMask,             XK_t,               setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       XK_y,               setlayout,      {.v = &layouts[2]} },
    { MODKEY|ShiftMask,             XK_y,               setlayout,      {.v = &layouts[3]} },
    { MODKEY,                       XK_u,               setlayout,      {.v = &layouts[4]} },
    { MODKEY|ShiftMask,             XK_u,               setlayout,      {.v = &layouts[5]} },
    { MODKEY,                       XK_i,               setlayout,      {.v = &layouts[6]} },
    { MODKEY|ShiftMask,             XK_i,               setlayout,      {.v = &layouts[7]} },
    { MODKEY,                       XK_a,               togglegaps,     {0} },
    { MODKEY|ShiftMask,             XK_a,               defaultgaps,    {0} },
    { MODKEY,                       XK_z,               incrgaps,       {.i = +3 } },
    { MODKEY,                       XK_x,               incrgaps,       {.i = -3 } },

    /* Shift-tools */
    { MODKEY,                       XK_semicolon,       shiftviewclients, { .i = +1 } },
    { MODKEY,                       XK_g,               shiftviewclients, { .i = -1 } },


    /* Actual fullscreen */
    { MODKEY,                       XK_f,               togglefullscr,  {0} },

    /* Application keys */
    { MODKEY,                       XK_w,               spawn,          SHCMD("$BROWSER")},
    { MODKEY|ShiftMask,             XK_w,               spawn,          SHCMD(TERMINAL " -e nmtui")},
    { MODKEY,                       XK_KP_Add,          spawn,          SHCMD("boomer")},
    { MODKEY|ShiftMask,             XK_q,               spawn,          SHCMD("sysact") },
    { ShiftMask,                    XK_Pause,           spawn,          SHCMD("remaps") },
    { MODKEY|ShiftMask,             XK_d,               spawn,          SHCMD("passmenu") },
    { MODKEY,                       XK_grave,           spawn,          SHCMD("dmenuunicode") },
    { MODKEY,                       XK_BackSpace,       spawn,          SHCMD("maim -s | xclip -selection clipboard -t image/png") },
    { MODKEY,                       XK_r,               spawn,          SHCMD(TERMINAL " -e lfub")},
    /* Function keys */
    { MODKEY,                       XK_F1,              spawn,          SHCMD("dmenumount") },
    { MODKEY,                       XK_F2,              spawn,          SHCMD("dmenuumount") },
    { MODKEY,                       XK_F3,              spawn,          SHCMD("displayselect") },
    { MODKEY,                       XK_F4,              spawn,          SHCMD(TERMINAL " -e pulsemixer") },
    { MODKEY,                       XK_F5,              xrdb,           {.v = NULL } },
    { MODKEY,                       XK_F6,              spawn,          SHCMD("") },
    /* { MODKEY,                       XK_F7,              spawn,          SHCMD("") }, */
    /* { MODKEY,                       XK_F8,              spawn,          SHCMD("") }, */
    /* { MODKEY,                       XK_F9,              spawn,          SHCMD("") }, */
    /* { MODKEY,                       XK_F10,             spawn,          SHCMD("") }, */
    { MODKEY,                       XK_F11,             spawn,          SHCMD("mpv --untimed --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
    /* { MODKEY,                       XK_F12,             spawn,          SHCMD("") }, */
    /* music player */
    { MODKEY,                       XK_m,               spawn,          SHCMD(TERMINAL " -e ncmpcpp-ueberzug") },
    { MODKEY|ShiftMask,             XK_m,               spawn,          SHCMD("pamixer -t") },
    { MODKEY,                       XK_apostrophe,      spawn,          SHCMD("mpc prev") },
    { MODKEY|ShiftMask,             XK_apostrophe,      spawn,          SHCMD("mpc seek 0%") },
    { MODKEY,                       XK_backslash,       spawn,          SHCMD("mpc next") },
    { MODKEY|ShiftMask,             XK_backslash,       spawn,          SHCMD("mpc repeat") },
    { MODKEY|ControlMask,           XK_backslash,       spawn,          SHCMD("mpc single") },
    { MODKEY,                       XK_p,               spawn,          SHCMD("mpc toggle") },
    { MODKEY|ShiftMask,             XK_p,               spawn,          SHCMD("mpc pause ; pauseallmpv") },
    { MODKEY,                       XK_bracketleft,     spawn,          SHCMD("mpc seek -10") },
    { MODKEY|ShiftMask,             XK_bracketleft,     spawn,          SHCMD("mpc seek -60") },
    { MODKEY,                       XK_bracketright,    spawn,          SHCMD("mpc seek +10") },
    { MODKEY|ShiftMask,             XK_bracketright,    spawn,          SHCMD("mpc seek +60") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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
