#pragma once

#include "ofMain.h"

#define FACE_NUM 9

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void exit();
    
    ofDirectory dir;
    string dirName;
    string completedDirName;
    vector<ofImage> images;
    vector<int> ID;
    vector<bool> isLabeled;
    
    int currentImage;
    bool isFinished = false;
    
    vector<ofRectangle> rect;
    
    string categories[FACE_NUM] = {
                            " 0-15 Male (1)", " 0-15 Female (2)",
                            "16-29 Male (3)", "16-29 Female (4)",
                            "30-49 Male (5)", "30-49 Female (6)",
                            "50-   Male (7)", "50-   Female (8)",
                            "no face\nmulti faces (9)",                      };
    
    string str;
    
    const string PATH = "../data/images/faces20171228/";
    
};


