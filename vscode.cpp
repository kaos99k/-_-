/* * / //=================================================================================
	vscode & mingW64 :: kaos99k.
/* * / //---------------------------------------------------------------------------------
	* d3d.h 	- a D3D class thats class.
	* mem.h 	- memory utils, tings, tweaks and shiz.
	* wndProc 	- drawing and esp goes here.
	* main 		- program entry point, initiation
					and other bits of scratch.

TODO:
	* scratch my head some more?..

/* */ //---------------------------------------------------------------------------------
#define STRICT
#define WIN32_LEAN_AND_MEAN
const int REDRAW=1, MW=2;

#include "windows.h"
//#include "winuser.h"
#include "stdio.h"
//#include "math.h"

/* * / //=================================================================================
     #include "d3d.h" // kaos99k.
/* */ //---------------------------------------------------------------------------------
#include "d3d9.h" // #pragma comment(lib, "d3d9.lib")
#include "d3dx9.h" // #pragma comment(lib, "d3dx9.lib")
#include "dwmapi.h" // #pragma comment(lib, "dwmapi.lib")
#include "d3d9types.h"
#include "time.h"

//	#define IDB_SPRITESHEET 101 //recource.h
//	IDB_SPRITESHEET RCDATA DISCARDABLE "bob.png" //resource.rc
//D3DXCreateTextureFromResource(d3ddev,0,MAKEINTRESOURCE(IDB_SPRITESHEET),&texture);

//-----------------------------------------------------------------------------
class D3D{
private:
	IDirect3D9Ex *d3d; IDirect3DDevice9Ex *d3ddev; 
	ID3DXFont *dfont; ID3DXLine *dline;

