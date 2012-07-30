#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofxLog.setLogLevel(LOG_VERBOSE);
    
    LOG(LOG_VERBOSE, "Setting up and logging bracket style");
    
    CLOG(LOG_NOTICE) << "Logging 'cout' style" << endl;
    
    CLOG(LOG_NOTICE) << "You can log to a file" << endl;
    
    ofxLog.setLogToFile(true);  // defaults to log.txt in your data path
    
    // ofxLog.setLogToFile(true, ofToDataPath("somepath/logName.txt") lets you set a custom path
    // OR
    // ofxLog.setLogFilePath(ofToDataPath("somepath/logName.txt"));
    // ofxLog.setLogOptions(LOG_USE_TIME | LOG_USE_CALL | LOG_USE_TYPE | LOG_USE_PADD | LOG_USE_FILE);
    // see below for option settings
    
    CLOG(LOG_NOTICE) << "You can set different options for the log messages" << endl;
    CLOG(LOG_NOTICE) << "Options are:" << endl;
    
    CLOG(LOG_NOTICE) << "LOG_USE_DATE - adds date to the log" << endl;
    CLOG(LOG_NOTICE) << "LOG_USE_TIME - adds time to the log" << endl;
    CLOG(LOG_NOTICE) << "LOG_USE_CALL - adds the class & function names to the log" << endl;
    CLOG(LOG_NOTICE) << "LOG_USE_TYPE - adds the log level type to the log" << endl;
    CLOG(LOG_NOTICE) << "LOG_USE_DIFF - adds time diff between logs to the log" << endl;
    CLOG(LOG_NOTICE) << "LOG_USE_LINE - adds line numbers to the log" << endl;
    CLOG(LOG_NOTICE) << "LOG_USE_PADD - adds padding to the log" << endl;
    CLOG(LOG_NOTICE) << "LOG_USE_FILE - logs to file" << endl;
    
    CLOG(LOG_NOTICE) << "Default setting is: LOG_USE_TIME | LOG_USE_CALL | LOG_USE_TYPE | LOG_USE_PADD" << endl;
    
    CLOG(LOG_NOTICE) << "You can set one option at a time using the setLog[SomeOption] nomenclature" << endl;
    
    ofxLog.setLogLineNumber(true);
    ofxLog.setLogCaller(false);
    ofxLog.setLogTimeBetweenLogs(true);
    
    CLOG(LOG_NOTICE) << "(<---NOISE) Time between logs can be reported in micro seconds" << endl;
    CLOG(LOG_NOTICE) << "But as you can see the first call is always going to give you some NOISE" << endl;
    
    ofSleepMillis(2500);
    
    CLOG(LOG_NOTICE) << "...As the time between log calls reports microsends between calls...Finsihed waiting 2500 millis or 2500000 micros" << endl;
    
    CLOG(LOG_NOTICE) << "Or you can set several options at once with the setLogOptions nomenclature" << endl;
    
    ofxLog.setLogOptions(LOG_USE_TIME | LOG_USE_CALL | LOG_USE_TYPE | LOG_USE_PADD);
    
    CLOG(LOG_WARNING) << "Just closed the log file because we reset all options without LOG_USE_FILE" << endl;
    
    CLOG(LOG_VERBOSE) << "And of course log levels are easy too..." << endl;
    
    CLOG(LOG_ERROR) << "This is not an error!!" << endl;
    
    ofxLog.clog(LOG_VERBOSE, typeid(this).name(), __func__, __LINE__) << "This is what the macro unpacks to...so..." << endl;
    
    ofxLog.clog(LOGTRICK_VERBOSE) << "...I guess you could do this if you wanted the macro in a different place..." << endl;
    
    // where LOGTRICK_VERBOSE is defined as LOG_VERBOSE, typeid(this).name(), __func__, __LINE__
    // obviously you wouldn't call it LOGTRICK, but actually define each 
    // LOG_VERBOSE, LOG_NOTICE, LOG_WARNING etc to pass the macro...
    // this kind of invisibles the macro action which depending
    // on your brain is either a good thing or a bad thing
    
    ofxLog.clog(LOG_VERBOSE, LOG_INFO) << "...or even like this if it suits your style" << endl;
    
    // where LOG_INFO is defined as typeid(this).name(), __func__, __LINE__
    // again, this kind of invisibles the macro action which depending
    // on your brain is either a good thing or a bad thing
    
    ofSetFrameRate(12);
    
}

//--------------------------------------------------------------
void testApp::update(){
    //CLOG(LOG_VERBOSE) << "Update" << endl;
}

//--------------------------------------------------------------
void testApp::draw(){
    //CLOG(LOG_NOTICE) << "Draw" << endl;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    CLOG(LOG_NOTICE) << "Key pressed " << key << endl;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    CLOG(LOG_NOTICE) << "Key released " << key << endl;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    CLOG(LOG_NOTICE) << "Mouse moved " << x << " " << y << endl;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    CLOG(LOG_NOTICE) << "Mouse dragged " << x << " " << y << endl;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    CLOG(LOG_NOTICE) << "Mouse pressed " << x << " " << y << endl;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    CLOG(LOG_NOTICE) << "Mouse released " << x << " " << y << endl;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    CLOG(LOG_NOTICE) << "Window resized " << w << " " << h << endl;
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}