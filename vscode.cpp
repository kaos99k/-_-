/* * / //=================================================================================
	vscode && mingW64 :: http://kaos99k.co.uk/ :: kaos99k.
/* * / //---------------------------------------------------------------------------------
	* d3d.h 	- a D3D class thats class.
	* mem.h 	- memory utils, tings, tweaks and shiz.
	* wndProc 	- esp goes here.
	* main 		- program entry point, initiation
					and other bits of scratch.

TODO:
	* scratch my head some more?..

/* */ //---------------------------------------------------------------------------------
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define REDRAW 1
#include "windows.h" 
#include "stdio.h"
#include "winuser.h"
#include "math.h"

#include "wingdi.h" //#pragma comment(lib, "GDI32")

int tick=0;

/* * / //=================================================================================
     #include "d3d.h" // kaos99k
/* */ //---------------------------------------------------------------------------------
#include "d3d9.h" // #pragma comment(lib, "d3d9.lib")
#include "d3dx9.h" // #pragma comment(lib, "d3dx9.lib")
#include "dwmapi.h" // #pragma comment(lib, "dwmapi.lib")
#include "d3d9types.h"
/* */ //  //-----------------------------------------------------------------------------
class D3D{
private:
	IDirect3D9Ex *d3d; IDirect3DDevice9Ex *d3ddev;
	LPD3DXSPRITE sprite; ID3DXFont *dfont; ID3DXLine *dline;
public: 
/* */ //  //-----------------------------------------------------------------------------
	int init( HWND hwnd ){
		BOOL dwm=false; DWORD AA=0; MARGINS margin={-1,-1,-1,-1};
		DwmExtendFrameIntoClientArea(hwnd,&margin); DwmIsCompositionEnabled(&dwm);

		Direct3DCreate9Ex( D3D_SDK_VERSION, &d3d );
		D3DPRESENT_PARAMETERS pp; ZeroMemory(&pp, sizeof(pp));
		pp.Windowed=true; pp.SwapEffect=D3DSWAPEFFECT_DISCARD;
		pp.BackBufferFormat=D3DFMT_A8R8G8B8; pp.MultiSampleType=D3DMULTISAMPLE_NONE;
		pp.hDeviceWindow=hwnd;

		d3d->CheckDeviceMultiSampleType( 0, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, 
		true, D3DMULTISAMPLE_NONMASKABLE, &AA ); pp.MultiSampleQuality = AA -1;
		pp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE; 

		d3d->CreateDeviceEx( 0, D3DDEVTYPE_HAL, hwnd, 
			D3DCREATE_MIXED_VERTEXPROCESSING, &pp, 0, &d3ddev );

		D3DXCreateLine( d3ddev, &dline ); dline->Begin();
		D3DXCreateSprite( d3ddev, &sprite ); int s=16; 
		D3DXCreateFont( d3ddev,s,s*0.5,0,0,0,0,0,0,0,(LPCSTR)"Calibri",&dfont ); 
		return 0; }

/* */ //  //-----------------------------------------------------------------------------
	int drop(){ 
		dfont->Release(); dline->Release(); sprite->Release(); 
		d3ddev->Release(); d3d->Release(); return 0; }

/* */ //  //-----------------------------------------------------------------------------
	int start(){
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);
		d3ddev->BeginScene(); return 0; }

/* */ //  //-----------------------------------------------------------------------------
	int end(){
		d3ddev->EndScene(); d3ddev->Present( 0,0,0,0 ); return 0; }

/* */ //---------------------------------------------------------------------------------
	void line( int x1, int y1, int x2, int y2, DWORD color = 0xFFFFFFFF ){	
		D3DXVECTOR2 lines[2] = { D3DXVECTOR2( x1, y1 ), D3DXVECTOR2( x2, y2 ) };
		dline->Draw( lines, 2, color ); } 
		
/* */ //  //-----------------------------------------------------------------------------
	void print( int x, int y, DWORD color, char* text, ... ){ RECT ta; 
		va_list va_alist; char szBuffer[1024] = { '\0' }; va_start( va_alist, text ); 
		vsprintf( szBuffer, text, va_alist ); va_end( va_alist ); SetRect( &ta, x, y, 0, 0 );
		dfont->DrawTextA( 0, szBuffer, -1, &ta, DT_LEFT|DT_NOCLIP, color ); }
}; D3D d3d;
/* */ //---------------------------------------------------------------------------------



