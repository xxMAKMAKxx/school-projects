<?php

#CST:xkacma03
    # Skript for the first project of IPP course
    # Martin Kacmarcik - Brno, University of technology (xkacma03)
    //===============//
    # For the Horde!!!
    //===============//


    #variable declaration
    global $argumentBoolean; //global, because it is modified by only one function so there shoudlnt be problem
    global $inPutValue; //global, because it is modified by only one function so there shoudlnt be problem
    global $outPutValue; //global, because it is modified by only one function so there shoudlnt be problem
    global $wValue; //global, because it is modified by only one function so there shoudlnt be problem
    global $cwd; //global, because it is modified by only one function so there shoudlnt be problem
    
    $cwd = getcwd(); //save current working directory for later writing
    $outPutValue = NULL;
    //array with arguments flags
    $argumentBoolean = array(
    "isHelp" => false,
    "isInput" => false,
    "isNoSubDir" => false,
    "isOutPut" => false,
    "isK" => false,
    "isO" => false,
    "isI" => false,
    "isW" => false,
    "isC" => false,
    "isP" => false,
    );    
    #end variable declaration
    
     #=============================#
     #MAIN BODY START
    
    parProcess($argc, $argv); 
    argumentSwitch($argumentBoolean, $inPutValue, $outPutValue, $wValue, $argumentBoolean["isP"]); //function that decide what to do next based on params
    
     #MAIN BODY END
     #=============================#
    
    
    function argumentSwitch($argumentBoolean, $inPutValue, $outPutValue, $wValue, $pBoolean){  //function that decide what to do next based on params
        
        if($argumentBoolean["isK"] == TRUE)
        {
            $inputMode = fileOrDir($argumentBoolean["isInput"], $inPutValue);
            $files = handleMode($inputMode, $inPutValue, $argumentBoolean["isNoSubDir"], $pBoolean); //this value should include file if mode is 2 or array of files in others
            
            $arrayOfResults = array();
            foreach ($files as $key => $item){ 
                $content = stream_get_contents($item); //save content of file into string
                $count = k($content); //v count mam vysledne cislo
                $arrayOfResults[$key] = $count; //save result into array

            }
            processResult($arrayOfResults);
        }
        else if($argumentBoolean["isO"] == TRUE)
        {
            $inputMode = fileOrDir($argumentBoolean["isInput"], $inPutValue);
            $files = handleMode($inputMode, $inPutValue, $argumentBoolean["isNoSubDir"], $pBoolean); //this value should include file if mode is 2 or array of files in others
            
            $arrayOfResults = array();
            foreach ($files as $key => $item){ 
                $content = stream_get_contents($item);
                $count = o($content); //v count mam vysledne cislo
                $arrayOfResults[$key] = $count;

            }
            processResult($arrayOfResults);
        }
        else if($argumentBoolean["isI"] == TRUE)
        {
            $inputMode = fileOrDir($argumentBoolean["isInput"], $inPutValue);
            $files = handleMode($inputMode, $inPutValue, $argumentBoolean["isNoSubDir"], $pBoolean); //this value should include file if mode is 2 or array of files in others
            
            $arrayOfResults = array();
            foreach ($files as $key => $item){ 
                $content = stream_get_contents($item);
                $count = i($content); //v count mam vysledne cislo
                $arrayOfResults[$key] = $count;

            }
            processResult($arrayOfResults);
        }
        else if($argumentBoolean["isW"] == TRUE)
        {
            $inputMode = fileOrDir($argumentBoolean["isInput"], $inPutValue);
            $files = handleMode($inputMode, $inPutValue, $argumentBoolean["isNoSubDir"], $pBoolean);
            $arrayOfResults = array();
            foreach ($files as $key => $item){ 
                $content = stream_get_contents($item);
                $count = w($content, $wValue); //v count mam vysledne cislo
                $arrayOfResults[$key] = $count;

            }
            processResult($arrayOfResults);
        }
        else if($argumentBoolean["isC"] == TRUE)
        {
            $inputMode = fileOrDir($argumentBoolean["isInput"], $inPutValue);
            $files = handleMode($inputMode, $inPutValue, $argumentBoolean["isNoSubDir"], $pBoolean); //this value should include file if mode is 2 or array of files in others
            
            $arrayOfResults = array();
            foreach ($files as $key => $item){ 
                $content = stream_get_contents($item);
                $count = c($content); //v count mam vysledne cislo
                $arrayOfResults[$key] = $count;

            }
            processResult($arrayOfResults);
        }
    }
    
    //function that handle the input mode. 1 dir, 2 file, 3 everything
    //returns opened files
    function handleMode($inputMode, $inPutValue, $noSubDir, $pBoolean){
        
        if($inputMode == 1) //dir
        { 
            $arrayOfFiles = array(); //array that conatins all names of files     
            
            if(chdir($inPutValue) == false)
                exit(2);
            foreach (glob("*.[ch]") as $filename) { //find all c h files
                $pomocna = fopen($filename, "r");
                if($pomocna == false)
                    exit(21);                
                $cwd = getcwd();
                if($pBoolean == true)
                    $absPath = $filename; 
                else
                    $absPath = $cwd."/".$filename; //save abs path for later use
                //$absPath = $cwd."/".$filename;      
                if(preg_match("~\/\.\/.*$~", $absPath) == 1){ //if there is . delete it
                    $absPath = preg_replace("~(^.*\/)\.\/(.*$)~", "$1$2", $absPath);
                }
                $arrayOfFiles[$absPath] = $pomocna;
            }   
            if($noSubDir == true) //if noSubDir is true dont continue
            {
                return $arrayOfFiles;
            }
            else{
                $dirs = array_filter(glob('*'), 'is_dir');  
                if(count($dirs) == 0) //when 0 dirs are found just return array of files
                {   
                    return $arrayOfFiles;                
                }
                else //when there is some directory found call again this function for more files
                {     
                    foreach($dirs as $item){                    
                        $newArrayOfFiles = handleMode($inputMode, $item, $noSubDir, $pBoolean);  
                        $arrayOfFiles = array_merge($arrayOfFiles, $newArrayOfFiles);  
                        $path = getcwd();
                        $path = ereg_replace('(.*)/.*$', '\1', $path); //get one dir lower
                        chdir($path);
                    }
                    return $arrayOfFiles;
                } 
            }

        }
        else if($inputMode == 2) //file, basicly same as 1st 
        {
            if($noSubDir == TRUE)
            {
                exit(1);
            }
            $arrayOfFiles = array();
            $open = fopen($inPutValue, "r");
            if($open == false)
                    exit(21);
            $cwd = getcwd();
            if($pBoolean == true)
                    $absPath = $inPutValue; 
                else
                    $absPath = $cwd."/".$inPutValue;  
                
                if(preg_match("~\/\.\/.*$~", $absPath) == 1){
                    $absPath = preg_replace("~(^.*\/)\.\/(.*$)~", "$1$2", $absPath);
                }
                
            $arrayOfFiles[$absPath] = $open;
            return $arrayOfFiles;
        }
        else if($inputMode == 3) //all, basicly same as 1st one
        {
            $arrayOfFiles = array(); //array that conatins all names of files 
            foreach (glob("*.[ch]") as $filename) {
                $pomocna2 = fopen($filename, "r");
                if($pomocna2 == false)
                    exit(21);
                $cwd = getcwd();
                if($pBoolean == true)
                    $absPath = $filename; 
                else
                    $absPath = $cwd."/".$filename;              
                    
                if(preg_match("~\/\.\/.*$~", $absPath) == 1){
                    $absPath = preg_replace("~(^.*\/)\.\/(.*$)~", "$1$2", $absPath);
                }
                $arrayOfFiles[$absPath] = $pomocna2;
            }           
            if($noSubDir == true)
            {
                return($arrayOfFiles);
            }
            else
            {
                $dirs = array_filter(glob('*'), 'is_dir');  
                if(count($dirs) == 0) //when 0 dirs are found just return array of files
                {   
                    return $arrayOfFiles;                
                }
                else //when there is some directory found call again this function for more files
                {     
                    foreach($dirs as $item){  
                        if(chdir($item) == false)
                            exit(2);
                        $newArrayOfFiles = handleMode($inputMode, $item, $noSubDir, $pBoolean);  
                        $arrayOfFiles = array_merge($arrayOfFiles, $newArrayOfFiles);  
                        $path = getcwd();
                        $path = ereg_replace('(.*)/.*$', '\1', $path);
                        chdir($path);
                    }
                    return $arrayOfFiles;
                } 
            }
     
        }
    }
    
    //function that look on whether user want to open file or dir or everything
    //also function check if the file can be even readen or if dir exist etc - basic checks
    function fileOrDir($isInput, $inPutValue){ //1 dir, 2 file, 3 everything
        
        if($isInput == TRUE){
            if(is_dir($inPutValue) == true) //ITS DIR!
            {
                return 1;
            }
            else //NOT DIR? MUST BE FILE!
            {
                $open = fopen($inPutValue, 'r');
                if($open == false) //if u cannot open it, just end with 21
                {                    
                    exit(2);
                }
                else //u succ opened the file. Now do your job!
                {
                    fclose($open);
                    return 2;
                }
            }
        }
        else{ //input wasnt even in params so just scan EVERYTHIIING
            return 3;
        }
             
    }
    

    
    
    #START Parametr processing 
    #=============================#
    function parProcess($argc, $argv){        
        
        global $inPutValue;
        global $outPutValue;
        global $wValue;
        global $argumentBoolean;

        $shortops = "koiw:cp"; 
        $longopts = array("help", "input:", "nosubdir", "output:");
        $options = getopt($shortops, $longopts);

        #START - now just do the math and control if there are no collisions within params 
        $foundHelp = false;       //When help is found no others can be
        $shortFound = 0;          //Atleast one of the shorts need to be found
        $otherFound = 0;
        
        foreach($options as $key => $value){
            switch($key) {
                case "help":
                    $argumentBoolean["isHelp"] = true;
                    $foundHelp = true;
                    break;
                case "input":
                    $argumentBoolean["isInput"] = true;
                    $inPutValue = $value;
                    $otherFound++;
                    break;
                case "nosubdir":
                    $argumentBoolean["isNoSubDir"] = true;
                    $otherFound++;
                    break;
                case "output":
                    $argumentBoolean["isOutPut"] = true;
                    $outPutValue = $value;
                    $otherFound++;
                    break;
                case "k":
                    $argumentBoolean["isK"] = true;
                    helpControl($foundHelp);      
                    $shortFound++;
                    break;
                case "o":
                    $argumentBoolean["isO"] = true;
                    helpControl($foundHelp);      
                    $shortFound++;
                    break;
                case "i":
                    $argumentBoolean["isI"] = true;
                    helpControl($foundHelp);      
                    $shortFound++;
                    break;
                case "w":
                    $argumentBoolean["isW"] = true;
                    helpControl($foundHelp);      
                    $shortFound++;
                    $wValue = $value;                    
                    break;
                case "c":
                    $argumentBoolean["isC"] = true;
                    helpControl($foundHelp);      
                    $shortFound++;
                    break;
                case "p":
                    $argumentBoolean["isP"] = true;
					$otherFound++;
                    break;	    
            }
        }
        if( $shortFound > 0 && $foundHelp == TRUE){
            exit(1);
        }
        else if($otherFound > 0 && $foundHelp == TRUE){
            exit(1);
        }
        #Taking care of duplicate parameters and unknown param
        $delkaPole = count($options);
        if($delkaPole != ($argc-1))            
            exit(1);
        #End of taking care of dup. parameters and unkown param

        
        if($shortFound != 1 && $foundHelp == FALSE) //short param need to be excatly one otherwise there is collision
        {
            exit(1);
        }
        #END - controlling collisions    
        if($foundHelp == true){
            vypisNapovedu();
        }        
    }
    
    #control function that assure help or some of shorts are alone
    function helpControl($foundHelp){
        if($foundHelp == true)
            exit(1);
    }
    #=============================#
    #END Parametr processing 
    

