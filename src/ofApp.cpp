#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//env
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    
//Open the Open File Dialog
    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a jpg or png");
    
    //Check if the user opened a file
    if (openFileResult.bSuccess)
    {
        ofLogVerbose("User selected a file");
    }else {
        ofLogVerbose("User hit cancel");
    }
    
    dirName = openFileResult.getPath();
    vector<string> eachWords = ofSplitString(dirName, "/");
    ofStringReplace(dirName, eachWords.back(), "");
    
    completedDirName = dirName;
    vector<string> eachWords2 = ofSplitString(completedDirName, "/");
    eachWords2.pop_back();
    ofStringReplace(completedDirName, eachWords2.back()+"/", "");
    completedDirName += "completed";

    cout << dirName << endl;
    cout << completedDirName << endl;

    
// load images
    dir.listDir(dirName);
    dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
    
    //allocate the vector to have as many ofImages as files
    if( dir.size() )
    {
        images.assign(dir.size(), ofImage());
        ID.assign(dir.size(), -1);
        isLabeled.assign(dir.size(), false);
    }
    
    // you can now iterate through the files and load them into the ofImage vector
    for(int i = 0; i < (int)dir.size(); i++)
    {
        images[i].load(dir.getPath(i));
    }
    currentImage = 0;
    
    ofBackground(255);
    
//rectangle setup
    rect.resize(FACE_NUM);
    
    for(int i=0; i<FACE_NUM; i++)
    {
        rect[i].setSize(150, 50);
        rect[i].setPosition(600 + 200*(i%2), 50 + 100*(i/2));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
//draw image and caption
    if (dir.size() > 0)
    {
        ofSetColor(255);
        if(images[currentImage].getWidth() > 800)
        {
            float ratio = ofMap(images[currentImage].getWidth(), 800, 4000, 0.5, 0.15);
            images[currentImage].draw(50,50, images[currentImage].getWidth()*ratio, images[currentImage].getHeight()*ratio);
        }
        else if (images[currentImage].getWidth() < 200)
        {
            images[currentImage].draw(50,50, images[currentImage].getWidth()*2, images[currentImage].getHeight()*2);
        }
        else
        {
            images[currentImage].draw(50,50, images[currentImage].getWidth()*0.75, images[currentImage].getHeight()*0.75);
        }
        
        ofSetColor(0);
        string pathInfo = dir.getName(currentImage) + " " + dir.getPath(currentImage) + "\n\n" +
        "down key to next image, up key to prev image\n\n" +
        "click rect or press key to set the label\n\n" +
        ofToString(currentImage+1) + " / " + ofToString(dir.size());
        ofDrawBitmapString(pathInfo, 50, 600);
    }
    
//draw face type
    ofSetColor(0);
    for(int i=0; i<rect.size(); i++)
    {
        ofColor c = i%2 ? ofColor(234,145,152) : ofColor(143,221,232);  //pink, sky blue
        if(i==(rect.size()-1)) c = ofColor(200);
        ofSetColor(c);
        ofDrawRectangle(rect[i]);
        ofSetColor(0);
        ofDrawBitmapString(categories[i], rect[i].getCenter().x - 65, rect[i].getCenter().y);
    }
    
//draw labeling result
    if(ID[currentImage] >= 0) ofDrawBitmapStringHighlight(categories[ID[currentImage]], 20, 20);
    
//finished
    if(isFinished)
    {
        ofSetColor(255, 200);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofDrawBitmapStringHighlight("task is completed, please press esc key to finish. thanks!", ofGetWidth()/2-200, ofGetHeight()/2);
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    //delete unlabeled images
    if(isFinished)
    {
        for(int i=0; i<dir.size(); i++)
        {
            if(ID[i] == 8 || ID[i] == -1)
            {
                string name = dir.getPath(i);
                string cmd = "rm -f "+name;   // create the command
                system(cmd.c_str());
                cout << "deleted: " << name << endl;
            }
        }
        
        //move completed directory
        string cmd = "mv " + completedDirName + "/ " + dirName;   // create the command
        system(cmd.c_str());
        cout << "moved　directory" << endl;
    }
    else
    {
        //save as txt
        string fileName = ofGetTimestampString() + ".txt";
        ofBuffer buffer = ofBuffer( str );
        ofBufferToFile( fileName, buffer );
        cout << "saved" << endl;
        
        ofDirectory completedDir(completedDirName);
        if(!completedDir.exists()){
            completedDir.create(true);
            cout << "created" << endl;
        }
        
        for(int i=0; i<dir.size(); i++)
        {
            if(isLabeled[i])
            {
                if(ID[i] == 8 || ID[i] == -1)
                {
                    string name = dir.getPath(i);
                    string cmd = "rm -f "+name;   // create the command
                    system(cmd.c_str());
                    cout << "deleted: " << name << endl;
                }
                
                string name = dir.getPath(i);
                string cmd = "mv "+ name + " " + completedDirName;   // create the command
                system(cmd.c_str());
                cout << "moved: " << name << endl;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //labeling
    if(key == '1')
    {
        ID[currentImage] = 0;
    }
    else if(key == '2')
    {
        ID[currentImage] = 1;
    }
    else if(key == '3')
    {
        ID[currentImage] = 2;
    }
    else if(key == '4')
    {
        ID[currentImage] = 3;
    }
    else if(key == '5')
    {
        ID[currentImage] = 4;
    }
    else if(key == '6')
    {
        ID[currentImage] = 5;
    }
    else if(key == '7')
    {
        ID[currentImage] = 6;
    }
    else if(key == '8')
    {
        ID[currentImage] = 7;
    }
    else if(key == '9')
    {
        ID[currentImage] = 8;
    }
    
    if (key == OF_KEY_DOWN && dir.size() > 0 && !isFinished)
    {
        //add txt data
        if(ID[currentImage] != 8 && ID[currentImage] != -1 && !isLabeled[currentImage])
        {
//            str += dir.getPath(currentImage) + " ";
            str += PATH + dir.getName(currentImage) + " ";
            str += ofToString(ID[currentImage]);
            str += "\n";
            cout << "labeling succeeded: " << currentImage+1 << endl;
            
            isLabeled[currentImage] = true;
        }
        
        //next image
        currentImage++;
        
        //finish
        if(currentImage == dir.size())
        {
            cout << "ended" << endl;
            currentImage = 0;
            isFinished = true;
            
            //save as txt
            string fileName = ofGetTimestampString() + ".txt";
            ofBuffer buffer = ofBuffer( str );
            ofBufferToFile( fileName, buffer );
            cout << "saved" << endl;
        }
        else
        {
            currentImage %= dir.size();
        }
    }
    
    //prev image
    if(key == OF_KEY_UP && currentImage > 0 && !isFinished)
    {
        currentImage--;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //labeling
    for(int i=0; i<rect.size(); i++)
    {
        if(rect[i].inside(x, y))
        {
            ID[currentImage] = i;
            cout << i << endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

