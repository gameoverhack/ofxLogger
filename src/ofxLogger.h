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

#ifndef _H_OFXLOGGER
#define _H_OFXLOGGER

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
    LOG_USE_FILE = 0x80         // 128 - log to file
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

using namespace std;

class ofxLogger{

public:

    ofxLogger();
    ~ofxLogger();
    
    // catch << pipe operator
    template <class T> 
    inline ofxLogger& operator<<(const T& value){
        clogMessage << value;// << padding; 
        return *this;
    }
    
    template <class T>
    inline ofxLogger& operator<<(const vector<T> &value){
        for(int i = 0; i < value.size(); i++){
            clogMessage << value[i] << (i == value.size() - 1 ? "" : ", ");
        }
        return *this;
    }

    template <class T>
    inline ofxLogger& operator<<(const deque<T> &value){
        for(int i = 0; i < value.size(); i++){
            clogMessage << value[i] << (i == value.size() - 1 ? "" : ", ");
        }
        return *this;
    }
    
    // catch the << ostream function pointers such as std::endl and std::hex
    ofxLogger& operator<<(std::ostream& (*func)(std::ostream&)){
        // check if it's the std::endl 
        // TODO: check if this is ok in VS - see: http://stackoverflow.com/questions/2319544/comparing-address-of-stdendl
        if(func == static_cast< std::ostream& (*)(std::ostream&) > ( &std::endl<char, std::char_traits<char> >)){
            end();
        }else{
            func(clogMessage);
        }
        return *this;
    }
    
    ofxLogger& log(LogLevel l, string className, string funcName, int lineNum);
    void log(LogLevel l, string className, string funcName, int lineNum, string msg);
    
	void setLogLevel(LogLevel l, string className, string funcName, int lineNum); // accepts dummy values so we can use the defines for logLevel!!!
    
    void setLogFilePath(string filePath);
    void setLogToFile(bool b, string filePath = ofToDataPath("log.txt"));
    
    void setLogDate(bool b);
    void setLogTime(bool b);
    void setLogCaller(bool b);
    void setLogType(bool b);
    void setLogLineNumber(bool b);
    void setLogTimeBetweenLogs(bool b);
    void setLogAutoPad(bool b);
    
    void setLogOptions(int options);
    
private:

    void end();
    
    bool openLogFile(string fileName);
    bool closeLogFile();
    
    string getLogLevelName(LogLevel l);
	void pad(stringstream &logMessage);
    void formatOptions(bool &hasOptions, stringstream &logMessage);
    char* getTimeStamp();
    char* getDateStamp();

	int padLength;

    ofstream logFile;
    string logFilePath;

    char timestamp[255];
    char datestamp[255];
            
    // temp vars for std::cout like logging
    LogLevel clogLevel;
    string clogClass;
    string clogFunc;
    int clogLine;
    stringstream clogMessage;
    
	LogLevel logLevel;
    int logOptions;
	int lastLogTime;

};

// Singleton definition
#ifndef __SINGLETON_HPP_
#define __SINGLETON_HPP_
#include "assert.h"
#include <cstdlib>
template <class T>
class Singleton{
public:
    static T* Instance() {
        if(!m_pInstance) m_pInstance = new T;
        assert(m_pInstance !=NULL);
        return m_pInstance;
    };
protected:
    Singleton();
    ~Singleton();
private:
    Singleton(Singleton const&);
    Singleton& operator=(Singleton const&);
    static T* m_pInstance;
};
template <class T> T* Singleton<T>::m_pInstance=NULL;
#endif

typedef Singleton<ofxLogger> ofxLoggerSingleton;   // Global declaration

static ofxLogger & _ofxLog = *(ofxLoggerSingleton::Instance()); // guess this could be an auto ptr

// PUBLIC C-STYLE INTERFACE

static void ofxLogSetLogLevel(LogLevel l, string className, string funcName, int lineNum){ // accepts dummy values so we can use the defines for logLevel!!!
    _ofxLog.setLogLevel(l, className, funcName, lineNum);
};

static void ofxLogSetLogFilePath(string filePath){
    _ofxLog.setLogFilePath(filePath);
};

static void ofxLogSetLogToFile(bool b, string filePath = ofToDataPath("log.txt")){
    _ofxLog.setLogToFile(b, filePath);
};

static void ofxLogSetLogDate(bool b){
    _ofxLog.setLogDate(b);
};

static void ofxLogSetLogTime(bool b){
    _ofxLog.setLogTime(b);
};

static void ofxLogSetLogCaller(bool b){
    _ofxLog.setLogCaller(b);
};

static void ofxLogSetLogType(bool b){
    _ofxLog.setLogType(b);
};

static void ofxLogSetLogLineNumber(bool b){
    _ofxLog.setLogLineNumber(b);
};

static void ofxLogSetLogTimeBetweenLogs(bool b){
    _ofxLog.setLogTimeBetweenLogs(b);
};

static void ofxLogSetLogAutoPad(bool b){
    _ofxLog.setLogAutoPad(b);
};

static void ofxLogSetLogOptions(int options){
    _ofxLog.setLogOptions(options);
};

class ofxLog{
    
public:
    
    ofxLog(){};
    
    // catch << pipe operator
    template <class T> 
    ofxLog& operator<<(const T& value){
        _ofxLog << value;
        return *this;
    }
    
    // catch the << ostream function pointers such as std::endl and std::hex
    ofxLog& operator<<(std::ostream& (*func)(std::ostream&)){
        // check if it's the std::endl 
        // TODO: check if this is ok in VS - see: http://stackoverflow.com/questions/2319544/comparing-address-of-stdendl
        _ofxLog << func;
        return *this;
    }
    
    ofxLog(LogLevel l, string className, string funcName, int lineNum){
        _ofxLog.log(l, className, funcName, lineNum);
    }
    
    ofxLog(LogLevel l, string className, string funcName, int lineNum, string msg){
        _ofxLog.log(l, className, funcName, lineNum, msg);
    };
    
    ofxLog(LogLevel l, string className, string funcName, int lineNum, const char* format, ...){
        va_list args;
        char buffer[256];
        va_start( args, format );
        vsprintf (buffer, format, args);
        va_end( args );
        _ofxLog.log(l, className, funcName, lineNum, string(buffer));
    }
    
    ~ofxLog(){
        //ofxLog.end(); // would force endlines as per ofLog but i like calling them explicitly :)
    }
    
};

#endif