//search operators in content
function o($content){
    $content = removeCommentsFromString($content); //gtfo with macro and comments, who need them?!
    $count = 0;
    //list of operators:
    $arrayOfTypes = array("char", "short", "int", "long", "long long", "enum", "float", "double", "long double", "void");
    $arrayOfOperators = array("\>\>\=", "\<\<\=", "\=\=", "\>\>", "\<\<", "\&\&", "\-\-", "\+\+", "\|\|", "\+\=", "\-\=", "\*\=", "\/\=", "\%\=", "\&\=", "\|\=", "\^\=", "\<\=", "\>\=", "\-\>", "\!\=", "\!", "\&", "\|", "\^", "\<", "\>", "\+", "-", "\/", "\%", "\~", "\.");
    foreach($arrayOfOperators as $item){ //find all operators that have beside them any whitespace or a-Z0-9_ etc.
        $helpCount = preg_match_all('~"[^"\\\]*(?s:\\\.[^"\\\]*)*+"(*SKIP)(*F)|[\sa-zA-Z_()=0-9\],]'.$item.'[\sa-zA-Z0-9_)(;,]~', $content, $out); 
        //var_dump($item);
        //echo("Operator: ".$item." ".$helpCount."\n");
        $count = $count + $helpCount;
    }
    
    //START SPECIAL SEARCH FOR STAR
    $starCounter = preg_match_all("~\*~", $content, $starOut);
    $minusStarCounter = preg_match_all("~\*\=~", $content, $starOut2);
    $starCounter -= $minusStarCounter; //I found all * but I dont need *=
    foreach($arrayOfTypes as $item2){ //find all * declaration. In these case * is not operator
        $declarationCount = preg_match_all("~".$item2."\s*\*+\s*[a-zA-Z_]~", $content, $output);
        if(empty($declarationCount) == FALSE){
            foreach ($output as $item3){
                foreach($item3 as $item4){
                    $starCount = preg_match_all("~\*~", $item4, $starOut3); //find all stars in this declaration
                    $starCounter -= $starCount;
                }
            }
        }
        //var_dump($output);
        
    }   
    $count += $starCounter; 
    //END SPECIAL SEARCH FOR STAR
    //
    //START special search for = 
    $helpCount9 = preg_match_all('~"[^"\\\]*(?s:\\\.[^"\\\]*)*+"(*SKIP)(*F)|[\sa-zA-Z_()]=[\sa-zA-Z0-9_)*(;+-]~', $content, $equalOut);  
	$helpCount8 = preg_match_all('~[\sa-zA-Z_(){}]=\".*?\"~', $content, $equalOut);  
    $count = $count + $helpCount9 + $helpCount8;
    //END special search for =
    
    //START find all 0.5 etc constants
    $constCount = preg_match_all('~"[^"\\\]*(?s:\\\.[^"\\\]*)*+"(*SKIP)(*F)|[0-9]\.[0-9]~', $content, $constOut); 
    $count -= $constCount;
    //END find all 0.5 etc constants
    
    if($constCount > 0){ //take care of 1.1e-1
        $neco69 = preg_match_all("~[0-9]+\.[0-9]+e-[0-9]+~", $content, $minusConstOut);
		$neco68 = preg_match_all("~[0-9]+\.[0-9]+e\+[0-9]+~", $content, $plusConstOut);
        $count -= $neco69;
		$count -= $neco68;
    }
    return $count;
}

