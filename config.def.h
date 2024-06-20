#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* 外观设置 */
static const unsigned int borderpx  = 1;        /* 窗口边框的像素宽度 */
static const Gap default_gap        = {.isgap = 1, .realgap = 10, .gappx = 10}; /* 窗口间隙，包含是否启用间隙和具体的间隙大小 */
static const unsigned int snap      = 32;       /* 窗口自动吸附的像素范围 */
static const unsigned int systraypinning = 0;   /* 0: 系统托盘跟随选中的显示器，>0: 系统托盘固定在特定显示器 */
static const unsigned int systrayonleft = 0;    /* 0: 系统托盘在状态栏右侧，>0: 系统托盘在状态栏左侧 */
static const unsigned int systrayspacing = 2;   /* 系统托盘图标之间的间距 */
static const int systraypinningfailfirst = 1;   /* 1: 如果固定系统托盘失败，显示在第一个显示器上，0: 显示在最后一个显示器上 */
static const int showsystray        = 1;        /* 0: 不显示系统托盘，1: 显示系统托盘 */
static const int showbar            = 1;        /* 0: 不显示状态栏，1: 显示状态栏 */
static const int topbar             = 1;        /* 0: 状态栏在底部，1: 状态栏在顶部 */
static const Bool viewontag         = True;     /* 切换标签时自动切换视图 */
static const char *fonts[]          = {  "Sauce Code Pro Nerd Font Mono:size=14","JoyPixels:size=14:antialias=true:autohint=true" }; /* 字体设置 */
static const char dmenufont[]       = "Sauce Code Pro Nerd Font Mono:size=14"; /* dmenu 的字体 */
static const char col_gray1[]       = "#222222"; /* 背景颜色 */
static const char col_gray2[]       = "#444444"; /* 未选中窗口边框颜色 */
static const char col_gray3[]       = "#bbbbbb"; /* 字体颜色 */
static const char col_gray4[]       = "#eeeeee"; /* 选中窗口字体颜色 */
static const char col_cyan[]        = "#005577"; /* 选中窗口边框颜色 */
static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 }, /* 普通窗口的颜色设置 */
    [SchemeSel]  = { col_gray4, col_cyan,  col_cyan  }, /* 选中窗口的颜色设置 */
};

/* 标签设置 */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" }; /* 标签名称 */

/* 锁文件 */
static char lockfile[] = "/tmp/dwm.lock"; /* 锁文件路径 */

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    { "GoldenDict",                        NULL,       NULL,       1 << 6,            0,           -1 },
    { "qBittorrent",                       NULL,       NULL,       1 << 8,            0,           -1 },
    { "lx-music-desktop",                  NULL,       NULL,       1 << 2,            1,           -1 },
    { "electron-netease-cloud-music",      NULL,       NULL,       1 << 3,            1,           -1 },
    { "yesplaymusic",                      NULL,       NULL,       1 << 3,            1,           -1 },
    { "electronic-wechat",                 NULL,       NULL,       1 << 7,            0,           -1 },
    { "wps",                               NULL,       NULL,       1 << 4,            0,           -1 },
    { "svsclient.exe",                     NULL,       NULL,       1 << 4,            0,           -1 },
    { "synology-note-station-client",      NULL,       NULL,       1 << 5,            0,           -1 },
    { "firefox",                           NULL,       NULL,       1 << 1,            0,           -1 },
    { "chromium",                          NULL,       NULL,       0,                 0,           -1 },
};

/* 布局设置 */
static const float mfact     = 0.55; /* 主区域大小比例 [0.05..0.95] */
static const int nmaster     = 1;    /* 主区域的客户端数量 */
static const int resizehints = 1;    /* 1: 尊重客户端的大小提示，0: 忽略大小提示 */
static const int lockfullscreen = 1; /* 1: 全屏时强制聚焦在全屏窗口上 */

#include "gaplessgrid.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* 第一项为默认布局 */
    { "><>",      NULL },    /* 没有布局函数意味着浮动行为 */
    { "[M]",      monocle }, /* 单窗口最大化布局 */
    { "..",      gaplessgrid }, /* 无间隙网格布局 */
};

/* 按键定义 */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* 辅助宏，用于在 dwm-5.0 之前的版本中启动 shell 命令 */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks" /* 状态栏的命令 */

