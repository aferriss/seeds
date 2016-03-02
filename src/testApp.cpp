#include "testApp.h"
#include <iostream>
#include <vector>
float theta = 0.0;
float g3 = 150;
//cp -r bin/data "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources";  

//--------------------------------------------------------------
void testApp::setup(){
    w = 866;
    h = 900;
    
    //ofSeedRandom(1000);
    
    inc = 0.000001;
    
    ofSetWindowShape(w, h);
    
    //ofSetDataPathRoot("../Resources/data");
        
    noiseShader.load("noise");
    reposShader.load("repos");
    emboss.load("emboss");
    
    sheepImg.loadImage("bighornsheep1.jpg");
    
    finishIt = false;
    numRandSeeds = 1;
    step = 0;
    steps = w*h;
    
    //ofSetFrameRate(80);
    ofSetBackgroundAuto(false);
    ofEnableAlphaBlending();
    //ofSetBackgroundColor(0);
    ofBackground(0);
    
    
    //noiseFbo.allocate(w,h,GL_RGBA);
    reposFbo.allocate(w,h,GL_RGBA);
    //feedback.allocate(w,h,GL_RGBA);
    //embossFbo.allocate(w, h,GL_RGBA);
    
    
    fbPix.allocate(w,h, 4);
    
    noiseFbo.setUseTexture(true);
    reposFbo.setUseTexture(true);
    embossFbo.setUseTexture(true);
    
    screen.grabScreen(0, 0, w, h);
    screen.setUseTexture(true);
    
    traversed = new bool[w*h];
    
    
    noise = new ofxPerlin();
    noise->noiseDetail(10, 0.0025);
	simplex = new ofxSimplex();
    
    //ofSetVerticalSync(true);
    
    //reposFbo.begin();
    //ofClear(255,255,255,0);
    //reposFbo.end();
    ofSetMinMagFilters(GL_NEAREST,GL_NEAREST);
    
    save = false;
    turnBlack = true;
    rr = 128;
    speed =1.0;
    
    //soundStream.setup(this, 2, 0, 44100, w*h, 4);
    //sampleCount = 100;
    
    pix = new unsigned char[w*h];
    
    vidRecorder.setVideoCodec("mpeg4"); 
    vidRecorder.setVideoBitrate("8000k");
    
    ofDisableArbTex();

}

//--------------------------------------------------------------
void testApp::update(){
    //ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    /*
    if(rr<0 || rr>255){
        speed*=-1;
    }
    rr+=speed;
    */
    
    
    if(save == true){
        //sort.grabScreen(0, 0, w, h);
        //saver.addFrame(screen.getPixels());
        //vidRecorder.addFrame(sort.getPixelsRef());
        ofPixels savePix;
        reposFbo.readToPixels(savePix);
        ofSaveImage(savePix, ofGetTimestampString()+".png");
        save = false;
    }
    
    
    //screen.update();
    //reposFbo.readToPixels(fbPix);
    //sort.grabScreen(0, 0, w, h);
    // pix = screen.getPixels();
    //sort.setFromPixels(spix, w, h, OF_IMAGE_COLOR);
    //unsigned char * pix = sort.getPixels();
    
    /*
    for(int y = 0; y<h; y++){
        for(int x = 0; x<w; x++){
            int loc = (x+y*w)*3;
            int r = loc;
            int g = loc+1;
            int b = loc+2;
            
            if(pix[loc]<210 && pix[loc]>2){
            if(pix[loc-3*w] > pix[loc] ){
            //swap(pix, r, r-3*w, g, g-3*w, b, b-3*w);
            //swap(pix, r, r+3, g, g+3, b, b+3);
            }
            }
        }
    }
    
     
        for(int y = 0; y<w*h - 3*w; y++){
    
            int loc = y*3;
            int r = loc;
            int g = loc+1;
            int b = loc+2;
            
            if(pix[loc] <150 && pix[loc] >5){
            if(pix[loc-w*3] > pix[loc]){
                swap(pix, r, r+w*3, g, g+w*3, b, b+w*3);
                swap(pix, r, r-3, g, g-3, b, b-3);
            }
        
            }
            
            if( loc-3 > 0 && pix[loc] > inc ){
                if(pix[loc-3] > pix[loc/(w)] ){
               //     swap(pix, r, r-3, g, g-3, b, b-3);
                }
            }
            
            
        }
    */
    
    
    //cout<<ofToString(inc)<<endl;
    //sort.setFromPixels(pix, w, h, OF_IMAGE_COLOR);
    //sort.update();
    //screen.update();
    //ofSetMinMagFilters(GL_NEAREST,GL_NEAREST);
    /*
    if(ofGetFrameNum()%40 == 0){
        inc++;
        inc = pow(inc,1.08f);
        //inc = (sin(2*PI*(inc-(1/4)))+1)/2;
       
    }
    
    
    if(inc >255){
        inc = 0.00001;
    }
    */
    
}


