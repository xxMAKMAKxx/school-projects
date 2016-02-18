#!/usr/bin/python

#CHA:xkacma03
#############################################################
#                   Martin Kacmarcik                        #
#                       xkacma03                            #
#           Faculty of Information Technology               #
#            University of Technology - Brno                #
#                   FOR THE HORDE!!!                        #
#############################################################

import sys, getopt, re, os, os.path, argparse

# START DECLRATION OF VARIABLES
argumentArray = {"help": 0, "input": 0, "output": 0, "pretty-xml": -1, "no-inline": 0, "max-par": -1, "no-duplicates": 0,
                 "remove-whitespace": 0} #array with values (bool or string etc) of arguments
# END DECLRATION OF VARIABLES

# FUNCTION HELP:
def help():
    print('Help for C Headers Analysis by Martin Kacmarcik - xkacma03')
    print('--help - Write help')
    print('--input=fileordir - Input file or dir with C source files')
    print('--output=filename - Output file name')
    print('--pretty-xml=k - Offset in XML file where the lenght of offset is decided by number k')
    print('--no-inline - Skip inline functions')
    print('--max-par=n - Skript will process only functions with n or less parameters')
    print('--no-duplicates - Ignore duplicate functions, process only first occurrence')
    print('--remove-whitespace - Remove with space in rettype and type')
# END OF HELP FUNCTION


# ARGUMENT PROCESSING: - function that process arguments and fill argumentArray
def arguments(argv):
    global argumentArray
    helpBool = 0
    otherBool = 0
    try:
        opts, args = getopt.getopt(argv, "", ['input=', "output=", "pretty-xml=", "help", "no-inline", "max-par=",
                                              "no-duplicates", "remove-whitespace"])
    except getopt.GetoptError:
        print('sem tu')
        sys.exit(1)
    for opt, arg in opts:
        if opt == "--help":
            print(arg)
            helpBool = 1
        elif opt == "--input":
            argumentArray["input"] = arg
            otherBool = 1
        elif opt == "--output":
            argumentArray["output"] = arg
            otherBool = 1
        elif opt == "--pretty-xml":
            argumentArray["pretty-xml"] = arg
            otherBool = 1
        elif opt == "--no-inline":
            argumentArray["no-inline"] = 1
            otherBool = 1
        elif opt == "--max-par":
            argumentArray["max-par"] = arg
            otherBool = 1
        elif opt == "--no-duplicates":
            argumentArray["no-duplicates"] = 1
            otherBool = 1
        elif opt == "--remove-whitespace":
            argumentArray["remove-whitespace"] = 1
            otherBool = 1

    if (helpBool == 1 and otherBool == 0):
        help()
        sys.exit(0)
    elif (helpBool == 1 and otherBool == 1):
        sys.exit(1)
    print(argumentArray)

# ARGUMENT PROCESSING: - function that process arguments and fill argumentArray
def arguments2(argv):
    global argumentArray
    helpBool = 0
    otherBool = 0

    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument("--no-inline", help = '--no-inline - Skip inline functions', action = "store_true")
    parser.add_argument("--no-duplicates", help = '--no-duplicates - Ignore duplicate functions, process only first occurrence', action = "store_true")
    parser.add_argument("--remove-whitespace", help = '--no-duplicates - Ignore duplicate functions, process only first occurrence', action = "store_true")
    parser.add_argument("--help", help = '--no-duplicates - Ignore duplicate functions, process only first occurrence', action = "store_true")
    parser.add_argument("--input")
    parser.add_argument("--output")
    parser.add_argument("--pretty-xml", nargs='?', default = -99)
    parser.add_argument("--max-par")

    #parser.add_argument("--no-inline", help = '--no-inline - Skip inline functions', action = "store_true")
    try:
        args = parser.parse_args()
    except:
        sys.exit(1)

    if(args.no_inline == True):
        argumentArray["no-inline"] = 1
        otherBool = 1
    if(args.no_duplicates == True):
        argumentArray["no-duplicates"] = 1
        otherBool = 1
    if(args.remove_whitespace == True):
        argumentArray["remove-whitespace"] = 1
        otherBool = 1
    if(args.help == True):
        helpBool = 1
    if(args.input != None):
        argumentArray["input"] = args.input
    if(args.output != None):
        argumentArray["output"] = args.output
    if(args.pretty_xml != -99):
        argumentArray["pretty-xml"] = args.pretty_xml
        if(args.pretty_xml == None):
            argumentArray["pretty-xml"] = 4
    if(args.max_par != None):
        argumentArray["max-par"] = args.max_par

    if (helpBool == 1 and otherBool == 0):
        help()
        sys.exit(0)
    elif (helpBool == 1 and otherBool == 1):
        sys.exit(1)

