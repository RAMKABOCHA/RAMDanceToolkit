//
//  dpScoreSceneMasterIncrement.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/18/15.
//
//

#include "dpScoreSceneMasterIncrement.h"

DP_SCORE_NAMESPACE_BEGIN

#pragma mark ___________________________________________________________________
MasterIncrementNode::MasterIncrementNode()
{
}

MasterIncrementNode& MasterIncrementNode::operator = (const MasterIncrementNode& rhs)
{
    return *this = rhs;
}

#pragma mark ___________________________________________________________________
void SceneMasterIncrement::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setColorBack(MH::kBackgroundColor);
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Dist Limit", 500.f, 3000.f, &mLimit);
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Mean Limit", 0.1f, 100.f, &mMean.mMeanLimit);
    mUICanvas->addSlider("Scene change span", 0.f, 60.f * 3.f, &mMean.mMinSetSceneTime);
    
    mCam.disableMouseInput();
}

void SceneMasterIncrement::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneMasterIncrement::enter()
{
    dpDebugFunc();
    
    mCam.enableMouseInput();
    
    mAdditions.assign(MH::kNumValvePins, 0.f);
}

void SceneMasterIncrement::exit()
{
    dpDebugFunc();
    
    mCam.disableMouseInput();
}

void SceneMasterIncrement::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCameraUnitMean) {
        if (m.getNumArgs() == mMean.mMean.DIM) {
            for (int i=0; i<mMean.mMean.DIM; i++) {
                mMean.mMean[i] = m.getArgAsInt32(i);
            }
            mMean.update();
        }
    }
    if (m.getAddress() == kOscAddrCameraUnitVector) {
        for (int i=0; i<kNumCameraunitVectors ; i++) {
            if (2*i+0 >= m.getNumArgs() || 2*i+1 >= m.getNumArgs()) break;
            ofVec2f v(m.getArgAsFloat(2*i+0), m.getArgAsFloat(2*i+1));
        }
    }
    if (m.getAddress() == kOscAddrMotioner) {
        if (mAdditions.empty() == false && getNumSkeletons() >= 1) {
            for (int i=0; i<getNumSkeletons(); i++) {
                auto skl = getSkeleton(i);
                if (i<mAdditions.size()) {
                    auto& a = mAdditions.at(i);
                    a = 0.f;
                    for (int k=0; k<skl->getNumJoints(); k++) {
                        auto& n = skl->getJoint(k);
                        n.update();
                        a += n.totalDistance;
                    }
                    if (a >= mLimit) {
                        getMH().turnOnValve(i);
                        for (auto& n : skl->getJoints()) n.totalDistance = 0.f;
                    }
                }
            }
        }
    }
}

void SceneMasterIncrement::draw()
{
    ofSetColor(MH::kTextColor);
    ofPushMatrix();
    alignedTranslate(getMH().mTextLeftCorner);
    string name{getName()};
    ofStringReplace(name, "dp::score::Scene", "");
    stringstream ss;
    ss << name << endl << endl
    << mMean.mMean << endl
    << mMean.mMeanAddtion << endl;
    ofDrawBitmapString(ss.str(), 0.f, 0.f);
    ofTranslate(0.f, MH::kTextSpacing * 6.f);
    int i=0;
    for (auto f : mAdditions) {
        ofPushMatrix();
        ofTranslate(0.f, 12.f * i);
        stringstream ss;
        ss << boolalpha << getMH().getIsOpeningValve(i)
        << ": " << ofToString(f, 3) + "/" + ofToString(mLimit);
        ofDrawBitmapString(ss.str(), ofPoint::zero());
        ofPopMatrix();
        i++;
    }
    ofPopMatrix();
}

#pragma mark ___________________________________________________________________
void SceneMasterIncrement::setupSkeleton(SkeletonPtr skl)
{
}

void SceneMasterIncrement::updateSkeleton(SkeletonPtr skl)
{
}

void SceneMasterIncrement::exitSkeleton(SkeletonPtr skl)
{
}

DP_SCORE_NAMESPACE_END