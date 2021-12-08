/*
	TobbySystem Windows Kernel
	v1.0 
*/
/*
	2021-12-8
	By TobbySQL
*/

#include <bits/stdc++.h> 
#include <windows.h> //system()
#include <conio.h> //_kbhit(),getch()
#include <iostream> 
#include <fstream> 
#include <stdlib.h> 
#include <string> 
#include <cstring> 
#include <cmath> 
#include <thread> //���߳� 
#include <chrono>

#define _zs 0
#define _ys 1
#define _zx 2
#define _yx 3
#define _s 4
#define _h 5
#define _jd 6

using namespace std;


const int defult_window_width = 60;
const int defult_window_height = 20;

struct Arrow{
	int x,y;//��� 
};

struct Window{
	int tx,ty;//���Ͻ�������
	int width,height;//��߶� 
	string title; 
	string load_type;
	bool maxsize;
}windows[1001];
int num_windows;
string focus_window="Welcome";//Unknow
string last_focus="Unkown";//�ϸ����� 

string cmap="OOOO|-X";//������������
void add_c_map(){
//	cmap[0]='��';//0,_zs
//	cmap[1]='��';//1,_ys
//	cmap[2]='��';//2,_zx
//	cmap[3]='��';//3,_yx
//	cmap[4]='��';//4,_s
//	cmap[5]='��';//5,_h
}

string ReadMe[31]; 
int num_ReadMe;
void add_Read_Me(){
	int p=0;
	ReadMe[++p]="Tobby System Windows Kernel";
	ReadMe[++p]="Tobby System �����ں�";
	ReadMe[++p]="���㴰��׷��:�Զ�׷�ٽ��㴰��,���㴰�����Ͻ���";
	ReadMe[++p]="X��Ϊ���";
	ReadMe[++p]="��С��:F2 ���:F3 �ر�:F4";
	ReadMe[++p]="����:TobbySQL";
	//�ڴ�׷������ 
	num_ReadMe=p;
}

void HideCursor(int x) //�������
{
    CONSOLE_CURSOR_INFO cursor_info = {1, x}; //��ߵ�0�����겻�ɼ�
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void gotoxy(int x, int y) //���꺯��
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}

Arrow getxy(){
	HANDLE hStdout;
	CONSOLE_SCREEN_BUFFER_INFO pBuffer; 
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	GetConsoleScreenBufferInfo(hStdout, &pBuffer); 
	Arrow tmp;
	tmp.x=pBuffer.dwCursorPosition.X;
	tmp.y=pBuffer.dwCursorPosition.Y;
	return tmp; 
}

Arrow getWindowxy(){
	COORD info = GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));
	int Tx=info.X;
	int Ty=info.Y;
	Arrow tmp;
//	tmp.x=Tx;
//	tmp.y=Ty;
	tmp.x=100;
	tmp.y=20;
	return tmp;
}

void UpdateFocusWindow(string window_name){
	int wt,ht,tx,ty; 
	for(int i=0;i<num_windows;i++){
		if(windows[i].title==window_name){
			wt=windows[i].width;
			ht=windows[i].height;
			tx=windows[i].tx;
			ty=windows[i].ty;
			break;
		}//��ȡ������Ϣ 
	}
	Arrow Now=getxy();
	gotoxy(tx-1,ty);
	printf("\b");
	cout<<_jd;
	//���½��㴰��
	if(focus_window!="Unknow"){//ȷ�����ڽ��㴰��,��Ȼ�޷�ɾ�� 
		int _tx,_ty; 
		for(int i=0;i<num_windows;i++){
			if(windows[i].title==focus_window){
				_tx=windows[i].tx;
				_ty=windows[i].ty;
				break;
			}//��ȡ�ɽ��㴰�ڴ�����Ϣ 
		} 
		gotoxy(_tx-1,_ty);
		printf("\b");
		cout<<_zs;
		//ɾ���ɽ��㴰�� 	
	}
	gotoxy(Now.x,Now.y);
	//����ԭ�� 
}

void printWindow(int tx,int ty,int width,int height,string window_name){
	gotoxy(tx,ty);
	int len=window_name.size();
	int st=(height+len)>>1;
	int end=st+len; 
	for(int y=1;y<=height;y++){
		for(int x=1;x<=width;x++){
			if(y==1){
				if(x==st){
					cout<<window_name;
					x=end;//��ӡ���� 
				}else if(x==1){
					cout<<cmap[_zs];
				}else if(x==width){
					cout<<cmap[_ys];
				}else{
					cout<<cmap[_h];
				}
			}
			if(x==1){
				if(y==1) continue;
				if(y==height){
					cout<<cmap[_ys];
				}else{
					cout<<cmap[_s];
				}
			}
			if(x==width){
				if(y==1) continue;
				if(y==height){
					printf("\b");//ɾȥһ��
					cout<<cmap[_yx];
				}else{
					printf("\b");//ɾȥһ�� 
					cout<<cmap[_s];
				}
			}
			if(y==height){
				if(x==1) continue;
				if(x==width) continue;
				cout<<cmap[_h];
			}
			if(x!=width&&y!=height&&x!=1&&y!=1){
				cout<<" ";
			}
		} 
		cout<<endl;
	}
	bool Check_Duplicate=false;
	for(int i=1;i<=num_windows;i++){
		if(window_name==windows[i].title){
			Check_Duplicate=true;
			break;
		}
	} 
	if(Check_Duplicate) return;//���ظ����ڣ������洰����Ϣ
	num_windows++; 
	windows[num_windows].title=window_name;
	windows[num_windows].tx=tx+1;
	windows[num_windows].ty=ty+1;//�Ѵ�����Ϣ������ 
	windows[num_windows].width=width-2;
	windows[num_windows].height=height-2;
	UpdateFocusWindow(window_name);//���½��㴰��
}