if __name__ == "__main__":
    arguments2(sys.argv[1:])
# END OF ARGUMENT PORCESSING

#START FILE PROCESSING
def fileProcessing(fileList, argumentArray, dirMode):

    if argumentArray["input"] == 0: #search all
        dirMode = 2
        for dirpath, dirnames, filenames, in os.walk("."):
            for filename in [f for f in filenames if f.endswith(".h")]:
                dirpath = re.sub("\.\/", "", dirpath)
                fileList.append(os.path.join(dirpath, filename))

    else:                           #search subdir or folder
        if os.path.isdir(argumentArray["input"]): #is dir
            dirMode = 0
            os.chdir(argumentArray["input"])
            for dirpath, dirnames, filenames, in os.walk("."):
                for filename in [f for f in filenames if f.endswith(".h")]:
                    dirpath = re.sub("\.\/", "", dirpath)
                    #print(dirpath, filename)
                    fileList.append(os.path.join(dirpath, filename))
        else: #is file
            dirMode = 1
            if(re.search(re.compile("^\.\/.*$", re.MULTILINE), argumentArray["input"])):
                fileList.append(argumentArray["input"])
            else:
                fileList.append("./"+argumentArray["input"])
    return dirMode
#END FILE PROCESSING

#START FILE GET CONTENT
def file_get_content(filename):
    try:
        with open(filename, 'r', encoding='utf8') as f:
            return f.read()
    except (OSError, IOError):
        exit(2)
#END FILE GET CONTENT

#START REMOVE COMMENTS AND STRINGS
def removeCommentsAndStrings(string):
    #delete inside of strings
    pattern2 = r'(?<!\\)".*?(?<!\\)"'
    string = re.sub(pattern2, "\"\"", string)
    #delete comments
    pattern = r'/(?:/.*|\*[^*]*(?:\*+(?!/)[^*]*)*(?:\*/)?)' # PROBLEM JE V *+ TOTO JE POLOFUNKCNI REGEX: '/(?:/.*|\*[^*]*(?:\*+(?!/)[^*]*)*(?:\*/)?)'
    string = re.sub(pattern, "", string)
    #remove macros etc
    pattern3 = r'^\s*#.*'
    string = re.sub(re.compile(pattern3,re.MULTILINE), "", string)
    return string
#END REMOVE COMMENTS FROM STRING

#START OF FUNCTION THAT PROCESS FUNCTIONS
def processFunctions(string, argumentArray):
    typeList = ["char", "short", "int", "long", "long long", "enum", "float", "double", "long double", "void"]
    funkce = []
    #for type in typeList:
    #pattern = r"^[a-zA-Z0-9_\s*]*?\([\S\s]*?\)"
    pattern = r"(?:(?:[a-zA-Z0-9_\s*]*?\s)|^)(?:(?:[a-zA-Z]+)|long long|long double)[\s*][a-zA-Z0-9_\s*]*?\([\S\s]*?\)" #regex that find all functions
    #pattern = "^\s*[a-zA-Z_]?.*"+type+"\s+[a-zA-Z_][a-zA-Z_0-9]*\s*\((?s).*?\).*?$"
    objekt = re.findall(pattern, string)

    for item in objekt: #create list of functions

        item = item.strip()
        funkce.append(item)
    return funkce
#END OF FUNCTION THAT PROCESS FUNCTIONS