//function that find all occurences of pattern given and return number of occ. found
function w($content, $pattern){
	$pattern = preg_quote($pattern);
    $counter = preg_match_all("~".$pattern."~", $content, $out); //easy, just find all occurences of pattern given
    return $counter;
}

//function that count chars in comment
function c($content){
    $pattern = '~"[^"\\\]*(?s:\\\.[^"\\\]*)*+"(*SKIP)(*F)|/(?:/.*|\*[^*]*(?:\*+(?!/)[^*]*)*+(?:\*/)?)\n~S'; //find all chars with \n at the end of line
    $pattern2 = '~"[^"\\\]*(?s:\\\.[^"\\\]*)*+"(*SKIP)(*F)|/(?:/.*|\*[^*]*(?:\*+(?!/)[^*]*)*+(?:\*/)?)\Z~S'; //if there is comment with EOF u will decrement counter
    $pattern3 = '~"[^"\\\]*(?s:\\\.[^"\\\]*)*+"(*SKIP)(*F)|/(?:\*[^*]*(?:\*+(?!/)[^*]*)*+(?:\*/)?)\n~S'; //find all chars with \n at the end of line
    $content = preg_replace('~^\s*\#.+$~m', '', $content); //remove macros and indef def etc.
    $helpCount = preg_match_all($pattern, $content, $out); //find all comments
    $counter = 0;
    $neco = preg_match_all($pattern3, $content, $outXXX);
    $counter -= $neco;
    foreach($out as $item){ //search array from preg match all
        if(is_array($item) == true)
        {
            foreach($item as $itemitem){   
                
                $counter += mb_strlen($itemitem, "ISO-8859-2"); //count lenght of string and add it to counter 
            }
        }
        else
        {
            $counter += mb_strlen($item, "ISO-8859-2"); // if u wont find array just take the lenght of string
        }    
    }
    return $counter;
}

