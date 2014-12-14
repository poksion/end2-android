#include <jni.h>

#include <stdio.h>
#include <stdlib.h>

#include "end2/src/main/system/ESystem.h"
#include "end2/src/main/graphics/EGraphicContext.h"
#include "end2/src/main/graphics/EGraphicContextOpenGL.h"
#include "end2/src/main/graphics/EGeometryBuffer.h"
#include "end2/src/main/graphics/EGeometryBufferUtil.h"

#include "end2/src/main/scenegraph/math/EMatrix4.h"
#include "end2/src/main/scenegraph/math/EVector3.h"

#include "end2/src/main/scenegraph/ECameraSceneNode.h"
#include "end2/src/main/scenegraph/EDataSceneNode.h"
#include "end2/src/main/scenegraph/ECameraUtil.h"

#include "end2/src/main/modules/ETouchScreenModule.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_net_poksion_end2_JniBridge_nativeOnSurfaceCreated(JNIEnv* env, jobject obj);
JNIEXPORT void JNICALL Java_net_poksion_end2_JniBridge_nativeOnSurfaceChanged(JNIEnv* env, jobject obj, jint width, jint height);
JNIEXPORT void JNICALL Java_net_poksion_end2_JniBridge_nativeOnDrawFrame(JNIEnv* env, jobject obj);

JNIEXPORT void JNICALL Java_net_poksion_end2_JniBridge_nativeOnMotionEventUp(JNIEnv* env, jobject obj, jint pointerIndex, jfloat x, jfloat y);
JNIEXPORT void JNICALL Java_net_poksion_end2_JniBridge_nativeOnMotionEventDown(JNIEnv* env, jobject obj, jint pointerIndex, jfloat x, jfloat y);
JNIEXPORT void JNICALL Java_net_poksion_end2_JniBridge_nativeOnMotionEventMove(JNIEnv* env, jobject obj, jint pointerIndex, jfloat x, jfloat y);

#ifdef __cplusplus
}
#endif

using namespace end2;

class Engine {
public:
    Engine(){
        ESystem::newSystem();

    }

    ~Engine(){
        ESystem::getSystem().clearModules();
        ESystem::deleteSystem();
    }
    ECameraSceneNode camera;
    EDataSceneNode<EGeometryBuffer> geometrySceneNode;
};
static Engine engine;

JNIEXPORT void JNICALL Java_net_poksion_end2_JniBridge_nativeOnSurfaceCreated(JNIEnv* env, jobject obj) {
    if(0 == ESystem::getSystem().getModule<EGraphicContext>()){
        EGraphicContextOpenGL* openGLContext = ESystem::getSystem().createModule<EGraphicContextOpenGL, EGraphicContext>();
        openGLContext->init();
    }

    if(0 == ESystem::getSystem().getModule<ETouchScreenModule>()){
        ESystem::getSystem().createModule<ETouchScreenModule>();
    }
}

JNIEXPORT void JNICALL Java_net_poksion_end2_JniBridge_nativeOnSurfaceChanged(JNIEnv* env, jobject obj, jint width, jint height) {
    EGeometryBuffer& test_box_buffer = engine.geometrySceneNode.getData();
    if(test_box_buffer.isAssignable()){
        EGeometryBufferUtil::buildCube(test_box_buffer, 2.0f);
    }

    // setup camera
    EMatrix4 tempMatrix;

    // position
    EVector3 pos(10, 0, 0);
    EVector3 dir(-1, 0, 0);
    EVector3 up(0, 0, 1);
    EMatrix4::makeFrame(tempMatrix, pos, dir, up);
    engine.camera.setTransformRelativeToWorld(tempMatrix);

    // projection
    EGraphicContext* gc = ESystem::getSystem().getModule<EGraphicContext>();
    gc->setDisplaySize(width, height);
    engine.camera.updateProjection(width, height);

    // setup from camera
    gc->setViewProjectionTransform(engine.camera.getViewTransform().get(), engine.camera.getProjectionTransform().get());

    // setup data scene node
    engine.geometrySceneNode.setTransformRelativeToWorld(EMatrix4::IDENTITY);
}

JNIEXPORT void JNICALL Java_net_poksion_end2_JniBridge_nativeOnDrawFrame(JNIEnv* env, jobject obj) {
    // start draw
    EGraphicContext* gc = ESystem::getSystem().getModule<EGraphicContext>();
    gc->clearBuffers();
    if(gc->beginDrawing()){
        gc->setWorldTransform(engine.geometrySceneNode.getTransformRelativeToParent().get());
        gc->drawGeometryBuffer(engine.geometrySceneNode.getData());
    }
    gc->endDrawing();
}

JNIEXPORT void JNICALL Java_net_poksion_end2_JniBridge_nativeOnMotionEventUp(JNIEnv* env, jobject obj, jint pointerIndex, jfloat x, jfloat y) {
    ETouchScreenModule* tm = ESystem::getSystem().getModule<ETouchScreenModule>();
    if(0 != tm){
        tm->setPositionBuffer(x, y);
    }
}

JNIEXPORT void JNICALL Java_net_poksion_end2_JniBridge_nativeOnMotionEventDown(JNIEnv* env, jobject obj, jint pointerIndex, jfloat x, jfloat y) {
    ETouchScreenModule* tm = ESystem::getSystem().getModule<ETouchScreenModule>();
    if(0 != tm){
        tm->setPositionBuffer(x, y);
    }
}

JNIEXPORT void JNICALL Java_net_poksion_end2_JniBridge_nativeOnMotionEventMove(JNIEnv* env, jobject obj, jint pointerIndex, jfloat x, jfloat y) {
    EGraphicContext* gc = ESystem::getSystem().getModule<EGraphicContext>();
    ETouchScreenModule* tm = ESystem::getSystem().getModule<ETouchScreenModule>();
    if(0 != gc && 0 != tm){
        tm->setPositionBuffer(x, y);

        float horizontal_move, vertical_move;
        tm->getDiffPosition(horizontal_move, vertical_move);
        horizontal_move = - horizontal_move * 0.005f;
        vertical_move = - vertical_move * 0.005f;
        ECameraUtil::rotate(engine.camera, horizontal_move, vertical_move);
        gc->setViewProjectionTransform(engine.camera.getViewTransform().get(), 0);
    }
}