#START OF FUNCTION THAT CONTROL PRINTING THE ELEMENETS
def printOutPut(dirMode, argumentArray, functionList, fileList): #sys.stdout.write

    functionNameList = []
    #print header and root elements based on pretty xml param
    if(argumentArray["pretty-xml"] == -1):
        sys.stdout.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>") #sys.stdout.write
    else:
        sys.stdout.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n") #sys.stdout.write
    if dirMode == 0:
        if(argumentArray["pretty-xml"] == -1):
            sys.stdout.write("<functions dir=\""+argumentArray["input"]+"\">")
        else:
            sys.stdout.write("<functions dir=\""+argumentArray["input"]+"\">\n")
    if dirMode == 1:
        if(argumentArray["pretty-xml"] == -1):
            sys.stdout.write("<functions dir=\"\">")
        else:
            sys.stdout.write("<functions dir=\"\">\n")
    if dirMode == 2:
        if(argumentArray["pretty-xml"] == -1):
            sys.stdout.write("<functions dir=\"./\">")
        else:
            sys.stdout.write("<functions dir=\"./\">\n")
    #for each file call process functions
    for item in fileList:
        string = file_get_content(item)
        string = removeCommentsAndStrings(string)
        if(dirMode == 2 or dirMode == 0):
            item = re.sub("\.\/", "", item)
        if(dirMode == 1):
            item = argumentArray["input"]
        item = re.sub("\/\/", "/", item)
        functionList = processFunctions(string, argumentArray)
        if(argumentArray["no-inline"] == 1):
            for oneFunc in functionList[:]:
                if(re.search(r"inline", oneFunc)):
                    functionList.remove(oneFunc)



        for func in functionList:
            returnFunctionString(func, item, argumentArray, functionNameList)
    #"""

    if(argumentArray["pretty-xml"] == -1):
        sys.stdout.write("</functions>")
    else:
        sys.stdout.write("</functions>\n")
#END OF FUNCTION THAT CONTROL PRINTING THE ELEMENETS

#START OF FUNCTION THAT PROCESS FUNCTIONS
def returnFunctionString(funcString, fileName, argumentArray, functionNameList):

    returnString = ""
    spaces = ""
    if(argumentArray["pretty-xml"] != -1): #if pretty was given make string with spaces
        for i in range (0, int(argumentArray["pretty-xml"])):
            spaces += " "

    dontWrite = 0
    #namePattern = "^[\s\S]*?\s?[*\s]*([a-zA-Z_][a-zA-Z_0-9]*)\s*\([\S\s]*?\).*?$"
    namePattern =  "(?:(?:[a-zA-Z0-9_\s*]*\s)|^)[a-zA-Z]+[\s*]+?([a-zA-Z0-9_]+?)\s*?\([\S\s]*?\)" #pattern that find names of fucntions
    functionName = re.sub(re.compile(namePattern, re.MULTILINE), r'\1', funcString)
    functionName = functionName.strip()
    varArgsPattern = "[\S\s]*\(([\S\s]*)\)[\S\s]*"
    varString = re.sub(varArgsPattern, r'\1', funcString)
    if(re.search(r'.*,\s*\.\.\..*', varString)): #if u found three dots, varArgs = yes
        varArgs = "yes"
    else:
        varArgs = "no"
    rettypePattern = "^\s*([a-zA-Z_]?.*?[a-zA-Z]+\s?[*\s]*)[a-zA-Z_][a-zA-Z_0-9]*\s*\((?s).*?\).*?$" #pattern that find return type of functions
    retType = re.sub(re.compile(rettypePattern, re.MULTILINE), r'\1', funcString)
    if(argumentArray["remove-whitespace"] == 1): #here the white spaces are removed if param was given
        retType = re.sub(r"\s+", " ", retType)
        retType = re.sub(r"(.*?)\s?(\*)\s?(.*?)", r'\1\2\3', retType)
    retType = re.sub(re.compile(r"(?:\r|\n)$"), "", retType)
    retType = retType.strip()

    if(argumentArray["no-duplicates"] == 1): #here is taken care of duplicates functions
        if not(functionName in functionNameList): 
            returnString = "<function file=\""+fileName+"\" name=\""+functionName+"\" varargs=\""+varArgs+"\" rettype=\""+retType+"\">"
        else:
            return
    else:
        returnString = "<function file=\""+fileName+"\" name=\""+functionName+"\" varargs=\""+varArgs+"\" rettype=\""+retType+"\">"
    if(argumentArray["no-duplicates"] == 1): #if duplicates param was given, store names of the functions so no further duplicates are written
        functionNameList.append(functionName)

    if not(re.search(".*\(\s*void\s*\).*", funcString) or re.search(".*\(\s*\).*", funcString)):
        dontWrite = returnParamString(funcString, argumentArray, returnString) #call param processing function that process paramaters

    else: #if not parameter is in the function just write down the function element itself
        if(argumentArray["pretty-xml"] == -1):
            sys.stdout.write(returnString)
        else:
            sys.stdout.write(spaces+returnString+"\n")
    if(dontWrite == 0):
        if(argumentArray["pretty-xml"] == -1):
            sys.stdout.write("</function>")
        else:
            sys.stdout.write(spaces+"</function>\n")
