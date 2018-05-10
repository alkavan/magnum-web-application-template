#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/Trade/MeshData3D.h>

#define CORRADE_TARGET_EMSCRIPTEN

using namespace Magnum;
using namespace Math::Literals;

class MyApplication: public Platform::Application {
    public:
        explicit MyApplication(const Arguments& arguments);

    private:
        void drawEvent() override;
        void mousePressEvent(MouseEvent& event) override;
        void mouseReleaseEvent(MouseEvent& event) override;
        void mouseMoveEvent(MouseMoveEvent& event) override;

        GL::Buffer _vertexBuffer{GL::Buffer::TargetHint::Array};
        GL::Buffer _indexBuffer{GL::Buffer::TargetHint::ElementArray};

        GL::Mesh _mesh;
        Shaders::Phong _shader;

        Matrix4 _transformation, _projection;
        Vector2i _previousMousePosition;
        Color3 _color;
};

MyApplication::MyApplication(const Arguments& arguments) : Platform::Application {
    arguments, Configuration{}.setTitle("Magnum Triangle Example")
} {
    /* TODO: Add your initialization code here */
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    const Trade::MeshData3D cube = Primitives::cubeSolid();

    _vertexBuffer.setData(MeshTools::interleave(cube.positions(0), cube.normals(0)), GL::BufferUsage::StaticDraw);

    Containers::Array<char> indexData;
    MeshIndexType indexType;
    UnsignedInt indexStart, indexEnd;
    std::tie(indexData, indexType, indexStart, indexEnd) = MeshTools::compressIndices(cube.indices());
    _indexBuffer.setData(indexData, GL::BufferUsage::StaticDraw);

    _mesh.setPrimitive(cube.primitive())
            .setCount(cube.indices().size())
            .addVertexBuffer(_vertexBuffer, 0, Shaders::Phong::Position{}, Shaders::Phong::Normal{})
            .setIndexBuffer(_indexBuffer, 0, indexType, indexStart, indexEnd);

    _transformation = Matrix4::rotationX(30.0_degf)*
                      Matrix4::rotationY(40.0_degf);
    _color = Color3::fromHsv(35.0_degf, 1.0f, 1.0f);

    _projection = Matrix4::perspectiveProjection(35.0_degf, Vector2{GL::defaultFramebuffer.viewport().size()}.aspectRatio(), 0.01f, 100.0f)*
                  Matrix4::translation(Vector3::zAxis(-10.0f));
}

void MyApplication::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color|GL::FramebufferClear::Depth);

    _shader.setLightPosition({7.0f, 5.0f, 2.5f})
            .setLightColor(Color3{1.0f})
            .setDiffuseColor(_color)
            .setAmbientColor(Color3::fromHsv(_color.hue(), 1.0f, 0.3f))
            .setTransformationMatrix(_transformation)
            .setNormalMatrix(_transformation.rotationScaling())
            .setProjectionMatrix(_projection);
    _mesh.draw(_shader);

    swapBuffers();
}

void MyApplication::mousePressEvent(MouseEvent& event) {
    if(event.button() != MouseEvent::Button::Left) return;

    _previousMousePosition = event.position();
    event.setAccepted();
}

void MyApplication::mouseReleaseEvent(MouseEvent& event) {
    _color = Color3::fromHsv(_color.hue() + 50.0_degf, 1.0f, 1.0f);

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

    _previousMousePosition = event.position();
    event.setAccepted();
    redraw();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
