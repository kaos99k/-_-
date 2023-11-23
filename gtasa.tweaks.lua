script_name('lua tweaks') script_author('kaos99k') script_description('pending.')

require "lib.moonloader" --glob = require "lib.game.globals"

--- Config

realtime=true
pickup=null

tags=false
coffee=false
ramp=false
hop=false
drift=true

regen=-2
lights=false
 
--- Main
function main() wait(1000) printHelpString("lua tweaks")
	
	setFreeResprays( true )
	setHeathazeEffect( false )
	rx,ry=getScreenResolution()
	
	while true do wait( 0 ) 
		if isPlayerPlaying( PLAYER_HANDLE ) then
		
			setPlayerNeverGetsTired(PLAYER_PED, true ) 	
			setPlayerFastReload(PLAYER_PED, true ) 	
			
			--freezeOnscreenTimer(bool timer)
			--
			--setEveryoneIgnorePlayer(Player player, bool ignored) 	
			--setCharNeverTargetted(Ped ped, bool untargetable) 	
			

		--	for i = 0, 9999 do if isButtonPressed( PLAYER_HANDLE, i ) then
		--	printString( string.format("pressed: %i", i ), 100 ) end end

			L_LR, L_UD, R_LR, R_UD = getPositionOfAnalogueSticks( 0 )
		--	if L_LR < -100 or L_LR > 100 then -- 0: ply1 1: ply2
		--	printString( string.format( "pad: %i", L_LR ), 100 ) end
					
			if testCheat("ty") then bb,bx,by,bz=getTargetBlipCoordinates()
				setCharCoordinates( PLAYER_PED,bx,by,bz )
				--setCharCoordinatesDontWarpGang( PLAYER_PED, x,y,z )
				--setCharCoordinatesDontWarpGangNoOffset(
				--setCharCoordinatesNoOffset( PLAYER_PED, x,y,z )		
			end
				
			if getCharHealth( PLAYER_PED ) < 125 then if regen >= 100 then 
				damageChar( PLAYER_PED, -2, false ) regen=0 end end regen = regen+1 
				
			if testCheat("real") then if realtime
				then realtime=false printString("Realtime OFF", 1000 )
				else realtime=true printString("Realtime ON", 1000 ) end end
			if realtime then setTimeOfDay( os.date("%H"), os.date("%M") ) --end
				freezeOnscreenTimer( true ) end 

			if testCheat("coffee") then if coffee 
				then coffee=false setGameGlobal( 1219, 0 ) printString("Hot Coffee OFF", 1000 )
				else coffee=true setGameGlobal( 1219, 1 ) printString("Hot Coffee ON", 1000 ) end end
			
			if testCheat("ramp") then if ramp
				then ramp=false printString("Ramp OFF", 1000 )
				else ramp=true printString("Ramp ON", 1000 ) end end
			if ramp and isPlayerControlOn( PLAYER_HANDLE ) then 
			
				--if isButtonPressed( PLAYER_HANDLE , 8 ) then if doesObjectExist( rampa ) then deleteObject( rampa ) else
				if isButtonPressed( PLAYER_HANDLE , 10 ) then if doesObjectExist( rampa ) then deleteObject( rampa ) else
					x,y,z = getOffsetFromCharInWorldCoords( PLAYER_PED, 0.0, 10, -1.2 ) a = getCharHeading( PLAYER_PED )
					rampa = createObject( 1634, x, y, z ) setObjectHeading( rampa, a )
					if isCharInAnyCar( PLAYER_PED ) then placeObjectRelativeToCar( rampa, car, 0, 7.50, -0.005 ) end
				end wait(99) end 
				
				--if isButtonPressed( PLAYER_HANDLE , 9 ) then if doesObjectExist( rampb ) then deleteObject( rampb ) else
				if isButtonPressed( PLAYER_HANDLE , 11 ) then if doesObjectExist( rampb ) then deleteObject( rampb ) else
					x,y,z = getOffsetFromCharInWorldCoords( PLAYER_PED, 0.0, 10, -1.2 ) a = getCharHeading( PLAYER_PED )
					rampb = createObject( 1634, x, y, z ) setObjectHeading( rampb, a )
					if isCharInAnyCar( PLAYER_PED ) then placeObjectRelativeToCar( rampb, car, 0, 7.50, -0.005 ) end
				end wait(99) end 
			end
			
			if testCheat("tags") then if tags 
				then tags=false printString("Tags OFF", 1000 ) 
				else tags=true printString("Tags ON", 1000 ) end end
			if tags then x,y,z = getCharCoordinates( PLAYER_PED )
				tx, ty, tz = getNearestTagPosition( x,y,z ) r=3.0
				if isPointOnScreen( tx, ty, tz, r ) then
					if getPercentageTaggedInArea( tx-r, ty-r, tx+r, ty+r ) < 100 then
						xx, yy = convert3DCoordsToScreen( tx, ty, tz )
						xx, yy = convertWindowScreenCoordsToGameScreenCoords( xx,yy )
						drawRect( xx-10, yy-10, 20, 20, 
							math.random(255), math.random(255), math.random(255), 128 )	
					end end end

			if isCharInAnyCar(PLAYER_PED) then car=storeCarCharIsInNoSave(PLAYER_PED)
				if isCharInFlyingVehicle(PLAYER_PED) then giveWeaponToChar(PLAYER_PED,46,1) end
				if isCharOnAnyBike(PLAYER_PED) then bike=storeCarCharIsInNoSave(PLAYER_PED) else bike=null end 		
				--if bike ~= null then warpCharIntoCar(PLAYER_PED,bike) setCharCanBeKnockedOffBike(PLAYER_PED,false) end --cleo steed mod works better
				
				renderDrawLine( 2.5, (ry*0.99), 2.5+getCarSpeed(car), (ry*0.99), 10, 0xFF990000 )
				displayText( 2.5, ry/2.5, setFreeGxtEntry(string.format("%i", getCarSpeed(car) )) )

				
				if isCarModel( car, 12611 ) then --printString( string.format( "lua craft" ), 100 ) #
				
					setPlayerCanDoDriveBy(PLAYER_HANDLE,false) --giveWeaponToChar(PLAYER_PED,46,1)
					pitch=getCarPitch(car) if pitch > 180 then pitch=pitch-360; end 
					lift=(getCarMass(car)*0.00001)+(pitch*0.0001) --printString( string.format(":: %f", lift ), 100 )
					
					if isButtonPressed( PLAYER_HANDLE, 4 ) then addToCarRotationVelocity( car, 0, 0, -L_LR*0.001 )
						applyForceToCar( car,0,0, lift, 0,0,0 ) if isButtonPressed(PLAYER_HANDLE,16) then 
							applyForceToCar( car, getCarForwardX(car)*0.01, getCarForwardY(car)*0.01, pitch*0.0001, 0,0,0 ) end end
						
					--ax,ay,az=getOffsetFromCarInWorldCoords(car,0,1,0) axx,ayy=convert3DCoordsToScreen(ax,ay,az)
					--tx,ty,tz=getOffsetFromCarInWorldCoords(car,0,10,-pitch/4) txx,tyy=convert3DCoordsToScreen(tx,ty,tz)
					--renderDrawLine(axx,ayy,txx,tyy,1,0xFF990000)
					
					--if isButtonPressed(PLAYER_HANDLE,17) then fireSingleBullet(ax,ay,az,tx,ty,tz,999999999) end
						

				else 
					if isButtonPressed( PLAYER_HANDLE, 4 ) then -- alterNoz 
						applyForceToCar( car, getCarForwardX(car)*0.01, getCarForwardY(car)*0.01, 0,0,0,0 ) end --giveNonPlayerCarNitro( car )
				end 

				
				if isButtonPressed(PLAYER_HANDLE,18) then if not hop then if doesCarHaveHydraulics(car) then 
					setCarHydraulics(car,false) else setCarHydraulics(car,true) end hop=true end else if hop then hop=false end end
					

				--isCarStuckOnRoof(Vehicle car) then --flip the car
				
				--setCarLightsOn( car, true )

				--if isCarOnFire( car ) then taskEveryoneLeaveCar( car ) bike=null car=null end
				
				if isPlayerControlOn(PLAYER_HANDLE) 
					and isButtonPressed(PLAYER_HANDLE,4) and isButtonPressed(PLAYER_HANDLE,15) then 
					
					--taskEveryoneLeaveCar( car )
					taskLeaveCarImmediately( PLAYER_PED, car )
					--removeCharFromCarMaintainPosition( PLAYER_PED, car )
					
					--cx, cy, cz = getOffsetFromCarInWorldCoords( car, 0.0, 0.0, 2.0 )
					--warpCharFromCarToCoord( PLAYER_PED, cx, cy, cz )
					
					bike=null car=null end
					
				if testCheat("diy") then bb,bx,by,bz=getTargetBlipCoordinates()
					carGotoCoordinates(car,bx,by,bz) 
					
					--setCarForwardSpeed(car,100)
					
					--carGotoCoordinatesAccurate(car,bx,by,bz)
					--carGotoCoordinatesRacing(car,bx,by,bz)
					--taskGoToCoordAnyMeans(PLAYER_PED,bx,by,bz,0,car)
					
				end
				
				if testCheat("drift") then if drift 
				then drift=false printString("Drift OFF",100) else drift=true printString("Drift ON",100) end end
				if drift then if isButtonPressed(PLAYER_HANDLE,17) and isVehicleOnAllWheels(car) then 
					addToCarRotationVelocity( car, 0, 0, -L_LR/1500 ) end end

				if testCheat("rep") then fixCar( car ) end
				
				
			else 
			
				if testCheat( "gg" ) then
					--drop weapon
					if pickup ~= null then removePickup( pickup ) end 
					x,y,z = getOffsetFromCharInWorldCoords( PLAYER_PED, 0.0, 3.0, 0.0 )
					w = getCurrentCharWeapon( PLAYER_PED )
					a = 99999 --getAmmoInCharWeapon( PLAYER_PED, w )
					m = getWeapontypeModel( w )
					requestModel( m ) loadAllModelsNow()
					pickup = createPickupWithAmmo( m, 2, a, x, y, z )
					markModelAsNoLongerNeeded( m )
					removeWeaponFromChar( PLAYER_PED, w ) end 
				if hasPickupBeenCollected( pickup ) then removePickup( pickup ) end
				
				if testCheat("nrg") then
					x,y,z = getCharCoordinates( PLAYER_PED )
					requestModel( 522 ) loadAllModelsNow()
					car = createCar( 522, x, y, z-5.0 )
					setCarHeading( car, getCharHeading( PLAYER_PED ) )
					warpCharIntoCar( PLAYER_PED, car )
					setCameraBehindPlayer()
					markModelAsNoLongerNeeded( 522 )
					markCarAsNoLongerNeeded( car )
					bike = storeCarCharIsInNoSave( PLAYER_PED )
					setCharCanBeKnockedOffBike( PLAYER_PED, false )
					setCharCanBeShotInVehicle( PLAYER_PED, false )
					vehicleDoesProvideCover( car, true )
					--addBlipForCar( car )
				end
				
				if testCheat("sav") then activateSaveMenu() end
			
		
	
			end					
	    end
		
	end
end

	-- TEXT TO SCREEN MOTHOD --
	---------------------------
	--drawRect( 20.0, 20.0, 20.0, 20.0, 50, 50, 50, 255)
	--setTextScale( 0.50, 0.75 ) setTextFont(3)                                                                                            
	--setTextColour(255, 255, 255, 255) setTextEdge(1, 0, 50, 50, 150)                                                                          
	--setTextWrapx(1000.0)
	--displayText( 20.0, 20.0, setFreeGxtEntry("lua tweaks") )
	---------------------------

