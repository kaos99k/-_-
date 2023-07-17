/* * / //=================================================================================
	#include "xtern.source.h" // ash.
/* */ //---------------------------------------------------------------------------------
class Source{ private:
/* */ //---------------------------------------------------------------------------------
	struct Entity{ DWORD_PTR base, flags; float ang[3], pos[3];					
	int health, armor, team; LPCSTR name; };  
	Entity Me; Entity Ent; int ents; float ffov; bool dj, bh, dh; 
	bool bReady; DWORD_PTR clientdll, enginedll; 
	DWORD_PTR eLocal,eList,eSize; int ecount; 
	DWORD_PTR health,armor,ang,pos,team,fov,flags,name,col;
	//DWORD rList, rSize; int rcount; int rads; // CS:Source Radar Data //
public:	
/* */ //---------------------------------------------------------------------------------
	void run(){ if(!bReady){ col=0xFFFFFFFF;

		clientdll=(DWORD_PTR)mem.getModuleBase(pId,"client.dll").modBaseAddr; 
		enginedll=(DWORD_PTR)mem.getModuleBase(pId,"engine.dll").modBaseAddr; 
		//printf("clientdll: 0x%x \nenginedll: 0x%x \n",clientdll,enginedll);
		
/* */ // GMOD // ------------------------------------------------------------------------
		eLocal=eList=clientdll+0x71957C; eSize=0x10; ecount=1024*5; fov=0x2688;
		health=0x90; armor=0x20AC; pos=0x260; ang=0x26C; flags=0x350;  		/* 11/7/23 */

/* * / // CSGO // ------------------------------------------------------------------------
		eLocal = eList = clientdll+0x4DFFF7C; eSize=0x10; ecount=1024*5; 
		health = 0x100; ang=0x12c; pos=0x138; flags=0x104; team=0x0f4; 		/* 11/7/23 */

/* * / // CSS // ------------------------------------------------------------------------
		rList = clientdll+0x0050C150; rSize=0x140; rcount=64; 				/* 0/0/10 */

/* */ //---------------------------------------------------------------------------------		
		bReady^=1; 
	}else{
		d3d.line((rT.right/2)-5,(rT.bottom/2),(rT.right/2)+5,(rT.bottom/2),0xFFFF00FF);
		d3d.line((rT.right/2),(rT.bottom/2)-5,(rT.right/2),(rT.bottom/2)+5,0xFFFF00FF);

/* */ // Toggle Duck & Hop // -------------------------------------------------------------------
		if(GetAsyncKeyState(0x04)&1)dh^=1; if(!dh){if(GetAsyncKeyState(VK_SPACE)){
		if(!dj){dj=true;keybd_event(0x11,MapVirtualKey(0x11,0),0,0);}}else 
		if(dj){dj=false;keybd_event(0x11,MapVirtualKey(0x11,0),0x0002,0);}
		}else{ if( GetAsyncKeyState(VK_SPACE) ){
		if(!bh){bh=true;keybd_event(VK_SPACE,MapVirtualKey(VK_SPACE,0),0,0);}
		if(bh){bh=false;keybd_event(VK_SPACE,MapVirtualKey(VK_SPACE,0),0x0002,0);}
		}}/* */

/* */ //---------------------------------------------------------------------------------		
		ReadProcessMemory( hProc, (void*)( eLocal ), &Me.base, 4, 0 );
		ReadProcessMemory( hProc, (void*)( Me.base+ang ), &Me.ang, 12, 0 );
		ReadProcessMemory( hProc, (void*)( Me.base+pos ), &Me.pos, 12, 0 ); 
		ReadProcessMemory( hProc, (void*)( Me.base+fov ), &ffov, 4, 0 ); 
		if( ffov < 1 ) ffov = 90.0f; w2s.MyPosAng( Me.pos, Me.ang, ffov );
		ReadProcessMemory( hProc, (void*)( Me.base+health ), &Me.health, 4, 0 );
		ReadProcessMemory( hProc, (void*)( Me.base+team ), &Me.team, 4, 0 ); 
		ReadProcessMemory( hProc, (void*)( Me.base+flags ), &Me.flags, 4, 0 );
		ReadProcessMemory( hProc, (void*)( Me.base+name ), &Me.name, 4, 0 );
		
/* */ // GMOD // ------------------------------------------------------------------
		d3d.print( 10,1,col, "Gmod" ); Me.pos[2]+=65;
		ReadProcessMemory( hProc, (void*)( clientdll+0x6BC970 ), &Me.pos, 12, 0 );
		ReadProcessMemory( hProc, (void*)( clientdll+0x679450 ), &Me.ang, 12, 0 );
		if( ffov < 1 ) ffov = 75.0f; w2s.MyPosAng( Me.pos, Me.ang, ffov );
		// FPS Tuning: GMOD // ---------------------------------------------
		ReadProcessMemory(hProc,(void*)(enginedll+0x6C046C),&fps,4,0); /* */
		
/* * / // CSGO // ------------------------------------------------------------------------
		d3d.print( 10,1,col, "CS:GO" ); 
		ReadProcessMemory( hProc, (void*)( clientdll+0x525E29C ), &ffov, 4, 0 ); 
		if( ffov < 1 ) ffov = 75.0f; w2s.MyPosAng( Me.pos, Me.ang, ffov ); 
		// FPS Tuning: CSGO // ---------------------------------------------
		//ReadProcessMemory(hProc,(void*)(enginedll+0x783658),&fps,4,0); /* */

/* * / //---------------------------------------------------------------------------------		
		d3d.print(10,30,col,"0x%x %f %i ", Me.base, ffov, Me.flags );
		d3d.print(10,40,col, "%f %f %f ", Me.pos[0], Me.pos[1], Me.pos[2] );
		d3d.print(10,50,col, "%f %f %f ", Me.ang[0], Me.ang[1], Me.ang[2] );	
		/* */


/* */ // Entity Data Loop // --------------------------------------------------------------
	d3d.print(10,15,col,"entcount: %i ~ %i",ecount,ents); ents=0;
	for( int i=0; i<ecount; i++ ){ ZeroMemory( &Ent, sizeof( Entity ) );
	ReadProcessMemory( hProc,(void*)((eList)+(eSize*i)),&Ent.base,4,0);
	if( !Ent.base || Ent.base == Me.base ) continue; //ents++;

/* */ // CSS & GMOD // ------------------------------------------------------------------
		char mStr[64]; DWORD moff,m=0x5C; // CSS=0x60 | GMOD=0x5C 
		ReadProcessMemory( hProc, (void*)( Ent.base+m ), &moff, 4, 0 ); 
		ReadProcessMemory( hProc, (void*)( moff+0x4 ), &moff, 4, 0 );
		ReadProcessMemory( hProc, (void*)( moff ), &mStr, 64, 0 );
		
/* * / // CSGO // ------------------------------------------------------------------------
		char mStr[64]; DWORD moff,m; m=0x6C; 
		ReadProcessMemory( hProc, (void*)( Ent.base+m ), &moff, 4, 0 ); 
		ReadProcessMemory( hProc, (void*)( moff+0x4 ), &mStr, 64, 0 ); /* */

/* */ // Model Filtering ----------------------------------------------------------------
		if( !strstr( mStr, ".mdl" ) ) continue; 
		bool filt=false; LPCSTR list[]={ "c_", "props","Gibs",
			"..","..","..","..","..","..","..","..","..",".." };
		for( int f=0;f<10;f++ ) if( strstr(mStr,list[f]) ) filt=true;
		if( filt ) continue; /* */ ents++;
/* * / //---------------------------------------------------------------------------------		
		d3d.print( 10+((ents/50)*375), (70+(ents*10))-(500*(ents/50)), col,
			"Ent: %i:%i 0x%x %s", ents, i, Ent.base, mStr ); /* */ 

/* */ //---------------------------------------------------------------------------------		
		ReadProcessMemory( hProc, (void*)( Ent.base+health ), &Ent.health, 4, 0 );
		ReadProcessMemory( hProc, (void*)( Ent.base+armor ), &Ent.armor, 4, 0 );
		ReadProcessMemory( hProc, (void*)( Ent.base+ang ), &Ent.ang, 12, 0 );
		ReadProcessMemory( hProc, (void*)( Ent.base+pos ), &Ent.pos, 12, 0 );
		ReadProcessMemory( hProc, (void*)( Ent.base+team ), &Ent.team, 4, 0 );
		ReadProcessMemory( hProc, (void*)( Ent.base+name ), &Ent.name, 4, 0 );
		//d3d.print(Ent.pos,col,"[%i:%i] %s",i,ents,mStr);
		
/*  */ // GMOD & HL2:DM // ---------------------------------------------------------------
		if(strstr(mStr,"w_")){d3d.poi(Ent.pos,0x33FFFFFF,strrchr(mStr,'/')+1);continue;}
		
		if(Ent.health>0){ Ent.name=strrchr(mStr,'/')+1; Ent.pos[2]+=65; Ent.team=0;
		//if(strstr(mStr,"hostile"))Ent.team=2;//if(strstr(mStr,"friendly"))Ent.team=3;
		d3d.esp(Ent.pos,Ent.ang,Ent.health,Ent.armor,Ent.team,"%s",Ent.name);continue; }
		/* */

/* * / // CSGO // ------------------------------------------------------------------
		Ent.pos[2]-=65;
		if(strstr(mStr,"w_")){d3d.poi(Ent.pos,0x33FFFFFF,strrchr(mStr,'/')+1);continue;}
		if(strstr(mStr,"chicken")){d3d.poi(Ent.pos,0x33FFFFFF, "sqwaaaark!" );continue;}

		if(Ent.health>0){ Ent.name=strrchr(mStr,'/')+1; Ent.pos[2]+=65;
		d3d.esp(Ent.pos,Ent.ang,Ent.health,Ent.armor,Ent.team,"%s",Ent.name);continue; }
		/* */
		
		d3d.printa(Ent.pos,col,"[%i:%i] %s",i,ents,mStr); }
/* */ //---------------------------------------------------------------------------------



/* * / // CSS Radar Data // -------------------------------------------------------------
	DWORD base; ReadProcessMemory( hProc, (void*)( rList ), &base, 4, 0 ); base+=0x38;
	//ReadProcessMemory( hProc, (void*)( base+0x28 ), &Me.pos, 12, 0 ); // local data via radar
	//ReadProcessMemory( hProc, (void*)( base+0x34 ), &Me.ang, 12, 0 ); // local data via radar
	if( fov < 1 ) fov = 75.0f; w2s.MyPosAng( Me.pos, Me.ang, fov );
	d3d.print( 400,10,0xFFFFFFFF, "rcount: %i ~ %i ", rcount, rads ); rads=0;
	for(int i=1;i<rcount;i++){ZeroMemory(&Ent,sizeof(Entity)); Ent.base=base+(rSize*i);
		ReadProcessMemory( hProc, (void*)( base + (rSize*i) ), &Ent.name, 20, 0 );
		ReadProcessMemory( hProc, (void*)( Ent.base+0x20 ), &Ent.team, 4, 0 );
		ReadProcessMemory( hProc, (void*)( Ent.base+0x24 ), &Ent.health, 4, 0 );
		ReadProcessMemory( hProc,(void*)( Ent.base+0x24 ), &Ent.armor, 4, 0 );
		ReadProcessMemory( hProc, (void*)( Ent.base+0x28 ), &Ent.pos, 12, 0 );
		ReadProcessMemory( hProc, (void*)( Ent.base+0x34 ), &Ent.ang, 12, 0 );
		DWORD c=0x00FFFFFF;if(Ent.team==2)c=0xFFFF0000;if(Ent.team==3)c=0xFF0000FF;
		Ent.armour=0; // tmp override //	
		if( Ent.health <= 0 ) continue; rads++; Ent.pos[2]-=65.0f; 
		d3d.esp(Ent.pos,Ent.ang,Ent.health,Ent.armor,Ent.team,"%s",Ent.name);}/* */


/* */ //---------------------------------------------------------------------------------
} } }; Source source;
/* */ //---------------------------------------------------------------------------------
