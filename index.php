<!DOCTYPE html>
<html> 		<!-- clear; php -S 0.0.0.0:8080 -->
    <head> 
        <meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>π : kaos99k.</title> <style>body{ font-family: calibri; }</style>
    </head> 
<body> 

    <br><br><br>

    <script> 
	//ip=location.origin;
	//qr='http://chart.googleapis.com/chart?chs=100x100&cht=qr&chl=http://'+ip;
	//document.body.innerHTML+='JS :: '+ip+'/<br><img src="'+qr+'"><br>';
    </script>

    <?php
	//foreach( $_SERVER as $key=>$val ) echo $key.' : '.$val.' <br>';
        if( isset( $_POST["Q"] ) ){ switch( $_POST["Q"] ){

			case '.wget':
				echo '<center>'.
				'<form method="POST" action="./?" enctype="multipart/form-data"> '.
				'<input type="hidden" name="Q" id="Q" value=".wget"> '.
				'<input type="text" name="wget" id="wget" width="25%"> '.
				'<input type="submit" value="&radic;"> </form> '.
				//'<div id="out" name="out">'.$_POST["wget"].'</div> '.
				'</center>';

				if(isset($_POST["wget"])){
					$wget='echo wget -v --backups=1 --show-progress '.$_POST["wget"]; echo $wget;
					$out='<br>'; $ret=''; exec(''.$wget,$out,$ret); echo implode($out,'<br>:: ');
					//$ret=''; echo passthru( $wget, $ret ); 
					//$ret=''; echo system( $wget );
				}
				break;

            case '.up': 
                
                echo '<center>' 
                .'<form method="POST" action="./?" enctype="multipart/form-data"> '
                .'<input type="hidden" name="Q" id="Q" value=".up">'
                .'<input type="file" name="up[]" id="up[]" multiple> '
                .'<input type="submit" value="&radic;"> </form> '
                .'</center>';

                //foreach($_FILES["up"] as $key=>$value)echo $key.'=>'.$value.'<br>';echo '<br>';
				
                if(isset($_FILES["up"])) for($i=0;$i<count($_FILES["up"]["name"]);$i++){ 
                echo $i.', '.$_FILES["up"]["name"][$i].', '.$_FILES["up"]["size"][$i].'kb, ';
                if( move_uploaded_file( $_FILES["up"]["tmp_name"][$i], $_FILES["up"]["name"][$i] ) ) 
                    echo 'ok!<br>'; } 

                break;

            default:
                echo 'Search: '.$_POST["Q"].'<br><br>';
                $d=scandir('./'); foreach( $d as $f ){
					if($_POST["Q"]=='')$sp=1; 
						else $sp=strstr(strtolower($f),strtolower($_POST["Q"]));
					if($sp>0){ echo '<a href="./'.$f.'">'.$f.'</a> '; 
						if( is_dir($f) ) echo '&ensp; <--'; echo '<br>'; }
				} break;

        } }else{
			
			$ip=$_SERVER['HTTP_HOST'];
			$qr='http://chart.googleapis.com/chart?chs=100x100&cht=qr&chl=http://'.$ip;
			echo 'PHP :: http://'.$ip.'/<br><img src="'.$qr.'"><br>';

			$ip='0.0.0.0';
			$socket=socket_create(AF_INET,SOCK_DGRAM,SOL_UDP );
			socket_connect( $socket, '8.8.8.8', 53 );
			socket_getsockname( $socket, $ip );
			socket_close( $socket ); 
			echo 'socket: '.$ip.'<br>';

			echo '<br>';
			$d=scandir('./'); foreach( $d as $f ){ 
	
				if( !is_dir($f) 
				&& substr($f,0,1) != '.'  
				&& strpos( strtolower($f), '.json' ) == 0 )
					echo '<a href="./'.$f.'">'.$f.'</a> <br>'; 
			}
        }
    ?>

    <div id="NAV" name="NAV" style=" 
        position:fixed; left:0px; top:0px; width:100%; border-bottom: solid 1px; "> 
        <table width="100%"> 
        <td width="1%"> <a href=".././"> <img width="50px" src=".ash.png"> </a> </td> 
        <td > kaos99k. </td> <td align="right">  <form action="./?" method="POST" > 
        <input id="Q" name="Q" type="search" size="5" value="Search" onfocus="Q.value=null"> 
        <input type="submit" value="&radic;" > &nbsp; </form> </td> </div> 

    </body>
</html>
