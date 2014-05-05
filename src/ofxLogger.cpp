/*
 * ofxLogger.cpp
 *
 * Version 0.1 30/07/12
 *
 * Copyright 2011 (c) Matthew Gingold [gameover] http://gingold.com.au
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Additional coding Oliver Marriott 1/07/11.
 *
 */

#include "ofxLogger.h"

// PUBLIC C-STYLE INTERFACE
static LogLevel clogLevel = _LOG_NOTICE;
static LogLevel logLevel = _LOG_NOTICE;
static int logOptions = LOG_USE_TIME | LOG_USE_CALL | LOG_USE_TYPE | LOG_USE_PADD;;
static int lastLogTime;
static string logFilePath = ofToDataPath("log.txt");
static ofstream logFile;
static int padLength = 30;

static map<string, LogLevel>& getModules(){
    static map<string, LogLevel> * modules = new map<string, LogLevel>;
    return *modules;
}

//--------------------------------------------------------------
bool ofxLogCloseLogFile(){
    
    if(!logFile.is_open()) return false;
    
    //ofxLog(LOG_VERBOSE, "Attempting to close log file..." + logFilePath);
	
    logFile.close();
    
    if(!logFile.fail()){
        //ofxLog(LOG_NOTICE, "...Closing log file");
		return true;
	}else{
        //ofxLog(LOG_ERROR, "Cannot close file");
        return false;
    }
}

//--------------------------------------------------------------
bool ofxLogOpenLogFile(string filePath){
    
    if(logFile.is_open()) ofxLogCloseLogFile();
    
    //ofxLog(LOG_VERBOSE, "Attempting to open log file..." + filePath);
    
    logFilePath = filePath;
    logFile.open(filePath.c_str(), ofstream::app);
	
    if(logFile.good()){
        //ofxLog(LOG_NOTICE, "Opening log file...");
        return true;
    }else{
        //ofxLog(LOG_ERROR, "Cannot open file");
        return false;
    }
    
}

//--------------------------------------------------------------
void ofxLogSetLogLevel(LogLevel l, string className, string funcName, int lineNum){ // accepts dummy values so we can use the defines for logLevel!!!
	logLevel = l;
}

//--------------------------------------------------------------
void ofxLogSetLogLevel(LogLevel l, string className, string funcName, int lineNum, void * t){ // accepts dummy values so we can use the defines for logLevel!!!
    getModules()[className] = l;
}

//--------------------------------------------------------------
void ofxLogSetLogFilePath(string filePath){
    logFilePath = filePath;
}

//--------------------------------------------------------------
void ofxLogSetLogToFile(bool b, string filePath){
    if(b){
        // maybe need to check logFilePath for != "" and != filePath ???
        logOptions |= LOG_USE_FILE;
        ofxLogOpenLogFile(logFilePath);
    }else{
        logOptions &= ~LOG_USE_FILE;
        ofxLogCloseLogFile();
    }
}

//--------------------------------------------------------------
void ofxSetLogDate(bool b){
    if(b){
        logOptions |= LOG_USE_DATE;
    }else{
        logOptions &= ~LOG_USE_DATE;
    }
}

//--------------------------------------------------------------
void ofxSetLogTime(bool b){
    if(b){
        logOptions |= LOG_USE_TIME;
    }else{
        logOptions &= ~LOG_USE_TIME;
    }
}

//--------------------------------------------------------------
void ofxSetLogCaller(bool b){
    if(b){
        logOptions |= LOG_USE_CALL;
    }else{
        logOptions &= ~LOG_USE_CALL;
    }
}

//--------------------------------------------------------------
void ofxSetLogType(bool b){
    if(b){
        logOptions |= LOG_USE_TYPE;
    }else{
        logOptions &= ~LOG_USE_TYPE;
    }
}

//--------------------------------------------------------------
void ofxSetLogLineNumber(bool b){
    if(b){
        logOptions |= LOG_USE_LINE;
    }else{
        logOptions &= ~LOG_USE_LINE;
    }
}

//--------------------------------------------------------------
void ofxSetLogTimeBetweenLogs(bool b){
    if(b){
        logOptions |= LOG_USE_DIFF;
    }else{
        logOptions &= ~LOG_USE_DIFF;
    }
}

//--------------------------------------------------------------
void ofxSetLogAutoPad(bool b){
    if(b){
        logOptions |= LOG_USE_PADD;
    }else{
        logOptions &= ~LOG_USE_PADD;
    }
}

//--------------------------------------------------------------
void ofxSetLogOptions(int options){
    
    // check if we need to start or stop logging to file
    if ((logOptions & LOG_USE_FILE) == LOG_USE_FILE && (options & LOG_USE_FILE) != LOG_USE_FILE){
        ofxLogCloseLogFile();
    }
    if ((logOptions & LOG_USE_FILE) != LOG_USE_FILE && (options & LOG_USE_FILE) == LOG_USE_FILE){
        ofxLogOpenLogFile(logFilePath);
    }
    
    logOptions = options;
}

//--------------------------------------------------------------
ofxLog::ofxLog(){
    clogLevel = logLevel;
    clogClass = "";
    clogFunc = "";
    clogLine = -1;
    clogMessage.clear();
    bLogged = false;
}

