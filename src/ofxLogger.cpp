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

//--------------------------------------------------------------
ofxLogger::ofxLogger(){
    clogLevel = logLevel = LOG_NOTICE;
    clogClass = clogFunc = "";
    logFilePath = ofToDataPath("log.txt"); // default log file path
    padLength = 30; // default whitespace padding (auto adjust)
    logOptions =  LOG_USE_TIME | LOG_USE_CALL | LOG_USE_TYPE | LOG_USE_PADD; // defaults
    log(LOG_NOTICE, typeid(this).name(), __func__, __LINE__, "/---------------------------------------------------\\");
    log(LOG_NOTICE, typeid(this).name(), __func__, __LINE__, "Created logger");
    clogLevel = logLevel = LOG_WARNING;
}

//--------------------------------------------------------------
ofxLogger::~ofxLogger(){
    clogLevel = logLevel = LOG_NOTICE;
    if ((logOptions & LOG_USE_FILE) == LOG_USE_FILE){
        closeLogFile();
    }
    log(LOG_NOTICE, typeid(this).name(), __func__, __LINE__, "Logging off...");
    log(LOG_NOTICE, typeid(this).name(), __func__, __LINE__, "\\___________________________________________________/");
}

//--------------------------------------------------------------
bool ofxLogger::openLogFile(string filePath){
    
    if(logFile.is_open()) closeLogFile();
    
    log(LOG_VERBOSE, typeid(this).name(), __func__, __LINE__, "Attempting to open log file..." + filePath);
    
    logFilePath = filePath;
    logFile.open(filePath.c_str(), ofstream::app);
	
    if(logFile.good()){
        log(LOG_NOTICE, typeid(this).name(), __func__, __LINE__, "Opening log file...");
        return true;
    }else{
        log(LOG_ERROR, typeid(this).name(), __func__, __LINE__, "Cannot open file");
        return false;
    }
    
}

//--------------------------------------------------------------
bool ofxLogger::closeLogFile(){
    
    if(!logFile.is_open()) return false;
    
    log(LOG_VERBOSE, typeid(this).name(), __func__, __LINE__, "Attempting to close log file..." + logFilePath);
	
    logFile.close();
    
    if(!logFile.fail()){
        log(LOG_NOTICE, typeid(this).name(), __func__, __LINE__, "...Closing log file");
		return true;
	}else{
        log(LOG_ERROR, typeid(this).name(), __func__, __LINE__, "Cannot close file");
        return false;
    }
}

//--------------------------------------------------------------
ofxLogger& ofxLogger::clog(LogLevel l, string className, string funcName, int lineNum){
    clogLevel = l;
    clogClass = className;
    clogFunc = funcName;
    clogLine = lineNum;
    clogMessage.str("");
    return *this;
}

//--------------------------------------------------------------
void ofxLogger::clend(){
    log(clogLevel, clogClass, clogFunc, clogLine, clogMessage.str());
}

//--------------------------------------------------------------
void ofxLogger::log(LogLevel l, string className, string funcName, int lineNum, string msg){
	
	if(l < logLevel) return;
    
    bool hasOptions = false;
    
    stringstream logMessage;
    
    // add date if option set
    if ((logOptions & LOG_USE_DATE) == LOG_USE_DATE){
        formatOptions(hasOptions, logMessage);
        logMessage << getDateStamp();
    }
    
    // add time if option set
    if ((logOptions & LOG_USE_TIME) == LOG_USE_TIME){
        formatOptions(hasOptions, logMessage);
        logMessage << getTimeStamp();
    }
    
    // add log level if option set
    if ((logOptions & LOG_USE_TYPE) == LOG_USE_TYPE){
        formatOptions(hasOptions, logMessage);
        logMessage << getLogLevelName(l);
    }
    
    // add function/class caller if option set
    if ((logOptions & LOG_USE_CALL) == LOG_USE_CALL){
        formatOptions(hasOptions, logMessage);
        string caller = className+"::"+funcName;
        logMessage << caller;
    }
    
    // add time between log calls if option set
    if ((logOptions & LOG_USE_DIFF) == LOG_USE_DIFF){
        formatOptions(hasOptions, logMessage);
        // calculate time between log calls - do it here?? or somewhere else??
        logMessage << ofGetElapsedTimeMicros() - lastLogTime;
        lastLogTime = ofGetElapsedTimeMicros();
    }
    
    // add line number to log calls if option set
    if ((logOptions & LOG_USE_LINE) == LOG_USE_LINE){
        formatOptions(hasOptions, logMessage);
        logMessage << "# " << lineNum;
    }
    
    // terminate option block
    if(hasOptions){
        logMessage << "]: ";
    }
    
    if ((logOptions & LOG_USE_PADD) == LOG_USE_PADD){
        pad(logMessage);
    }
    
	logMessage << msg.c_str() << "\n";
    
    // log to console
    cout << logMessage.str();
    
    // log to file
    if ((logOptions & LOG_USE_FILE) == LOG_USE_FILE){
        logFile << logMessage.str();
    }

}