/* * / //=================================================================================
		#include "mem.h" // kaos99k.
/* */ //---------------------------------------------------------------------------------
#include "TlHelp32.h"
#include "psapi.h" //#pragma comment(lib, "psapi.lib")
/* */ //  //-----------------------------------------------------------------------------
class MemoryUtils{ 
public:
/* */ //  // ---------------------------------------------------------------------
	DWORD getHwndProcessId( LPCSTR tag ){ HWND h; DWORD pId=0x0;		
		printf( "waiting for target" ); 
		while( !(h=FindWindow(tag,0)) && !(h=FindWindow(0,tag)) ){  
		printf("."); Sleep( 1024 ); } printf( " found!\n" );
		GetWindowThreadProcessId( h, &pId ); return pId; } /* */

/* */ //  // ---------------------------------------------------------------------
	DWORD getExeProcessId( LPCSTR tag ){ DWORD pId=0x0;
		PROCESSENTRY32 pE; pE.dwSize = sizeof( PROCESSENTRY32 );
		HANDLE h = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
		if(h==INVALID_HANDLE_VALUE){printf("h fail");CloseHandle(h);return pId;}
		if(!Process32First(h,&pE)){printf("pE fail.");CloseHandle(h);return pId;}
		while( Process32Next( h, &pE ) ){ char tmp[255];
		for(int i=0;i<255;i++){tmp[i]=pE.szExeFile[i];} //printf("%s\n",tmp);
		if(!strcmp(tag,tmp)){pId=pE.th32ProcessID;break;}}CloseHandle(h);return pId;}
		/* */

/* */ //---------------------------------------------------------------------------------
	MODULEENTRY32 getModuleBase( DWORD pId, LPCSTR tag ){ 
		MODULEENTRY32 mE; mE.dwSize = sizeof( MODULEENTRY32 );
		MODULEENTRY32 mB; mB.dwSize = sizeof( MODULEENTRY32 );
		HANDLE h=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32,pId);
		if(h==INVALID_HANDLE_VALUE){printf("hSnap fail.");CloseHandle(h);return mE;}
		if(!Module32First(h,&mE)){printf("M32 fail.");CloseHandle(h);return mE;}
		char tmp[255]; for(int i=0;i<255;i++){tmp[i]=mE.szModule[i];}
		mB=mE; if(strcmp((LPCSTR)mB.szModule,tag)==0||strcmp(tmp,tag)==0){ 
		//printf("%s:0x%x (0x%x)\n",tmp,mB.modBaseAddr,mB.modBaseSize ); 
		CloseHandle(h); return mB; }else while( Module32Next( h, &mE ) ){  
			char tmp[255]; for(int i=0;i<255;i++){tmp[i]=mE.szModule[i];}
			//printf("%s 0x%x (0x%x)\n",tmp,mB.modBaseAddr,mB.modBaseSize );
			if(strcmp(tmp,tag)==0){mB=mE;break;}}CloseHandle(h);return mB;} /* */

}; MemoryUtils mem;
/* */ //---------------------------------------------------------------------------------



/* * / //=================================================================================
		vscode.cpp // kaos99k.
/* */ //---------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg){
/* */ //  //-----------------------------------------------------------------------------
	case WM_CREATE:
		d3d.init( hwnd );
		SetTimer(hwnd, REDRAW, 1000 / 10, 0);
		//::FreeConsole();
		break;

/* */ //  //-----------------------------------------------------------------------------
	case WM_DESTROY:
		d3d.drop();
		KillTimer(hwnd, REDRAW);
		UnregisterClass((LPSTR)"kaos99k.",0);
		PostQuitMessage(0);
		break;

/* */ //  //-----------------------------------------------------------------------------
	case WM_SIZE:
		d3d.init( hwnd );
		InvalidateRect(hwnd, 0, true);
		break;

