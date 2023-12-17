/* * / //=================================================================================
	vscode & mingW64 :: kaos99k.
/* * / //---------------------------------------------------------------------------------
	* d3d.h 		- a D3D class thats class.
	* d3dSprite.h 	- sprite sheet animation, extends d3d.h
	* mem.h 		- memory utils, tweaks and shiz.
	* wndProc 		- drawing and window messages here.
	* main 			- program entry point, initiation.
	*				- other bits of scratch.

TODO:
	* scratch my head some more?..


/* */ //---------------------------------------------------------------------------------
#define STRICT
#define WIN32_LEAN_AND_MEAN
const int REDRAW=1, MW=2; char DIR[1024];

#include "windows.h" 
#include "stdio.h" 		




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

}; MemoryUtils mem; /* */ //---------------------------------------------------------------------------------





/* * / //=================================================================================
     #include "d3d.h" // kaos99k.
/* */ //---------------------------------------------------------------------------------
#include "d3d9.h" // #pragma comment(lib, "d3d9.lib")
#include "d3dx9.h" // #pragma comment(lib, "d3dx9.lib")
#include "dwmapi.h" // #pragma comment(lib, "dwmapi.lib")
#include "d3d9types.h"

//	#define IDB_SPRITESHEET 101 //recource.h
//	IDB_SPRITESHEET RCDATA DISCARDABLE "cpp.png" //resource.rc
// 	D3DXCreateTextureFromResource(d3ddev,0,MAKEINTRESOURCE(IDB_SPRITESHEET),&texture); //.cpp

//-----------------------------------------------------------------------------
class D3D{
private:
	IDirect3D9Ex *d3d; ID3DXFont *d3dfont; ID3DXLine *d3dline; //IDirect3DDevice9Ex *d3ddev; 
public:
	IDirect3DDevice9Ex *d3ddev; //IDirect3DDevice9 *d3ddev; 
	//-----------------------------------------------------------------------------
	int init( HWND hwnd ){ BOOL dwm=false; DWORD AA=0; MARGINS margin={-1,-1,-1,-1};
		DwmExtendFrameIntoClientArea(hwnd,&margin); DwmIsCompositionEnabled(&dwm);
		Direct3DCreate9Ex(D3D_SDK_VERSION,&d3d);
		D3DPRESENT_PARAMETERS pp;ZeroMemory(&pp,sizeof(pp));pp.Windowed=true; 
		pp.SwapEffect=D3DSWAPEFFECT_DISCARD; pp.hDeviceWindow=hwnd;
		pp.BackBufferFormat=D3DFMT_A8R8G8B8; pp.MultiSampleType=D3DMULTISAMPLE_NONE;
		d3d->CheckDeviceMultiSampleType( 0, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, 
		true, D3DMULTISAMPLE_NONE, &AA ); pp.MultiSampleQuality = AA -1;
		d3d->CreateDeviceEx(0,D3DDEVTYPE_HAL,hwnd,D3DCREATE_MIXED_VERTEXPROCESSING,&pp,0,&d3ddev);
		//d3d->CreateDevice(0,D3DDEVTYPE_HAL,hwnd,D3DCREATE_MIXED_VERTEXPROCESSING,&pp,&d3ddev);
		int s=16;D3DXCreateFont(d3ddev,s,s*0.5,0,0,0,0,0,0,0,(LPCSTR)"Calibri",&d3dfont); 
		D3DXCreateLine( d3ddev, &d3dline ); d3dline->Begin(); return 0; }
	//-----------------------------------------------------------------------------
	int drop(){ d3dfont->Release();d3dline->Release();d3ddev->Release();d3d->Release();return 0; } 
	//-----------------------------------------------------------------------------
	int start(){ d3ddev->Clear(0,0,D3DCLEAR_TARGET,0x0,1.0f,0);d3ddev->BeginScene();return 0; } 
	//-----------------------------------------------------------------------------
	int end(){ d3ddev->EndScene(); d3ddev->Present(0,0,0,0); return 0; } 
	//-----------------------------------------------------------------------------
	void line(int x1,int y1,int x2,int y2,DWORD color=0xFFFFFFFF){	
		D3DXVECTOR2 lines[2]={D3DXVECTOR2(x1,y1),D3DXVECTOR2(x2,y2)};
		d3dline->Draw(lines,2,color); } 
	//-----------------------------------------------------------------------------
	void rect(int x,int y,int w,int h,DWORD color=0xFFFFFFFF){
		D3DRECT r={x,y,x+w,y+h}; d3ddev->Clear(1,&r,D3DCLEAR_TARGET,color,0,0); }
	//-----------------------------------------------------------------------------
	void rectf(int x,int y,int w,int h,DWORD color=0xFFFFFFFF){
		D3DRECT r={x,y,x+w,y+h}; d3ddev->Clear(1,&r,D3DCLEAR_TARGET,color,0,0);
		r={x+2,y+2,x+w-2,y+h-2}; d3ddev->Clear(1,&r,D3DCLEAR_TARGET,0x00000000,0,0); }
	//-----------------------------------------------------------------------------
	void print( int x, int y, DWORD color, char* text, ... ){ 
		RECT ta; va_list list; char buf[1024] = { '\0' }; va_start( list, text ); 
		vsprintf( buf, text, list ); va_end( list ); SetRect( &ta, x, y, 0, 0 );
		d3dfont->DrawTextA( 0, buf, -1, &ta, DT_LEFT|DT_NOCLIP, color ); } 
	//-----------------------------------------------------------------------------
}; D3D d3d; /* */ //---------------------------------------------------------------------