//--------------------------------------------------------------
void testApp::draw(){
    //sort.draw(w,0);
    
    //glEnable(GL_BLEND);
    
    //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
    
    //ofSetMinMagFilters(GL_NEAREST,GL_NEAREST);
    //ofPushStyle();
    
    //ofScale(2.5,2.5);
    //glScalef(2,2,1);
    //glScalef(2, -2, 1); // Invert Y axis so increasing Y goes down  
    //glTranslatef(0, -h, 0);

    
    

    
    if(step == 0){
        cout<<"finding edges"<<endl;
        
        for(int i = 0; i<=numRandSeeds; i++){
            int seed = int(ofRandom(w*h));
            seeds.push_back(seed);
            traversed[seed] = true;
        }
    }
    else{
        updateSeeds();
    }
    
    if(step < steps){
        step++;
        //cout<<ofToString(step)+"/"+ofToString(steps)<<endl;
        //screen.update();
    }
    
    else{
        cout<<"done"<<endl;
    }
    
    
    if(step%1000 == true){
        numRandSeeds = int(ofRandom(5));
        //for(int i=0; i<numRandSeeds; i++){
            int seed = (int)ofRandom(w*h);
            seeds.push_back(seed);
            traversed = new bool[w*h];
            noise->noiseSeed(ofRandom(10000000));
        //}
    }
    //ofPopStyle();
    
    if(step%1000 == true){
        //noise->noiseSeed(ofRandom(100000));
    }
    
    //screen.draw(0,0);
    
    /*
    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
    glBegin(GL_QUADS);  
    glColor3f( 0.0f, 1.0f, 1.0f );  
    glVertex3f( 0.0f, 0.0f, 0.0f );  
    glVertex3f( ofGetWidth(), 0.0f, 0.0f );  
    glColor3f( 0.0f, 1.0f, 1.0f );  
    glVertex3f( ofGetWidth(), ofGetHeight(), 0.0f );  
    glVertex3f( 0.0f, ofGetHeight(), 0.0f );  
    glEnd();
    glDisable(GL_BLEND);
    */
    
    
    /*
    noiseFbo.begin();
    
    ofSetMinMagFilters(GL_NEAREST,GL_NEAREST);
    //ofClear(0,0,0,0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    noiseShader.begin();
    noiseShader.setUniform1f("time", ofGetFrameNum()*0.09);
    noiseShader.setUniform1f("lac", 12.0);
    noiseShader.setUniform1f("gai", 0.05);
    noiseShader.setUniform1i("oct", 1);
    noiseShader.setUniform2f("resolution", w, h);
    ofRect(0,0,w,h);
    noiseShader.end();
    glDisable(GL_BLEND);
    noiseFbo.end();
    */
    
    //noiseFbo.draw(0,0);
    
    if(ofGetKeyPressed('u')){
    //noiseFbo.readToPixels(fbPix);
    
    feedback.loadData(screen.getPixelsRef());
    //feedback.loadData(fbPix);
    }
    
    /*
    reposFbo.begin();
    
    ofPushStyle();
    ofSetColor(0,0,0,5);
    ofRect(0,0,w,h);
    ofPopStyle();
    
    
    ofSetMinMagFilters(GL_NEAREST,GL_NEAREST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_DST_COLOR);
   
    
    glPushMatrix();
    reposShader.begin();
    
    
    glTranslatef(w/2, h/2, ofMap(mouseX,0,w,0,100));
    glRotatef(1, 0, 0, 1);
    screen.draw(-w/2,-h/2);
    
    reposShader.setUniformTexture("tex0", feedback, 0);
    reposShader.setUniformTexture("tex1", noiseFbo.getTextureReference(), 1);
    reposShader.setUniform2f("amt", 5.0,50.0);
    feedback.draw(-w/2,-h/2);
    reposShader.end();
    glPopMatrix();
    glDisable(GL_BLEND);
    reposFbo.end();
    
    reposFbo.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    reposFbo.draw(0,0);
    reposFbo.readToPixels(fbPix);
    feedback.loadData(fbPix);
    
    
    embossFbo.begin();
    emboss.begin();
    emboss.setUniformTexture("tex", reposFbo.getTextureReference(), 0);
    reposFbo.draw(0, 0);
    emboss.end();
    embossFbo.end();
    
    embossFbo.draw(0, 0);
    
    reposFbo.draw(0,0);
    */
    
    reposFbo.begin();
        reposShader.begin();
        reposShader.setUniformTexture("tex0", screen.getTextureReference(), 0);
        reposShader.setUniformTexture("tex1", sheepImg.getTextureReference(), 1);
        reposShader.setUniform2f("params", ofMap(ofGetMouseX(),0,800,-2,2), ofMap(ofGetMouseY(),0,800,0.5,50) );
        reposShader.setUniform2f("res", w, h);
    
        screen.draw(0,0);
        reposShader.end();
    reposFbo.end();
    
    reposFbo.draw(0,0);
    
    
    //ofDrawBitmapString(ofToString(ofGetFrameRate()), 10,10);
   // if(ofGetFrameNum() == 0){
        g3--;
    //}
    
    //screen.draw(0,0);
}

