#include <Magnum/Buffer.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Shaders/VertexColor.h>

#define CORRADE_TARGET_EMSCRIPTEN

using namespace Magnum;

class MyApplication: public Platform::Application {
    public:
        explicit MyApplication(const Arguments& arguments);

    private:
        void drawEvent() override;

        Buffer _buffer;
        Mesh _mesh;
        Shaders::VertexColor2D _shader;
};

MyApplication::MyApplication(const Arguments& arguments)
        : Platform::Application{arguments, Configuration{}.setTitle("Magnum Triangle Example")} {
    /* TODO: Add your initialization code here */
    using namespace Math::Literals;

    struct TriangleVertex {
        Vector2 position;
        Color3 color;
    };
    const TriangleVertex data[]{
            {{-0.5f, -0.5f}, 0xff0000_rgbf},    /* Left vertex, red color */
            {{ 0.5f, -0.5f}, 0x00ff00_rgbf},    /* Right vertex, green color */
            {{ 0.0f,  0.5f}, 0x0000ff_rgbf}     /* Top vertex, blue color */
    };

    _buffer.setData(data, BufferUsage::StaticDraw);
    _mesh.setPrimitive(MeshPrimitive::Triangles)
            .setCount(3)
            .addVertexBuffer(_buffer, 0,
                             Shaders::VertexColor2D::Position{},
                             Shaders::VertexColor2D::Color{Shaders::VertexColor2D::Color::Components::Three});
}

void MyApplication::drawEvent() {
    defaultFramebuffer.clear(FramebufferClear::Color);

    _mesh.draw(_shader);

    swapBuffers();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
