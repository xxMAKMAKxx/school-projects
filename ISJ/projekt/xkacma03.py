#!/usr/bin/python
import sys, getopt, urllib2, re, os, zipfile, xmlrpclib, os.path, collections, shutil

#DECLARATION OF VARIABLE
url1="http://www.opensubtitles.org/cs/search2/sublanguageid-"
urlLNG="" #LANGUAGE
url2="/moviename-"
urlMOVIE="" #NAME OF MOVIE
movieName =""
helpUrl=""
urlDatabase="http://www.opensubtitles.org"
urlBeforeDownload=""
pocitadlo = 0
listOfSubtitles = []
listOfENGSizes = []
originSize = 0
slovnikTitulku = {}
czeSubName = ""
reverseActive = 0
#END OF DECLARATION

#return the size of file
def getSize(filename):
    st = os.stat(filename)
    return st.st_size
#end of function that returns size of file 

#Validace url
def validaceUrl():
	validUrl = re.search("^http://www.opensubtitles.(org|com)/[a-z]+/subtitles/[0-9]+/[^/]+", url)

	if(validUrl == None):
		print("Spatne url, pouzijte parametr -h pro napovedu")
		exit(1)
	else:
		print("[Check url: URL OK!]")

#FUNCTION HELP:
def help():
	print("Program pro automaticke stahovani a zarovnani titulku\nautor: Martin Kacmarcik (c) 2014 pro FIT VUTBR - kurz ISJ")
	print("usage: projekt [-h] [-r] [-u[\"url\"]]")
	print("Argumenty: -h je napoveda, -r je reverse (default je CZE to ENG, po parametru -r je to ENG to CZE), -u je url filmu")
	print("Pozor! Nutno dat adresu z opensubtitles")
	print("Napriklad: http://www.opensubtitles.com/cs/subtitles/3258272/the-mist-cs")

#END OF HELP FUNCTION

#function for download the original subtitles (cze language)
def originSubDownload():
	rightline=""
	downloadLine="http://www.opensubtitles.org/cs/subtitleserve/sub/"
	urlObjOrigin = urllib2.urlopen(url)
	for lines in urlObjOrigin.readlines():
	    if(re.search(".*/[0-9]+\">St.*", lines)):
		rightline = lines
	
	rightline = re.sub(".*/([0-9]+)\">St.*", r'\1', rightline)
	rightline = re.sub("(.*)\n", r'\1', rightline)
	downloadLine = downloadLine+rightline
	print("Stahuji titulky zadane v parametru...")
        
	urlObjDownloadOrigin = urllib2.urlopen(downloadLine)
	if not os.path.exists("CZE"):
	  print("Vytvarim slozku CZE")
	  os.makedirs("CZE")
	outFile = open("CZE/originSub","wb")
	outFile.write(urlObjDownloadOrigin.read())
	outFile.close()
	if(zipfile.is_zipfile("CZE/originSub")):
	  isZipFile = True
	else:
	  isZipFile = False
	
	if(isZipFile == True):
          zfile = zipfile.ZipFile("CZE/originSub")
          for name in zfile.namelist():

            if(re.search(".*\.nfo$",name)):
              continue

            (dirname, filename) = os.path.split(name)

	    zfile.extract(name, "CZE")

	else:
	  try:
	    os.rename("CZE/originSub", "CZE/originSub.srt")
	  except:
	    pass
	
#end of origin sub download function

#function for subtitles download based on ID
def subtitlesDownload(listOfId):
	global pocitadlo
	print("Stahuji titulky k filmu s opacnym jazykem...")
	for item in listOfId:
	  pocitadlo = pocitadlo + 1
	  download = "http://www.opensubtitles.org/cs/subtitleserve/sub/"+item
	  if not os.path.exists("ENG"):
	    print("Vytvarim slozku ENG")
	    os.makedirs("ENG")
	  urlObjDownload = urllib2.urlopen(download)
	  outFile = open("ENG/sub"+`pocitadlo`,"wb")
	  outFile.write(urlObjDownload.read())
	  if(zipfile.is_zipfile(outFile)):
	    isZipFile = True
	  else:
	    isZipFile = False
	  outFile.close()

	print "Rozbaluji stazene zip file s titulkama..."        
	pocitadlo = pocitadlo + 1
	for i in range(1,pocitadlo):
		if(zipfile.is_zipfile("ENG/sub"+`i`)):
			zfile = zipfile.ZipFile("ENG/sub"+`i`)
			for name in zfile.namelist():			  
			  if(re.search(".*\.nfo$",name)):
			    continue
			  
			  (dirname, filename) = os.path.split(name)
			  zfile.extract(name, "ENG")
		else:
		  try:
		    os.rename("ENG/sub"+`i`, "ENG/sub"+`i`+".srt")
		  except:
		    pass
		  