//--------------------------------------------------------------
void testApp::updateSeeds(){

    
    
    //float xOff = ofGetElapsedTimef()*0.9;
    //float zOff = ofGetElapsedTimef()*0.8;
    
    float xOff = 0.0;
    float zOff = 0.0;
    float yOff = 0.0;
    
    /*
    float inc  = 0;
    float z = 0;
    z+=0.5;
    if(z == w){
        z = 0;
    }
    inc+=0.0000000000000005;
     */
    
    auto pixPtr = screen.getPixels();
    auto sheepPtr = sheepImg.getPixels();
    
    for(int i = seeds.size()-1; i>=0; i--){
        //float yOff = ofGetElapsedTimef()*0.4;
        
        
        int x = int(seeds[i] % w);
        int y = int(seeds[i] / w);
        
        
        
        //float r = 255-ofSignedNoise(zOff,xOff,yOff,ofGetElapsedTimef()/30)*512; //red
        //float g = 255-ofSignedNoise(xOff,yOff,zOff,ofGetElapsedTimef()/30)*512; //green
        //float b = 255-ofSignedNoise(yOff,zOff,yOff,ofGetElapsedTimef()/30)*512; //blue
        
        float r2 = ofMap((noise->noise(xOff, yOff)),0,1,0,255);
        float g2 = ofMap((noise->noise(yOff)),0,1,0,255);
        float b2 = ofMap((noise->noise(zOff)),0,1,0,255);//(noise->noise(zOff));
        
        float briMod = r2;
        briMod = int(ofMap(briMod,0,255,0,3));
        briMod = ofClamp(briMod, 0, 2);
        
        //float r4 = simplex->noise(zOff, xOff, yOff,ofGetElapsedTimef()/5);
        //float g4 = simplex->noise(yOff, ofGetElapsedTimef()/5, xOff,zOff);
        //float b4 = simplex->noise(ofGetElapsedTimef()/5, yOff, zOff,xOff);
        
        //r2*=255;
        //g2*=255;
        //b2*=255;
        
        //r4*=255;
        //g4*=255;
        //b4*=255;
        
        //cout<<r2<<endl;
        //float r3 = ofGetFrameNum()%512;
        //float g3 = ofGetFrameNum()%720;
        //float b3 = ofGetFrameNum()%256;
        
        
        //cout<<ofToString(briMod)<<endl;
        
        //theta+=0.0005;
        //float t = (sin(theta)+1)*1;
        //int n = int(t);
        //cout<<ofToString(n)<<endl;
        
        //c = ofColor(ofMap(r2,-255,255,0,255),ofMap(g2,-255,255,0,255),ofMap(b2,-255,255,0,255));
        //c = ofColor(r3,g3,b3);

        //c.r = pow(c.r,1.4f);
        //c.g = pow(c.g,1.5f);
        //c.b = pow(c.b,1.6f);
        /*
        if(c.r <=40 || c.g <= 40 || c.b <= 40){
        //    c.g*=3;
        //    c.b*=3;
        }
        
        if(c.g<=100 && c.b<=100){
        //    c.g*=2;
        }
        */
        //float s  = int(((sin(ofGetFrameNum()*0.02)+1)*140));
        
        //cout<<ofToString(s)<<endl;
        
        //c.setHue(ofMap((ofGetFrameNum()%int((sin(ofGetFrameNum())+1.1)*512))%255,0,255, 143,147));
        //c.setSaturation(ofMap(s,0,255,0,50));
        //c.setBrightness(int(ofClamp(r2*2, 0, 255))%255);
        /*
        if((c.r+c.g+c.b)/3 <=110){
        //    c.setBrightness(ofClamp(r2, 0, 5));
        }
         */
        //c.r = 0;
        
        
        //screen.setColor(x, y, c);
        int loc = (y*w+ x)*3;
        pixPtr[loc] = r2;//sheepPtr[loc] * (r2*0.025);
        pixPtr[loc+1] = r2;//sheepPtr[loc] * (r2*0.025);
        pixPtr[loc+2] = r2;//sheepPtr[loc] * (r2*0.025);
        //screen.setFromPixels(pixPtr, w, h, OF_IMAGE_COLOR);
        //screen.setFromPixels();
        
        
        //cout<<ofToString(c)<<endl;
        briMod = int(ofRandom(0,2));
        
        //---------Up, Right, Down, Left
        int upIndex = seeds[i] - w + briMod;// (int)ofMap(ofNoise(xOff,yOff),0,1,0,int(ofRandom(2)));
        int rightIndex = seeds[i] + 1 - briMod;// (int)ofMap(ofNoise(xOff,yOff),0,1,0,int(ofRandom(2)));
        int downIndex = seeds[i] + w - 0;// (int)ofMap(ofNoise(xOff,yOff),0,1,0,int(ofRandom(2)));
        int leftIndex = seeds[i] - 1 + briMod;// (int)ofMap(ofNoise(xOff,yOff),0,1,0,int(ofRandom(2)));
        
        //ofSetColor(c);
        
        //ofRect(0, 0, 100, 100);
        //inc+=0.001;
        
        if(y>i && !traversed[upIndex]){
            seeds.push_back(upIndex);
            traversed[upIndex] = true;
        }
        
        if(x < w-1-i && !traversed[rightIndex]){
            seeds.push_back(rightIndex);
            traversed[rightIndex] = true;
        }
        
        if(y< h-1 && !traversed[downIndex]){
            seeds.push_back(downIndex);
            traversed[downIndex] = true;
        }
        
        if(x > i && !traversed[leftIndex]){
            seeds.push_back(leftIndex);
            traversed[leftIndex] = true;
        }
         
         /*
        //---------Upper Left, Upper Right, Lower Left, Lower Right 
        int ulIndex = seeds[i] - w -1+briMod;
        int urIndex = seeds[i] - w +1+briMod;
        int llIndex = seeds[i] + w -1-briMod;
        int lrIndex = seeds[i] + w +1-briMod;
        
        if(x>int(i*0.1) && y>int(i*0.1) && !traversed[ulIndex]){
            seeds.push_back(ulIndex);
            traversed[ulIndex] = true;
        }
        
        if( y>int(i*0.1) && !traversed[urIndex]){
            seeds.push_back(urIndex);
            traversed[urIndex] = true;
        }
        
        if(y< h-1-int(i*0.1) && !traversed[llIndex]){
            seeds.push_back(llIndex);
            traversed[llIndex] = true;
        }
        
        if(x< w-1-int(i*0.1) && y< h-1-int(i*0.1) && !traversed[lrIndex]){
            seeds.push_back(lrIndex);
            traversed[lrIndex] = true;
        }
        */
        

        seeds.erase(seeds.begin()+i);
        
        xOff+=0.001;
        yOff+=0.002;
        zOff+=0.003;
        
        
        
    }
    
    //if(ofGetFrameNum()%300 == 0){
        screen.setFromPixels(pixPtr, w, h, OF_IMAGE_COLOR);
    //}
    
    }

