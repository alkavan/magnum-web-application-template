#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/DebugTools/ResourceManager.h>
#include <Magnum/Timeline.h>

#define CORRADE_TARGET_EMSCRIPTEN

using namespace Magnum;
using namespace Math::Literals;
using namespace Magnum::SceneGraph;
using namespace Magnum::DebugTools;
using namespace Magnum::MeshTools;

typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;
typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;

class ExampleCube: public Object3D, public SceneGraph::Drawable3D {
public:
    explicit ExampleCube(Object3D* parent, SceneGraph::DrawableGroup3D* group): Object3D{parent}, SceneGraph::Drawable3D{*this, group} {
        std::tie(_mesh, _vertices, _indices) = MeshTools::compile(Primitives::cubeSolid(), GL::BufferUsage::StaticDraw);
        _color = Color3::fromHsv(216.0_degf, 0.85f, 1.0f);
    }

private:
    Color3 _color;

    void draw(const Matrix4& transformationMatrix, Camera3D& camera) override {
        _shader.setDiffuseColor(_color)
                .setLightPosition(camera.cameraMatrix().transformPoint({5.0f, 5.0f, 7.0f}))
                .setTransformationMatrix(transformationMatrix)
                .setNormalMatrix(transformationMatrix.rotation())
                .setProjectionMatrix(camera.projectionMatrix());

        _mesh.draw(_shader);
    }
public:
    void changeColor() {
        _color = Color3::fromHsv(_color.hue() + 50.0_degf, 1.0f, 1.0f);
        _shader.setDiffuseColor(_color);
    }

    GL::Mesh _mesh;
    std::unique_ptr<GL::Buffer> _vertices, _indices;
    Shaders::Phong _shader;
};


class MyApplication: public Platform::Application {
    public:
        explicit MyApplication(const Arguments& arguments);

    private:
        void drawEvent() override;
        void viewportEvent(const Vector2i& size) override;
        void keyPressEvent(KeyEvent& event) override;
        void mousePressEvent(MouseEvent& event) override;
        void mouseReleaseEvent(MouseEvent& event) override;
        void mouseMoveEvent(MouseMoveEvent& event) override;

        DebugTools::ResourceManager _manager;

        Scene3D _scene;
        Object3D *_cameraRig, *_cameraObject;
        SceneGraph::Camera3D* _camera;

        SceneGraph::DrawableGroup3D _drawables;
        ExampleCube *_cube;
        Matrix4 _transformation;
        Vector2i _previousMousePosition;
        Color3 _color;
};

MyApplication::MyApplication(const Arguments& arguments) : Platform::Application {
    arguments, Configuration{}.setTitle("Magnum Triangle Example")
} {
    /* TODO: Add your initialization code here */
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    /* Camera setup */
    (_cameraRig = new Object3D(&_scene))
            ->translate({0.f, 0.f, 0.f});
    (_cameraObject = new Object3D(_cameraRig))
            ->translate({0.f, 0.f, 10.f});
    (_camera = new SceneGraph::Camera3D(*_cameraObject))
            ->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
            .setProjectionMatrix(Matrix4::perspectiveProjection(Rad(Float(M_PI_4)), 1.0f, 0.001f, 100.0f))
            .setViewport(GL::defaultFramebuffer.viewport().size());

    _cube = new ExampleCube(&_scene, &_drawables);

    _transformation = Matrix4::rotationX(30.0_degf)*
                      Matrix4::rotationY(40.0_degf);

    _cube->setTransformation(_transformation);

    _color = Color3::fromHsv(35.0_degf, 1.0f, 1.0f);
}

void MyApplication::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color|GL::FramebufferClear::Depth);
    _camera->draw(_drawables);
}

void MyApplication::viewportEvent(const Vector2i& size) {
    GL::defaultFramebuffer.setViewport({{}, size});
    _camera->setViewport(size);
}

void MyApplication::keyPressEvent(KeyEvent& event) {
    if(event.key() == KeyEvent::Key::Down)
        _cameraObject->rotateX(Deg(5.0f));
    else if(event.key() == KeyEvent::Key::Up)
        _cameraObject->rotateX(Deg(-5.0f));
    else if(event.key() == KeyEvent::Key::Left)
        _cameraRig->rotateY(Deg(-5.0f));
    else if(event.key() == KeyEvent::Key::Right)
        _cameraRig->rotateY(Deg(5.0f));
    else return;

    event.setAccepted();
}

void MyApplication::mousePressEvent(MouseEvent& event) {
    if(event.button() != MouseEvent::Button::Left) return;

    _previousMousePosition = event.position();
    event.setAccepted();
}

void MyApplication::mouseReleaseEvent(MouseEvent& event) {

    _cube->changeColor();

    event.setAccepted();
    redraw();
}

void MyApplication::mouseMoveEvent(MouseMoveEvent& event) {
    if(!(event.buttons() & MouseMoveEvent::Button::Left)) return;

    const Vector2 delta = 3.0f*
                          Vector2{event.position() - _previousMousePosition}/
                          Vector2{GL::defaultFramebuffer.viewport().size()};

    _transformation =
            Matrix4::rotationX(Rad{delta.y()})*
            _transformation*
            Matrix4::rotationY(Rad{delta.x()});

    _cube->setTransformation(_transformation);

    _previousMousePosition = event.position();
    event.setAccepted();
    redraw();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