void Load_ReadMe(int wt,int ht,int tx,int ty){
//	printf("!%d %d\n",tx,ty); 
	Arrow Now=getxy();
	gotoxy(tx,ty);
	for(int i=1;i<=num_ReadMe;i++){
//		printf("!%d %d\n",tx,ty+i-1);
		int len=ReadMe[i].size();
		gotoxy(tx,ty+i-1);
		Sleep(100);
		cout<<ReadMe[i];
		Arrow tmp=getxy();
		gotoxy(tmp.x,tmp.y+len);
		for(int i=1;i<=len;i++){
			printf("\b");//ɾ�������������Ŀո� 
		}
	} 
	gotoxy(Now.x,Now.y);
} 

void loadWindow(string window_name,string load_type){//���ش������� 
	int wt,ht,tx,ty; 
	int num;
	for(int i=1;i<=num_windows;i++){
		if(windows[i].title==window_name){
			wt=windows[i].width;
			ht=windows[i].height;
			tx=windows[i].tx;
			ty=windows[i].ty;
			num=i;
			break;
		}//��ȡ������Ϣ 
	}
//	printf("!%d %d\n",tx,ty);
	if(load_type=="ReadMe"){
		windows[num].load_type="ReadMe";
		windows[num].maxsize=false;
		Load_ReadMe(wt,ht,tx,ty); 
	}
}

void init(){ 
	add_c_map(); 
	add_Read_Me();
	printWindow(0,0,defult_window_width,defult_window_height,"Welcome");
	loadWindow("Welcome","ReadMe");
}

void Minsize(){
	if(focus_window=="Unknow"){
		if(last_focus=="Unknow"){
			gotoxy(0,0);
			printf("Error:No Windows Opened!\n");
			exit(0);
		}
		int wt,ht,tx,ty;
		string lt;
		for(int i=1;i<=num_windows;i++){
			if(windows[i].title==last_focus){
				wt=windows[i].width;
				ht=windows[i].height;
				tx=windows[i].tx;
				ty=windows[i].ty;
				lt=windows[i].load_type;
				break;
			}//��ȡ������Ϣ 
		}
		printWindow(tx-1,ty-1,wt+2,ht+2,last_focus);
		loadWindow(last_focus,lt);
		focus_window=last_focus;
		last_focus="Unknow";
	}else{
		last_focus=focus_window;
		focus_window="Unknow";
		system("cls");
	}
}

void Maxsize(){
	if(focus_window!="Unknow"){
		int wt,ht,tx,ty;
		string lt;
		bool ms;
		int num;
		for(int i=1;i<=num_windows;i++){
			if(windows[i].title==focus_window){
				wt=windows[i].width;
				ht=windows[i].height;
				tx=windows[i].tx;
				ty=windows[i].ty;
				lt=windows[i].load_type;
				ms=windows[i].maxsize;
				num=i;
				break;
			}//��ȡ������Ϣ 
		}
		system("cls");
		if(ms){
			printWindow(tx-1,ty-1,wt+2,ht+2,focus_window);
			loadWindow(focus_window,lt);
			windows[num].maxsize=false;
		}else{
			Arrow tmp=getWindowxy();
			printWindow(0,0,tmp.x,tmp.y,focus_window);
			loadWindow(focus_window,lt);
			windows[num].maxsize=true;
		}
	}else{
		gotoxy(0,0);
		printf("Error:No Windows Opened!\n");
		exit(0);
	}
}

void Close(){
	if(focus_window!="Unknow"){
		int wt,ht,tx,ty;
		string lt;
		bool ms;
		int num;
		for(int i=1;i<=num_windows;i++){
			if(windows[i].title==focus_window){
				wt=windows[i].width;
				ht=windows[i].height;
				tx=windows[i].tx;
				ty=windows[i].ty;
				lt=windows[i].load_type;
				ms=windows[i].maxsize;
				num=i;
				break;
			}//��ȡ������Ϣ 
		}
		focus_window=last_focus;
		last_focus="Unknow";
		windows[num]=windows[num_windows+1];//ɾ������ 
		system("cls");
	}else{
		gotoxy(0,0);
		printf("Error:No Windows Opened!\n");
		exit(0);
	}
}

void Input_Scan(){
	while(1){
		if(_kbhit()){
			int ch=getch();
			if(ch==60){
				Minsize();
			}
			if(ch==61){
				Maxsize();
			}
			if(ch==62){
				Close();
			}
		}
	}
}

int main(){
	init();
	Input_Scan();
	return 0;
} 