#end of function for subtitles download based on ID

#ARGUMENT PROCESSING:
def arguments(argv): 
    global url
    global reverseActive
    url=''
    try:
        opts, args = getopt.getopt(argv, "hm:u:r",["url="])
    except getopt.GetoptError:
        help()
        sys.exit(2)
    for opt, arg in opts:
        if opt == "-h":
            help()
            sys.exit()
        elif opt in ("-u","--url"):
            url = arg
	elif opt == "-r":
	    reverseActive = 1

if __name__ == "__main__":
    arguments(sys.argv[1:])    
#END OF ARGUMENT PORCESSING

#fuction that returns ID of subtitles
def titulkyID():
	global movieName
	global reverseActive
	year=""	
	#extracting year from url
	urlObjOrigin = urllib2.urlopen(url)
	for lines in urlObjOrigin.readlines():
	    if(re.search("<h2>.*\([0-9][0-9][0-9][0-9]\).*<\/h2>", lines)):
		year = lines
	year = re.sub(".*([0-9][0-9][0-9][0-9]).*",r'\1',year)
	year = re.sub("(.*)\n",r'\1',year)
	#end of exctracting


	#extracting movie name from url
	movieName = url
	movieName = re.sub(".*/[0-9]*/(.*)$",r'\1',url)

	if(reverseActive == 1):
	  urlLNG = "CZE"
	  if re.search(".*en($|/.*)",movieName):
	    movieName = re.sub("(.*)-en($|/)",r'\1',movieName)
	  if re.search(".*-cs($|/.*)",movieName):
	    print("Chyba! Snazite se porovnavat anglicke titulky s ceskyma, ale zadavate ceske! Doporucuji -h pro napovedu")
	    exit(2)
	else:
	  urlLNG = "ENG"
	  if re.search(".*cs($|/.*)",movieName):
	    movieName = re.sub("(.*)-cs($|/)",r'\1',movieName)
	  if re.search(".*-en($|/.*)",movieName):
	    print("Chyba! Snazite se porovnavat ceske titulky s anglickyma, ale zadavate anglicke! Doporucuji -h pro napovedu")
	    exit(2)
	  #end of extracting


	#CREATING ADRESS FROM -T LANGAUGE
	urlMovie = re.sub('-','+', movieName)
	urlt = url1+urlLNG+url2+urlMovie
	#END OFCREATING ADRESS FROM -T LANGAUGE


	#FINDING URL OF THE MOVIE
	podezreni = 0
	suspiciousUrl = ""
        helpUrl=""
	urlObjTo = urllib2.urlopen(urlt)
	movieName = re.sub("-"," ",movieName)
	#regex = "(?i).*>"+movieName+" ("+year+").*"
	regex = "(?i).*titulky - "+movieName+".*"
	for lines in urlObjTo.readlines():
	  if(podezreni == 1):
	    if(re.search(".*"+year+".*", lines)):
	      helpUrl = suspiciousUrl
	    podezreni = 0
	  if(re.search(regex, lines)):
	    podezreni = 1
	    suspiciousUrl=lines
	#END OF FINDING - NOW IN HELP IS CODE WITH URL of the movie at opensubtitles

	if(helpUrl == ""):
	  print("Chyba! Problem s URL! Zadejte -h pro napovedu")
	  exit(2)

	helpUrl = re.sub(".*href=\"(.*)\".*",r'\1',helpUrl)
	helpUrl = re.sub("(.*)\n",r'\1',helpUrl)
	#now in helpUrl is start of url of sub

	urlBeforeDownload = urlDatabase+helpUrl
	listOfSubtitles = []

	#FINDING ID OF SUB
	urlObjTo = urllib2.urlopen(urlBeforeDownload)
	for lines in urlObjTo.readlines():
	    if(re.search("subtitles/[0-9]*/", lines)):
		listOfSubtitles.append(lines)
	#END OF FINDING - NOW IN HELP IS CODE WITH URL of the movie at opensubtitles


	#extracting list of IDs
	listOfId = []
	for item in listOfSubtitles:
	    listOfId.append(re.sub("([0-9]*)\n", r'\1' , re.sub(".*id=\"name([0-9]*)\".*", r'\1', item)))#
	#now we have list of IDs
	return(listOfId)