//--------------------------------------------------------------
ofxLog::ofxLog(LogLevel l, string className, string funcName, int lineNum){
    bLogged = false;
    log(l, className, funcName, lineNum);
}

//--------------------------------------------------------------
ofxLog::ofxLog(LogLevel l, string className, string funcName, int lineNum, string msg){
    bLogged = false;
    log(l, className, funcName, lineNum, msg);
};

//--------------------------------------------------------------
ofxLog::ofxLog(LogLevel l, string className, string funcName, int lineNum, const char* format, ...){
    bLogged = false;
    va_list args;
    char buffer[256];
    va_start( args, format );
    vsprintf (buffer, format, args);
    va_end( args );
    log(l, className, funcName, lineNum, string(buffer));
}

//--------------------------------------------------------------
ofxLog::~ofxLog(){
    if(!bLogged) log(clogLevel, clogClass, clogFunc, clogLine, clogMessage.str());
}

//--------------------------------------------------------------
ofxLog& ofxLog::log(LogLevel l, string className, string funcName, int lineNum){
    clogLevel = l;
    clogClass = className;
    clogFunc = funcName;
    clogLine = lineNum;
    clogMessage.str("");
    return *this;
}

//--------------------------------------------------------------
void ofxLog::log(LogLevel l, string className, string funcName, int lineNum, string msg){
	
    if(getModules().find(className) == getModules().end()){
        if(l < logLevel) return;
	}else{
		if(l < getModules()[className]) return;
	}
    
    bool hasOptions = false;
    
    stringstream logMessage;
    
    // add date if option set
    if((logOptions & LOG_USE_DATE) == LOG_USE_DATE){
        formatOptions(hasOptions, logMessage);
        logMessage << getDateStamp();
    }
    
    // add time if option set
    if((logOptions & LOG_USE_TIME) == LOG_USE_TIME){
        formatOptions(hasOptions, logMessage);
        logMessage << getTimeStamp();
    }
    
    // add log level if option set
    if((logOptions & LOG_USE_TYPE) == LOG_USE_TYPE){
        formatOptions(hasOptions, logMessage);
        logMessage << getLogLevelName(l);
    }
    
    // add function/class caller if option set
    if((logOptions & LOG_USE_CALL) == LOG_USE_CALL){
        formatOptions(hasOptions, logMessage);
        string caller = className+"::"+funcName;
        logMessage << caller;
    }
    
    // add time between log calls if option set
    if((logOptions & LOG_USE_DIFF) == LOG_USE_DIFF){
        formatOptions(hasOptions, logMessage);
        // calculate time between log calls - do it here?? or somewhere else??
        logMessage << ofGetElapsedTimeMicros() - lastLogTime;
        lastLogTime = ofGetElapsedTimeMicros();
    }
    
    // add line number to log calls if option set
    if((logOptions & LOG_USE_LINE) == LOG_USE_LINE){
        formatOptions(hasOptions, logMessage);
        logMessage << "# " << lineNum;
    }
    
    // terminate option block
    if(hasOptions){
        logMessage << "]: ";
    }
    
    if((logOptions & LOG_USE_PADD) == LOG_USE_PADD){
        pad(logMessage);
    }
    
	logMessage << msg.c_str();// << "\n";
    
    if((logOptions & LOG_USE_CRRT) == LOG_USE_CRRT){
        logMessage << "\n";
    }
    
    // log to console
    cout << logMessage.str();
    
    bLogged = true;
    
    // log to file
    if ((logOptions & LOG_USE_FILE) == LOG_USE_FILE){
        logFile << logMessage.str();
    }

    if(logLevel == _LOG_FATAL){
        ofxLogCloseLogFile();
        exit(0);
    }
    
}

//--------------------------------------------------------------
inline void ofxLog::pad(stringstream &logMessage){
	
    // add whitespace as difference between last log message 
    // length and padLength (using options only not msg)
    
    int logMessageSize = logMessage.str().size();
    
    padLength = padLength - logMessageSize;
	
    for (int i = 0; i < padLength; i++) logMessage << " ";
    
    if (logMessageSize > padLength){
        padLength = logMessage.str().size();
    }
}

//--------------------------------------------------------------
inline string ofxLog::getLogLevelName(LogLevel l){
	switch(l){
		case _LOG_VERBOSE:
			return "VERB";
		case _LOG_NOTICE:
			return "NOTE";
		case _LOG_WARNING:
			return "WARN";
		case _LOG_ERROR:
			return "ERRR";
		case _LOG_FATAL:
			return "FATE";
		default:
			return "UNKO";
	}
}

//--------------------------------------------------------------
inline void ofxLog::formatOptions(bool &hasOptions, stringstream &logMessage){
    if(!hasOptions){
        hasOptions = true;
        logMessage << "[";
    }else{
        logMessage << " :: ";
    }
}

//--------------------------------------------------------------
inline char * ofxLog::getTimeStamp(){
    sprintf(timestamp, "%02i:%02i:%02i", ofGetHours(),  ofGetMinutes(), ofGetSeconds());
    return timestamp;
}

//--------------------------------------------------------------
inline char * ofxLog::getDateStamp(){
    sprintf(datestamp, "%02i/%02i/%i", ofGetDay(), ofGetMonth(), ofGetYear());
    return datestamp;
}