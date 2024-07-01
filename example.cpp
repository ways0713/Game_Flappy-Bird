//本程序所有代码均为单逸凡本人所写，无任何抄袭或代写行为
#include<stdio.h>
#include<graphics.h>
#include<stdlib.h>
#include <time.h>

//用于记录坐标的结构体
struct pos
{
	int px;
	int py;
};

//用于记录不同难度下变化数据的结构体
struct difficulty
{
	int length;//障碍物可通过宽度
	int b_time;//障碍物生成间隔
	int speed;//小球移动速度
	int goal;//通关目标
};
//定义不同难度的数据
struct difficulty diff[4] = {
	{0, 0, 0, 0},
	{100, 120, 20, 40},//Easy
	{80, 100, 15, 30},//Average
	{80, 90, 10, 20}//Hard
};


//用于标记通过障碍物的数量
int detect;
//用于记录游戏结束的原因
int flag1;
//用于记录难度
int mode;

//碰撞检测
bool meet(int ppy, int bpx, int bpy)
{
	//超过边界检测
	if (ppy == 5 || ppy == 715)
	{
		flag1 = 2;
		return false;
	}

	//与障碍物的碰撞检测
	if (bpx == 550 || bpx == 530)
	{
		if (bpx == 530)
			detect++;
		if (ppy < bpy || ppy > bpy + diff[mode].length)
		{
			flag1 = 3;
			return false;
		}
		return true;
	}
	if (bpx == 545 || bpx == 535)
	{
		if (ppy - 7 < bpy || ppy + 7 > bpy + diff[mode].length)
		{
			flag1 = 3;
			return false;
		}
		return true;
	}
	else if (bpx == 540)
	{
		if ( ppy - 10 < bpy || ppy + 10 > bpy + diff[mode].length)
		{
			flag1 = 3;
			return false;
		}
		return true;
	}
	else
		return true;
}

//检测开始菜单的鼠标点击位置
void click_det1()
{
	MOUSEMSG m;
	while (1)
	{
		// 获取鼠标消息
		m = GetMouseMsg();
		// 检查鼠标左键是否按下
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			//Easy420, 300, 650, 350
			if (m.x >= 420 && m.x <= 650 && m.y >= 300 && m.y <= 350)
			{
				mode = 1;
				break;
			}
			//Average420, 400, 650, 450
			else if (m.x >= 420 && m.x <= 650 && m.y >= 400 && m.y <= 450)
			{
				mode = 2;
				break;
			}
			//Hard420, 500, 650, 550
			else if (m.x >= 420 && m.x <= 650 && m.y >= 500 && m.y <= 550)
			{
				mode = 3;
				break;
			}
			//Quit720, 600, 950, 650
			else if ((m.x >= 720 && m.x <= 950 && m.y >= 600 && m.y <= 650))
			{
				flag1 = 4;
				break;
			}
		}
	}
}

//打印开始菜单
void start_menu()
{
	//生成白色背景
	initgraph(1080, 720);
	setbkcolor(WHITE);
	cleardevice();

	settextcolor(BLACK);
	settextstyle(60, 0, _T("Consolas"));
	outtextxy(380, 80, _T("Flappy Bird"));

	settextstyle(40, 0, _T("Consolas"));
	outtextxy(280, 200, _T("Please select the difficulty:"));

	//难度选项打印
	outtextxy(500, 305, _T("Easy"));
	outtextxy(475, 405, _T("Average"));
	outtextxy(500, 505, _T("Hard"));
	outtextxy(800, 605, _T("Quit"));

	setlinecolor(BLACK);

	rectangle(420, 300, 650, 350);
	rectangle(420, 400, 650, 450);
	rectangle(420, 500, 650, 550);
	rectangle(720, 600, 950, 650);
	
	//检测点击情况
	click_det1();
}

