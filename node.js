
var omsg,LOG=[];function log(msg){if(msg!=omsg){omsg=msg;console.log(msg);
msg='['+new Date().toLocaleString()+'] '+msg; LOG.push(msg); }}

var exec=require('child_process');exec.exec('echo ...',(e,s,r)=>{if(e)log(e);log(s);log(r);});

log('=================================================');
log('    node.js :: kaos99k. ');
log('-------------------------------------------------');

var nav=[ '<!DOCTYPE html><html><head> <meta charset="UTF-8">',
	'<meta name="viewport" content="width=device-width, initial-scale=1.0">',
	'<title>π : kaos99k.</title></head>',
	'<style>body{ font-family: calibri; }</style>',
	'<div id="NAV" name="NAV" ', 'style="position:fixed; ',
	'left:0px; top:0px; width:100%; border-bottom: solid 1px; "> ', 
	'<table style="width:100%;"><tr>', 
	'<td style="width:1%;"> <a href=".././"><img width="50" src=".ash.png"></a></td>',
	'<td>kaos99k.</td>', '<td style="text-align:right;"><form action="./?" method="POST">',
	'<input id="Q" name="Q" type="search" size="5" value="Search" onfocus="Q.value=null"> ',
	'<input type="submit" value="&radic;">&nbsp;','</form></td></table></div><br><br><br>',
	'<div id="proc" name="proc" style=" position:absolute; ',
	' left:50%; top:50%; border:solid 1px; ">100%</div>',
	'<script>function scrll(){window.scrollTo(0,document.body.scrollHeight);}</script>',
'' ].join('');

var os=require('os'); var ip='0.0.0.0',prt='8080',qr; log(os.hostname());
try{ ip=os.networkInterfaces()['Loopback Pseudo-Interface 1'][1].address;prt='80';}catch(e){}
try{ ip=os.networkInterfaces()['WiFi'][1].address;prt='80'; log('WiFi: '+ip); }catch(e){} 
try{ ip=os.networkInterfaces()['lo'][1].address;prt='8080'; log('lo: '+ip); }catch(e){}
try{ ip=os.networkInterfaces()['ap0'][0].address+':'+prt; log('ap0: '+ip); }catch(e){}
try{ ip=os.networkInterfaces()['ccmni0'][0].address+':'+prt; log('ccmni0: '+ip); }catch(e){}
log('http://'+ip+':'+prt+'/');
//qr="http://chart.googleapis.com/chart?chs=100x100&cht=qr&chl=http://"+ip+':'+prt;

var files; var ulf='^', fs=require('fs'); function update(){ files=[]; 
	fs.readdir( './', function( err, data ){ if( err ) log( err ); 
	else data.forEach( f=>{ if( f.includes(".") && !f.includes("json") 
	&& !f.includes(".1") ) files.push( f.toLocaleLowerCase() ); } ); } ); } update();

var mimetypes={ 
	html:'text/html', css:'text/css', js:'application/javascript', ico:'image/vnd',
	gif:'image/gif', jpg:'image/jpeg', jpeg:'image/jpeg', png:'image/png',
	svg:'image/svg+xml', pdf:'application/pdf',	//php: 'text/html',
	
	nfo: 'text/plain',
	exe:'application/vnd.microsoft.portable-executable',
	zip:'application/zip', // ? 7z: 'application/x-7z-compressed', 
	x:'text/plain' }; var contains='null';

var formidable = require('formidable'); var form=new formidable.IncomingForm(); 
form.on( "error", function( err ){ log( err ); } ); var upp=100; //log(form);
form.on( "field", function( field, value ){ log('field::'+field+':'+value ); } );
form.on( "file", function( field, file ){ log('file::'+field+':'+file ); } );
//form.on( "progress", function(r,e){ upp=Math.floor((r/e)*100); }); //log('upload:'+upp+'%'); });

form.on( "fileBegin", function( field, file ){ switch( field ){
	
	case '.up':
		ulf=file.originalFilename; 
		file.filepath='./'+ulf; 
		break;
	
	case '.ffmpeg':
		break;
		
	default: // reject data? //
		log( field );
		break;			
} });

