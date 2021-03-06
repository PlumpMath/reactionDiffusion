#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    width = ofGetWidth();
    height = ofGetHeight();
    readFilesDirectory();
    ofEnableSmoothing();
    restartButton.addListener(this, &ofApp::restartButtonPressed);
    if (!ofIsGLProgrammableRenderer()) {
        ofLogError("this app supports only open the programmable render pipeline");
        return 1;
    } else {
        shader.load(shadersFolder+"/passthru.vert", shadersFolder+"/grayscott.frag");
    };
    clearBuffersAndAllocate();
    addGui();
}

//--------------------------------------------------------------
void ofApp::update(){
    for( int i = 0; i < nPasses ; i++ ){
        pingPong.dst->begin();
        shader.begin();
        shader.setUniformTexture("prevTexture", pingPong.src->getTexture(), 0 );
        shader.setUniformTexture("tex0", output.getTexture(), 1 );
        shader.setUniform1f( "ru", ru);
        shader.setUniform1f( "rv", rv);
        shader.setUniform1f( "f", f );
        shader.setUniform1f( "k", k );
        pingPong.src->draw(0, 0); // draw the source texture here!!!
        shader.end();
        pingPong.dst->end();
        pingPong.swap();
    }
    pingPong.swap();
}

//--------------------------------------------------------------
void ofApp::draw(){
    pingPong.src->draw(0,0);
    maybeDrawGui();
}

void ofApp::clearBuffersAndAllocate(){
    // to use an image as source instead the mouse click, uncomment this:
    if (useImage) {
        image.load("img2.jpg");
        width = image.getWidth();
        height = image.getHeight();
    }
    output.allocate(width, height, GL_RGBA);
    pingPong.allocate(width, height, GL_RGBA);
    pingPong.clear();
    pingPong.src->begin();
    if (useImage) {
        image.draw(0,0, width, height);
    }
    pingPong.src->end();

    output.begin();
    ofClear(0, 0, 0, 255);
    output.end();
}

// GUI
void ofApp::addGui(){
    gui.setup();
    gui.setPosition(ofPoint(0, 30));
    gui.add(ru.setup("ru", 0.25, 0.09, 0.26));
    gui.add(rv.setup("rv", 0.04, 0.04, 0.16));
    gui.add(f.setup("feed", 0.0195, 0.018, 0.060));
    gui.add(k.setup("k", 0.066, 0.050, 0.070));

    gui.add(nPasses.setup("passes", 10, 1, 30));
    gui.add(radius.setup("radius", 10, 3, 200));
    gui.add(restartButton.setup("restart"));
}

void ofApp::maybeDrawGui(){
    if(!hideGui){
        gui.draw();
        ofPushStyle();
        string displayGui = "press 'g' to toggle the gui, up and down to change presets";
        ofDrawBitmapString(displayGui, 0, 10);
        //fps
        string msg = "\n\nfps: " + ofToString(ofGetFrameRate(), 2);
        ofDrawBitmapString(msg, 0, 0);
        // file selection
        for (unsigned i = 0; i < fileNames.size(); ++i){
            if (i == selected) ofSetColor(0, 255, 0);
            else ofSetColor(0, 0, 255);
            ofDrawBitmapString(fileNames[i], 0, (gui.getHeight()+20)
                               +(20 * (i + 1)));
        }
        ofPopStyle();
    }
}

void ofApp::restartButtonPressed(){
    clearBuffersAndAllocate();
}

void ofApp::readFilesDirectory(){
    selected = 0;
    dir.listDir("presets");
    dir.sort();
    for(auto file:dir){
        fileNames.push_back(file.path());
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'g':
            hideGui = !hideGui;
            break;
        case 's':
            gui.saveToFile("settings.json");
            break;
        case OF_KEY_RETURN:
            gui.loadFromFile(fileNames[selected]);
            break;

        case OF_KEY_UP:
            selected = (selected - 1);
            if (selected < 0) selected = fileNames.size() - 1;
            break;

        case OF_KEY_DOWN:
            selected = (selected + 1) % fileNames.size();
            break;
        default:
            break;
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
    if(mouseDown){
        drawInSource(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    mouseDown = true;
    drawInSource(x, y);
}

void ofApp::drawInSource(int x, int y){
    pingPong.src->begin();
    ofSetColor(ofNoise( ofGetElapsedTimef()*0.2 )*255);
    //ofSetColor(0,255,0, 255);
    ofDrawCircle(x, y, radius);
    //ofDrawRectangle(x, y, 10, 2);
    pingPong.src->end();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    mouseDown = true;
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