//function for coutning identif. It count all the words, minus literals and keywords
function i($content){
    $content = removeCommentsFromString($content); //first we need to remove comments
    $tryCount = preg_match_all('~"[^"\\\]*(?s:\\\.[^"\\\]*)*+"(*SKIP)(*F)|[a-zA-Z_][a-zA-Z0-9_]*~', $content, $out); //find all ident.
	//var_dump($out);
    $literalFound = preg_match_all("/\'.\'/", $content, $matches); //find literals
	//var_dump($literalFound);
    $helpCounter = k($content); //find key words
    $tryCount = $tryCount - $helpCounter - $literalFound; //all - keywords - literals = result I want
    return $tryCount;
}

//using regex to find all keywords
function k($content) {
    $content = removeCommentsFromString($content);
    $count = 0;
    $arrayOfKeyWords = array("auto", "double", "int", "switch", "_Bool", "else", "long", "typedef", "break", "enum", "register", "union", "case", "extern", "restrict", "unsigned", "char", "float", "return", "void", "_Complex", "for", "short", "volatile", "const", "goto", "signed", "while", "continue", "if", "sizeof", "default", "_Imaginary", "static", "do", "inline", "struct");
    foreach ($arrayOfKeyWords as $item) { //just find all keywords in content... nothing more.. easy regex  
        $helpCount = preg_match_all("~[\s+|^|(*+-/%[;{}]" . $item . "[]\s+|$|)(;{]~", $content, $out);
	$helpCountX = preg_match_all("~\".*?" . $item . ".*?\"~", $content, $outX);
        $helpCountXa = preg_match_all("~\"" . $item . ".*?\"~", $content, $outXa);
        $helpCountXb = preg_match_all("~\".*?" . $item . "\"~", $content, $outXb);
        $helpCountX = $helpCountX - $helpCountXa - $helpCountXb;
        $help2Count = preg_match_all("~\A" . $item . "[]\s+|$|)(;]~", $content, $out0); //for these that are at the start of the file
        $count += $help2Count;
	$count -= $helpCountX;
        $count = $count + $helpCount;
        //var_dump($out);
    }
    return $count;
}



