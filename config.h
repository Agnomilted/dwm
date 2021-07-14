/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 10;       /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "xft:Noto Sans:size=10" }; //{ "monospace:size=10" };
static const char dmenufont[]       = "xft:Noto Sans:size=10";
static const char col_gray1[]       = "#2f2f2f"; //"#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char white[] = "#ffffff";
static const char light_gray[] = "#ebdbb2";
static const char bg0[] = "#2f2f2f";
static const char bg1[] = "#504945";
static const char bg2[] = "#7c6f64";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { light_gray, bg0, bg0 },
	[SchemeSel]  = { light_gray, bg1,  bg1  },
};

/* tagging */
static const char *tags[] = { "Main", "Web", "3", "4", "5" }; //{ "1", "2", "3", "4", "5", "6", "7", "8", "9" };


static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Firefox",  NULL,       NULL,       2,            0,           -1 },
	{ "krita",    NULL,       NULL,       NULL,         1,           -1 },

//	{ "Tor Browser",  NULL,   NULL,       2,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "T",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
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
static const char *dmenucmd[] = 
{ "dmenu_run", "-b", "-m", dmenumon, "-fn", dmenufont, "-nb", bg0, "-nf", bg2, "-sb", bg1, "-sf", white, NULL };
static const char *firefox[]  = { "firefox", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *screenshot[] = { "scrot", NULL };
static const char *lock[] = { "slock", NULL };
static const char* tor[]       = { "/home/agnom/.tor", NULL };
static const char* setvolume[] = {"setvolume", NULL};
static const char* ranger[] = {"st", "-e", "ranger", "/home/agnom", NULL };
static const char* calendar[] = { "gnome-calendar", NULL};
static const char* rofimenu[] = { "rofi", "-show", "drun", NULL };


static void
sus_getvolume()
{
	system("herbe \"$(amixer | grep 'Playback.*\\%' | sed 's/^.*[1234567890] \/\/')\" &");
}
static void
sus_raisevolume()
{
	const char* incvolume = "amixer sset Master unmute 10%+";
	system(incvolume);
	sus_getvolume();
}
static void
sus_lowervolume()
{
	const char* decvolume = "amixer sset Master unmute 10%-";
	sus_getvolume();
	system(decvolume);
}
static void
sus_mute()
{
	const char* mute = "amixer sset Master mute";
	system(mute);
	sus_getvolume();
	
}
static void
sus_exit()
{
	FILE* prompt1 = popen("echo 'logout\npoweroff\nreboot' | rofi -dmenu -p 'Logout, poweroff or reboot?'", "r");
	char amogus[128];
	char amogus2[256];
	fscanf(prompt1, "%s", amogus);
	fclose(prompt1);
	if(!strcmp(amogus, "logout")){
		quit(NULL);
	}
	else if(!strcmp(amogus, "poweroff")){
		system("rofi -dmenu -password -p 'Please enter your user password' | sudo -S poweroff");
	}
	else if(!strcmp(amogus, "reboot")){
		system("rofi -dmenu -password -p 'Please enter your user password' | sudo -S reboot");
	}
}

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_l,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_h,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_k,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_j,      incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ Mod1Mask,                     XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[3]} }, /*XK_u*/
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 10  } },
        { MODKEY|ShiftMask,             XK_Print,  spawn,          {.v = screenshot } },	
	{ MODKEY|ShiftMask,		XK_Escape, spawn,          {.v = lock } },
	{ MODKEY|ShiftMask,		XK_Up,     sus_raisevolume, {0} },
	{ MODKEY|ShiftMask,		XK_Down,   sus_lowervolume, {0} },
	{ 0,				0x1008ff13, sus_raisevolume, {0}}, /*raise volume*/
	{ 0,				0x1008ff11, sus_lowervolume, {0}}, /*lower volume*/
	{ 0,				0x1008ff12, sus_mute,        {0}},
	{ MODKEY|ShiftMask,             XK_t,      spawn,          {.v = tor } },
	{ MODKEY,             	        XK_r,      spawn,          {.v = ranger } },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          {.v = firefox } },
	
	{ MODKEY,                       XK_q,      spawn,          {.v = termcmd } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,		XK_v,      spawn,          {.v = setvolume} },
//	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      sus_exit,       {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
//	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkStatusText,        0,              Button3,        spawn,          {.v = calendar } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkRootWin,		0,		Button3,	spawn,		{.v = rofimenu } },
};