var http=require('http'); //var path=require('path');
http.createServer( function( req, res ){ //log( req.url );
	contains = req.url.substring( req.url.lastIndexOf('.')+1 ); 
	//log( 'contains: '+contains+', '+mimetypes[contains] );
	
	switch( contains ){
		
		default:
			//fs.readFile( './'+(req.url), function( err, data ){
			fs.readFile( './'+unescape(req.url), function( err, data ){
				if( err ){ res.writeHead( 404, { 'Content-Type': 'text/html' } );
				res.end( nav+err, 'utf8' );
			}else{ 
				mime = mimetypes[contains] || 'text/plain';
				res.writeHead( 200, { 'Content-Type': mime } ); 
				if( contains == 'html' )//|| contains == 'php' ) 
				res.end( nav+data,'utf8'); else res.end( data,'utf8'); } }); 
			break;

		case '/':
			res.writeHead(200,{'Content-Type':'text/html'}); res.write( nav );
			//res.write( 'http://'+ip+':'+prt+'/<br><img src="'+qr+'"><br>' );

			for( i=0; i<files.length; i++ ) if( !files[i].startsWith('.') ) 
				res.write( '<a href="./'+files[i]+'">'+files[i]+'</a> <br>');
			
			res.end('','utf8');
			break;
			
		case '/?':
			
			/* * / // reject upload ------------------------------------------------------
			if( req.headers['content-length'] > 64 ){ log( req );
				res.writeHead( 312, { 'Content-Type': 'text/html' } ); //19
				res.write('.nope.' ); res.end( nav, 'utf8' );
			}else /* */ //---------------------------------------------------------------
			
			form.parse( req, function( err, fields, file ){
				res.writeHead( 200, { 'Content-Type': 'text/html' } );
				res.write( nav );
				
				switch( fields["Q"] ){	

					case '.up': case '.upload': var up=[ '<center>',
						'<form method="POST" action="./?" enctype="multipart/form-data"> ',
						'<input type="hidden" name="Q" id="Q" value=".up">',
						'<input type="file" name=".up" id=".up" multiple> ',
						'<input type="submit" value="&radic;"> </form> ',
						'<div id="out" name="out"> </div>',
						'</center>' ].join(''); res.write( up ); update();
						
						if( file[".up"] ){
							res.write('Uploading: '+file[".up"]);
						}//else 
						res.end('','utf8');
						break;
					
					case '.wget': var wget=[ '<br><center>',
						'<form method="POST" action="./?" enctype="multipart/form-data"> ',
						'<input type="hidden" name="Q" id="Q" value=".wget"> ',
						'<input type="text" name=".wget" id=".wget" width="25%"> ',
						'<input type="submit" value="&radic;"> </form> ', 
						'</center>' ].join(''); res.write(wget); 
							
						if( fields[".wget"] ){ 
							
							wgets=exec.exec('wget -v --backups=1 '+fields[".wget"] );
							res.write('wget -v --backups=1 '+fields[".wget"] );
							
							res.write('<font size="-2" face="monospace">'+'<br>');
							wgets.stdout.on('data',(data)=>{ log( data );
							res.write(data+'<br><script>scrll();</script>'); });
							wgets.stderr.on('data',(data)=>{ log( data );
							res.write(data+'<br><script>scrll();</script>'); });
							wgets.on('exit',(data)=>{ res.end('</font>end.','utf8'); });
						
						}else res.end('','utf8');
						break;
					
					case '.ffmpeg': var ffmpeg=[ '<center>',
						'<form method="POST" action="./?" enctype="multipart/form-data"> ',
						'<input type="hidden" name="Q" id="Q" value=".ffmpeg">',
						'<input type="file" name=".ffmpeg" id=".ffmpeg"> ',
						'<select name=".codec" id=".codec">',
						'<option value="mp4">Mp4</option>',
						'<option value="mp3">Mp3</option>',
						'<option value="webm">WebM</option>',
						'<option value="webp">WebP</option>',
						'</select> &nbsp; ', 
						'<input type="submit" value="&radic;"> </form> ',
						'<div id="out" name="out"> </div> ',
						'</center>' ].join(''); res.write(ffmpeg); 

						if( file[".ffmpeg"] ){
							fn=file[".ffmpeg"].originalFilename; 
							fn=fn.substring( 0, fn.lastIndexOf(".") );

							ffmpeg='ffmpeg -i '+file[".ffmpeg"].filepath
							+' -vcodec libx264 '
							+' -pix_fmt yuv420p '
							+' -qscale 0 '
							+' '+fn+'.'+fields[".codec"]
							+' -y';

							ffmpegs=exec.exec( ffmpeg );
							res.write(ffmpeg);

							res.write('<font size="-2" face="monospace">'+'<br>');
							ffmpegs.stdout.on('data',(data)=>{ //log( data );
							res.write(data+'<br><script>scrll();</script>'); });
							ffmpegs.stderr.on('data',(data)=>{ //log( data );
							res.write(data+'<br><script>scrll();</script>'); });
							
							ffmpegs.on('exit',(data)=>{ res.end('</font>end. <br> <center> '
								+'<h1> <a href="./'+fn+'.'+fields[".codec"]+'">'
								+fn+'.'+fields[".codec"]+' </a> </h1> </center> <br>'
								+'<br> <script>scrll();</script>','utf8'); 
								update(); });
						
						}else res.end('','utf8');
						break;
						
					case '.log': //res.write('<script>window.location.href="./!";</script>');
						res.write('Log: <i>'+LOG.length+' entries</i>');
						res.write('<font size="-2" face="monospace"><br>');
						log(LOG.length+' entries');
						for( i=0; i<LOG.length; i++ ) res.write( LOG[i]+'<br>'); 
						res.end('</font>','utf8');
						break;
					
					case '.update': ulf='yep'; 
					
					default: // search //
					
						if( ulf != '^' ){ ulf='^'; update(); // update & reirect //
							res.write('<script>window.location.href="./";</script>'); }
						
						if( fields["Q"] &&
							fields["Q"].toLocaleLowerCase().substring(0,3).includes('qr.') ){
								fields["Q"]=fields["Q"].substring( 3 ); qr='^'; }
	
						res.write( 'Search: '+fields["Q"]+'<br>' );	
						var f=[]; for( i=0; i<files.length; i++ ) if( fields["Q"] && 
						files[i].includes( fields["Q"].toLocaleLowerCase() ) ) f.push( files[i] );
					
						if(f.length==0) res.write('nothing found...<br>');

						for(i=0;i<f.length;i++) // list findings //
							res.write('<a href="./'+f[i]+'">'+f[i]+'</a> <br>');
						
						if(f.length==1 && qr=='^'){ 
						qr="http://chart.googleapis.com/chart?chs=126x126&cht=qr&chl=http://"+f[0];
						res.write('<a href="./'+f[0]+'"><img src="'+qr+'"></a><br>'); }
						
						res.end('','utf8');
						break; 
				} 
			} );
		break;
	}  
} ).listen( prt, '0.0.0.0' );