#end of fuction that returns ID of subtitles

#function that create list of sizes of sub
def makeSizes():
  global pocitadlo
  global listOfENGSizes
  global listOfSubtitles
  global originSize
  global czeSubName
  pocitadlo = 0
  for file in os.listdir("ENG"):
    if file.endswith(".srt"):
        listOfSubtitles.append(file)
        pocitadlo = pocitadlo + 1
  #now in listOfSubtitles has names of all subtitles
  for i in range(0,pocitadlo):
    pomocna = "ENG/"+listOfSubtitles[i]
    listOfENGSizes.append(getSize(pomocna))
  for file in os.listdir("CZE"):
    if file.endswith(".srt"):
        czeSubName = file
  originSize = getSize("CZE/"+czeSubName)

#end of function, originSize has size of cze subtitles and listOfEndSizes has sizes of sub and listOfSubtitles has names

#function for compare
def compareAndWrite():
  global listOfENGSizes
  global listOfSubtitles
  global originSize
  global pocitadlo
  global movieName
  global slovnikTitulku
  bestValue = listOfENGSizes[0]
  bestSize = 0
  bestName = ""
  ridici = 0
  print("Porovnavam titulky...")

  for i in range(0,pocitadlo):
    minus = originSize - listOfENGSizes[i]
    if(minus < 0):
      minus = minus * (-1)
    if(minus < bestValue):
      bestValue = minus
      bestSize = listOfENGSizes[i]
      bestName = listOfSubtitles[i]
  if(bestSize > originSize):
    shoda = (originSize * 100 / bestSize)
  else:
    shoda = (bestSize * 100 / originSize)

  print("Nejlepsi hodnota je: "+`shoda`+"%")
  print("S titulky: "+bestName+" ("+`bestSize`+"b)")
  print("Puvodni titulky: "+czeSubName+" ("+`originSize`+"b)") 
  
  movieName = re.sub(" ","_",movieName)
  movieName = re.sub("$","_compare.out",movieName)
  outFile = open(movieName,"wb")
  originFile = open("CZE/"+czeSubName, "r")
  englishFile = open("ENG/"+bestName, "r")
  #now in outFile is new sub, originFile origin sub, englishFIle english sub
  pomocnaDictKey = 0
  pomocnaDictValue = ""

  for item in englishFile.readlines():
    if(re.search("!DOCTYPE", item)):
      print("[[[!!!WARNING!!!]]]")
      print("Nejspise jste dosahli limitu pro stazeni titulku, stazene soubory jsou html soubor, program budete proto ukoncen")
      cleaning()
      exit(3)


  for item in englishFile.readlines():
    if(re.search("[0-9][0-9]:[0-9][0-9]:.*", item)):    
      item = re.sub("(.*)\n",r'\1', item)
      item = re.sub("(.*)\r",r'\1', item)       
      pomocnaDictKey = timeToSeconds(item)                  
      ridici = 1      
      continue
    elif(not re.search("([a-z]|[A-Z])", item)):
      continue  
    if(ridici == 1): 
      item = re.sub("(.*)\n",r'\1', item) 
      item = re.sub("(.*)\r",r'\1', item) 
      if(pomocnaDictKey in slovnikTitulku):
        slovnikTitulku[pomocnaDictKey].append(item)
      else:
        mujList=[]
        mujList.append(item)
        slovnikTitulku[pomocnaDictKey] = mujList


  for item in originFile.readlines():
    if(re.search("[0-9][0-9]:[0-9][0-9]:.*", item)):    
      item = re.sub("(.*)\n",r'\1', item)
      item = re.sub("(.*)\r",r'\1', item)       
      pomocnaDictKey = timeToSeconds(item)            
      ridici = 1      
      continue
    elif(not re.search("([a-z]|[A-Z])", item)):
      continue  
    if(ridici == 1): 
      item = re.sub("(.*)\n",r'\1', item)   
      item = re.sub("(.*)\r",r'\1', item) 
      if(pomocnaDictKey in slovnikTitulku):
        slovnikTitulku[pomocnaDictKey].append(item)
      else:
        mujList=[]
        mujList.append(item)
        slovnikTitulku[pomocnaDictKey] = mujList
          



  #slovnikTitulku obsahuje dvojce - promluva a cas (time and speech)
  f = open(movieName, "wb")
  #listOfBanned = []

  #print(slovnikTitulku)

  while(1):
    if(is_empty(slovnikTitulku)):
      break
    else:
      smallestTime = findSmallestKey(slovnikTitulku)      
      for time, speech in slovnikTitulku.items():
        if(time == smallestTime):          
          f.write(`speech`)
          del slovnikTitulku[time]
      f.write("\n")
    

  f.close()
  print("[Dokoncil jsem zapisovani do souboru]")
  print("Pravdepodobne dvojice ulozeny do: "+movieName)


  #close of writing sub
  outFile.close()
  originFile.close()
  englishFile.close()