#END OF FUNCTION THAT PROCESS FUNCTIONS

#START OF THE FUNCTION THAT PROCESS PARAMS
def returnParamString(funcString, argumentArray, returnString):
    spaces = ""
    if(argumentArray["pretty-xml"] != -1): #calculate spaces and create string again
        for i in range (0, int(argumentArray["pretty-xml"])):
            spaces += " "
    spaces2 = ""
    if(argumentArray["pretty-xml"] != -1): #and again for parameters elements which has spaces multiplied by 2
        cislo = int(argumentArray["pretty-xml"]) * 2
        for i in range (0, cislo):
            spaces2 += " "
    numberOfParams = 0
    dontWrite = 0
    pattern = "[\S\s]*\(([\S\s]*)\)[\S\s]*" #using this pattern I take content between () which gives me parameters
    paramString = re.sub(pattern, r'\1', funcString)
    paramList = re.split(r',', paramString) #easy way to get params
    for item3 in paramList: #remove three dots, which are not params
        if(re.search("\.\.\.", item3)):
            paramList.remove(item3)
    numberOfParams = int(paramList.__len__()) #number of params is the lenght of the list
    if(int(argumentArray["max-par"]) >= 0): #handling number of params if the parameter was given, easy
        if(numberOfParams <= int(argumentArray["max-par"])):
            if(argumentArray["pretty-xml"] == -1):
                sys.stdout.write(returnString)
            else:
                sys.stdout.write(spaces+returnString+"\n")
        else:
            dontWrite = 1
    else:
        if(argumentArray["pretty-xml"] == -1):
            sys.stdout.write(returnString)
        else:
            sys.stdout.write(spaces+returnString+"\n")
    for place, item1 in enumerate(paramList): #adjust the parameters strings and then write them out
        modifiedString = item1.strip()
        paramList[place] = modifiedString
        rettypePattern = "^([\S\s]*?)[a-zA-Z_][a-zA-Z_0-9]*$" #find return type of parameter
        item1 = item1.strip()
        paramType = re.sub(rettypePattern, r'\1', item1)
        paramType = paramType.strip()
        paramType = re.sub(re.compile(r"(?:\r|\n)$"), "", paramType)
        if(argumentArray["remove-whitespace"] == 1): #handling remove w-s param
            paramType = re.sub("\s+", " ", paramType)
            if(argumentArray["remove-whitespace"] == 1):
                paramType = re.sub(r"\s+", " ", paramType) #easy regex to handle w-spaces
                paramType = re.sub(r"(.*?)\s?(\*)\s?(.*?)", r'\1\2\3', paramType) #easy regex to handle w-spaces
        if(dontWrite == 0):
            if(argumentArray["pretty-xml"] == -1):
                sys.stdout.write("<param number=\""+str(place+1)+"\" type=\""+paramType+"\" />") #write down param elemenets
            else:
                sys.stdout.write(spaces2+"<param number=\""+str(place+1)+"\" type=\""+paramType+"\" />\n") #write down param elemenets
    return dontWrite
#END OF THE FUNCTION THAT PROCESS PARAMS

#PSEUDOMAIN START

if(argumentArray["output"] != 0):
    try:
        sys.stdout = open(argumentArray["output"], 'w+', encoding='utf8')
    except (OSError, IOError):
        print("sem tu")
        sys.exit(3)

dirMode = 0     #0 - is dir, 1 - is file, 2 - par not given
functionList = []
fileList = []                            #list of files to be processed
dirMode = fileProcessing(fileList, argumentArray, dirMode) #process files
#string = file_get_content(argumentArray["input"])
#string = removeCommentsAndStrings(string)
#funkce = processFunctions(string)
printOutPut(dirMode, argumentArray, functionList, fileList) #this function print out the elements of XML

#PSEUDOMAIN END