//function to remove string from source file
function removeCommentsFromString($content){    
    #           --skip and fail all the comments in strings -- //comments|  /* comments */ and also analyse the pattern to speed it up
    $pattern = '~"[^"\\\]*(?s:\\\.[^"\\\]*)*+"(*SKIP)(*F)|/(?:/.*|\*[^*]*(?:\*+(?!/)[^*]*)*+(?:\*/)?)~S'; //pattern for comments
    $content = preg_replace($pattern, '', $content);       //remove comments
    $content = preg_replace('~^\s*\#.+$~m', '', $content); //remove macros and indef def etc.
    $result = preg_replace('/\n\s*\n/',"\n", $content);   //blank lines delete   
    return $result;
}

function vypisNapovedu(){
    echo("Skript by Martin Kacmarcik (c) 2015 for FIT VUTBR | FOR THE HORDE!!!\n");
    echo("Prepinace:\n");
    echo("--help vypise napovedu\n");
    echo("--input=fileordir zadany vstupni soubor nebo adresar se zdrojaky v jazyce C\n");
    echo("--nosubidr prohledavani bude bez zanoreni adresaru\n");
    echo("--output=filename zadany textovy vystupni soubor\n");
    echo("-k vypise pocet klicovych slov\n");
    echo("-o vypise pocet vyskytu jednoduchych operatoru\n");
    echo("-i vypise pocet vyskytu identifikatoru\n");
    echo("-w=pattern vyhleda presny textovy retezec pattern\n");
    echo("-c vypise celkovy pocet znaku komentaru\n");
    echo("-p v kombinaci s predchozimi az na help zpusobi, ze se soubory budou vypisovat bez absolutni cesty\n");
    
    exit(0);
}