/* 命令 */
static const char *lockcmd[] = { "i3lock", NULL };
static const char *dmenucmd[] = { "rofi", "-no-lazy-grab", "-show", "drun", "-modi", "drun", "-theme", "/home/ken/.config/rofi/launchers/colorful/style_2.rasi", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *termcmd_st[]  = { "st", NULL };
static const char *volup[]  = { "/home/ken/github/scripts/volup.sh", NULL };
static const char *voldown[]  = { "/home/ken/github/scripts/voldown.sh", NULL };
static const char *volmute[]  = { "/home/ken/github/scripts/volmute.sh", NULL };
static const char *lightup[]  = { "/home/ken/github/scripts/lightup.sh", NULL };
static const char *lightdown[]  = { "/home/ken/github/scripts/lightdown.sh", NULL };
static const char *mic[]  = { "/home/ken/github/scripts/mic.sh", NULL };
static const char *getweather[]  = { "/home/ken/github/scripts/getweather.sh", NULL };
static const char *record_voice[]  = { "/home/ken/github/scripts/record_voice.sh", NULL };
static const char *record_video[]  = { "/home/ken/github/scripts/record_video.sh", NULL };
static const char *flameshot[]  = { "flameshot", "gui", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

/* 键盘快捷键 */
static const Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } }, /* 启动 rofi */
    { MODKEY,                       XK_l,      spawn,          {.v = lockcmd } }, /* 启动 i3lock锁 */
    { MODKEY|ShiftMask,             XK_backslash, spawn,      {.v = termcmd } },  /* 启动 alacritty */
    { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd_st } }, /* 启动 st 终端 */
    { Mod4Mask,                     XK_space,  spawn,          {.v = volmute } }, /* 静音 */
    { Mod4Mask|ShiftMask,           XK_equal,  spawn,          {.v = volup } }, /* 音量增加 */
    { Mod4Mask,                     XK_minus,  spawn,          {.v = voldown } }, /* 音量减少 */
    { Mod4Mask,                     XK_w,      spawn,          {.v = getweather } }, /* 获取天气信息 */
    { Mod4Mask,                     XK_r,      spawn,          {.v = record_voice } }, /* 录音 */
    { Mod4Mask,                     XK_v,      spawn,          {.v = record_video } }, /* 录制视频 */
    { MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } }, /* 启动 scratchpad */
    { MODKEY|ShiftMask,             XK_a,      spawn,          {.v = flameshot } }, /* 启动 flameshot 截图工具 */
    { MODKEY,                       XK_b,      togglebar,      {0} }, /* 切换状态栏显示 */
    { MODKEY,                       XK_j,      focusstack,     {.i = +1 } }, /* 切换到下一个窗口 */
    { MODKEY,                       XK_k,      focusstack,     {.i = -1 } }, /* 切换到上一个窗口 */
    { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } }, /* 增加主区域窗口数量 */
    { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } }, /* 减少主区域窗口数量 */
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} }, /* 减少主区域大小 */
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} }, /* 增加主区域大小 */
    { MODKEY,                       XK_Return, zoom,           {0} }, /* 切换主区域和堆栈区域的窗口 */
    { MODKEY,                       XK_Tab,    view,           {0} }, /* 切换到之前的标签 */
    { MODKEY|ShiftMask,             XK_c,      killclient,     {0} }, /* 关闭当前窗口 */
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, /* 切换到 tile 布局 */
    { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} }, /* 切换到浮动布局 */
    { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} }, /* 切换到 monocle 布局 */
    { MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} }, /* 切换到 gaplessgrid 布局 */
    { MODKEY|ShiftMask,             XK_f,      fullscreen,     {0} }, /* 切换全屏模式 */
    { MODKEY,                       XK_space,  setlayout,      {0} }, /* 切换布局 */
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} }, /* 切换浮动模式 */
    { MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } }, /* 向下移动窗口 */
    { MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } }, /* 向上移动窗口 */
    { MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } }, /* 向右移动窗口 */
    { MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } }, /* 向左移动窗口 */
    { MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } }, /* 增加窗口高度 */
    { MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } }, /* 减少窗口高度 */
    { MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } }, /* 增加窗口宽度 */
    { MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } }, /* 减少窗口宽度 */
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } }, /* 查看所有标签 */
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } }, /* 将窗口标记到所有标签 */
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } }, /* 切换到前一个显示器 */
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } }, /* 切换到下一个显示器 */
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } }, /* 将窗口标记到前一个显示器 */
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } }, /* 将窗口标记到下一个显示器 */
    { MODKEY,                       XK_minus,  setgaps,        {.i = -5 } }, /* 减小间隙 */
    { MODKEY,                       XK_equal,  setgaps,        {.i = +5 } }, /* 增大间隙 */
    { MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = GAP_RESET } }, /* 重置间隙 */
    { MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = GAP_TOGGLE} }, /* 切换间隙 */
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)
    { MODKEY|ShiftMask,             XK_q,      quit,           {0} }, /* 退出 dwm */
    { MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} }, /* 强制退出 dwm */
};

/* 鼠标按键定义 */
/* click 可以是 ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, 或 ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} }, /* 设置布局 */
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} }, /* 设置 monocle 布局 */
    { ClkWinTitle,          0,              Button2,        zoom,           {0} }, /* 切换主区域和堆栈区域的窗口 */
    { ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} }, /* 状态栏单击操作 */
    { ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} }, /* 状态栏中键操作 */
    { ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} }, /* 状态栏右键操作 */
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} }, /* 移动窗口 */
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} }, /* 切换浮动模式 */
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} }, /* 调整窗口大小 */
    { ClkTagBar,            0,              Button1,        view,           {0} }, /* 查看标签 */
    { ClkTagBar,            0,              Button3,        toggleview,     {0} }, /* 切换查看标签 */
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} }, /* 将窗口标记到标签 */
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} }, /* 切换窗口的标签 */
};

