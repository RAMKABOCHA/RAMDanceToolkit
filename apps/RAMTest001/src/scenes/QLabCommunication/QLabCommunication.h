//
//  QLabCommunication.h
//  RAM Dance Toolkit
//
//  Created by James Kong on 26/8/2018.
//

#ifndef QLabCommunication_h
#define QLabCommunication_h
#include "ofxOSCvariable.h"

class QLabCommunication{
public:
    
    void setup(string senderHost, int senderPort, int receivePort);
    void update();
    template <typename T>
    ofxOscVariablePath_<T>* addVariableToPath(T& var, string path){
        return oscVar.addVariableToPath(var, path);
    }
    
    inline static QLabCommunication& instance()
    {
        if (__instance == NULL)
            __instance = new QLabCommunication;
        return *__instance;
    }
    
private:
    static QLabCommunication *__instance;
    
    QLabCommunication() {};
    QLabCommunication(const QLabCommunication&){}
    QLabCommunication& operator=(const QLabCommunication&){return *__instance;}
    ~QLabCommunication() {};
    ofxOscVariable oscVar;
};
//

#endif /* QLabCommunication_h */
