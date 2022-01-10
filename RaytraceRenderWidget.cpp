//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  September, 2020
//
//  -----------------------------
//  Raytrace Render Widget
//  -----------------------------
//
//	Provides a widget that displays a fixed image
//	Assumes that the image will be edited (somehow) when Render() is called
//  
////////////////////////////////////////////////////////////////////////

#include <math.h>

// include the header file
#include "RaytraceRenderWidget.h"
#include <thread>

// constructor
RaytraceRenderWidget::RaytraceRenderWidget
        (   
        // the geometric object to show
        TexturedObject      *newTexturedObject,
        // the render parameters to use
        RenderParameters    *newRenderParameters,
        // parent widget in visual hierarchy
        QWidget             *parent
        )
    // the : indicates variable instantiation rather than arbitrary code
    // it is considered good style to use it where possible
    : 
    // start by calling inherited constructor with parent widget's pointer
    QOpenGLWidget(parent),
    // then store the pointers that were passed in
    texturedObject(newTexturedObject),
    renderParameters(newRenderParameters)
{ // constructor
// leaves nothing to put into the constructor body
    frameBuffer = std::shared_ptr<RGBAImage>(new RGBAImage());
     raytracer = std::shared_ptr<ray::Raytracer>(new ray::Raytracer());
} // constructor

// destructor
RaytraceRenderWidget::~RaytraceRenderWidget()
    { // destructor
    // empty (for now)
    // all of our pointers are to data owned by another class
    // so we have no responsibility for destruction
    // and OpenGL cleanup is taken care of by Qt

    renderThread.join();

    } // destructor                                                                 

// called when OpenGL context is set up
void RaytraceRenderWidget::initializeGL()
{ // RaytraceRenderWidget::initializeGL()
// this should remain empty
    if(openRaytracing)
    {
        renderThread.setDraw([&](){

            if(raytracer){
                static int32_t frame = 0;
                static float FPS = 0;
                static uint64_t period = 0;

                raytracer->render(renderParameters);
                auto elapsed = timer.stop();
                frame++;
                period += elapsed;

                 if(frame >= 30)
                 {
                     FPS = frame *  1000000 / (double) period;
                     std::cout<<"FPS : "<< FPS << std::endl;
                     frame = 0;
                     period = 0;
                 }
                 else
                 {
                     auto second = elapsed / 1000000.f ;
                     if( second > 1.f)
                     {
                         std::cout<<"elapsed : "<< second <<std::endl;
                     }
                 }
                std::lock_guard<std::mutex> lock(mu);
                frames.emplace_back(std::shared_ptr<RGBAImage>(new RGBAImage( raytracer->frameBuffer)));
                QMetaObject::invokeMethod(this,"vsync",Qt::AutoConnection);
            }
        });

        timer.start();
        texturedObject->TransferAssetsToRaytracer(raytracer.get());
        raytracer->obj = *texturedObject;
        raytracer->loadScene(renderParameters->sceneFile);
        renderThread.init();

    }

}

// called every time the widget is resized
void RaytraceRenderWidget::resizeGL(int w, int h)
{ // RaytraceRenderWidget::resizeGL()
// resize the render image
    glViewport(0, 0, w, h);
    frameBuffer->Resize(w, h);

    if(openRaytracing){
        frames.clear();
        renderThread.postEvent([&,w,h](){
          raytracer->resize(w,h);
        });
    }

} // RaytraceRenderWidget::resizeGL()

// called every time the widget needs painting
void RaytraceRenderWidget::paintGL()
{ // RaytraceRenderWidget::paintGL()
// set background colour to white
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    if(openRaytracing)
    {
        renderWithRaytracing();
    }
    else
    {
        renderWithFakeGL();

    }

} // RaytraceRenderWidget::paintGL()

    // routine that generates the image
void RaytraceRenderWidget::Raytrace()
{ // RaytraceRenderWidget::Raytrace()
// This is where you will invoke your raytracing
    renderWithRaytracing();
} // RaytraceRenderWidget::Raytrace()

// mouse-handling
void RaytraceRenderWidget::mousePressEvent(QMouseEvent *event)
    { // RaytraceRenderWidget::mousePressEvent()
    // store the button for future reference
    int whichButton = event->button();
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width   
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0 * event->x() - size) / size;
    float y = (size - 2.0 * event->y() ) / size;

    
    // and we want to force mouse buttons to allow shift-click to be the same as right-click
    int modifiers = event->modifiers();
    
    // shift-click (any) counts as right click
    if (modifiers & Qt::ShiftModifier)
        whichButton = Qt::RightButton;
    
    // send signal to the controller for detailed processing
    emit BeginScaledDrag(whichButton, x,y);
    } // RaytraceRenderWidget::mousePressEvent()
    
void RaytraceRenderWidget::mouseMoveEvent(QMouseEvent *event)
{ // RaytraceRenderWidget::mouseMoveEvent()
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0 * event->x() - size) / size;
    float y = (size - 2.0 * event->y() ) / size;

    // send signal to the controller for detailed processing
    emit ContinueScaledDrag(x,y);
} // RaytraceRenderWidget::mouseMoveEvent()
    
void RaytraceRenderWidget::mouseReleaseEvent(QMouseEvent *event)
{ // RaytraceRenderWidget::mouseReleaseEvent()
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0 * event->x() - size) / size;
    float y = (size - 2.0 * event->y() ) / size;

    // send signal to the controller for detailed processing
    emit EndScaledDrag(x,y);
} // RaytraceRenderWidget::mouseReleaseEvent()

void RaytraceRenderWidget::vsync(){
    update();
}


void RaytraceRenderWidget::renderWithRaytracing()
{
    static bool first = true;
    if(first){
      std::lock_guard<std::mutex> lock(mu);
      frames.clear();
      first = false;
    }

    std::unique_lock<std::mutex> lock(mu);
    if(frames.size() > 0){
        auto & frameBuffer = frames.front();
        glDrawPixels(frameBuffer->width, frameBuffer->height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer->block);
        this->frameBuffer = frameBuffer;
        frames.pop_front();
    }else{
        glDrawPixels(frameBuffer->width, frameBuffer->height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer->block);
    }
    lock.unlock();
}

void RaytraceRenderWidget::renderWithFakeGL()
{

}
