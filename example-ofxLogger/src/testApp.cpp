#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofxLogSetLogLevel(LOG_VERBOSE);
    
    ofxLog(LOG_VERBOSE, "Setting up and logging bracket style");
    
    ofxLog(LOG_NOTICE) << "Logging 'cout' style" << endl;
    
    ofxLogNotice() << "You can log to a file" << endl;
    
    ofxLogSetLogToFile(true);  // defaults to log.txt in your data path
    
    // ofxLogSetLogToFile(true, ofToDataPath("somepath/logName.txt") lets you set a custom path
    // OR
    // ofxLogSetLogFilePath(ofToDataPath("somepath/logName.txt"));
    // ofxLogSetLogOptions(LOG_USE_TIME | LOG_USE_CALL | LOG_USE_TYPE | LOG_USE_PADD | LOG_USE_FILE);
    // see below for option settings
    
    ofxLogNotice() << "You can set different options for the log messages" << endl;
    ofxLogNotice() << "Options are:" << endl;
    
    ofxLogNotice() << "LOG_USE_DATE - adds date to the log" << endl;
    ofxLogNotice() << "LOG_USE_TIME - adds time to the log" << endl;
    ofxLogNotice() << "LOG_USE_CALL - adds the class & function names to the log" << endl;
    ofxLogNotice() << "LOG_USE_TYPE - adds the log level type to the log" << endl;
    ofxLogNotice() << "LOG_USE_DIFF - adds time diff between logs to the log" << endl;
    ofxLogNotice() << "LOG_USE_LINE - adds line numbers to the log" << endl;
    ofxLogNotice() << "LOG_USE_PADD - adds padding to the log" << endl;
    ofxLogNotice() << "LOG_USE_FILE - logs to file" << endl;
    
    ofxLogNotice() << "Default setting is: LOG_USE_TIME | LOG_USE_CALL | LOG_USE_TYPE | LOG_USE_PADD" << endl;
    
    ofxLogNotice() << "You can set one option at a time using the setLog[SomeOption] nomenclature" << endl;
    
    ofxLogSetLogLineNumber(true);
    ofxLogSetLogCaller(false);
    ofxLogSetLogTimeBetweenLogs(true);
    
    ofxLogNotice() << "(<---NOISE) Time between logs can be reported in micro seconds" << endl;
    ofxLogNotice() << "But as you can see the first call is always going to give you some NOISE" << endl;
    
    ofSleepMillis(2500);
    
    ofxLogNotice() << "...As the time between log calls reports microsends between calls...Finsihed waiting 2500 millis or 2500000 micros" << endl;
    
    ofxLogNotice() << "Or you can set several options at once with the setLogOptions nomenclature" << endl;
    
    ofxLogSetLogOptions(LOG_USE_TIME | LOG_USE_CALL | LOG_USE_TYPE | LOG_USE_PADD);
    
    ofxLog(LOG_WARNING) << "Just closed the log file because we reset all options without LOG_USE_FILE" << endl;
    
    ofxLog(LOG_VERBOSE) << "And of course log levels are easy too..." << endl;
    
    ofxLog(LOG_ERROR) << "This is not an error!!" << endl;
    
    ofxLog(LOG_VERBOSE, "HEEEEELLLLLLOOOOO");
    
    ofxLog(LOG_VERBOSE) << "HEEELLLLYESSS" << "88" << 99 << endl;
    
    // and you can do sprintf style too ;)
    
    ofxLog(LOG_VERBOSE, "Does Sprintf %s %i", "style", 2);
    
    ofSetFrameRate(12);
    
}

//--------------------------------------------------------------
void testApp::update(){
    //ofxLog(LOG_VERBOSE) << "Update" << endl;
}

//--------------------------------------------------------------
void testApp::draw(){
    //ofxLog(LOG_NOTICE) << "Draw" << endl;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    ofxLogNotice() << "Key pressed " << key << endl;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    ofxLogNotice() << "Key released " << key << endl;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    ofxLogNotice() << "Mouse moved " << x << " " << y << endl;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    ofxLogNotice() << "Mouse dragged " << x << " " << y << endl;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    ofxLogNotice() << "Mouse pressed " << x << " " << y << endl;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    ofxLogNotice() << "Mouse released " << x << " " << y << endl;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    ofxLogNotice() << "Window resized " << w << " " << h << endl;
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}