	LPD3DXSPRITE sprite; LPDIRECT3DTEXTURE9 texture; 
	RECT sR; int sW=39,sH=30,sF=2,sA=0,cF=0,cA=0; 
	float sS=1.0f; clock_t sT;
public:
//-----------------------------------------------------------------------------
	int init( HWND hwnd ){
		BOOL dwm=false; DWORD AA=0; MARGINS margin={-1,-1,-1,-1};
		DwmExtendFrameIntoClientArea(hwnd,&margin); DwmIsCompositionEnabled(&dwm);

		Direct3DCreate9Ex( D3D_SDK_VERSION, &d3d );
		D3DPRESENT_PARAMETERS pp; ZeroMemory(&pp, sizeof(pp));
		pp.Windowed=true; pp.SwapEffect=D3DSWAPEFFECT_DISCARD;
		pp.BackBufferFormat=D3DFMT_A8R8G8B8; pp.MultiSampleType=D3DMULTISAMPLE_NONE;
		pp.hDeviceWindow=hwnd;

		d3d->CheckDeviceMultiSampleType( 0, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, 
		true, D3DMULTISAMPLE_NONE, &AA ); pp.MultiSampleQuality = AA -1;
		pp.MultiSampleType = D3DMULTISAMPLE_NONE; 

		d3d->CreateDeviceEx( 0, D3DDEVTYPE_HAL, hwnd, 
			D3DCREATE_MIXED_VERTEXPROCESSING, &pp, 0, &d3ddev );

		int s=16; D3DXCreateFont( d3ddev,s,s*0.5,0,0,0,0,0,0,0,(LPCSTR)"Calibri",&dfont ); 
		
		D3DXCreateLine( d3ddev, &dline ); dline->Begin();
		D3DXCreateSprite( d3ddev, &sprite ); 
		
		sprite_init("C:/Users/kaos9/Desktop/ash/cpp.png",128,128,15,14,1.0f);
	//	https://www.spriters-resource.com/fullview/143386/  // the sprite sheet
		
		return 0;
	}

//-----------------------------------------------------------------------------
	int sprite_init( LPCSTR path, int sWidth, int sHeight, int sFrames, int sActions, float sScale ){ 
		sW=sWidth;sH=sHeight;sS=sScale;sF=sFrames;sA=sActions;cF=0;cA=sActions; sT=clock();
		HRESULT hr=D3DXCreateTextureFromFile(d3ddev,path,&texture);
		D3DXMATRIX dM; D3DXMatrixIdentity(&dM); D3DXVECTOR2 vS(sS,sS);
		D3DXMatrixTransformation2D(&dM,0,0,&vS,0,0,0); sprite->SetTransform(&dM);

		//	sFrames : total number of frames per action
		//	sActions : total number of actions per sheet
		//
		//	cF & cA : current Frame & Action
		//

		return 0; } 

//-----------------------------------------------------------------------------
	int sprite_draw(){ 
		clock_t nT=clock(); 
		if(nT-sT>256){if(cF<sF)cF++;else{cF=0;if(cA<sA)cA++;else cA=0;}sT=nT;}

		sR.left=(cF*sW); sR.right=(cF*sW)+sW; 
		sR.top=(cA*sH); sR.bottom=sR.top+sH;

		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		sprite->Draw( texture, &sR, 0,0, 0xFAFFFFFF );
		sprite->End();

		return 0; } 

//-----------------------------------------------------------------------------
	int something(){ 
		return 0; } 

//-----------------------------------------------------------------------------
	int drop(){ 
		dfont->Release(); dline->Release(); 
		d3ddev->Release(); d3d->Release();
		sprite->Release(); texture->Release(); 
		return 0; } 

//-----------------------------------------------------------------------------
	int start(){ 
		d3ddev->Clear(0,0,D3DCLEAR_TARGET,0x0,1.0f,0); 
		d3ddev->BeginScene(); 
		return 0; } 

//-----------------------------------------------------------------------------
	int end(){ 
		d3ddev->EndScene(); 
		d3ddev->Present(0,0,0,0); 
		return 0; } 

//-----------------------------------------------------------------------------
	void line(int x1,int y1,int x2,int y2,DWORD color=0xFFFFFFFF){	
		D3DXVECTOR2 lines[2]={D3DXVECTOR2(x1,y1),D3DXVECTOR2(x2,y2)};
		dline->Draw(lines,2,color); } 
		
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
class MemoryUtils{ 
public:
//-----------------------------------------------------------------------------
	DWORD getHwndProcessId( LPCSTR tag ){ HWND h; DWORD pId=0x0;		
		printf( "waiting for target" ); 
		while( !(h=FindWindow(tag,0)) && !(h=FindWindow(0,tag)) ){  
		printf("."); Sleep( 1024 ); } printf( " found!\n" );
		GetWindowThreadProcessId( h, &pId ); return pId; } 
		
//-----------------------------------------------------------------------------
	DWORD getExeProcessId( LPCSTR tag ){ DWORD pId=0x0;
		PROCESSENTRY32 pE; pE.dwSize = sizeof( PROCESSENTRY32 );
		HANDLE h = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
		if(h==INVALID_HANDLE_VALUE){printf("h fail.");CloseHandle(h);return pId;}
		if(!Process32First(h,&pE)){printf("pE fail.");CloseHandle(h);return pId;}
		while( Process32Next( h, &pE ) ){ char tmp[255];
		for(int i=0;i<255;i++){tmp[i]=pE.szExeFile[i];} //printf("%s\n",tmp);
		if(!strcmp(tag,tmp)){pId=pE.th32ProcessID;break;}}CloseHandle(h);return pId;}

//-----------------------------------------------------------------------------
	MODULEENTRY32 getModuleBase( DWORD pId, LPCSTR tag ){ 
		MODULEENTRY32 mE; mE.dwSize = sizeof( MODULEENTRY32 );
		MODULEENTRY32 mB; mB.dwSize = sizeof( MODULEENTRY32 );
		HANDLE h=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32,pId);
		if(h==INVALID_HANDLE_VALUE){printf("hSnap fail.");CloseHandle(h);return mE;}
		if(!Module32First(h,&mE)){printf("m32 fail.");CloseHandle(h);return mE;}
		char tmp[255]; for(int i=0;i<255;i++){tmp[i]=mE.szModule[i];}
		mB=mE; if(strcmp((LPCSTR)mB.szModule,tag)==0||strcmp(tmp,tag)==0){ 
		//printf("%s:0x%x (0x%x)\n",tmp,mB.modBaseAddr,mB.modBaseSize ); 
		CloseHandle(h); return mB; }else while( Module32Next( h, &mE ) ){  
			char tmp[255]; for(int i=0;i<255;i++){tmp[i]=mE.szModule[i];}
			//printf("%s 0x%x (0x%x)\n",tmp,mB.modBaseAddr,mB.modBaseSize );
			if(strcmp(tmp,tag)==0){mB=mE;break;}}CloseHandle(h);return mB;} 

}; MemoryUtils mem;
/* */ //---------------------------------------------------------------------------------


/* * / //=================================================================================
		vscode.cpp // kaos99k.
/* */ //---------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg){
//-----------------------------------------------------------------------------
	case WM_CREATE:	
		d3d.init(hwnd); 
		SetTimer(hwnd,REDRAW,1000/10,0); 
		::FreeConsole();
		break; 

//-----------------------------------------------------------------------------
	case WM_DESTROY: 
		d3d.drop(); 
		KillTimer(hwnd, REDRAW); KillTimer(hwnd, MW); 
		UnregisterClass((LPSTR)" ",0);
		PostQuitMessage(0);
		break; 

//-----------------------------------------------------------------------------
	case WM_SIZE: 
		d3d.init(hwnd); 
		InvalidateRect(hwnd,0,true); 
		break; 