/* * / //=================================================================================
     #include "d3dsprite.h" // extends d3d.h, kaos99k.
/* */ //---------------------------------------------------------------------------------
#include "time.h"
class D3Dsprite{
private:
	LPD3DXSPRITE sprite; LPDIRECT3DTEXTURE9 texture; D3DXMATRIX dM; D3DXVECTOR2 vC, vS;
	D3DXVECTOR3 sP;RECT sR;float sS=1.0f;clock_t sT;int sW,sH,sF,sA,cF,sD=100;
public:
	//-----------------------------------------------------------------------------
	int init(LPCSTR path, int sWidth, int sHeight, int sFrames, float sScale ){ 
		sW=sWidth;sH=sHeight;sS=sScale;sF=sFrames;cF=0;sT=clock();sP.y=0;sP.x=0;sA=2;
		D3DXCreateSprite(d3d.d3ddev,&sprite);D3DXCreateTextureFromFile(d3d.d3ddev,path,&texture);
		D3DXMatrixIdentity(&dM); vS=D3DXVECTOR2(sS,sS); vC=D3DXVECTOR2(sW/2,sW/2);
		D3DXMatrixTransformation2D(&dM,&vC,0,&vS,&vC,0,0); sprite->SetTransform(&dM);
		return 0; } // sFrames : total number of frames per row //
	//-----------------------------------------------------------------------------
	int drop(){ sprite->Release(); texture->Release(); return 0; } 
	//-----------------------------------------------------------------------------
	int draw(){ 
		clock_t nT=clock();
		if( (nT-sT) > sD*sS ){ sT=nT; switch( sA ){

			default: if( cF < (sF*sF) ) cF++; else cF=0; break;

			case 1: // walk right // 	
				if( cF < 8 ) cF++; else cF=1; vS=D3DXVECTOR2(sS,sS); 
				if( sP.x+(sW*sS) < GetSystemMetrics(0) ) sP.x+=(sW/sF)*sS; 
					else sA=2;
				break;

			case 2: // walk left // 	
				if( cF < 8 ) cF++; else cF=1; vS=D3DXVECTOR2(-sS,sS);
				if( sP.x > 0 ) sP.x-=(sW/sF)*sS;
					else sA=1;
				break;

		} } 	sP.y=(sH*(1-sS));

		sR.left=((cF%sF)*sW);sR.right=sR.left+sW;
		sR.top=((cF/sF)*sH);sR.bottom=sR.top+sH;

		vC=D3DXVECTOR2( sP.x+((sW/2)*sS), sP.y+((sH/2)*sS) );
		D3DXMatrixTransformation2D(&dM,&vC,0,&vS,&vC,0,0); 
		sprite->SetTransform(&dM);

		sprite->Begin(D3DXSPRITE_DONOTSAVESTATE);
		sprite->Draw(texture,&sR,0,&sP,0xFFFFFFFF);
		sprite->End();

		return 0; } 
	//-----------------------------------------------------------------------------
}; D3Dsprite sprite; /* */ //---------------------------------------------------------------------------------





