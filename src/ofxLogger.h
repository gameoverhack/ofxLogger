/*
 * ofxLogger.h
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

#ifndef _H_OFXLOG
#define _H_OFXLOG

#include <iostream>
#include <fstream>
#include <typeinfo>

#include "ofMain.h"

// logging levels
enum LogLevel{
	_LOG_VERBOSE,
	_LOG_NOTICE,
	_LOG_WARNING,
	_LOG_ERROR,
	_LOG_FATAL
};

// logging options for fine grain control of log output
enum LogOption{
    LOG_USE_DATE = 0x01,        // 1   - adds date to the log
    LOG_USE_TIME = 0x02,        // 2   - adds time to the log
    LOG_USE_CALL = 0x04,        // 4   - adds the class & function names to the log
    LOG_USE_TYPE = 0x08,        // 8   - adds the log level type to the log
    LOG_USE_DIFF = 0x10,        // 16  - adds time diff between logs to the log
    LOG_USE_LINE = 0x20,        // 32  - adds line numbers to the log
    LOG_USE_PADD = 0x40,        // 64  - adds padding to the log
    LOG_USE_FILE = 0x80,        // 128 - adds log to file
    LOG_USE_CRRT = 0x100        // 256 - adds carriage return
};

// DEFINES

// helper define for getting function from caller
#if __STDC_VERSION__ < 199901L
    #if __GNUC__ >= 2
        #define __func__ __FUNCTION__
    #else
        #define __func__ "<unknown>"
    #endif
#endif

#define LOG_VERBOSE _LOG_VERBOSE, typeid(this).name(), __func__, __LINE__
#define LOG_NOTICE  _LOG_NOTICE, typeid(this).name(), __func__, __LINE__
#define LOG_WARNING _LOG_WARNING, typeid(this).name(), __func__, __LINE__
#define LOG_ERROR _LOG_ERROR, typeid(this).name(), __func__, __LINE__
#define LOG_FATAL _LOG_FATAL, typeid(this).name(), __func__, __LINE__

#define ofxLogNotice() ofxLog(LOG_NOTICE)
#define ofxLogVerbose() ofxLog(LOG_VERBOSE)
#define ofxLogWarning() ofxLog(LOG_WARNING)
#define ofxLogError() ofxLog(LOG_ERROR)
#define ofxLogFatal() ofxLog(LOG_FATAL)

// helper defines for different log levels

inline ostream& operator<<(ostream& os, ofRectangle &r){
    os << "(" << r.x << ", " << r.y << ", " << r.width << ", " << r.height << ")";
    return os;
};

inline ostream& operator<<(ostream& os, ofPoint &p){
    os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return os;
};

bool ofxLogCloseLogFile();
bool ofxLogOpenLogFile(string filePath);
void ofxLogSetLogLevel(LogLevel l, string className, string funcName, int lineNum);
void ofxLogSetLogLevel(LogLevel l, string className, string funcName, int lineNum, void * t);
void ofxLogSetLogFilePath(string filePath);
void ofxLogSetLogToFile(bool b, string filePath);
void ofxSetLogDate(bool b);
void ofxSetLogTime(bool b);
void ofxSetLogCaller(bool b);
void ofxSetLogType(bool b);
void ofxSetLogLineNumber(bool b);
void ofxSetLogTimeBetweenLogs(bool b);
void ofxSetLogAutoPad(bool b);
void ofxSetLogOptions(int options);

using namespace std;

class ofxLog{

public:

    ofxLog();
    ~ofxLog();
    
    ofxLog(LogLevel l, string className, string funcName, int lineNum);
    
    ofxLog(LogLevel l, string className, string funcName, int lineNum, string msg);
    
    ofxLog(LogLevel l, string className, string funcName, int lineNum, const char* format, ...);
    
    // catch << pipe operator
    template <class T> 
    inline ofxLog& operator<<(const T& value){
        clogMessage << value;// << padding;
        return *this;
    }
    
    template <class T>
    inline ofxLog& operator<<(const vector<T> &value){
        for(int i = 0; i < value.size(); i++){
            clogMessage << value[i] << (i == value.size() - 1 ? "" : ", ");
        }
        return *this;
    }

    template <class T>
    inline ofxLog& operator<<(const deque<T> &value){
        for(int i = 0; i < value.size(); i++){
            clogMessage << value[i] << (i == value.size() - 1 ? "" : ", ");
        }
        return *this;
    }
    
    // catch the << ostream function pointers such as std::endl and std::hex
    ofxLog& operator<<(std::ostream& (*func)(std::ostream&)){
        // check if it's the std::endl 
        // TODO: check if this is ok in VS - see: http://stackoverflow.com/questions/2319544/comparing-address-of-stdendl
//        if(func == static_cast< std::ostream& (*)(std::ostream&) > ( &std::endl<char, std::char_traits<char> >)){
//            func(clogMessage);
//            end();
//        }else{
//            func(clogMessage);
//        }
        func(clogMessage);
        return *this;
    }
    
    ofxLog& log(LogLevel l, string className, string funcName, int lineNum);
    void log(LogLevel l, string className, string funcName, int lineNum, string msg);
    
private:

    bool bLogged;
    
    string getLogLevelName(LogLevel l);
	void pad(stringstream &logMessage);
    void formatOptions(bool &hasOptions, stringstream &logMessage);
    char* getTimeStamp();
    char* getDateStamp();

    char timestamp[255];
    char datestamp[255];
            
    // temp vars for std::cout like logging
    string clogClass;
    string clogFunc;
    int clogLine;
    stringstream clogMessage;

};

#endif