//-----------------------------------------------------------------------------
	case WM_PAINT:
		d3d.start();	
			
			d3d.print(130,10,0xFFFFFFFF, "kaos99k." );
			d3d.line( 130,26,130+clock()%512,26,0xFF00FF00 );

			d3d.sprite_draw();

		d3d.end();
		break; 

//---------------------------------------------------------------------------------
	case WM_TIMER: switch (wParam) {
//-----------------------------------------------------------------------------
		case REDRAW:  
				InvalidateRect( hwnd, NULL, true ); // redraw //
				if(GetAsyncKeyState(VK_F5)&1) PostQuitMessage(0); 
				if(GetKeyState(VK_CAPITAL)) 
					SetTimer(hwnd,MW,1000/10,0); else KillTimer(hwnd,MW);
			break;
//-----------------------------------------------------------------------------
		case MW: 
				INPUT mi={0}; ZeroMemory( &mi, sizeof( INPUT ) );
				mi.type = INPUT_MOUSE; mi.mi.dwFlags = MOUSEEVENTF_MOVE;
				mi.mi.dx = ( -1+(rand()%3) ); mi.mi.dy = ( -1+(rand()%3) );
				SendInput(1, &mi, sizeof(INPUT)); 
			break;
	
		} break; 
	} return DefWindowProc(hwnd, msg, wParam, lParam); }
/* */ //---------------------------------------------------------------------------------



/* */ // main //=========================================================================
/* */ //---------------------------------------------------------------------------------
int main(){ system("TITLE vscode && mingw64 :: kaos99k. "); 

	SYSTEM_POWER_STATUS sPs; GetSystemPowerStatus(&sPs);
	printf("Batt: %i%, Flag: %x \n", sPs.BatteryLifePercent, sPs.BatteryFlag );
	printf("Res:  %i x %i \n", GetSystemMetrics(0), GetSystemMetrics(1) );
	printf("Arc: %i %i %i \n", sizeof(void *), sizeof(DWORD), sizeof(DWORD_PTR) );

/* */ // Farproc an external proc? // ---------------------------------------------------
	DWORD pid = mem.getExeProcessId("vscode.exe"); printf("pId: 0x%x \n", pid );
	printf("winGDI base: 0x%x \n", mem.getModuleBase( pid, "GDI32.dll").modBaseAddr );
	
	FARPROC createPenOff=GetProcAddress(mem.getModuleBase(pid,"GDI32.dll").hModule,"CreatePen");
	printf("createPen: 0x%x \n", createPenOff ); /* */

/* * / // WinGDI // ---------------------------------------------------------------------
//	#include "wingdi.h" //#pragma comment(lib, "GDI32")

	POINT p; DWORD col=RGB(80,80,80); HPEN pen; int tick=0;
	HDC hdc=GetDC( FindWindow( (LPCSTR)"SysListView32" ,0 ) ); 
	printf(" 	* hold ALT to winGDI. \n"); 
	while(!GetAsyncKeyState(VK_F5)&1){
		if( GetAsyncKeyState( 0x12 ) ){ //ALT
			GetCursorPos(&p); pen=CreatePen( 0, 1, col ); tick++;
			SelectObject( hdc, pen ); GetPixel( hdc, p.x, p.y );
			col = RGB( 100+rand()%155, 100+rand()%155, 100+rand()%155 ); 
			SetPixel(hdc,p.x+cos(tick/M_PI)*tick/M_PI,p.y+sin(tick/M_PI)*tick/M_PI,col);
		}else{ DeleteObject( pen ); tick=0; } Sleep(1); }printf("end."); /* */

/* */ // Initiate Window //------------------------------------------------------------------
	WNDCLASSEX wc={sizeof(WNDCLASSEX),0,WndProc,0,0,0,0,0,0,0,(LPSTR)" ",0};
	if (!RegisterClassEx(&wc)){ printf("ERR RegClass\n"); Sleep(2000); exit(0); }
	HWND hWnd = CreateWindowEx( 0x0A080028l, wc.lpszClassName,(LPSTR)" ", WS_POPUP,//WINDOW,
		(GetSystemMetrics(0)/2)-(128/2), (GetSystemMetrics(1)/1)-160,256,128,0,0,wc.hInstance,0);
	if(!hWnd){ printf("ERR hWnd\n"); Sleep(2000); return 0; } ShowWindow(hWnd, SW_SHOW);
	MSG msg; while(GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg); DispatchMessage(&msg); } /* */
	//WS_EX_TOPMOST|WS_EX_NOACTIVATE|WS_EX_COMPOSITED|WS_EX_TRANSPARENT|WS_EX_LAYERED,

//---------------------------------------------------------------------------------		
	return 0; }
/* */ //---------------------------------------------------------------------------------
