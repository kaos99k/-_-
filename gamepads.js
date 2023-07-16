/* * / //=================================================================================
	gamepads.js : kaos99k. 
/* */ //---------------------------------------------------------------------------------
class GamePads{

	constructor(){ console.log('gamepads.js : kaos99k. '); this.gamepads={}; this.deadzone=0.2;
		window.addEventListener( "gamepadconnected", e=>gamepads.events(e), false );
		window.addEventListener( "gamepaddisconnected", e=>gamepads.events(e), false ); }
	
	events( event ){ switch( event.type ){
		case 'gamepadconnected': 
			log( 'GamePad:['+event.gamepad.index+'] Connected'
			+', '+event.gamepad.id //+', mapping: '+event.gamepad.mapping 
			+', buttons: '+event.gamepad.buttons.length 
			+', axes: '+event.gamepad.axes.length +', ' );		
			gamepads.gamepads[event.gamepad.index]=event.gamepad;
			gamepads.poll(); break;	
		case 'gamepaddisconnected':
			log( 'GamePad: ['+event.gamepad.index+'] Disconnected!' );
			delete gamepads.gamepads[event.gamepad.index]; break; 
		default: log( event.type ); break; };}

	A(i=0){ return gamepads.gamepads[i].buttons[0].touched 
				 || gamepads.gamepads[i].buttons[0].pressed; }
	B(i=0){ return gamepads.gamepads[i].buttons[1].touched 
				 || gamepads.gamepads[i].buttons[1].pressed; }
	X(i=0){ return gamepads.gamepads[i].buttons[2].touched 
				 || gamepads.gamepads[i].buttons[2].pressed; }
	Y(i=0){ return gamepads.gamepads[i].buttons[3].touched 
				 || gamepads.gamepads[i].buttons[3].pressed; }
				 
	LB(i=0){ return gamepads.gamepads[i].buttons[4].touched 
				 || gamepads.gamepads[i].buttons[4].pressed; }
	RB(i=0){ return gamepads.gamepads[i].buttons[5].touched 
				 || gamepads.gamepads[i].buttons[5].pressed; }
	LT(i=0){ return gamepads.gamepads[i].buttons[6].touched 
				 || gamepads.gamepads[i].buttons[6].pressed; }	
	RT(i=0){ return gamepads.gamepads[i].buttons[7].touched 
				 || gamepads.gamepads[i].buttons[7].pressed; }	
				 
	Back(i=0){ return gamepads.gamepads[i].buttons[8].touched
				|| gamepads.gamepads[i].buttons[8].pressed; }
	Start(i=0){ return gamepads.gamepads[i].buttons[9].touched 
				 || gamepads.gamepads[i].buttons[9].pressed; }
	L3(i=0){ return gamepads.gamepads[i].buttons[10].touched
				|| gamepads.gamepads[i].buttons[10].pressed; }
	R3(i=0){ return gamepads.gamepads[i].buttons[11].touched
				|| gamepads.gamepads[i].buttons[11].pressed; }
				
	Up(i=0){ return gamepads.gamepads[i].buttons[12].touched
				|| gamepads.gamepads[i].buttons[12].pressed; }
	Down(i=0){ return gamepads.gamepads[i].buttons[13].touched
				|| gamepads.gamepads[i].buttons[13].pressed; }
	Left(i=0){ return gamepads.gamepads[i].buttons[14].touched
				|| gamepads.gamepads[i].buttons[14].pressed; }
	Right(i=0){ return gamepads.gamepads[i].buttons[15].touched
				|| gamepads.gamepads[i].buttons[15].pressed; }
				
	LStick(i=0){ var x=[0,0];
		if( gamepads.gamepads[i].axes[0] < -gamepads.deadzone 
		||  gamepads.gamepads[i].axes[0] > gamepads.deadzone 
		) x[0] = gamepads.gamepads[i].axes[0];
		if( gamepads.gamepads[i].axes[1] < -gamepads.deadzone 
		||  gamepads.gamepads[i].axes[1] > gamepads.deadzone 
		) x[1] = gamepads.gamepads[i].axes[1];
		if( x[0] || x[1] ) return x; }
	
	RStick(i=0){ var x=[0,0];
		if( gamepads.gamepads[i].axes[2] < -gamepads.deadzone 
		||  gamepads.gamepads[i].axes[2] > gamepads.deadzone 
		) x[0] = gamepads.gamepads[i].axes[2];
		if( gamepads.gamepads[i].axes[3] < -gamepads.deadzone 
		||  gamepads.gamepads[i].axes[3] > gamepads.deadzone 
		) x[1] = gamepads.gamepads[i].axes[3];
		if( x[0] || x[1] ) return x; }

	LTrig(i=0){ return gamepads.gamepads[i].buttons[6].value; }
	RTrig(i=0){ return gamepads.gamepads[i].buttons[7].value; }
				 

	poll(){ if( gamepads.gamepads[0] ){ var gp=gamepads.gamepads[0];

	/* */ try{
		if( gamepads.A() ) log( 'A '+gamepads.A() );
		if( gamepads.B() ) log( 'B '+gamepads.B() );
		if( gamepads.X() ) log( 'X '+gamepads.X() );
		if( gamepads.Y() ) log( 'Y '+gamepads.Y() );	
		if( gamepads.LB() ) log( 'LB '+gamepads.LB() );
		if( gamepads.RB() ) log( 'RB '+gamepads.RB() );
		if( gamepads.LT() ) log( 'LT '+gamepads.LT() );
		if( gamepads.RT() ) log( 'RT '+gamepads.RT() );	
		if( gamepads.Back() ) log( 'Back '+gamepads.Back() );
		if( gamepads.Start() ) log( 'Start '+gamepads.Start() );
		if( gamepads.L3() ) log( 'L3 '+gamepads.L3() );
		if( gamepads.R3() ) log( 'R3 '+gamepads.R3() );	
		if( gamepads.Up() ) log( 'Up '+gamepads.Up() );
		if( gamepads.Down() ) log( 'Down '+gamepads.Down() );
		if( gamepads.Left() ) log( 'Left '+gamepads.Left() );
		if( gamepads.Right() ) log( 'Right '+gamepads.Right() );
		
		if( gamepads.LStick() ) log( 'LStick '+gamepads.LStick() );
		if( gamepads.RStick() ) log( 'RStick '+gamepads.RStick() );
		
		if( gamepads.LTrig() ) log( 'LTrig '+gamepads.LTrig() );
		if( gamepads.RTrig() ) log( 'RTrig '+gamepads.RTrig() );
		}catch( e ){ }
	/* */
	
	/* */	
		for( var a=0; a<gp.axes.length; a++ )
			if( gp.axes[a] < -gamepads.deadzone || gp.axes[a] > gamepads.deadzone )
				log( 'Pad: 0 Axes: '+a+' '+gp.axes[a] );
				
		for( var b=0; b<gp.buttons.length; b++ )
			if( gp.buttons[b].pressed ) 
				log( 'Pad: 0 Button: '+b
					+', '+gp.buttons[b].value 
					+', '+gp.buttons[b].touched 
					+', '+gp.buttons[b].pressed 
				);
	/* */
		
		//navigator.getGamepads();
		//window.requestAnimationFrame( gamepads.poll ); 
		setTimeout( "gamepads.poll()", 1000/10 ); 
	}}
}; var gamepads = new GamePads(); 