/* * / //=================================================================================
	WndProc //
/* */ //---------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg){
	//-----------------------------------------------------------------------------
	case WM_CREATE:	
		d3d.init(hwnd); 

		char img[1024]; strcpy(img,DIR); strcat(img,"cpp.png");
		sprite.init(img,128,128,16,1.0f); sprite.init(img,128,128,16,0.75f); 
		//	https://www.spriters-resource.com/fullview/143386/  // the sprite sheet

		SetTimer(hwnd,REDRAW,1024/10,0); 
		::FreeConsole();
		break; 

	//-----------------------------------------------------------------------------
	case WM_DESTROY: 
		d3d.drop(); sprite.drop(); 
		
		KillTimer(hwnd, REDRAW); KillTimer(hwnd, MW); 
		UnregisterClass((LPSTR)" ",0);
		PostQuitMessage(0);
		break; 

	//-----------------------------------------------------------------------------
	case WM_SIZE: 
		//d3d.init(hwnd); 					// have to init d3dsprite again...
		InvalidateRect(hwnd,0,true); 
		break; 

	//-----------------------------------------------------------------------------
	case WM_DISPLAYCHANGE:
		SetWindowPos(hwnd,HWND_TOPMOST,0,GetSystemMetrics(1)-168,GetSystemMetrics(0),168,0);
		break;

	//-----------------------------------------------------------------------------
	case WM_PAINT:
		d3d.start();	

			d3d.print(10,100,0xffffffff, "kaos99k." );

			sprite.draw();
			
		d3d.end();
		break; 

	//---------------------------------------------------------------------------------
	case WM_TIMER: switch (wParam) {
	//-----------------------------------------------------------------------------
		case REDRAW:  
				InvalidateRect( hwnd, NULL, true ); // redraw //
				if(GetAsyncKeyState(VK_F5)&1) PostQuitMessage(0); 
				if(GetKeyState(VK_CAPITAL)) SetTimer(hwnd,MW,1024/10,0); else KillTimer(hwnd,MW);
			break;
	//-----------------------------------------------------------------------------
		case MW: 
				INPUT mi={0}; ZeroMemory( &mi, sizeof( INPUT ) );
				mi.type = INPUT_MOUSE; mi.mi.dwFlags = MOUSEEVENTF_MOVE;
				mi.mi.dx = ( -1+(rand()%3) ); mi.mi.dy = ( -1+(rand()%3) );
				SendInput(1, &mi, sizeof(INPUT)); 
			break;
	//-----------------------------------------------------------------------------
		} break; 
	} return DefWindowProc(hwnd, msg, wParam, lParam); }
/* */ //---------------------------------------------------------------------------------





/* * / // main //=========================================================================
	code is poetry :: kaos99k.
/* */ //   // ---------------------------------------------------------------------------
int main(int argc, char* argv[]){ //system("TITLE vsCode + mingW64 :: kaos99k. "); 

	SYSTEM_POWER_STATUS sPs; GetSystemPowerStatus(&sPs);
	printf("Batt: %i%, Flag: %i \n", sPs.BatteryLifePercent, sPs.BatteryFlag );
	printf("Res: %i x %i \n", GetSystemMetrics(0), GetSystemMetrics(1) );
	printf("Arc: %i %i %i \n", sizeof(void *), sizeof(DWORD), sizeof(DWORD_PTR) );
	strncpy(DIR,argv[0],(strlen(argv[0])-strlen(strrchr(argv[0],'\\'))+1));printf("%s \n",DIR);
	

/* */ // FARPROC an external proc?  //---------------------------------------------------
	typedef bool(WINAPI *setCurPosExternal)(int x, int y );

	DWORD pId=mem.getExeProcessId("cpp.exe"); printf("pId: 0x%x \n", pId );
	printf("USER32.dll: 0x%x \n", mem.getModuleBase( pId, "USER32.dll").modBaseAddr );

	FARPROC setCurPosAddr=GetProcAddress(mem.getModuleBase(pId,"USER32.dll").hModule,"SetCursorPos");
	printf("SetCursorPos: 0x%x \n", setCurPosAddr );

	setCurPosExternal setCurPosExt=(setCurPosExternal)GetProcAddress(mem.getModuleBase(pId,"USER32.dll").hModule,"SetCursorPos"); 
	setCurPosExt( 100, 100 ); /* */


/* */ // Initiate Window //------------------------------------------------------------------
	WNDCLASSEX wc={sizeof(WNDCLASSEX),0,WndProc,0,0,0,0,0,0,0,(LPSTR)" ",0};
	if (!RegisterClassEx(&wc)){ printf("ERR RegClass\n"); Sleep(2000); exit(0); }
	HWND hWnd = CreateWindowEx( 0x0A080028l, wc.lpszClassName,(LPSTR)" ", WS_POPUP,//WINDOW,
		0, GetSystemMetrics(1)-168,GetSystemMetrics(0),168,0,0,wc.hInstance,0);
	if(!hWnd){ printf("ERR hWnd\n"); Sleep(2000); return 0; } ShowWindow(hWnd, SW_SHOW);
	MSG msg; while(GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg); DispatchMessage(&msg); } /* */
	//WS_EX_TOPMOST|WS_EX_NOACTIVATE|WS_EX_COMPOSITED|WS_EX_TRANSPARENT|WS_EX_LAYERED,


//---------------------------------------------------------------------------------		
	return 0; }
/* */ //---------------------------------------------------------------------------------