//--------------------------------------------------------------
void ofxLogger::setLogLevel(LogLevel l){
	logLevel = l;
}

//--------------------------------------------------------------
void ofxLogger::setLogFilePath(string filePath){
    logFilePath = filePath;
}

//--------------------------------------------------------------
void ofxLogger::setLogToFile(bool b, string filePath){
    if(b){
        // maybe need to check logFilePath for != "" and != filePath ???
        openLogFile(filePath);
        logOptions |= LOG_USE_FILE;
    }else{
        closeLogFile();
        logOptions &= ~LOG_USE_FILE;
    }
}

//--------------------------------------------------------------
void ofxLogger::setLogDate(bool b){
    if(b){
        logOptions |= LOG_USE_DATE;
    }else{
        logOptions &= ~LOG_USE_DATE;
    }
}

//--------------------------------------------------------------
void ofxLogger::setLogTime(bool b){
    if(b){
        logOptions |= LOG_USE_TIME;
    }else{
        logOptions &= ~LOG_USE_TIME;
    }
}

//--------------------------------------------------------------
void ofxLogger::setLogCaller(bool b){
    if(b){
        logOptions |= LOG_USE_CALL;
    }else{
        logOptions &= ~LOG_USE_CALL;
    }
}

//--------------------------------------------------------------
void ofxLogger::setLogType(bool b){
    if(b){
        logOptions |= LOG_USE_TYPE;
    }else{
        logOptions &= ~LOG_USE_TYPE;
    }
}

//--------------------------------------------------------------
void ofxLogger::setLogLineNumber(bool b){
    if(b){
        logOptions |= LOG_USE_LINE;
    }else{
        logOptions &= ~LOG_USE_LINE;
    }
}

//--------------------------------------------------------------
void ofxLogger::setLogTimeBetweenLogs(bool b){
    if(b){
        logOptions |= LOG_USE_DIFF;
    }else{
        logOptions &= ~LOG_USE_DIFF;
    }
}

//--------------------------------------------------------------
void ofxLogger::setLogAutoPad(bool b){
    if(b){
        logOptions |= LOG_USE_PADD;
    }else{
        logOptions &= ~LOG_USE_PADD;
    }
}


//--------------------------------------------------------------
void ofxLogger::setLogOptions(int options){
    
    // check if we need to start or stop logging to file
    if ((logOptions & LOG_USE_FILE) == LOG_USE_FILE && (options & LOG_USE_FILE) != LOG_USE_FILE){
        closeLogFile();
    }
    if ((logOptions & LOG_USE_FILE) != LOG_USE_FILE && (options & LOG_USE_FILE) == LOG_USE_FILE){
        openLogFile(logFilePath);
    }
    
    logOptions = options;
}

//--------------------------------------------------------------
inline void ofxLogger::pad(stringstream &logMessage){
	
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
inline string ofxLogger::getLogLevelName(LogLevel l){
	switch(l){
		case LOG_VERBOSE:
			return "VERB";
		case LOG_NOTICE:
			return "NOTE";
		case LOG_WARNING:
			return "WARN";
		case LOG_ERROR:
			return "ERRR";
		case LOG_FATAL_ERROR:
			return "FATE";
		default:
			return "UNKO";
	}
}

//--------------------------------------------------------------
inline void ofxLogger::formatOptions(bool &hasOptions, stringstream &logMessage){
    if(!hasOptions){
        hasOptions = true;
        logMessage << "[";
    }else{
        logMessage << " :: ";
    }
}

//--------------------------------------------------------------
inline char * ofxLogger::getTimeStamp(){
    sprintf(timestamp, "%02i:%02i:%02i", ofGetHours(),  ofGetMinutes(), ofGetSeconds());
    return timestamp;
}

//--------------------------------------------------------------
inline char * ofxLogger::getDateStamp(){
    sprintf(datestamp, "%02i/%02i/%i", ofGetDay(), ofGetMonth(), ofGetYear());
    return datestamp;
}