//游戏主体
void game()
{
	cleardevice();

	//初始化小球位置并生成小球
	pos p;
	p.px = 540;
	p.py = 360;
	setfillcolor(BLACK);
	solidcircle(p.px, p.py, 10);

	//障碍物的结构体b
	pos b[60];
	b[0].py = time(NULL);

	//定义变量
	ExMessage msg;

	int flag = 0;
	int timee = 0;

	//上障碍物底面高度限制范围
	int lower = 40, upper = 600;
	int count = 0;

	while (meet(p.py, b[detect].px, b[detect].py) && flag1 != 4)
	{
		if (detect == 1 + diff[mode].goal)
			break;

		timee++;

		//开始批量绘图
		BeginBatchDraw();

		//显示得分
		settextcolor(BLUE);
		settextstyle(30, 0, _T("Consolas"));
		outtextxy(100, 100, _T("Current score:"));
		TCHAR s[50];
		_stprintf_s(s, _T("%d"), detect - 1);
		outtextxy(310, 100, s);

		settextstyle(30, 0, _T("Consolas"));
		outtextxy(100, 150, _T("Target score:"));
		TCHAR s1[50];
		_stprintf_s(s, _T("%d"), diff[mode].goal);
		outtextxy(290, 150, s);



		//控制小球的运动
		Sleep(diff[mode].speed);
		while (peekmessage(&msg, EX_KEY, 1)) {}

		//S键控制小球向下移动
		if (msg.vkcode == 0x53)
		{
			flag = 1;
			setfillcolor(WHITE);
			solidcircle(p.px, p.py, 10);

			p.py += 5;
			setfillcolor(BLACK);
			solidcircle(p.px, p.py, 10);
		}

		//W键控制小球向上移动
		else if (msg.vkcode == 0x57)
		{
			flag = 2;
			setfillcolor(WHITE);
			solidcircle(p.px, p.py, 10);

			p.py -= 5;
			setfillcolor(BLACK);
			solidcircle(p.px, p.py, 10);
		}

		//如果接收的是键盘上的其他按键则不影响小球的移动方向
		else
		{
			if (flag == 1)
			{
				setfillcolor(WHITE);
				solidcircle(p.px, p.py, 10);

				p.py += 5;
				setfillcolor(BLACK);
				solidcircle(p.px, p.py, 10);
			}
			else if (flag == 2)
			{
				setfillcolor(WHITE);
				solidcircle(p.px, p.py, 10);

				p.py -= 5;
				setfillcolor(BLACK);
				solidcircle(p.px, p.py, 10);
			}
		}


		//生成随机障碍
		if (timee == diff[mode].b_time)
		{
			timee = 0;
			//生成障碍上端的位置
			srand(b[count].py);
			count++;
			b[count].py = (rand() % (upper - lower + 1)) + lower;
			b[count].px = 1070;

			//生成矩形
			setfillcolor(RED);
			solidrectangle(b[count].px, 0, b[count].px + 10, b[count].py);
			solidrectangle(b[count].px, b[count].py + diff[mode].length, b[count].px + 10, 720);
		}

		int i = 1;
		//控制障碍物矩形的移动
		for (i; i <= count; i++)
		{
			if (b[i].px < -10)
				i++;

			setfillcolor(WHITE);
			solidrectangle(b[i].px, 0, b[i].px + 10, b[i].py);
			solidrectangle(b[i].px, b[i].py + diff[mode].length, b[i].px + 10, 720);

			b[i].px -= 5;
			setfillcolor(RED);
			solidrectangle(b[i].px, 0, b[i].px + 10, b[i].py);
			solidrectangle(b[i].px, b[i].py + diff[mode].length, b[i].px + 10, 720);
		}
		//结束批量绘图
		EndBatchDraw();
	}
}

//打印结束菜单
void end_menu()
{
	cleardevice();
	settextcolor(BLUE);
	if (flag1 == 1)
	{
		settextstyle(40, 0, _T("Consolas"));
		outtextxy(400, 200, _T("Conguatulations!"));
		outtextxy(410, 250, _T("Final Score:"));
		TCHAR s[50];
		_stprintf_s(s, _T("%d"), detect - 1);
		outtextxy(670, 250, s);
	}
	else if (flag1 == 2)
	{
		settextstyle(40, 0, _T("Consolas"));
		outtextxy(355, 200, _T("You are out of limit!"));
		outtextxy(410, 250, _T("Final Score:"));
		TCHAR s[50];
		_stprintf_s(s, _T("%d"), detect - 1);
		outtextxy(670, 250, s);
	}
	else if (flag1 == 3)
	{
		settextstyle(40, 0, _T("Consolas"));
		outtextxy(360, 200, _T("You hit the barrier!"));
		outtextxy(410, 250, _T("Final Score:"));
		TCHAR s[50];
		_stprintf_s(s, _T("%d"), detect - 1);
		outtextxy(670, 250, s);
	}

	if (flag1 != 4)
	{
		settextcolor(BLACK);
		settextstyle(40, 0, _T("Consolas"));
		outtextxy(465, 355, _T("Try Again"));
		setlinecolor(BLACK);
		rectangle(410, 350, 690, 400);

		settextstyle(40, 0, _T("Consolas"));
		outtextxy(510, 455, _T("Quit"));
		setlinecolor(BLACK);
		rectangle(410, 450, 690, 500);
	}

}

//检测结束菜单的鼠标点击位置
bool click_det2()
{
	if (flag1 == 4)
		return false;
	MOUSEMSG m;
	while (1)
	{
		// 获取鼠标消息
		m = GetMouseMsg();
		// 检查鼠标左键是否按下
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			//若点到第一个矩形框内则重新开始游戏
			if (m.x >= 410 && m.x <= 690 && m.y >= 350 && m.y <= 400)
				return true;
			//若点到第二个矩形框则结束游戏
			else if(m.x >= 410 && m.x <= 690 && m.y >= 450 && m.y <= 500)
				return false;
		}
	}
}

int main()
{
	do
	{
		detect = 1;
		flag1 = 1;
		start_menu();
		game();
		end_menu();
	}
	while (click_det2());

	closegraph();
	return 0;
}