#end of compare function

def is_empty(any_structure):
    if any_structure:       
        return False
    else:
        return True

def findSmallestKey(directory):
  smallestTime = 999999
  smallestSpeech = ""
  for time, speech in directory.items():
    if(time < smallestTime):
      smallestTime = time
      smallestSpeech = speech
  return smallestTime

def timeToSeconds(string):
  #"00:00:02,000 --> 00:00:12,000"
  hodiny = re.sub("^([0-9][0-9]):[0-9][0-9]:[0-9][0-9],.*",r'\1', string)
  minuty = re.sub("^[0-9][0-9]:([0-9][0-9]):[0-9][0-9],.*",r'\1', string)
  sekundy = re.sub("^[0-9][0-9]:[0-9][0-9]:([0-9][0-9]),.*",r'\1', string)

  hodiny = int(hodiny)
  minuty = int(minuty)
  sekundy = int(sekundy)

  minuty = minuty + (hodiny*60)
  sekundy = sekundy + (minuty*60)


  #hodiny2 = re.sub(".*--> ([0-9][0-9]):[0-9][0-9]:[0-9][0-9],[0-9][0-9][0-9]$",r'\1', string)
  #minuty2 = re.sub(".*--> [0-9][0-9]:([0-9][0-9]):[0-9][0-9],[0-9][0-9][0-9]$",r'\1', string)
  #sekundy2 = re.sub(".*--> [0-9][0-9]:[0-9][0-9]:([0-9][0-9]),[0-9][0-9][0-9]$",r'\1', string)

  #hodiny2 = int(hodiny2)
  #minuty2 = int(minuty2)
  #sekundy2 = int(sekundy2)

  #minuty2 = minuty2 + (hodiny2*60)
  #sekundy2 = sekundy2 + (minuty2*60)  


  #someList = [sekundy, sekundy2]

  return sekundy

def cleaning():
  print("[Uklid:]")
  print("Chcete po sobe uklidit? (Smazat CZE and ENG slozku)")
  odpoved = raw_input("Y/N?\n")
  if(odpoved == "Y"):
    print("Slozky i soubory jsou smazany")
    try:
      shutil.rmtree('CZE')
    except:
      pass
    try:
      shutil.rmtree('ENG')
    except:
      pass
  else:
    print("Slozky zustaly zachovany")

#________________________Hlavni cast programu______________________________#
print("[UPOZORNENI!]")
print("!!! Pozor! Od Kvetna 2014 zavedl server opensubtitles (ktery tento program vyuziva) limit pro stazeni titulku a to 200 titulku na den !!!")
print("!!! Pokud presahnete tento limit nebude mozne dalsi titulky stahovat !!! Program vyhodi chybu !!!")

if(reverseActive == 1):
  print("Pouzivate nastroj reverse (beta). Bohuzel nemuzu zarucit funkcnost tohoto nastroje,\njelikoz jsem pred odevzdanim dosahl limitu pro stazene soubory")
  print("Take upozornuji, ze ENG a CZE slozky jsou vyznamove prohozeny!!!")
print("======[TELO PROGRAMU]======")

validaceUrl()

listOfId = titulkyID()

subtitlesDownload(listOfId)

originSubDownload()

makeSizes()  #originSize has size of cze subtitles and listOfEndSizes has sizes of sub and listOfSubtitles has names

compareAndWrite() 

cleaning()

print("Dekujeme, ze pouzivate nas program, (c) MAKsoft 2014")

#______________________Konec hlavni cast programu__________________________#