/* */ //  //-----------------------------------------------------------------------------
	case WM_PAINT:
		d3d.start();
			d3d.print(10+cos(tick/M_PI)*(10),10+sin(tick/M_PI)*(10),0xFFFFFFFF,"vscode :: kaos99k.");

			d3d.line( 10, 10, 10+cos(tick/M_PI)*tick/M_PI, 10+sin(tick/M_PI)*tick/M_PI, 0xFF00FF00 );
			d3d.line( 10, 10, 10+cos(tick)*20, 10+sin(tick)*20, 0xFF00FF00 );
			
		d3d.end();
		break;

/* */ //  //-----------------------------------------------------------------------------
	case WM_TIMER: switch (wParam) {
		case REDRAW: 
				tick++;
				InvalidateRect( hwnd, 0, true ); // <-------------------// redraw //
				if(GetAsyncKeyState(VK_F5)&1) PostQuitMessage(0);
			break;
		} break;
	} return DefWindowProc(hwnd, msg, wParam, lParam);
}
/* */ //---------------------------------------------------------------------------------

/* */ // main //=========================================================================
int main(){ system("TITLE vscode :: kaos99k. "); 
/* */ //---------------------------------------------------------------------------------

	printf(" 	\n");
	printf(" 	* press F5 again to end. \n");
	
	SYSTEM_POWER_STATUS sPs; GetSystemPowerStatus(&sPs);
	printf("Batt: %i%, Flag: %x \n", sPs.BatteryLifePercent, sPs.BatteryFlag );
	printf("Res:  %i x %i \n", GetSystemMetrics(0), GetSystemMetrics(1) );
	printf("Arc: %i %i %i \n", sizeof(void *), sizeof(DWORD), sizeof(DWORD_PTR) );

/* */ // Farproc an external proc? // ---------------------------------------------------
	DWORD pid = mem.getExeProcessId("vscode.exe"); printf("pId: 0x%x \n", pid );
	printf("base: 0x%x \n", mem.getModuleBase( pid, "GDI32.dll").modBaseAddr );
	
	FARPROC createPenOff=GetProcAddress(mem.getModuleBase(pid,"GDI32.dll").hModule,"CreatePen");
	printf("createPen: 0x%x \n", createPenOff ); /* */


/* * / // WinGDI // ---------------------------------------------------------------------
	POINT p; DWORD col=RGB(80,80,80); HPEN pen; //int tick=0;
	HDC hdc=GetDC( FindWindow( (LPCSTR)"SysListView32" ,0 ) ); 
	printf(" 	* press ALT to winGDI. \n");
	while(!GetAsyncKeyState(VK_F5)&1){
		if( GetAsyncKeyState( 0x12 ) ){ //ALT
			GetCursorPos(&p); pen=CreatePen( 0, 1, col ); tick++;
			SelectObject( hdc, pen ); GetPixel( hdc, p.x, p.y );
			col = RGB( 100+rand()%155, 100+rand()%155, 100+rand()%155 ); 
			SetPixel(hdc,p.x+cos(tick/M_PI)*tick/M_PI,p.y+sin(tick/M_PI)*tick/M_PI,col);
		}else{ DeleteObject( pen ); tick=0; } Sleep(1); }printf("end."); /* */


/* */ // Window Init //------------------------------------------------------------------
	WNDCLASSEX wc={sizeof(WNDCLASSEX),0,WndProc,0,0,0,0,0,0,0,(LPSTR)"vscode :: kaos99k.",0};
	if (!RegisterClassEx(&wc)){ printf("ERR RegClass\n"); Sleep(2000); exit(0); }
	HWND hWnd = CreateWindowEx(
		WS_EX_TOPMOST|WS_EX_NOACTIVATE|WS_EX_LAYERED|WS_EX_TRANSPARENT|WS_EX_COMPOSITED,
		wc.lpszClassName,(LPSTR)"kaos99k.", WS_POPUP,//WINDOW,
		(GetSystemMetrics(0)/2)-150, (GetSystemMetrics(1)/2)-150,
		300,300,0,0,wc.hInstance,0);
	if(!hWnd){ printf("ERR hWnd\n"); Sleep(2000); return 0; }
	ShowWindow(hWnd, SW_SHOW);
	MSG msg; while(GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg); DispatchMessage(&msg);
	} /* */


	return 0; }
/* */ //---------------------------------------------------------------------------------