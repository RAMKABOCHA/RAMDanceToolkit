#pragma once

class Lines : public rdtk::BaseScene{
public:
    float threshold = 40.;
    float opacity = 0.3;
    float lineWidth;
    vector<vector<ofVec3f>> pos;
    int mode = 0;
    
    void drawImGui(){
        ImGui::SliderFloat("threshold", &threshold, 0.0, 200.0);
        ImGui::SliderFloat("opacity", &opacity, 0.0, 1.0);
        ImGui::SliderInt("mode", &mode, 0, 2);
        
        ImGui::SliderFloat("lineWidth", &lineWidth, 0.0, 10.0);
    };
    void setup(){
        
        vector<ofVec3f> plane;
        int w = 20;
        int h = 20;
        for (int i = 0; i < w; i++){
            for (int j = 0; j < h; j++){
                ofVec3f v;
                v.x = (i - w / 2. + 0.5) * 30.;
                v.y = 0.;
                v.z = (j - h / 2. + 0.5) * 30.;
                
                plane.push_back(v);
            }
        }
        pos.push_back(plane);
        
        vector<ofVec3f> sphere;
        ofMesh mesh = ofMesh::icosphere(150., 2);
        for (int i = 0; i < mesh.getNumVertices(); i++) {
            ofVec3f v = mesh.getVertex(i) + ofVec3f(0, 75, 0);
            sphere.push_back(v);
        }
        pos.push_back(sphere);
        
        vector<ofVec3f> grid;
        w = 8;
        h = 8;
        int d = 8;
        for (int i = 0; i < w; i++){
            for (int j = 0; j < h; j++){
                for (int k = 0; k < d; k++){
                    ofVec3f v;
                    v.x = (i - w / 2. + 0.5) * 60.;
                    v.y = j * 30.;
                    v.z = (k - d / 2. + 0.5) * 60.;
                    
                    grid.push_back(v);
                }
            }
        }
        pos.push_back(grid);
        
    };
    
    void update(){};
    
    void draw(){
        // validate
        if (getNumNodeArray() <= 0) return;
        
        rdtk::BeginCamera();
        ofPushStyle();
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofSetLineWidth(lineWidth);
        glBegin(GL_LINES);
        
        for (int k = 0; k < getNumNodeArray(); k++) {
            rdtk::NodeArray actor = getNodeArray(k);
            for (int i = 0; i < actor.getNumNode(); i++) {
                
                ofVec3f p = actor.getNode(i).getGlobalPosition();
                
                for (int j = 0; j < pos[mode].size(); j++) {
                    
                    float dist = pos[mode][j].distance(p);
                    ofFloatColor c(1., 1. / threshold * dist * opacity);
                    
                    if (dist < threshold) {
                        glColor4f(c.r, c.g, c.b, c.a);
                        glVertex3f(pos[mode][j].x, pos[mode][j].y, pos[mode][j].z);
                        glColor4f(c.r, c.g, c.b, c.a);
                        glVertex3f(p.x, p.y, p.z);
                    }
                }
            }
        }
        
        glEnd();
        
        ofDisableBlendMode();
        ofPopStyle();
        rdtk::EndCamera();
        
    };
    
    string getName() const { return "Lines"; }
};