//--------------------------------------------------------------
/*
void testApp::audioOut(float * output, int bufferSize, int nChannels){
    
    for (int i = 0; i < bufferSize; i++){
        float val = ofMap(pix[sampleCount],0,255,-1,1);
        output[i*nChannels    ] = sin(val) * 0.9;
        output[i*nChannels + 1] = sin(val) * 0.9;
        
        sampleCount++;
        sampleCount %= w*w;
    }
    
    
}
*/

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == '='){
        for(int i=0; i<numRandSeeds; i++){
            int seed = (int)ofRandom(w*h);
            seeds.push_back(seed);
            traversed = new bool[w*h];
            noise->noiseSeed(ofRandom(10000000));
            inc+=1;
        }
    }
    
    if(key == 's'){
        //saver.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_HIGH);
        //saver.setup(w, h, "nike"+ofToString(ofGetTimestampString())+".mov");
        
        //vidRecorder.setup("nike.mov", w, h, 15);
        
        save = !save;
    }
    
    if(key == 'f'){
        ofSetFullscreen(true);
    }
    
    if(key == 'u'){
        reposFbo.draw(0,0,w/4,h/4);
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::swap(unsigned char a[], int rx, int ry, int gx, int gy, int bx, int by ) {
    // if(a[rx-3]>a[rx]){ //swap reds
    int tempR = a[rx];
    a[rx] = a[ry];
    a[ry] = tempR;
    // }
    // if(a[gx-3]>a[gx]){ //swap greens
    int tempG = a[gx];
    a[gx] = a[gy];
    a[gy] = tempG;
    // }
    // if(a[bx-3]>a[bx]){ //swap blues
    int tempB = a[bx];
    a[bx] = a[by];
    a[by] = tempB;
    // }
}


void testApp::exit(){
    //saver.finishMovie();
    vidRecorder.close();
}
