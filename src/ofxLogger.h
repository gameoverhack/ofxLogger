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
	LOG_VERBOSE,
	LOG_NOTICE,
	LOG_WARNING,
	LOG_ERROR,
	LOG_FATAL_ERROR
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

// helper defines for different log levels
#define LOGGER                  ofxLoggerSingleton::Instance()
#define SETLOGLEVEL(level)      ofxLoggerSingleton::Instance()->setLogLevel(level)
#define SETLOGOPTIONS(options)  ofxLoggerSingleton::Instance()->setLogOptions(options)
#define LOG(level, str)         ofxLoggerSingleton::Instance()->log(level, typeid(*this).name(), __func__, __LINE__, (str))
#define LOG_ERROR(str)          LOG(LOG_ERROR, (str))
#define LOG_WARNING(str)        LOG(LOG_WARNING, (str))
#define LOG_NOTICE(str)         LOG(LOG_NOTICE, (str))
#define LOG_VERBOSE(str)        LOG(LOG_VERBOSE, (str))

#define CLOG(level)     ofxLoggerSingleton::Instance()->clog(level, typeid(*this).name(), __func__, __LINE__)
#define CLOG_ERROR		CLOG(LOG_ERROR)
#define CLOG_WARNING	CLOG(LOG_WARNING)
#define CLOG_NOTICE     CLOG(LOG_NOTICE)
#define CLOG_VERBOSE	CLOG(LOG_VERBOSE)

#define LOG_OPEN_FILE	ofxLoggerSingleton::Instance()->openLogFile
#define LOG_CLOSE_FILE	ofxLoggerSingleton::Instance()->closeLogFile()

#define LOGTRICK_VERBOSE LOG_VERBOSE, typeid(this).name(), __func__, __LINE__
#define LOG_INFO typeid(this).name(), __func__, __LINE__

using namespace std;

class ofxLogger {

public:

    ofxLogger();
    ~ofxLogger();
    
    // catch << pipe operator
    template <class T> 
    ofxLogger& operator<<(const T& value){
        clogMessage << value;// << padding; 
        return *this;
    }
    
    // catch the << ostream function pointers such as std::endl and std::hex
    ofxLogger& operator<<(std::ostream& (*func)(std::ostream&)){
        // check if it's the std::endl TODO: check if this is ok in VS - see: http://stackoverflow.com/questions/2319544/comparing-address-of-stdendl
        if(func == static_cast< std::ostream& (*)(std::ostream&) > ( &std::endl<char, std::char_traits<char> >)){
            clend();
        }else{
            func(clogMessage);
        }
        return *this;
    }
    
    ofxLogger& clog(LogLevel l, string className, string funcName, int lineNum);
    
    void clend();
    
    void log(LogLevel l, string className, string funcName, int lineNum, string msg);
    
	void setLogLevel(LogLevel l);
    
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
class Singleton
{
public:
    static T* Instance() {
        if(!m_pInstance) m_pInstance = new T;
        assert(m_pInstance !=NULL);
        return m_pInstance;
    }
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

static ofxLogger & ofxLog = *(ofxLoggerSingleton::Instance());

#endif