function processResult($array){
    
    global $outPutValue; //where to write result
    global $cwd; //get the cwd for writing
    chdir($cwd); //set cwd for writing, u dont wanna write into some empty space nowhere
    if($outPutValue != NULL){ //can u open it?
        if (($file = fopen($outPutValue, "w")) == FALSE){
            exit(3);
        }
        fclose($file);
    }

    
    $celkem = 0; //summary for CELKEM:
    $arrayOfLenght = array();
    $biggestNumber = 0;
    $isFirst = true;
    $firstKey = "";
    foreach($array as $key => $value){
        $celkem += $value;  //increment celkem
        $arrayOfLenght[$key] = strlen($key); //set lenght of curr into array
        if($biggestNumber < $value){
            $biggestNumber = $value; //find biggest number
        }
    }
	ksort($array);
    $array["CELKEM:"] = $celkem; //create CELKEM in the array
    $arrayOfLenght["CELKEM:"] = strlen("CELKEM:");
    
    arsort($arrayOfLenght); //sort array of lenght, because they want it
	
    $greatestLenght = reset($arrayOfLenght)+1;
    $biggestNumberLenght = strlen((string)$biggestNumber); //save biggest number lenght for the math further

    foreach ($array as $key => $value){
        
        $numberOfSpaces = $greatestLenght - $arrayOfLenght[$key]; //math for number of spaces, I think it is clear
        $numberOfSpaces += ($biggestNumberLenght - strlen((string)$value));
        $stringOfSpaces = "";
        for($i = 0; $i < $numberOfSpaces; $i++){ //add numberOfSpaces times space
            $stringOfSpaces = $stringOfSpaces." ";
        }
        if($key == "CELKEM:"){ 
            if($outPutValue == NULL){ //if no file was given put it into output
                echo($key.$stringOfSpaces.$value."\n");
            }
            else{
                chdir($cwd);                
                file_put_contents($outPutValue, $key.$stringOfSpaces.$value."\n", FILE_APPEND); //write into file                
            }
            
        }
        else{
            if($outPutValue == NULL){
                echo($key.$stringOfSpaces.$value."\n"); //if no file was given put it into output
            }
            else{
                chdir($cwd);                
                file_put_contents($outPutValue, $key.$stringOfSpaces.$value."\n", FILE_APPEND); //write
            }
        }

    }
}
